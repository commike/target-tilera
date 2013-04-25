/*
 *  Tilera emulation cpu translation for qemu.
 *
 *  Copyright (c) 2013 Sarmad Tanwir
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "cpu.h"
#include "disas.h"
#include "host-utils.h"
#include "tcg-op.h"

#include "helper.h"
#define GEN_HELPER 1
#include "helper.h"

#undef TILERA_DEBUG_DISAS
#define CONFIG_SOFTFLOAT_INLINE

#ifdef TILERA_DEBUG_DISAS
#  define LOG_DISAS(...) qemu_log_mask(CPU_LOG_TB_IN_ASM, ## __VA_ARGS__)
#else
#  define LOG_DISAS(...) do { } while (0)
#endif

typedef struct DisasContext DisasContext;
struct DisasContext {
    struct TranslationBlock *tb;
    CPUTileraState *env;
    uint64_t pc;
    int mem_idx;


};

/* Return values from translate_one, indicating the state of the TB.
   Note that zero indicates that we are not exiting the TB.  */

typedef enum {
    NO_EXIT,

    /* We have emitted one or more goto_tb.  No fixup required.  */
    EXIT_GOTO_TB,

    /* We are not using a goto_tb (for whatever reason), but have updated
       the PC (for whatever reason), so there's no need to do it again on
       exiting the TB.  */
    EXIT_PC_UPDATED,

    /* We are exiting the TB, but have neither emitted a goto_tb, nor
       updated the PC for the next instruction to be executed.  */
    EXIT_PC_STALE,

    /* We are ending the TB with a noreturn function call, e.g. longjmp.
       No following code will be executed.  */
    EXIT_NORETURN,
} ExitStatus;

/* global register indexes */
static TCGv_ptr cpu_env;
static TCGv tile_gpr[56];
/*static TCGv tile_fp;
static TCGv tile_tp;
static TCGv tile_sp;
static TCGv tile_lr;*/
static TCGv tile_sn;
static TCGv tile_idn[2];
static TCGv tile_udn[4];
static TCGv tile_zero;
static TCGv tile_pc;
static TCGv tile_spr[SPR_MAX];

static char spu_reg_names[55+SPR_MAX];

#include "gen-icount.h"

static void tilera_translate_init(void)
{
    int i;
    char *p;
    static int done_init = 0; /* to make sure this func is called once only */

    if (done_init)
        return;

    cpu_env = tcg_global_reg_new_ptr(TCG_AREG0, "env");

    p = cpu_reg_names;
    for (i = 0; i < 52; i++) {
        sprintf(p, "gpr%d", i);
        tile_gpr[i] = tcg_global_mem_new_i64(TCG_AREG0,
                                         offsetof(CPUTileraState, gpr[i]), p);
    }

	tile_gpr[TILE_REG_FP] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, gpr[TILE_REG_FP]), "fp");
	tile_gpr[TILE_REG_TP] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, gpr[TILE_REG_TP]), "tp");
	tile_gpr[TILE_REG_SP] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, gpr[TILE_REG_SP]), "sp");
	
	tile_gpr[TILE_REG_LR] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, gpr[TILE_REG_LR]), "lr");

	tile_sn = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, sn), "sn");
	tile_idn[0] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, idn0), "idn0");
	tile_idn[1] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, idn1), "idn1");
	tile_udn[0] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, udn0), "udn0");
	tile_udn[1] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, udn1), "udn1");
	tile_udn[2] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, udn2), "udn2");
	tile_udn[3] = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, udn3), "udn3");
	
	tile_zero = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, zero), "zero");

	tile_pc = tcg_global_mem_new_i64(TCG_AREG0,
                                    offsetof(CPUTileraState, pc), "pc");

	for (i = 0; i < SPR_MAX; i++) {
        sprintf(p, "spr%d", i);
        tile_spr[i] = tcg_global_mem_new_i64(TCG_AREG0,
                                         offsetof(CPUTileraState, spr[i]), p);
    }

	/* TODO: define sprs? */
    /* register helpers */
#define GEN_HELPER 2
#include "helper.h"

    done_init = 1;
}

static void gen_excp_1(int exception, int error_code)
{
    TCGv_i32 tmp1, tmp2;

    tmp1 = tcg_const_i32(exception);
    tmp2 = tcg_const_i32(error_code);
    gen_helper_excp(cpu_env, tmp1, tmp2);
    tcg_temp_free_i32(tmp2);
    tcg_temp_free_i32(tmp1);
}

static ExitStatus gen_excp(DisasContext *ctx, int exception, int error_code)
{
    tcg_gen_movi_i64(cpu_pc, ctx->pc);
    gen_excp_1(exception, error_code);
    return EXIT_NORETURN;
}

static inline ExitStatus gen_invalid(DisasContext *ctx)
{
    return gen_excp(ctx, EXCP_OPCDEC, 0);
}


#ifdef __tilegx__
#define TILE_MAX_INSTRUCTIONS_PER_BUNDLE TILEGX_MAX_INSTRUCTIONS_PER_BUNDLE
#define tile_decoded_instruction tilegx_decoded_instruction
#define tile_mnemonic tilegx_mnemonic
#define parse_insn_tile parse_insn_tilegx
#define TILE_OPC_IRET TILEGX_OPC_IRET
#define TILE_OPC_ADDI TILEGX_OPC_ADDI
#define TILE_OPC_ADDLI TILEGX_OPC_ADDLI
#define TILE_OPC_INFO TILEGX_OPC_INFO
#define TILE_OPC_INFOL TILEGX_OPC_INFOL
#define TILE_OPC_JRP TILEGX_OPC_JRP
#define TILE_OPC_MOVE TILEGX_OPC_MOVE
#define OPCODE_STORE TILEGX_OPC_ST
typedef int64_t bt_int_reg_t;
#else
#define TILE_MAX_INSTRUCTIONS_PER_BUNDLE TILEPRO_MAX_INSTRUCTIONS_PER_BUNDLE
#define tile_decoded_instruction tilepro_decoded_instruction
#define tile_mnemonic tilepro_mnemonic
#define parse_insn_tile parse_insn_tilepro
#define TILE_OPC_IRET TILEPRO_OPC_IRET
#define TILE_OPC_ADDI TILEPRO_OPC_ADDI
#define TILE_OPC_ADDLI TILEPRO_OPC_ADDLI
#define TILE_OPC_INFO TILEPRO_OPC_INFO
#define TILE_OPC_INFOL TILEPRO_OPC_INFOL
#define TILE_OPC_JRP TILEPRO_OPC_JRP
#define TILE_OPC_MOVE TILEPRO_OPC_MOVE
#define OPCODE_STORE TILEPRO_OPC_SW
typedef int bt_int_reg_t;
#endif


struct insn_bundle {
	uint64_t bits;
	int num_insns;
	struct tilegx_decoded_instruction
	insns[TILE_MAX_INSTRUCTIONS_PER_BUNDLE];
};

static int use_goto_tb(DisasContext *ctx, uint64_t dest)
{
    /* Check for the dest on the same page as the start of the TB.  We
       also want to suppress goto_tb in the case of single-steping and IO.  */
    return (((ctx->tb->pc ^ dest) & TARGET_PAGE_MASK) == 0
            && !ctx->env->singlestep_enabled);
}

static ExitStatus gen_bdirect(DisasContext *ctx, int64_t jump_off)
{
    uint64_t dest = ctx->pc + (sign_extend_17(jump_off) << (INSTRUCTION_SIZE_LOG_2 - BYTE_SIZE_LOG_2));

    if (jump_off == 0) {
        return 0;
    } else if (use_goto_tb(ctx, dest)) {
        tcg_gen_goto_tb(0);
        tcg_gen_movi_i64(tile_pc, dest);
        tcg_gen_exit_tb((tcg_target_long)ctx->tb);
        return EXIT_GOTO_TB;
    } else {
        tcg_gen_movi_i64(tile_pc, dest);
        return EXIT_PC_UPDATED;
    }
}

static ExitStatus gen_bdirectr (DisasContext *ctx, TCGv_i64 reg)
{
	uint64_t dest = reg.i64 & ALIGNED_INSTRUCTION_MASK;
		
    if (reg.i64 == 0) {
        return 0;
    } else if (use_goto_tb(ctx, dest)) {
        tcg_gen_goto_tb(0);
        tcg_gen_movi_i64(tile_pc, dest);
        tcg_gen_exit_tb((tcg_target_long)ctx->tb);
        return EXIT_GOTO_TB;
    } else {
        tcg_gen_movi_i64(tile_pc, dest);
        return EXIT_PC_UPDATED;
    }
}


static ExitStatus gen_bcond_internal(DisasContext *ctx, TCGCond cond,
                                     TCGv cmp, int64_t br_off)
{
    uint64_t dest = ctx->pc + (sign_extend_17(br_off) << (INSTRUCTION_SIZE_LOG_2 - BYTE_SIZE_LOG_2));
    int lab_true = gen_new_label();

    if (use_goto_tb(ctx, dest)) {
        tcg_gen_brcondi_i64(cond, cmp, 0, lab_true);

        tcg_gen_goto_tb(0);
        tcg_gen_movi_i64(tile_pc, ctx->pc);
        tcg_gen_exit_tb((tcg_target_long)ctx->tb);

        gen_set_label(lab_true);
        tcg_gen_goto_tb(1);
        tcg_gen_movi_i64(tile_pc, dest);
        tcg_gen_exit_tb((tcg_target_long)ctx->tb + 1);

        return EXIT_GOTO_TB;
    } else {
        TCGv_i64 z = tcg_const_i64(0);
        TCGv_i64 d = tcg_const_i64(dest);
        TCGv_i64 p = tcg_const_i64(ctx->pc);

        tcg_gen_movcond_i64(cond, tile_pc, cmp, z, d, p);

        tcg_temp_free_i64(z);
        tcg_temp_free_i64(d);
        tcg_temp_free_i64(p);
        return EXIT_PC_UPDATED;
    }
}


static ExitStatus translate_one(DisasContext *ctx, uint64_t insn)
{

insn_bundle bundle;
uint8_t ins_num, opr_num, dest_reg, src_reg[4], src_reg_num, imm_num;
int64_t imm64[3];
int8_t imm8;
int16_t imm16;
int32_t imm32;
TCGv_i32 l_1, l_2, l_3, l_4;
TCGv_i64 ll_1, ll_2, ll_3;

#ifndef CONFIG_USER_ONLY

#endif

    ExitStatus ret;

    /* Decode all instruction fields */
	bundle.bits = insn;
	bundle.num_insns = parse_insn_tilegx(bundle.bits, ctx->pc, bundle.insns);

	
	
   	/* TODO: log the insns to be disassembled */
    LOG_DISAS("\n",);

    ret = NO_EXIT;

	for (ins_num=0;ins_num<bundle.num_insns;ins_num++) {
		for (opr_num=0, src_reg_num=0, imm_num=0 ;opr_num<bundle.insns[ins_num].opcode->num_operands;opr_num++) {	
			if (bundle.insns[ins_num].operands[opr_num]->type = TILEGX_OP_TYPE_REGISTER) {
				if (bundle.insns[ins_num].operands[opr_num]->is_dest_reg) dest_reg = bundle.insns[ins_num].operand_values[opr_num];
				else if (bundle.insns[ins_num].operands[opr_num]->is_src_reg) src_reg[src_reg_num++]= bundle.insns[ins_num].operand_values[opr_num];
				else ;
			}

			else if (bundle.insns[ins_num].operands[opr_num]->type = TILEGX_OP_TYPE_IMMEDIATE) imm64[imm_num++] = bundle.insns[ins_num].operand_values[opr_num];
			else ;
		}	
		
		TCGv_i32 l_1 = tcg_temp_new_i32();
		TCGv_i32 l_2 = tcg_temp_new_i32();
		TCGv_i32 l_3 = tcg_temp_new_i32();
		TCGv_i32 l_4 = tcg_temp_new_i32();
		TCGv_i64 ll_1 = tcg_temp_new_i64();
		TCGv_i64 ll_2 = tcg_temp_new_i64();
		TCGv_i64 ll_3 = tcg_temp_new_i64();
		
	    switch (bundle.insns[ins_num].opcode->mnemonic) {
		case TILEGX_OPC_BPT:
		
			break;
		case TILEGX_OPC_INFO:
		
			break;
		case TILEGX_OPC_INFOL:
		
			break;
		case TILEGX_OPC_MOVE:
			tcg_gen_mov_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]);
			break;
		case TILEGX_OPC_MOVEI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_mov_i64(tile_gpr[dest_reg], ll_1);
		case TILEGX_OPC_MOVELI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend16(ll_1, ll_1);
			tcg_gen_mov_i64(tile_gpr[dest_reg], imm64[0]);
			break;
		case TILEGX_OPC_PREFETCH:
			
			break;
		case TILEGX_OPC_PREFETCH_ADD_L1:
		
			break;
		case TILEGX_OPC_PREFETCH_ADD_L1_FAULT:
		
			break;
		case TILEGX_OPC_PREFETCH_ADD_L2:
		
			break;
		case TILEGX_OPC_PREFETCH_ADD_L2_FAULT:
		
			break;
		case TILEGX_OPC_PREFETCH_ADD_L3:
		
			break;
		case TILEGX_OPC_PREFETCH_ADD_L3_FAULT:
		
			break;
		case TILEGX_OPC_PREFETCH_L1:
		
			break;
		case TILEGX_OPC_PREFETCH_L1_FAULT:
		
			break;
		case TILEGX_OPC_PREFETCH_L2:
		
			break;
		case TILEGX_OPC_PREFETCH_L2_FAULT:
		
			break;
		case TILEGX_OPC_PREFETCH_L3:
		
			break;
		case TILEGX_OPC_PREFETCH_L3_FAULT:
		
			break;
		case TILEGX_OPC_RAISE:
		
			break;
		case TILEGX_OPC_ADD:
			tcg_gen_add_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_ADDI:
		case TILEGX_OPC_ADDLI:
			tcg_gen_addi_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0]);
			break;
		case TILEGX_OPC_ADDX:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_trunc_i64_i32(l_2, tile_gpr[src_reg[1]]);
			tcg_gen_add_i32(l_3,l_1,l_2);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_3);
			break;
		case TILEGX_OPC_ADDXI:
		case TILEGX_OPC_ADDXLI:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_trunc_i64_i32(l_2, imm64[0]);
			tcg_gen_add_i32(l_3,l_1,l_2);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_3);
			break;
		case TILEGX_OPC_ADDXSC:
			tcg_gen_add_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			gen_saturate_64s_i32(l_3, tile_gpr[dest_reg]);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_3);
			break;
		case TILEGX_OPC_AND:
			tcg_gen_and_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_ANDI:
			tcg_gen_andi_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0]);
			break;
		case TILEGX_OPC_BEQZ:
			gen_bcond_internal(ctx, TCG_COND_EQ, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BEQZT:
			gen_bcond_internal(ctx, TCG_COND_EQ, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BFEXTS:
			gen_helper_bfexts(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0], imm64[1]);	
			break;
		case TILEGX_OPC_BFEXTU:
			gen_helper_bfextu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0], imm64[1]);
			break;
		case TILEGX_OPC_BFINS:
			gen_helper_bfins(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0], imm64[1]);
			break;
		case TILEGX_OPC_BGEZ:
			gen_bcond_internal(ctx, TCG_COND_GE, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BGEZT:
			gen_bcond_internal(ctx, TCG_COND_GE, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BGTZ:
			gen_bcond_internal(ctx, TCG_COND_GT, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BGTZT:
			gen_bcond_internal(ctx, TCG_COND_GT, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BLBC:
			tcg_gen_andi_i64(ll_1, tile_gpr[src_reg[0], 0x01);
			gen_bcond_internal(ctx, TCG_COND_EQ, ll_1, imm64[0]);
			break;
		case TILEGX_OPC_BLBCT:
			tcg_gen_andi_i64(ll_1, tile_gpr[src_reg[0], 0x01);
			gen_bcond_internal(ctx, TCG_COND_EQ, ll_1, imm64[0]);
			break;
		case TILEGX_OPC_BLBS:
			tcg_gen_andi_i64(ll_1, tile_gpr[src_reg[0], 0x01);
			gen_bcond_internal(ctx, TCG_COND_NE, ll_1, imm64[0]);
			break;
		case TILEGX_OPC_BLBST:
			tcg_gen_andi_i64(ll_1, tile_gpr[src_reg[0], 0x01);
			gen_bcond_internal(ctx, TCG_COND_NE, ll_1, imm64[0]);
			break;
		case TILEGX_OPC_BLEZ:
			gen_bcond_internal(ctx, TCG_COND_LE, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BLEZT:
			gen_bcond_internal(ctx, TCG_COND_LE, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BLTZ:
			gen_bcond_internal(ctx, TCG_COND_LT, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BLTZT:
			gen_bcond_internal(ctx, TCG_COND_LT, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BNEZ:
			gen_bcond_internal(ctx, TCG_COND_NE, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_BNEZT:
			gen_bcond_internal(ctx, TCG_COND_NE, tile_gpr[src_reg[0], imm64[0]);
			break;
		case TILEGX_OPC_CLZ:
			gen_helper_clz(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_CMOVEQZ:
			gen_helper_cmoveqz(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMOVNEZ:
			gen_helper_cmovnez(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMPEQ:
			gen_helper_cmpeq(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMPEQI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_cmpeqi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], );ll_1
			break;
		case TILEGX_OPC_CMPEXCH:
			
			break;
		case TILEGX_OPC_CMPEXCH4:
		
			break;
		case TILEGX_OPC_CMPLES:
			gen_helper_cmples(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMPLEU:
			gen_helper_cmpleu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMPLTS:
			gen_helper_cmplts(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMPLTSI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_cmpltsi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_CMPLTU:
			gen_helper_cmpltu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMPLTU:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_cmpltu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_CMPNE:
			gen_helper_cmpne(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMUL:
			gen_helper_cmul(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMULA:
			gen_helper_cmula(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMULAF:
			gen_helper_cmulaf(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMULF:
			gen_helper_cmulf(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMULFR:
			gen_helper_cmulfr(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMULH:
			gen_helper_cmulh(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CMULHR:
			gen_helper_cmulhr(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CRC32_32:
			gen_helper_crc32_32(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CRC32_8:
			gen_helper_crc32_8(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_CTZ:
			gen_helper_ctz(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_DBLALIGN:
			gen_helper_dblalign(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_DBLALIGN2:
			gen_helper_dblalign2(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_DBLALIGN4:
			gen_helper_dblalign4(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_DBLALIGN6:
			gen_helper_dblalign6(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_DRAIN:
		
			break;
		case TILEGX_OPC_DTLBPR:
		
			break;
		case TILEGX_OPC_EXCH:
		
			break;
		case TILEGX_OPC_EXCH4:
		
			break;
		case TILEGX_OPC_FDOUBLE_ADD_FLAGS:
		
			break;
		case TILEGX_OPC_FDOUBLE_ADDSUB:
		
			break;
		case TILEGX_OPC_FDOUBLE_MUL_FLAGS:
		
			break;
		case TILEGX_OPC_FDOUBLE_PACK1:
		
			break;
		case TILEGX_OPC_FDOUBLE_PACK2:
		
			break;
		case TILEGX_OPC_FDOUBLE_SUB_FLAGS:
		
			break;
		case TILEGX_OPC_FDOUBLE_UNPACK_MAX:
		
			break;
		case TILEGX_OPC_FDOUBLE_UNPACK_MIN:
		
			break;
		case TILEGX_OPC_FETCHADD:
		
			break;
		case TILEGX_OPC_FETCHADD4:
		
			break;
		case TILEGX_OPC_FETCHADDGEZ:
		
			break;
		case TILEGX_OPC_FETCHADDGEZ4:
		
			break;
		case TILEGX_OPC_FETCHAND:
		
			break;
		case TILEGX_OPC_FETCHAND4:
		
			break;
		case TILEGX_OPC_FETCHOR:
		
			break;
		case TILEGX_OPC_FETCHOR4:
		
			break;
		case TILEGX_OPC_FINV:
		
			break;
		case TILEGX_OPC_FLUSH:
		
			break;
		case TILEGX_OPC_FLUSHWB:
		
			break;
		case TILEGX_OPC_FNOP:
		
			break;
		case TILEGX_OPC_FSINGLE_ADD1:
		
			break;
		case TILEGX_OPC_FSINGLE_ADDSUB2:
		
			break;
		case TILEGX_OPC_FSINGLE_MUL1:
		
			break;
		case TILEGX_OPC_FSINGLE_MUL2:
		
			break;
		case TILEGX_OPC_FSINGLE_PACK1:
		
			break;
		case TILEGX_OPC_FSINGLE_PACK2:
		
			break;
		case TILEGX_OPC_FSINGLE_SUB1:
		
			break;
		case TILEGX_OPC_ICOH:
		
			break;
		case TILEGX_OPC_ILL:
		
			break;
		case TILEGX_OPC_INV:
		
			break;
		case TILEGX_OPC_IRET:
		
			break;
			/*TODO: analyze whether we need to define flags for prediction */
		case TILEGX_OPC_J:
			gen_bdirect(ctx, imm64[0]);
			break;
		case TILEGX_OPC_JAL:
			tcg_gen_movi_i64(tile_lr, ctx->pc + (INSTRUCTION_SIZE / BYTE_SIZE));
			gen_bdirect(ctx, imm64[0]);
			break;
		case TILEGX_OPC_JALR:
			tcg_gen_movi_i64(tile_lr, ctx->pc + (INSTRUCTION_SIZE / BYTE_SIZE));
			gen_bdirectr(ctx, tile_gpr[src_reg[0]);
			break;
		case TILEGX_OPC_JALRP:
			cg_gen_movi_i64(tile_lr, ctx->pc + (INSTRUCTION_SIZE / BYTE_SIZE));
			gen_bdirectr(ctx, tile_gpr[src_reg[0]);
			break;
		case TILEGX_OPC_JR:
			gen_bdirectr(ctx, tile_gpr[src_reg[0]);
			break;
		case TILEGX_OPC_JRP:
			gen_bdirectr(ctx, tile_gpr[src_reg[0]);
			break;
			/* TODO: generate interrupt for unaligned accesses */
		case TILEGX_OPC_LD:
		case TILEGX_OPC_LDNT:
			tcg_gen_ld_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LD1S:
		case TILEGX_OPC_LDNT1S:
			tcg_gen_ld8s_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LD1S_ADD:
		case TILEGX_OPC_LDNT1S_ADD:	
			tcg_gen_ld8s_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LD1U:
		case TILEGX_OPC_LDNT1U:	
			tcg_gen_ld8u_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LD1U_ADD:
		case TILEGX_OPC_LDNT1U_ADD:
			tcg_gen_ld8u_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LD2S:
		case TILEGX_OPC_LDNT2S:
			tcg_gen_ld16s_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LD2S_ADD:
		case TILEGX_OPC_LDNT2S_ADD:
			tcg_gen_ld16s_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LD2U:
		case TILEGX_OPC_LDNT2U:
			tcg_gen_ld16u_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LD2U_ADD:
		case TILEGX_OPC_LDNT2U_ADD:
			tcg_gen_ld16u_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LD4S:
		case TILEGX_OPC_LDNT4S:
			tcg_gen_ld32s_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LD4S_ADD:
		case TILEGX_OPC_LDNT4S_ADD:
			tcg_gen_ld32s_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LD4U:
		case TILEGX_OPC_LDNT4U:
			tcg_gen_ld32u_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LD4U_ADD:
		case TILEGX_OPC_LDNT4U_ADD:
			tcg_gen_ld32u_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LD_ADD:
		case TILEGX_OPC_LDNT_ADD:
			tcg_gen_ld_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LDNA:
			tcg_gen_shli_i64(tile_gpr[src_reg[0], tile_gpr[src_reg[0], 3);
			tcg_gen_ld_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_LDNA_ADD:
			tcg_gen_shli_i64(tile_gpr[src_reg[0], tile_gpr[src_reg[0], 3);
			tcg_gen_ld_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_LNK:
		
			break;
		case TILEGX_OPC_MF:
		
			break;
		case TILEGX_OPC_MFSPR:
			tcg_gen_mov_i64(tile_gpr[src_reg[0]], tile_spr[spr_index(imm64[0])]);
			break;
		case TILEGX_OPC_MM:
			gen_helper_mm(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0], imm64[1]);
			break;
		case TILEGX_OPC_MNZ:
			gen_helper_mnz(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MTSPR:
			tcg_gen_mov_i64(tile_spr[spr_num[imm64[0]]], tile_gpr[src_reg[0]] );
			break;
		case TILEGX_OPC_MUL_HS_HS:
			gen_helper_mul_hs_hs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_HS_HU:
			gen_helper_mul_hs_hu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_HS_LS:
			gen_helper_mul_hs_ls(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_HS_LU:
			gen_helper_mul_hs_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_HU_HU:
			gen_helper_mul_hu_hu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_HU_LS:
			gen_helper_mul_hu_ls(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_HU_LU:
			gen_helper_mul_hu_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_LS_LS:
			gen_helper_mul_ls_ls(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_LS_LU:
			gen_helper_mul_ls_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MUL_LU_LU:
			gen_helper_mul_lu_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_HS_HS:
			gen_helper_mula_hs_hs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_HS_HU:
			gen_helper_mula_hs_hu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_HS_LS:
			gen_helper_mula_hs_ls(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_HS_LU:
			gen_helper_mula_hs_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_HU_HU:
			gen_helper_mula_hs_hu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_HU_LS:
			gen_helper_mula_hu_ls(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_HU_LU:
			gen_helper_mula_hu_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_LS_LS:
			gen_helper_mula_ls_ls(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_LS_LU:
			gen_helper_mula_ls_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULA_LU_LU:
			gen_helper_mula_lu_lu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULAX:
			gen_helper_mulax(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MULX:
			gen_helper_mulx(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_MZ:
			gen_helper_mz(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_NAP:
		
			break;
		case TILEGX_OPC_NOP:
		
			break;
		case TILEGX_OPC_NOR:
			tcg_gen_nor_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_OR:
			tcg_gen_or_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_ORI:
			tcg_gen_ori_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0]]);
			break;
		case TILEGX_OPC_PCNT:
			gen_helper_pcnt(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_REVBITS:
			gen_helper_revbits(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_REVBYTES:
			gen_helper_revbytes(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_ROTL:
			tcg_gen_andi_i64(tile_gpr[src_reg[1]], tile_gpr[src_reg[1]], WORD_SIZE-1);
			tcg_gen_rotl_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_ROTLI:
			tcg_gen_rotli_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0]&(WORD_SIZE-1));
			break;
		case TILEGX_OPC_SHL:
			tcg_gen_andi_i64(ll_1, tile_gpr[src_reg[1]], WORD_SIZE-1);
			tcg_gen_shl_i64(tile_gpr[dest_reg],tile_gpr[src_reg[0]],ll_1);
			break;
		case TILEGX_OPC_SHL16INSLI:
			tcg_gen_shli_i64(ll_1,tile_gpr[src_reg[0]],16);
			tcg_gen_ori_i64(tile_gpr[dest_reg],ll_1,imm64[0]&MASK16);			
			break;
		case TILEGX_OPC_SHL1ADD:
			tcg_gen_shli_i64(ll_1,tile_gpr[src_reg[0]],(int64_t) 1);
			tcg_gen_add_i64(tile_gpr[dest_reg],ll_1,tile_gpr[src_reg[1]);
			break;
		case TILEGX_OPC_SHL1ADDX:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_trunc_i64_i32(l_2, tile_gpr[src_reg[1]]);
			tcg_gen_shli_i32(l_3,l_1, 1);
			tcg_gen_add_i32(l_4,l_3,l_2);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_4);
			break;
		case TILEGX_OPC_SHL2ADD:
			tcg_gen_shli_i64(ll_1,tile_gpr[src_reg[0]], 2);
			tcg_gen_add_i64(tile_gpr[dest_reg],ll_1,tile_gpr[src_reg[1]);
			break;
		case TILEGX_OPC_SHL2ADDX:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_trunc_i64_i32(l_2, tile_gpr[src_reg[1]]);
			tcg_gen_shli_i32(l_3,l_1, 2);
			tcg_gen_add_i32(l_4,l_3,l_2);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_4);
			break;
		case TILEGX_OPC_SHL3ADD:
			tcg_gen_shli_i64(ll_1,tile_gpr[src_reg[0]], 3);
			tcg_gen_add_i64(tile_gpr[dest_reg],ll_1,tile_gpr[src_reg[1]);
			break;
		case TILEGX_OPC_SHL3ADDX:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_trunc_i64_i32(l_2, tile_gpr[src_reg[1]]);
			tcg_gen_shli_i32(l_3,l_1, 3);
			tcg_gen_add_i32(l_4,l_3,l_2);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_4);
			break;
		case TILEGX_OPC_SHLI:
			tcg_gen_shli_i64(tile_gpr[dest_reg],tile_gpr[src_reg[0]],imm64[0]%WORD_SIZE);
			break;
		case TILEGX_OPC_SHLX:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_trunc_i64_i32(l_2, tile_gpr[src_reg[1]]);
			tcg_gen_andi_i32(l_2, l_2, 31);
			tcg_gen_shli_i32(l_3, l_1, l_2);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_3);
			break;
		case TILEGX_OPC_SHLXI:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_shli_i32(l_3, l_1 , imm64[0]&31);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_3);
			break;
		case TILEGX_OPC_SHRS:
			tcg_gen_andi_i64(ll_1, tile_gpr[src_reg[1]], WORD_SIZE-1);
			tcg_gen_shr_i64(tile_gpr[dest_reg],tile_gpr[src_reg[0]],ll_1);
			break;
		case TILEGX_OPC_SHRSI:
			tcg_gen_shri_i64(tile_gpr[dest_reg],tile_gpr[src_reg[0]],imm64[0]%WORD_SIZE);
			break;
		case TILEGX_OPC_SHRU: 
			gen_helper_shru(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_SHRUI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_shrui(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_SHRUX:
			gen_helper_shrux(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_SHRUXI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_shruxi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);			
			break;
		case TILEGX_OPC_SHUFFLEBYTES:
			gen_helper_shufflebytes(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_ST:
		case TILEGX_OPC_STNT:
			tcg_gen_st_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_ST1:
		case TILEGX_OPC_STNT1:
			tcg_gen_st8_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_ST1_ADD:
		case TILEGX_OPC_STNT1_ADD:	
			tcg_gen_st8_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_ST2:
		case TILEGX_OPC_STNT2:	
			tcg_gen_st16_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_ST2_ADD:
		case TILEGX_OPC_STNT2_ADD:	
			tcg_gen_st16_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_ST4:
		case TILEGX_OPC_STNT4:
			tcg_gen_st32_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			break;
		case TILEGX_OPC_ST4_ADD:
		case TILEGX_OPC_STNT4_ADD:
			tcg_gen_st32_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_ST_ADD:
		case TILEGX_OPC_STNT_ADD:
			tcg_gen_st_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], 0);
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_sign_extend8(ll_1, ll_1);
			tcg_gen_add_i64(tile_gpr[src_reg[0], ll_1);
			break;
		case TILEGX_OPC_SUB:
			tcg_gen_sub_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_SUBX:
			tcg_gen_trunc_i64_i32(l_1, tile_gpr[src_reg[0]]);
			tcg_gen_trunc_i64_i32(l_2, tile_gpr[src_reg[1]]);
			tcg_gen_sub_i32(l_3,l_1,l_2);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_3);
			break;
		case TILEGX_OPC_SUBXSC:
			tcg_gen_sub_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			gen_saturate_64s_i32(l_3, tile_gpr[dest_reg]);
			tcg_gen_ext32s_i64(tile_gpr[dest_reg],(TCGv_i64) l_3);
			break;
		case TILEGX_OPC_SWINT0:
		
			break;
		case TILEGX_OPC_SWINT1:
		
			break;
		case TILEGX_OPC_SWINT2:
		
			break;
		case TILEGX_OPC_SWINT3:
		
			break;
		case TILEGX_OPC_TBLIDXB0:
			gen_helper_tblidxb0(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_TBLIDXB1:
			gen_helper_tblidxb1(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_TBLIDXB2:
			gen_helper_tblidxb2(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_TBLIDXB3:
			gen_helper_tblidxb3(tile_gpr[dest_reg], tile_gpr[src_reg[0]]);
			break;
		case TILEGX_OPC_V1ADD:
			gen_helper_v1add(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1ADDI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1addi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1ADDUC:
			gen_helper_v1adduc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1ADIFFU:
			gen_helper_v1adiffu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1AVGU:
			gen_helper_v1avgu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1CMPEQ:
			gen_helper_v1cmpeq(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1CMPEQI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1cmpeqi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1CMPLES:
			gen_helper_v1cmples(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1CMPLEU:
			gen_helper_v1cmpleu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1CMPLTS:
			gen_helper_v1cmplts(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1CMPLTSI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1cmpltsi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1CMPLTU:
			gen_helper_v1cmpltu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1CMPLTUI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1cmpltui(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1CMPNE:
			gen_helper_v1cmpne(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DDOTPU:
			gen_helper_v1ddotpu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DDOTPUA:
			gen_helper_v1ddotpua(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DDOTPUS:
			gen_helper_v1ddotpus(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DDOTPUSA:
			gen_helper_v1ddotpusa(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DOTP:
			gen_helper_v1dotp(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DOTPA:
			gen_helper_v1dotpa(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DOTPU:
			gen_helper_v1dotpu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DOTPUA:
			gen_helper_v1dotpua(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DOTPUS:
			gen_helper_v1dotpus(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1DOTPUSA:
			gen_helper_v1dotpusa(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1INT_H:
			gen_helper_v1int_h(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1INT_L:
			gen_helper_v1int_l(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1MAXU:
			gen_helper_v1maxu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1MAXUI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1maxui(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1MINU:
			gen_helper_v1minu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1MINUI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1minui(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1MNZ:
			gen_helper_v1mnz(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1MULTU:
			gen_helper_v1multu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1MULU:
			gen_helper_v1mulu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1MULUS:
			gen_helper_v1mulus(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1MZ:
			gen_helper_v1mz(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1SADAU:
			gen_helper_v1sadau(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1SADU:
			gen_helper_v1sadu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1SHL:
			gen_helper_v1shl(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1SHLI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1shli(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1SHRS:
			gen_helper_v1shrs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1SHRSI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1shrsi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1SHRU:
			gen_helper_v1shru(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1SHRUI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v1shrui(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V1SUB:
			gen_helper_v1sub(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V1SUBUC:
			gen_helper_v1subuc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2ADD:
			gen_helper_v2add(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2ADDI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2addi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2ADDSC:
			gen_helper_v2addsc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2ADIFFS:
			gen_helper_v2adiffs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2AVGS:
			gen_helper_v2avgs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2CMPEQ:
			gen_helper_v2cmpeq(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2CMPEQI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2cmpeqi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2CMPLES:
			gen_helper_v2cmples(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2CMPLEU:
			gen_helper_v2cmpleu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2CMPLTS:
			gen_helper_v2cmplts(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2CMPLTSI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2cmpltsi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2CMPLTU:
			gen_helper_v2cmpltu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2CMPLTUI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2cmpltui(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2CMPNE:
			gen_helper_v2cmpne(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2DOTP:
			gen_helper_v2dotp(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2DOTPA:
			gen_helper_v2dotpa(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2INT_H:
			gen_helper_v2int_h(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2INT_L:
			gen_helper_v2int_l(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2MAXS:
			gen_helper_v2maxs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2MAXSI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2maxsi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2MINS:
			gen_helper_v2mins(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2MINSI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2minsi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2MNZ:
			gen_helper_v2mnz(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2MULFSC:
			gen_helper_v2mulfc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2MULS:
			gen_helper_v2muls(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2MULTS:
			gen_helper_v2mults(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2MZ:
			gen_helper_v2mz(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2PACKH:
			gen_helper_v2packh(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2PACKL:
			gen_helper_v2packl(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2PACKUC:
			gen_helper_v2packuc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SADAS:
			gen_helper_v2sadas(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SADAU:
			gen_helper_v2sadau(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SADS:
			gen_helper_v2sads(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SADU:
			gen_helper_v2sadu(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SHL:
			gen_helper_v2shl(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SHLI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2shli(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2SHLSC:
			gen_helper_v2shlsc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SHRS:
			gen_helper_v2shrs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SHRSI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2shrsi(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2SHRU:
			gen_helper_v2shru(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SHRUI:
			tcg_gen_movi_i64(ll_1, imm64[0]);
			gen_helper_v2shrui(tile_gpr[dest_reg], tile_gpr[src_reg[0]], ll_1);
			break;
		case TILEGX_OPC_V2SUB:
			gen_helper_v2sub(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V2SUBSC:
			gen_helper_v2subsc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4ADD:
			gen_helper_v4add(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4ADDSC:
			gen_helper_v4addsc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4INT_H:
			gen_helper_v4int_h(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4INT_L:
			gen_helper_v4int_l(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4PACKSC:
			gen_helper_v4packsc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4SHL:
			gen_helper_v4shl(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4SHLSC:
			gen_helper_v4shlsc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4SHRS:
			gen_helper_v4shrs(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4SHRU:
			gen_helper_v4shru(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4SUB:
			gen_helper_v4sub(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_V4SUBSC:
			gen_helper_v4subsc(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_WH64:
		
			break;
		case TILEGX_OPC_XOR:
			tcg_gen_xor_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], tile_gpr[src_reg[1]]);
			break;
		case TILEGX_OPC_XORI:
			tcg_gen_xori_i64(tile_gpr[dest_reg], tile_gpr[src_reg[0]], imm64[0]);
			break;
		case TILEGX_OPC_NONE:
			break;
				
	    invalid_opc:
	        ret = gen_invalid(ctx);
	        break;
	    }

		tcg_temp_free_i32(l_1);
		tcg_temp_free_i32(l_2);
		tcg_temp_free_i32(l_3);
		tcg_temp_free_i32(l_4);
		tcg_temp_free_i64(ll_1);
		tcg_temp_free_i64(ll_2);
		tcg_temp_free_i64(ll_3);
		
	}
	return ret;
}

static inline void gen_intermediate_code_internal(CPUTileraState *env,
                                                  TranslationBlock *tb,
                                                  int search_pc)
{
    DisasContext ctx, *ctxp = &ctx;
    target_ulong pc_start;
    uint64_t insn;
    uint16_t *gen_opc_end;
    CPUBreakpoint *bp;
    int j, lj = -1;
    ExitStatus ret;
    int num_insns;
    int max_insns;

    pc_start = tb->pc;
    gen_opc_end = tcg_ctx.gen_opc_buf + OPC_MAX_SIZE;

    ctx.tb = tb;
    ctx.env = env;
    ctx.pc = pc_start;
    ctx.mem_idx = cpu_mmu_index(env);

    num_insns = 0;
    max_insns = tb->cflags & CF_COUNT_MASK;
    if (max_insns == 0)
        max_insns = CF_COUNT_MASK;

    gen_icount_start();
    do {
        if (unlikely(!QTAILQ_EMPTY(&env->breakpoints))) {
            QTAILQ_FOREACH(bp, &env->breakpoints, entry) {
                if (bp->pc == ctx.pc) {
                    gen_excp(&ctx, EXCP_DEBUG, 0);
                    break;
                }
            }
        }
        if (search_pc) {
            j = tcg_ctx.gen_opc_ptr - tcg_ctx.gen_opc_buf;
            if (lj < j) {
                lj++;
                while (lj < j)
                    gen_opc_instr_start[lj++] = 0;
            }
            gen_opc_pc[lj] = ctx.pc;
            gen_opc_instr_start[lj] = 1;
            gen_opc_icount[lj] = num_insns;
        }
        if (num_insns + 1 == max_insns && (tb->cflags & CF_LAST_IO))
            gen_io_start();
        insn = cpu_ldl_code(env, ctx.pc);
        num_insns++;

	if (unlikely(qemu_loglevel_mask(CPU_LOG_TB_OP | CPU_LOG_TB_OP_OPT))) {
            tcg_gen_debug_insn_start(ctx.pc);
        }

        ctx.pc += 4;
        ret = translate_one(ctxp, insn);

        /* If we reach a page boundary, are single stepping,
           or exhaust instruction count, stop generation.  */
        if (ret == NO_EXIT
            && ((ctx.pc & (TARGET_PAGE_SIZE - 1)) == 0
                || tcg_ctx.gen_opc_ptr >= gen_opc_end
                || num_insns >= max_insns
                || singlestep
                || env->singlestep_enabled)) {
            ret = EXIT_PC_STALE;
        }
    } while (ret == NO_EXIT);

    if (tb->cflags & CF_LAST_IO) {
        gen_io_end();
    }

    switch (ret) {
    case EXIT_GOTO_TB:
    case EXIT_NORETURN:
        break;
    case EXIT_PC_STALE:
        tcg_gen_movi_i64(cpu_pc, ctx.pc);
        /* FALLTHRU */
    case EXIT_PC_UPDATED:
        if (env->singlestep_enabled) {
            gen_excp_1(EXCP_DEBUG, 0);
        } else {
            tcg_gen_exit_tb(0);
        }
        break;
    default:
        abort();
    }

    gen_icount_end(tb, num_insns);
    *tcg_ctx.gen_opc_ptr = INDEX_op_end;
    if (search_pc) {
        j = tcg_ctx.gen_opc_ptr - tcg_ctx.gen_opc_buf;
        lj++;
        while (lj <= j)
            gen_opc_instr_start[lj++] = 0;
    } else {
        tb->size = ctx.pc - pc_start;
        tb->icount = num_insns;
    }

#ifdef DEBUG_DISAS
    if (qemu_loglevel_mask(CPU_LOG_TB_IN_ASM)) {
        qemu_log("IN: %s\n", lookup_symbol(pc_start));
        log_target_disas(env, pc_start, ctx.pc - pc_start, 1);
        qemu_log("\n");
    }
#endif
}

void gen_intermediate_code (CPUTileraState *env, struct TranslationBlock *tb)
{
    gen_intermediate_code_internal(env, tb, 0);
}

void gen_intermediate_code_pc (CPUTileraState *env, struct TranslationBlock *tb)
{
    gen_intermediate_code_internal(env, tb, 1);
}

/*	TODO? use for defining tilera processor packages 
struct cpu_def_t {
    const char *name;
    int implver, amask;
};

static const struct cpu_def_t cpu_defs[] = {
    
}; */

CPUTileraState * cpu_tilera_init (const char *cpu_model)
{
    TileraCPU *cpu;
    CPUTileraState *env;

    cpu = TILERA_CPU(object_new(TYPE_TILERA_CPU));
    env = &cpu->env;

    tilera_translate_init();

    qemu_init_vcpu(env);
    return env;
}

void restore_state_to_opc(CPUTileraState *env, TranslationBlock *tb, int pc_pos)
{
    env->pc = gen_opc_pc[pc_pos];
}

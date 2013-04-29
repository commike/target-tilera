/*
 * QEMU Tilera CPU
 *
 * Copyright (c) 2013 Sarmad Tanwir
 * Copyright (c) 2013 System Software Research Group, ECE, Virginia Tech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/lgpl-2.1.html>
 */

#include "cpu.h"
#include "qemu-common.h"


static void tilera_cpu_initfn(Object *obj)
{
    TileraCPU *cpu = TILERA_CPU(obj);
    CPUTileraState *env = &cpu->env;

    cpu_exec_init(env);
    tlb_flush(env, 1);

}

static const TypeInfo tilera_cpu_type_info = {
    .name = TYPE_TILERA_CPU,
    .parent = TYPE_CPU,
    .instance_size = sizeof(TileraCPU),
    .instance_init = tilera_cpu_initfn,
    .abstract = false,
    .class_size = sizeof(TileraCPUClass),
};

static void tilera_cpu_register_types(void)
{
    type_register_static(&tilera_cpu_type_info);
}

type_init(tilera_cpu_register_types)

/* returns spr_index (serial no) against given address in spr file */
uint16_t spr_index (uint16_t addr)
{
	int i;
	for (i = 0; i < SPR_MAX; i++)
		if (spr_addr[i] == addr) return i;
	return SPR_MAX;
}

tilegx_tlb_t tilegx_tlb;

void do_interrupt (CPUTileraState *env)
{
    env->exception_index = -1;
	env->error_code = 0;
}

void cpu_dump_state (CPUTileraState *env, FILE *f, fprintf_function cpu_fprintf,
                     int flags)
{
    
    int i;
	
    for (i = 0; i < 52; i++) {
        cpu_fprintf(f, "gpr[%02d] " TARGET_FMT_lx " ", i, env->gpr[i]);
        if ((i % 3) == 0)
            cpu_fprintf(f, "\n");
    }

	cpu_fprintf(f, "      FP  " TARGET_FMT_lx "      TP  " TARGET_FMT_lx "\n", 
					env->gpr[TILE_REG_FP], env->gpr[TILE_REG_TP]);

	cpu_fprintf(f, "      SP  " TARGET_FMT_lx "      LR  " TARGET_FMT_lx "\n", 
					env->gpr[TILE_REG_SP], env->gpr[TILE_REG_LR]);

	cpu_fprintf(f, "      SN  " TARGET_FMT_lx "      PC  " TARGET_FMT_lx "\n", 
					env->sn, env->pc);

 	cpu_fprintf(f, "    IDN0  " TARGET_FMT_lx "    IDN1  " TARGET_FMT_lx "\n", 
					env->idn0, env->idn1);

	cpu_fprintf(f, "    UDN0  " TARGET_FMT_lx "    UDN1  " TARGET_FMT_lx "\n", 
					env->udn0, env->udn1);	

	cpu_fprintf(f, "    UDN2  " TARGET_FMT_lx "    UDN3  " TARGET_FMT_lx "\n", 
					env->udn2, env->udn3);

	
    cpu_fprintf(f, "\n");
}

void do_restore_state(CPUTileraState *env, uintptr_t retaddr)
{
    if (retaddr) {
        TranslationBlock *tb = tb_find_pc(retaddr);
        if (tb) {
            cpu_restore_state(tb, env, retaddr);
        }
    }
}

hwaddr cpu_get_phys_page_debug(CPUTileraState *env, target_ulong addr)
{
    target_ulong phys;
    /*int prot, fail;

    fail = get_physical_address(env, addr, 0, 0, &phys, &prot);
    return (fail >= 0 ? -1 : phys); */
    return phys;
}

int cpu_tilera_handle_mmu_fault(CPUTileraState *env, target_ulong addr, int rw,
                               int mmu_idx)
{
    /*target_ulong phys;
    int prot, fail;

    fail = get_physical_address(env, addr, 1 << rw, mmu_idx, &phys, &prot);
    if (unlikely(fail >= 0)) {
        env->exception_index = EXCP_MMFAULT;
        env->trap_arg0 = addr;
        env->trap_arg1 = fail;
        env->trap_arg2 = (rw == 2 ? -1 : rw);
        return 1;
    }

    tlb_set_page(env, addr & TARGET_PAGE_MASK, phys & TARGET_PAGE_MASK,
                 prot, mmu_idx, TARGET_PAGE_SIZE);*/
    return 0;
}

inline int little_endian (CPUTileraState *env)
{
	if (env->spr_names.big_endian_config.big_endian_config) return 0;
	else return 1;
}

/* This may be called from any of the helpers to set up EXCEPTION_INDEX.  */
void QEMU_NORETURN dynamic_excp(CPUTileraState *env, uintptr_t retaddr,
                                int excp, int error)
{
    env->exception_index = excp;
    env->error_code = error;
    do_restore_state(env, retaddr);
    cpu_loop_exit(env);
}






/*
 *  Tilera emulation cpu helpers for qemu.
 *
 *  Copyright (c) 2013 Sarmad Tanwir
 *  Copyright (c) 2013 System Software Research Group, ECE, Virginia Tech
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

/*#include "def-helper.h" */
#ifndef __tilegx__	/* Tile64 and TilePro  */

#define WORD_SIZE 32 						/*The size of a machine word in bits. The 
											TILE-Gx Processor is a 64-bit machine.*/

#define WORD_MASK 0xFFFFffff 				/*A mask to represent all of the bits in a word. */

#define WORD_ADDR_MASK 0xFFFFfff8 			/*A mask that represents the portion of an 
											address that forms a word aligned mask.*/

#define HALF_WORD_SIZE 16					/*The size of half of a machine word in bits. The Tile 
											Processor is a 32-bit machine thus half the word 
											length is 16.*/

#define	HALF_WORD_ADDR_MASK 0xFFFFfffe 		/*A mask that represents the portion of an address 
											that forms a half word aligned mask.*/


#define BYTE_SIZE 8 						/*The number of bits in a byte.*/

#define BYTE_SIZE_LOG_2 3 					/*The logarithm base 2 of the number of bits in a byte.*/

#define BYTE_MASK 0xFF 						/*A mask to represent all of the bits in a byte.*/

#define INSTRUCTION_SIZE 64 				/*The length in bits of an instruction (bundle) 
											in the TILE-Gx Processor architecture.*/

#define INSTRUCTION_SIZE_LOG_2 6 			/*The logarithm base 2 of the length in bits of 
											an instruction (bundle) in the TILE-Gx Processor. */

#define ALIGNED_INSTRUCTION_MASK 0xFFFFfff8	/*A mask that selects the relevant bits for the 
											address of an aligned instruction.*/

#define BYTE_16_ADDR_MASK 0xFFFFfff0 		/*A mask that represents the portion of an 
											address that forms a 16-byte aligned block*/

#define ZERO_REGISTER 63 					/*The ZERO_REGISTER always reads as 0, and 
											ignores all writes.*/

#define NUMBER_OF_REGISTERS 64 				/*The number of architecturally visible general 
											purpose registers in the main processor.*/

#define LINK_REGISTER 55 					/*The LINK_REGISTER is used as an implicit 
											destination for some control instructions.*/

#define EX_CONTEXT_SPRF_OFFSET 				/*The starting SPR address of the interrupt 
											context save blocks. The save blocks are 
											indexed by protection level of the interrupt 
											handler being invoked.*/

#define EX_CONTEXT_SIZE 					/*The length of the interrupt context save 
											block.*/

#define PC_EX_CONTEXT_OFFSET 				/*The register offset of the saved PC in the 
											interrupt save context block.*/

#define PROTECTION_LEVEL_EX_CONTEXT_OFFSET 	/*The register offset of the saved protection 
											level in the interrupt save context block.*/

#define INTERRUPT_MASK_EX_CONTEXT_OFFSET 	/*The register offset of the saved interrupt 
											mask in the interrupt save context block.*/	

#else

#define WORD_SIZE 64 						/*The size of a machine word in bits. The 
											TILE-Gx Processor is a 64-bit machine.*/

#define WORD_MASK 0xFFFFffffFFFFffff 		/*A mask to represent all of the bits in a word. */

#define WORD_ADDR_MASK 0xFFFFffffFFFFfff8 	/*A mask that represents the portion of an 
											address that forms a word aligned mask.*/

#define BYTE_SIZE 8 						/*The number of bits in a byte.*/

#define BYTE_SIZE_LOG_2 3 					/*The logarithm base 2 of the number of bits in a byte.*/

#define BYTE_MASK 0xFF 						/*A mask to represent all of the bits in a byte.*/

#define INSTRUCTION_SIZE 64 				/*The length in bits of an instruction (bundle) 
											in the TILE-Gx Processor architecture.*/

#define INSTRUCTION_SIZE_LOG_2 6 			/*The logarithm base 2 of the length in bits of 
											an instruction (bundle) in the TILE-Gx Processor. */

#define ALIGNED_INSTRUCTION_MASK 0xFFFFffffFFFFfff8		/*A mask that selects the relevant bits for the 
														address of an aligned instruction.*/

#define BYTE_16_ADDR_MASK 0xFFFFffffFFFFfff0 			/*A mask that represents the portion of an 
														address that forms a 16-byte aligned block*/

#define ZERO_REGISTER 63 					/*The ZERO_REGISTER always reads as 0, and 
											ignores all writes.*/

#define NUMBER_OF_REGISTERS 64 				/*The number of architecturally visible general 
											purpose registers in the main processor.*/

#define LINK_REGISTER 55 					/*The LINK_REGISTER is used as an implicit 
											destination for some control instructions.*/

#define EX_CONTEXT_SPRF_OFFSET 				/*The starting SPR address of the interrupt 
											context save blocks. The save blocks are 
											indexed by protection level of the interrupt 
											handler being invoked.*/

#define EX_CONTEXT_SIZE 					/*The length of the interrupt context save 
											block.*/

#define PC_EX_CONTEXT_OFFSET 				/*The register offset of the saved PC in the 
											interrupt save context block.*/

#define PROTECTION_LEVEL_EX_CONTEXT_OFFSET 	/*The register offset of the saved protection 
											level in the interrupt save context block.*/

#define INTERRUPT_MASK_EX_CONTEXT_OFFSET 	/*The register offset of the saved interrupt 
											mask in the interrupt save context block.*/											
											
#endif

inline int64_t sign_extend(int64_t n, int num_bits);

inline int64_t sign_extend_1(int64_t n);

inline int64_t sign_extend_8(int64_t n);

inline int64_t sign_extend_16(int64_t n);

inline int64_t sign_extend_17(int64_t n);

inline int64_t sign_extend_32(int64_t n);

inline unsigned char unsigned_saturate8(int64_t n);

inline char signed_saturate8 (int64_t n);

inline short signed_saturate16 (int64_t n);

inline int signed_saturate32 (int64_t n);

inline int get_byte (int64_t word, int num);

inline int64_t set_byte(int64_t word, int num, int val);

inline int get_2_byte (int64_t word, int num);

inline int64_t set_2_byte(int64_t word, int num, int val);

inline int get_4_byte (int64_t word, int num);

inline int64_t set_4_byte(int64_t word, int num, int val);

DEF_HELPER_2(excp, void, env, i32)
DEF_HELPER_FLAGS_1(sign_extend8 , TCG_CALL_NO_RWG_SE , i64, i64)
DEF_HELPER_FLAGS_1(sign_extend16 , TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_3(bfexts, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(bfextu, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(bfins, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmoveqz, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmovnez, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(mm, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmnz, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmz, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(shru, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(shrui , TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(shrux, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(shruxi , TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_1(tblidxb0 , TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(tblidxb1 , TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(tblidxb2 , TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(tblidxb3 , TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(clz, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_2(crc32_32, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(crc32_8, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_1(ctz, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_3(dblalign, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(dblalign2, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(dblalign4, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(dblalign6, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_1(pcnt, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(revbits, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(revbytes, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_2(shufflebytes, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmpeq, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmpeqi , TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmples, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmpleu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmplts, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmpltsi , TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmpltu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmpltui , TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmpne, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmul, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(cmula , TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(cmulaf , TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmulf, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmulfr, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmulh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(cmulhr, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_hs_hs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_hs_hu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_hs_ls, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_hs_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_hu_hu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_hu_ls, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_hu_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_ls_ls, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_ls_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mul_lu_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_hs_hs, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_hs_hu, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_hs_ls, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_hs_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_hu_hu, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_hu_ls, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_hu_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_ls_ls, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_ls_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mula_lu_lu, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mulax, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(mulx, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1add, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1addi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1adduc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1adiffu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1avgu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmpeq, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmpeqi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmples, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmpleu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmplts, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmpltsi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmpltu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmpltui, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1cmpne, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1ddotpu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(v1ddotpua, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1ddotpus, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(v1ddotpusa, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1dotp, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(v1dotpa, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1dotpu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(v1dotpua, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1dotpus, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(v1dotpusa, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1int_h, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1int_l, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1maxu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1maxui, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1minu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1minui, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1mnz, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1multu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1mulu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1mulus, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1mz, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(v1sadau, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1sadu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shl, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shli, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shrs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shrsi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shru, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shrui, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1sub, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1subuc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2add, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2addi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2addsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2adiffs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2avgs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmpeq, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmpeqi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmples, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmpleu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmplts, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmpltsi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmpltu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmpltui, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2cmpne, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2dotp, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2dotpa, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2int_h, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2int_l, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2maxs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2maxsi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2mins, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2minsi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2mnz, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2mulfsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2muls, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2mults, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2mz, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2packh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2packl, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2packuc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_3(v2sadas, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(v2sadau, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2sads, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2sadu, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2shl, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2shli, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2shlsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2shrs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2shrsi, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2shru, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2shrui, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2sub, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v2subsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4add, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4addsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4int_h, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4int_l, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4packsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4shl, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4shlsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4shrs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4shru, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4sub, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v4subsc, TCG_CALL_NO_RWG_SE, i64, i64, i64)


#if !defined (CONFIG_USER_ONLY)

#endif

/*#include "def-helper.h"*/

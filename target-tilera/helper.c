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


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/*#include "cpu.h"
#include "softfloat.h"*/
#include "helper.h"

#define bool int

inline int64_t sign_extend(int64_t n, int num_bits)
{
  int shift = (int)(sizeof(int64_t) * 8 - num_bits);
  return (n << shift) >> shift;
}

inline int64_t sign_extend_1(int64_t n)
{
	return sign_extend(n,1);
}

inline int64_t sign_extend_8(int64_t n)
{
	return sign_extend(n,8);
}

inline int64_t sign_extend_16(int64_t n)
{
	return sign_extend(n,16);
}

inline int64_t sign_extend_17(int64_t n)
{
	return sign_extend(n,17); 
}

inline int64_t sign_extend_32(int64_t n)
{
	return sign_extend(n,32);
}

inline unsigned char unsigned_saturate8 (int64_t n)
{
	if (n > UCHAR_MAX) return UCHAR_MAX;
	else if (n < 0) return 0;
	else return n;
}

inline char signed_saturate8 (int64_t n)
{
	if (n > SCHAR_MAX) return SCHAR_MAX;
	else if (n < SCHAR_MIN) return SCHAR_MIN;
	else return n;
}


inline short signed_saturate16 (int64_t n)
{
	if (n > SHRT_MAX) return SHRT_MAX;
	else if (n < SHRT_MIN) return SHRT_MIN;
	else return n;
}

inline int signed_saturate32 (int64_t n)
{
	if (n > INT_MAX) return INT_MAX;
	else if (n < INT_MIN) return INT_MIN;
	else return n;
}

inline int get_byte (int64_t word, int num)
{
	return (word&(0x00000000000000ffULL<<(8*num)))>>(8*num);
}

inline int64_t set_byte(int64_t word, int num, int val)
{
	return (word&~(0x00000000000000ffULL<<(8*num)))|((uint64_t) val<<(8*num));
}

inline int get_2_byte (int64_t word, int num)
{
	return (word&(0x00000000000000ffffULL<<(16*num)))>>(16*num);
}

inline int64_t set_2_byte(int64_t word, int num, int val)
{
	return (word&~(0x000000000000ffffULL<<(16*num)))|((uint64_t) val<<(16*num));
}

inline int get_4_byte (int64_t word, int num)
{
	return (word&(0x00000000FFFFffffULL<<(32*num)))>>(32*num);
}

inline int64_t set_4_byte(int64_t word, int num, int64_t val)
{
	return (word&~(0x00000000FFFFffffULL<<(32*num)))|((uint64_t) val<<(32*num));
}

int64_t helper_sign_extend8 (uint64_t src_a)
{
	sign_extend_8(src_a);
}

int64_t helper_sign_extend16(uint64_t src_a)
{
	sign_extend_16(src_a);
}

/* logical instructions */
uint64_t helper_bfexts (uint64_t src_a, uint64_t bfstart, uint64_t bfend)
{
	uint64_t dest;
	uint64_t mask = 0;
	int64_t background = ((src_a >> bfend) & 1) ? -1ULL : 0ULL;
	mask = ((-1ULL) ^ ((-1ULL << ((bfend - bfstart) & 63)) << 1));
	uint64_t rot_src =
	  (((uint64_t) src_a) >> bfstart) | (src_a << (64 - bfstart));
	dest = (rot_src & mask) | (background & ~mask);
	return dest;
}

uint64_t helper_bfextu (uint64_t src_a, uint64_t bfstart, uint64_t bfend)
{
	uint64_t dest;
	uint64_t mask = 0;
	mask = ((-1ULL) ^ ((-1ULL << ((bfend - bfstart) & 63)) << 1));
	uint64_t rot_src =
	  (((uint64_t) src_a) >> bfstart) | (src_a << (64 - bfstart));
	dest = rot_src & mask;
	return dest;
}

uint64_t helper_bfins (uint64_t src_a, uint64_t bfstart, uint64_t bfend)
{
	uint64_t dest;
	uint64_t mask = 0;
	int start;
	int end;
	start = bfstart;
	end = bfend;
	mask =
	  (start <=
	   end) ? ((-1ULL << start) ^ ((-1ULL << end) << 1)) : ((-1ULL << start) |
								(-1ULL >>
								 ((64 - 1) - end)));
	uint64_t rot_src =
	  (src_a << start) | ((uint64_t) src_a >> (64 - start));
	dest = (rot_src & mask) | (dest & (-1ULL ^ mask));
	return dest;
}

uint64_t helper_cmoveqz (uint64_t src_a, uint64_t src_b)
{
	uint64_t dest;
	uint64_t localsrc_b = src_b;
	uint64_t localdest = dest;
	dest = (src_a == 0) ? (localsrc_b) : (localdest);
	return dest;
}

uint64_t helper_cmovnez (uint64_t src_a, uint64_t src_b)
{
	uint64_t dest;
	uint64_t localsrc_b = src_b;
	uint64_t localdest = dest;
	dest = (src_a != 0) ? (localsrc_b) : (localdest);
	return dest;
}

uint64_t helper_mm (uint64_t src_a, uint64_t bfstart, uint64_t bfend)
{
	uint64_t dest;
	uint64_t mask = 0;
	int start;
	int end;
	start = bfstart;
	end = bfend;
	mask =
	  (start <=
	   end) ? ((-1ULL << start) ^ ((-1ULL << end) << 1)) : ((-1ULL << start) |
								(-1ULL >>
								 ((64 - 1) - end)));
	dest = (dest & mask) | (src_a & (-1ULL ^ mask));
	return dest;
}

uint64_t helper_cmnz (uint64_t src_a, uint64_t src_b)
{
	uint64_t dest;
	dest = sign_extend((src_a != 0) ? 1 : 0, 1) & src_b;
	return dest;
}

uint64_t helper_cmz (uint64_t src_a, uint64_t src_b)
{
	uint64_t dest;
	dest = sign_extend((src_a == 0) ? 1 : 0, 1) & src_b;
	return dest;
}

uint64_t helper_shru (uint64_t src_a, uint64_t src_b)
{
	uint64_t dest;
	dest = (uint64_t) src_a >> (src_b & 63);
	return dest;
}

uint64_t helper_shrui (uint64_t src_a, uint64_t ShAmt)
{
	uint64_t dest;
	dest = (uint64_t) src_a >> (ShAmt & 63);
	return dest;
}

uint64_t helper_shrux (uint64_t src_a, uint64_t src_b)
{
	uint64_t dest = sign_extend_32((uint32_t) src_a >> (src_b & 31));
	return dest;
}

uint64_t helper_shruxi (uint64_t src_a, uint64_t ShAmt)
{
	uint64_t dest = sign_extend_32 (((uint32_t) src_a) >> (ShAmt & 31));
	return dest;
}

uint64_t helper_tblidxb0 (uint64_t dest, uint64_t src_a)
{
	dest = (dest & ~0x3FC) | (((src_a >> 0) & BYTE_MASK) << 2);
	return dest;
}

uint64_t helper_tblidxb1 (uint64_t dest, uint64_t src_a)
{
	dest = (dest & ~0x3FC) | (((src_a >> 8) & BYTE_MASK) << 2);
	return dest;
}

uint64_t helper_tblidxb2 (uint64_t dest, uint64_t src_a)
{
	dest = (dest & ~0x3FC) | (((src_a >> 16) & BYTE_MASK) << 2);
	return dest;
}

uint64_t helper_tblidxb3 (uint64_t dest, uint64_t src_a)
{
	dest = (dest & ~0x3FC) | (((src_a >> 24) & BYTE_MASK) << 2);
	return dest;
}

/* bit manipulation instructions */

uint64_t helper_clz(uint64_t src_a)
{
	uint64_t dest;
	unsigned int counter;
	for (counter = 0; counter < WORD_SIZE; counter++)
	  {
	    if ((src_a >> (WORD_SIZE - 1 - counter)) & 0x1)
	      {
		break;
	      }
	  }

	dest = counter;
	return dest;
}

uint64_t helper_crc32_32(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest;
	unsigned int Counter;
	uint32_t accum = src_a;
	uint32_t input = src_b;
	for (Counter = 0; Counter < 32; Counter++)
	  {
	    accum =
	      (accum >> 1) ^ (((input & 1) ^ (accum & 1)) ? 0xEDB88320 : 0x00000000);
	    input = input >> 1;
	  } dest = accum;
	return dest;
}

uint64_t helper_crc32_8(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest;
	unsigned int Counter;
	uint32_t accum = src_a;
	uint32_t input = src_b;
	for (Counter = 0; Counter < 8; Counter++)
	  {
	    accum =
	      (accum >> 1) ^ (((input & 1) ^ (accum & 1)) ? 0xEDB88320 : 0x00000000);
	    input = input >> 1;
	  } dest = accum;
	return dest;
}

uint64_t helper_ctz(uint64_t src_a)
{
	uint64_t dest;
	unsigned int counter;
	for (counter = 0; counter < WORD_SIZE; counter++)
	  {
	    if ((src_a >> counter) & 0x1)
	      {
		break;
	      }
	  }

	dest = counter;
	return dest;
}

uint64_t helper_dblalign(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	int shift = (src_b & 7) * BYTE_SIZE;
	uint64_t a = src_a;
	uint64_t b = dest;
	dest = (little_endian ()? (shift == 0 ? b : ((a << (64 - shift)) | (b >> shift))) \
		: (shift == 0 ? b : ((b << shift) | (a >> (64 - shift)))));
	return dest;
}

uint64_t helper_dblalign2(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t a = src_a;
	uint64_t b = src_b;
	dest = (little_endian ()? ((a << (64 - 2 * BYTE_SIZE)) | (b >> 2 * BYTE_SIZE)) \
	   : ((b << 2 * BYTE_SIZE) | (a >> (64 - 2 * BYTE_SIZE))));
	return dest;
}

uint64_t helper_dblalign4(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t a = src_a;
	uint64_t b = src_b;
	dest = (little_endian ()? ((a << (64 - 4 * BYTE_SIZE)) | (b >> 4 * BYTE_SIZE)) \
   		: ((b << 4 * BYTE_SIZE) | (a >> (64 - 4 * BYTE_SIZE))));
	return dest;
}

uint64_t helper_dblalign6(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t a = src_a;
	uint64_t b = src_b;
	dest =(little_endian ()? ((a << (64 - 6 * BYTE_SIZE)) | (b >> 6 * BYTE_SIZE)) \
	   : ((b << 6 * BYTE_SIZE) | (a >> (64 - 6 * BYTE_SIZE))));
	return dest;
}


uint64_t helper_pcnt(uint64_t src_a)
{
	uint64_t dest;
	unsigned int counter;
	int numberOfOnes = 0;
	for (counter = 0; counter < WORD_SIZE; counter++)
	  {
	    numberOfOnes += (src_a >> counter) & 0x1;
	  }

	dest = numberOfOnes;
	return dest;
}

uint64_t helper_revbits(uint64_t src_a)
{
	uint64_t dest;
	uint64_t output = 0;
	unsigned int counter;
	for (counter = 0; counter < (WORD_SIZE); counter++)
	  {
	    output |=
	      (((src_a >> (counter)) & 0x1) << ((WORD_SIZE - 1) - counter));
	  }

	dest = output;
	return dest;
}

uint64_t helper_revbytes(uint64_t src_a)
{
	uint64_t dest;
	uint64_t output = 0;
	unsigned int counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output |=
	      (((src_a >> (counter * BYTE_SIZE)) & BYTE_MASK) <<
	       ((((WORD_SIZE / BYTE_SIZE) - 1) - counter) * BYTE_SIZE));
	  }

	dest = output;
	return dest;
}

uint64_t helper_shufflebytes(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest;
	uint64_t a = src_a;
	uint64_t b = src_b;
	uint64_t d = dest;
	uint64_t output = 0;
	unsigned int counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int sel = get_byte (b, counter) & 0xf;
	    uint8_t byte = (sel < 8) ? get_byte (d, sel) : get_byte (a, (sel - 8));
	    output = set_byte (output, counter, byte);
	  }

	dest = output;
	return dest;
}

/* compare instructions */
uint64_t helper_cmpeq (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest = ((uint64_t) src_a == (uint64_t) src_b) ? 1 : 0;
	return dest;
}

uint64_t helper_cmpeqi (uint64_t src_a, uint64_t imm8)
{ 
	uint64_t dest = ((uint64_t) src_a == (uint64_t) sign_extend(imm8, 8)) ? 1 : 0;
	return dest;
}

uint64_t helper_cmples (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest = ((int64_t) src_a <= (int64_t) src_b) ? 1 : 0;
	return dest;
}

uint64_t helper_cmpleu (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest = ((uint64_t) src_a <= (uint64_t) src_b) ? 1 : 0;
	return dest;
}

uint64_t helper_cmplts (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest = ((int64_t) src_a < (int64_t) src_b) ? 1 : 0;
	return dest;
}

uint64_t helper_cmpltsi (uint64_t src_a, uint64_t imm8)
{ 
	uint64_t dest = ((int64_t) src_a < ((int64_t) sign_extend(imm8, 8))) ? 1 : 0;
	return dest;
}

uint64_t helper_cmpltu (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest = ((uint64_t) src_a < (uint64_t) src_b) ? 1 : 0;
	return dest;
}

uint64_t helper_cmpltui (uint64_t src_a, uint64_t imm8)
{ 
	uint64_t dest = ((uint64_t) src_a < ((uint64_t) sign_extend(imm8, 8))) ? 1 : 0;
	return dest;
}

uint64_t helper_cmpne (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t dest = ((uint64_t) src_a != (uint64_t) src_b) ? 1 : 0;
	return dest;
}


/* multiply instructions */

uint64_t helper_cmul (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	int32_t realA = sign_extend_16 (get_2_byte (src_a, 0));
	int32_t imagA = sign_extend_16 (get_2_byte (src_a, 1));
	int32_t realB = sign_extend_16 (get_2_byte (src_b, 0));
	int32_t imagB = sign_extend_16 (get_2_byte (src_b, 1));
	int32_t realRes = realA * realB - imagA * imagB;
	int32_t imagRes = realA * imagB + imagA * realB;
	output = set_4_byte (output, 0, realRes);
	output = set_4_byte (output, 1, imagRes);
	return output;
}

uint64_t helper_cmula (uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	int32_t realA = sign_extend_16 (get_2_byte (src_a, 0));
	int32_t imagA = sign_extend_16 (get_2_byte (src_a, 1));
	int32_t realB = sign_extend_16 (get_2_byte (src_b, 0));
	int32_t imagB = sign_extend_16 (get_2_byte (src_b, 1));
	int32_t realRes = realA * realB - imagA * imagB;
	int32_t imagRes = realA * imagB + imagA * realB;
	output = set_4_byte (output, 0, realRes + get_4_byte (dest, 0));
	output = set_4_byte (output, 1, imagRes + get_4_byte (dest, 1));
	return output;
}

uint64_t helper_cmulaf (uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	int32_t realA = sign_extend_16 (get_2_byte (src_a, 0));
	int32_t imagA = sign_extend_16 (get_2_byte (src_a, 1));
	int32_t realB = sign_extend_16 (get_2_byte (src_b, 0));
	int32_t imagB = sign_extend_16 (get_2_byte (src_b, 1));
	int32_t realRes = realA * realB - imagA * imagB;
	int32_t imagRes = realA * imagB + imagA * realB;
	output = set_2_byte (output, 0, (realRes >> 15) + get_2_byte (dest, 0));
	output = set_2_byte (output, 1, (imagRes >> 15) + get_2_byte (dest, 1));
	return output;
}

uint64_t helper_cmulf (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	int32_t realA = sign_extend_16 (get_2_byte (src_a, 0));
	int32_t imagA = sign_extend_16 (get_2_byte (src_a, 1));
	int32_t realB = sign_extend_16 (get_2_byte (src_b, 0));
	int32_t imagB = sign_extend_16 (get_2_byte (src_b, 1));
	int32_t realRes = realA * realB - imagA * imagB;
	int32_t imagRes = realA * imagB + imagA * realB;
	output = set_2_byte (output, 0, (realRes >> 15));
	output = set_2_byte (output, 1, (imagRes >> 15));
	return output;
}

uint64_t helper_cmulfr (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	int32_t realA = sign_extend_16 (get_2_byte (src_a, 0));
	int32_t imagA = sign_extend_16 (get_2_byte (src_a, 1));
	int32_t realB = sign_extend_16 (get_2_byte (src_b, 0));
	int32_t imagB = sign_extend_16 (get_2_byte (src_b, 1));
	int32_t realRes = realA * realB - imagA * imagB + (1 << 14);
	int32_t imagRes = realA * imagB + imagA * realB + (1 << 14);
	output = set_2_byte (output, 0, (realRes >> 15));
	output = set_2_byte (output, 1, (imagRes >> 15));
	return output;
}

uint64_t helper_cmulh (uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	int32_t realA = sign_extend_16 (get_2_byte (src_a, 0));
	int32_t imagA = sign_extend_16 (get_2_byte (src_a, 1));
	int32_t realB = sign_extend_16 (get_2_byte (src_b, 0));
	int32_t imagB = sign_extend_16 (get_2_byte (src_b, 1));
	int32_t realRes = realA * realB - imagA * imagB;
	int32_t imagRes = realA * imagB + imagA * realB;
	output = set_2_byte (output, 0, (realRes >> 16));
	output = set_2_byte (output, 1, (imagRes >> 16));
	return output;
}

uint64_t helper_cmulhr(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	int32_t realA = sign_extend_16 (get_2_byte (src_a, 0));
	int32_t imagA = sign_extend_16 (get_2_byte (src_a, 1));
	int32_t realB = sign_extend_16 (get_2_byte (src_b, 0));
	int32_t imagB = sign_extend_16 (get_2_byte (src_b, 1));
	int32_t realRes = realA * realB - imagA * imagB + (1 << 15);
	int32_t imagRes = realA * imagB + imagA * realB + (1 << 15);
	output = set_2_byte (output, 0, (realRes >> 16));
	output = set_2_byte (output, 1, (imagRes >> 16));
	return output;
}

uint64_t helper_mul_hs_hs(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 (src_a >> 32) * sign_extend_32 (src_b >> 32);
	return output;
}

uint64_t helper_mul_hs_hu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 (src_a >> 32) * ((uint64_t) src_b >> 32);
	return output;
}


uint64_t helper_mul_hs_ls(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 (src_a >> 32) * sign_extend_32 (src_b);
	return output;
}

uint64_t helper_mul_hs_lu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 (src_a >> 32) * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mul_hu_hu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = ((uint64_t) src_a >> 32) * ((uint64_t) src_b >> 32);
	return output;
}

uint64_t helper_mul_hu_ls(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = ((uint64_t) src_a >> 32) * sign_extend_32 (src_b);
	return output;
}

uint64_t helper_mul_hu_lu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = ((uint64_t) src_a >> 32) * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mul_ls_ls(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 (src_a) * sign_extend_32 (src_b);
	return output;
}

uint64_t helper_mul_ls_lu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 (src_a) * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mul_lu_lu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = (uint64_t) (uint32_t) src_a * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mula_hs_hs(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + sign_extend_32 (src_a >> 32) * sign_extend_32 (src_b >> 32);
	return output;
}

uint64_t helper_mula_hs_hu(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + sign_extend_32 (src_a >> 32) * ((uint64_t) src_b >> 32);
	return output;
}

uint64_t helper_mula_hs_ls(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + sign_extend_32 (src_a >> 32) * sign_extend_32 (src_b);
	return output;
}

uint64_t helper_mula_hs_lu(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + sign_extend_32 (src_a >> 32) * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mula_hu_hu(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + ((uint64_t) src_a >> 32) * ((uint64_t) src_b >> 32);
	return output;
}

uint64_t helper_mula_hu_ls(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + ((uint64_t) src_a >> 32) * sign_extend_32 (src_b);
	return output;
}

uint64_t helper_mula_hu_lu(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + ((uint64_t) src_a >> 32) * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mula_ls_ls(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + sign_extend_32 (src_a) * sign_extend_32 (src_b);
	return output;
}

uint64_t helper_mula_ls_lu(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + sign_extend_32 (src_a) * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mula_lu_lu(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = dest + (uint64_t) (uint32_t) src_a * (uint64_t) (uint32_t) src_b;
	return output;
}

uint64_t helper_mulax(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 ((int32_t) dest + (int32_t) src_a * (int32_t) src_b);
	return output;
}

uint64_t helper_mulx(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = sign_extend_32 ((int32_t) src_a * (int32_t) src_b);
	return output;
}

uint64_t helper_v1add(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (get_byte (src_a, counter) + get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1addi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output = set_byte (output, counter, (get_byte (src_a, counter) + Imm8));
	  }

	return output;
}

uint64_t helper_v1adduc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       unsigned_saturate8 (get_byte (src_a, counter) +
					   get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1adiffu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       abs (get_byte (src_a, counter) - get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1avgu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint64_t srca = get_byte (src_a, counter);
	    uint64_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca + srcb + 1) >> 1));
	  }

	return output;
}

uint64_t helper_v1cmpeq(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca == srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmpeqi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = sign_extend_8 (Imm8);
	    output = set_byte (output, counter, ((srca == srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmples(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca <= srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmpleu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint8_t srca = get_byte (src_a, counter);
	    uint8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca <= srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmplts(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmpltsi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = sign_extend_8 (Imm8) & BYTE_MASK;
	    output = set_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmpltu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint8_t srca = get_byte (src_a, counter);
	    uint8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmpltui(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint8_t srca = get_byte (src_a, counter);
	    uint8_t srcb = sign_extend_8 (Imm8);
	    output = set_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1cmpne(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca != srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v1ddotpu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t packed_output = 0;
	int32_t output;
	uint32_t counter;
	uint32_t half;
	for (half = 0; half < 2; half++)
	  {
	    uint32_t offset = half * ((WORD_SIZE / 2) / 8);
	    output = 0;
	    for (counter = 0; counter < ((WORD_SIZE / 2) / 8); counter++)
	      {
			output += ((uint16_t) get_byte (src_a, counter + offset) * \
		   	(uint16_t) get_byte (src_b, counter + offset));
	      }
	    packed_output = set_4_byte (packed_output, half, output);
	  }

	return packed_output;
}

uint64_t helper_v1ddotpua(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t packed_output = 0;
	int32_t output;
	uint32_t counter;
	uint32_t half;
	for (half = 0; half < 2; half++)
	  {
	    uint32_t offset = half * ((WORD_SIZE / 2) / 8);
	    output = 0;
	    for (counter = 0; counter < ((WORD_SIZE / 2) / 8); counter++)
	      {
			output += ((uint16_t) get_byte (src_a, counter + offset) * \
			(uint16_t) get_byte (src_b, counter + offset));
	      }
	    packed_output = set_4_byte (packed_output, half, get_4_byte (dest, half) + output);
	  }

	return packed_output;
}

uint64_t helper_v1ddotpus(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t packed_output = 0;
	int32_t output;
	uint32_t counter;
	uint32_t half;
	for (half = 0; half < 2; half++)
	  {
	    uint32_t offset = half * ((WORD_SIZE / 2) / 8);
	    output = 0;
	    for (counter = 0; counter < ((WORD_SIZE / 2) / 8); counter++)
	      {
			output += ((uint16_t) get_byte (src_a, counter + offset) * \
		   (int16_t) sign_extend_8 (get_byte (src_b, counter + offset)));
	      }
	    packed_output = set_4_byte (packed_output, half, output);
	  }

	return packed_output;
}

uint64_t helper_v1ddotpusa(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t packed_output = 0;
	int32_t output;
	uint32_t counter;
	uint32_t half;
	for (half = 0; half < 2; half++)
	  {
	    uint32_t offset = half * ((WORD_SIZE / 2) / 8);
	    output = 0;
	    for (counter = 0; counter < ((WORD_SIZE / 2) / 8); counter++)
	      {
			output += ((uint16_t) get_byte (src_a, counter + offset) * \
		   	(int16_t) sign_extend_8 (get_byte (src_b, counter + offset)));
	      }
	    packed_output = set_4_byte (packed_output, half, get_4_byte (dest, half) + output);
	  }

	return packed_output;
}

uint64_t helper_v1dotp(uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 8); counter++)
	  {
	    output += ((int16_t) sign_extend_8 (get_byte (src_a, counter)) * \
	    (int16_t) sign_extend_8 (get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1dotpa(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 8); counter++)
	  {
	    output += ((int16_t) sign_extend_8 (get_byte (src_a, counter)) * \
	    (int16_t) sign_extend_8 (get_byte (src_b, counter)));
	  }

	output = dest + output;
	return output;
}

uint64_t helper_v1dotpu(uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 8); counter++)
	  {
	    output += ((uint16_t) get_byte (src_a, counter) * \
	       (uint16_t) get_byte (src_b, counter));
	  }

	return output;
}

uint64_t helper_v1dotpua(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 8); counter++)
	  {
	    output += ((uint16_t) get_byte (src_a, counter) * \
	       (uint16_t) get_byte (src_b, counter));
	  }

	output = dest + output;
	return output;
}

uint64_t helper_v1dotpus(uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 8); counter++)
	  {
	    output += \
	      ((uint16_t) get_byte (src_a, counter) * \
	       (int16_t) sign_extend_8 (get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1dotpusa(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 8); counter++)
	  {
	    output += \
	      ((uint16_t) get_byte (src_a, counter) * \
	       (int16_t) sign_extend_8 (get_byte (src_b, counter)));
	  }
	output = dest + output;
	return output;
}

uint64_t helper_v1int_h(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    bool asel = ((counter & 1) == 1);
	    int in_sel = 4 + counter / 2;
	    int8_t srca = get_byte (src_a, in_sel);
	    int8_t srcb = get_byte (src_b, in_sel);
	    output = set_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v1int_l(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    bool asel = ((counter & 1) == 1);
	    int in_sel = 0 + counter / 2;
	    int8_t srca = get_byte (src_a, in_sel);
	    int8_t srcb = get_byte (src_b, in_sel);
	    output = set_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v1maxu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint8_t srca = get_byte (src_a, counter);
	    uint8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca > srcb) ? srca : srcb));
	  }

	return output;
}

uint64_t helper_v1maxui(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	uint8_t immb = Imm8;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint8_t srca = get_byte (src_a, counter);
	    output = set_byte (output, counter, ((srca > immb) ? srca : immb));
	  }

	return output;
}

uint64_t helper_v1minu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint8_t srca = get_byte (src_a, counter);
	    uint8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca < srcb) ? srca : srcb));
	  }

	return output;
}

uint64_t helper_v1minui(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	uint8_t immb = Imm8;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    uint8_t srca = get_byte (src_a, counter);
	    output = set_byte (output, counter, ((srca < immb) ? srca : immb));
	  }

	return output;
}

uint64_t helper_v1mnz(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca != 0) ? srcb : 0));
	  }

	return output;
}


uint64_t helper_v1multu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 8); counter++)
	  {
	    output =
	      set_byte (output, counter,((uint8_t) get_byte (src_a, counter) * 
			(uint8_t) get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1mulu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output = set_2_byte (output, counter,((uint16_t) get_byte (src_a, counter) * 
			 (uint16_t) get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1mulus(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			((int16_t) get_byte (src_a, counter) *
			 (int16_t) sign_extend_8 (get_byte (src_b, counter))));
	  }

	return output;
}

uint64_t helper_v1mz(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    int8_t srca = get_byte (src_a, counter);
	    int8_t srcb = get_byte (src_b, counter);
	    output = set_byte (output, counter, ((srca == 0) ? srcb : 0));
	  }

	return output;
}

uint64_t helper_v1sadau(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output += abs (get_byte (src_a, counter) - get_byte (src_b, counter));
	  }

	output = output + dest;
	return output;
}

uint64_t helper_v1sadu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output += abs (get_byte (src_a, counter) - get_byte (src_b, counter));
	  }

	return output;
}

uint64_t helper_v1shl(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (get_byte (src_a, counter) <<
			(((uint64_t) src_b) % BYTE_SIZE)));
	  }

	return output;
}

uint64_t helper_v1shli(uint64_t src_a, uint64_t ShAmt)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (get_byte (src_a, counter) <<
			(((uint64_t) ShAmt) % BYTE_SIZE)));
	  }

	return output;
}

uint64_t helper_v1shrs(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (sign_extend_8 (get_byte (src_a, counter)) >>
			(((uint64_t) src_b) % BYTE_SIZE)));
	  }

	return output;
}

uint64_t helper_v1shrsi(uint64_t src_a, uint64_t ShAmt)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (sign_extend_8 (get_byte (src_a, counter)) >>
			(((uint64_t) ShAmt) % BYTE_SIZE)));
	  }

	return output;
}

uint64_t helper_v1shru(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (get_byte (src_a, counter) >>
			(((uint64_t) src_b) % BYTE_SIZE)));
	  }

	return output;
}

uint64_t helper_v1shrui(uint64_t src_a, uint64_t ShAmt)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (get_byte (src_a, counter) >>
			(((uint64_t) ShAmt) % BYTE_SIZE)));
	  }

	return output;
}

uint64_t helper_v1sub(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       (get_byte (src_a, counter) - get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v1subuc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    output =
	      set_byte (output, counter,
		       unsigned_saturate8 (get_byte (src_a, counter) -
					   get_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v2add(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(get_2_byte (src_a, counter) +
			 get_2_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v2addi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(get_2_byte (src_a, counter) + sign_extend_8 (Imm8)));
	  }

	return output;
}

uint64_t helper_v2addsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			signed_saturate16 (sign_extend_16 (get_2_byte (src_a, counter))
					   +
					   sign_extend_16 (get_2_byte
							 (src_b, counter))));
	  }

	return output;
}

uint64_t helper_v2adiffs(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			abs (sign_extend_16 (get_2_byte (src_a, counter)) -
			     sign_extend_16 (get_2_byte (src_b, counter))));
	  }

	return output;
}

uint64_t helper_v2avgs(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int64_t srca = sign_extend_16 (get_2_byte (src_a, counter));
	    int64_t srcb = sign_extend_16 (get_2_byte (src_b, counter));
	    output = set_2_byte (output, counter, ((srca + srcb + 1) >> 1));
	  }

	return output;
}

uint64_t helper_v2cmpeq(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca == srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmpeqi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = sign_extend_8 (Imm8);
	    output = set_2_byte (output, counter, ((srca == srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmples(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca <= srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmpleu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    uint16_t srca = get_2_byte (src_a, counter);
	    uint16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca <= srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmplts(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmpltsi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = sign_extend_8 (Imm8);
	    output = set_2_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmpltu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    uint16_t srca = get_2_byte (src_a, counter);
	    uint16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmpltui(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    uint16_t srca = get_2_byte (src_a, counter);
	    uint16_t srcb = sign_extend_8 (Imm8);
	    output = set_2_byte (output, counter, ((srca < srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2cmpne(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca != srcb) ? 1 : 0));
	  }

	return output;
}

uint64_t helper_v2dotp(uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output +=
	      ((int32_t) sign_extend_16 (get_2_byte (src_a, counter)) *
	       (int32_t) sign_extend_16 (get_2_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v2dotpa(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	int64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output +=
	      ((int32_t) sign_extend_16 (get_2_byte (src_a, counter)) *
	       (int32_t) sign_extend_16 (get_2_byte (src_b, counter)));
	  }

	output = output + dest;
	return output;
}

uint64_t helper_v2int_h(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    bool asel = ((counter & 1) == 1);
	    int in_sel = 2 + counter / 2;
	    int16_t srca = get_2_byte (src_a, in_sel);
	    int16_t srcb = get_2_byte (src_b, in_sel);
	    output = set_2_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v2int_l(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    bool asel = ((counter & 1) == 1);
	    int in_sel = 0 + counter / 2;
	    int16_t srca = get_2_byte (src_a, in_sel);
	    int16_t srcb = get_2_byte (src_b, in_sel);
	    output = set_2_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v2maxs(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca > srcb) ? srca : srcb));
	  }

	return output;
}

uint64_t helper_v2maxsi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    output =
	      set_2_byte (output, counter,
			((srca > sign_extend_8 (Imm8)) ? srca : sign_extend_8 (Imm8)));
	  }

	return output;
}

uint64_t helper_v2mins(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca < srcb) ? srca : srcb));
	  }

	return output;
}

uint64_t helper_v2minsi(uint64_t src_a, uint64_t Imm8)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    output =
	      set_2_byte (output, counter,
			((srca < sign_extend_8 (Imm8)) ? srca : sign_extend_8 (Imm8)));
	  }

	return output;
}

uint64_t helper_v2mnz(uint64_t src_a, uint64_t src_b)
{ 
	
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
		int16_t srca = get_2_byte (src_a, counter);
		int16_t srcb = get_2_byte (src_b, counter);
		output = set_2_byte (output, counter, ((srca != 0) ? srcb : 0));
	  }
	
	return output;
	
}

uint64_t helper_v2mulfsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    int64_t mul_res =
	      sign_extend_16 (get_2_byte (src_a, counter)) *
	      sign_extend_16 (get_2_byte (src_b, counter));
	    mul_res = signed_saturate32 (mul_res << 1);
	    output = set_4_byte (output, counter, mul_res);
	  }

	return output;
}

uint64_t helper_v2muls(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			((int32_t) sign_extend_16 (get_2_byte (src_a, counter)) *
			 (int32_t) sign_extend_16 (get_2_byte (src_b, counter))));
	  }

	return output;
}

uint64_t helper_v2mults(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			((int16_t) get_2_byte (src_a, counter) *
			 (int16_t) get_2_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v2mz(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    int16_t srca = get_2_byte (src_a, counter);
	    int16_t srcb = get_2_byte (src_b, counter);
	    output = set_2_byte (output, counter, ((srca == 0) ? srcb : 0));
	  }

	return output;
}

uint64_t helper_v2packh(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    bool asel = (counter >= 4);
	    int in_sel = 1 + (counter & 3) * 2;
	    int8_t srca = get_byte (src_a, in_sel);
	    int8_t srcb = get_byte (src_b, in_sel);
	    output = set_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v2packl(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    bool asel = (counter >= 4);
	    int in_sel = 0 + (counter & 3) * 2;
	    int8_t srca = get_byte (src_a, in_sel);
	    int8_t srcb = get_byte (src_b, in_sel);
	    output = set_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v2packuc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / BYTE_SIZE); counter++)
	  {
	    bool asel = (counter >= 4);
	    int in_sel = counter & 3;
	    int16_t srca = sign_extend_16 (get_2_byte (src_a, in_sel));
	    int16_t srcb = sign_extend_16 (get_2_byte (src_b, in_sel));
	    output =
	      set_byte (output, counter, unsigned_saturate8 (asel ? srca : srcb));
	  } return output;
}

uint64_t helper_v2sadas(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output +=
	      abs (sign_extend_16 (get_2_byte (src_a, counter)) -
		   sign_extend_16 (get_2_byte (src_b, counter)));
	  }

	output = dest + output;
	return output;
}

uint64_t helper_v2sadau(uint64_t dest, uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output +=
	      abs (get_2_byte (src_a, counter) - get_2_byte (src_b, counter));
	  }

	output = dest + output;
	return output;
}
uint64_t helper_v2sads(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output +=
	      abs (sign_extend_16 (get_2_byte (src_a, counter)) -
		   sign_extend_16 (get_2_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v2sadu(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output +=
	      abs (get_2_byte (src_a, counter) - get_2_byte (src_b, counter));
	  }

	return output;
}
uint64_t helper_v2shl(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(get_2_byte (src_a, counter) <<
			 (((uint64_t) src_b) % 16)));
	  }

	return output;
}
uint64_t helper_v2shli(uint64_t src_a, uint64_t ShAmt)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(get_2_byte (src_a, counter) <<
			 (((uint64_t) ShAmt) % 16)));
	  }

	return output;
}
uint64_t helper_v2shlsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			signed_saturate16 (sign_extend_16 (get_2_byte (src_a, counter))
					   << (((uint64_t) src_b) %
					       16)));
	  }

	return output;
}
uint64_t helper_v2shrs(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(sign_extend_16 (get_2_byte (src_a, counter)) >>
			 (((uint64_t) src_b) % 16)));
	  }

	return output;
}
uint64_t helper_v2shrsi(uint64_t src_a, uint64_t ShAmt)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(sign_extend_16 (get_2_byte (src_a, counter)) >>
			 (((uint64_t) ShAmt) % 16)));
	  }

	return output;
}
uint64_t helper_v2shru(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(get_2_byte (src_a, counter) >>
			 (((uint64_t) src_b) % 16)));
	  }

	return output;
}

uint64_t helper_v2shrui(uint64_t src_a, uint64_t ShAmt)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(get_2_byte (src_a, counter) >>
			 (((uint64_t) ShAmt) % 16)));
	  }

	return output;
}

uint64_t helper_v2sub(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			(get_2_byte (src_a, counter) -
			 get_2_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v2subsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    output =
	      set_2_byte (output, counter,
			signed_saturate16 (sign_extend_16 (get_2_byte (src_a, counter))
					   -
					   sign_extend_16 (get_2_byte
							 (src_b, counter))));
	  }

	return output;
}

uint64_t helper_v4add(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			(get_4_byte (src_a, counter) +
			 get_4_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v4addsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			signed_saturate32 (sign_extend_32 (get_4_byte (src_a, counter))
					   +
					   sign_extend_32 (get_4_byte
							 (src_b, counter))));
	  }

	return output;
}

uint64_t helper_v4int_h(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    bool asel = ((counter & 1) == 1);
	    int in_sel = 1 + counter / 2;
	    int32_t srca = get_4_byte (src_a, in_sel);
	    int32_t srcb = get_4_byte (src_b, in_sel);
	    output = set_4_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v4int_l(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    bool asel = ((counter & 1) == 1);
	    int in_sel = 0 + counter / 2;
	    int32_t srca = get_4_byte (src_a, in_sel);
	    int32_t srcb = get_4_byte (src_b, in_sel);
	    output = set_4_byte (output, counter, (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v4packsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 16); counter++)
	  {
	    bool asel = (counter >= 2);
	    int in_sel = counter & 1;
	    int64_t srca = sign_extend_32 (src_a >> (in_sel * 32));
	    int64_t srcb = sign_extend_32 (src_b >> (in_sel * 32));
	    output =
	      set_2_byte (output, counter, signed_saturate16 (asel ? srca : srcb));
	  } 
	return output;
}

uint64_t helper_v4shl(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			(get_4_byte (src_a, counter) <<
			 (((uint64_t) src_b) % 32)));
	  }

	return output;
}
uint64_t helper_v4shlsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			signed_saturate32 (sign_extend_32 (get_4_byte (src_a, counter))
					   << (((uint64_t) src_b) %
					       32)));
	  }

	return output;
}

uint64_t helper_v4shrs(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			(sign_extend_32 (get_4_byte (src_a, counter)) >>
			 (((uint64_t) src_b) % 32)));
	  }

	return output;
}

uint64_t helper_v4shru(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			(get_4_byte (src_a, counter) >>
			 (((uint64_t) src_b) % 32)));
	  }

	return output;
}

uint64_t helper_v4sub(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			(get_4_byte (src_a, counter) -
			 get_4_byte (src_b, counter)));
	  }

	return output;
}

uint64_t helper_v4subsc(uint64_t src_a, uint64_t src_b)
{ 
	uint64_t output = 0;
	uint32_t counter;
	for (counter = 0; counter < (WORD_SIZE / 32); counter++)
	  {
	    output =
	      set_4_byte (output, counter,
			signed_saturate32 (sign_extend_32 (get_4_byte (src_a, counter))
					   -
					   sign_extend_32 (get_4_byte
							 (src_b, counter))));
	  }

	return output;
}

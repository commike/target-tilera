/*
 *  Special purpose registers definitions for 64 bit tilera processors 
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

#if !defined (__CPU_TILERA_SPR_H__)
#define __CPU_TILERA_SPR_H__

#ifdef __tilegx__
#include "spr_64bit.h"
#else
#include "spr_32bit.h"
#endif

/* returns spr_index (serial no) against given address in spr file */
uint16_t spr_index (uint16_t addr)
{
	int i;
	for (i = 0; i < SPR_MAX; i++)
		if (spr_addr[i] == addr) return i;
	return SPR_MAX;
}


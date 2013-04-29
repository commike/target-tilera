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

#if !defined (__CPU_TILERA_SPR_64_H__)
#define __CPU_TILERA_SPR_64_H__

#include <stdint.h>

/* Tilera interrupt and exception list */

typedef enum 
{
	MEM_ERROR,
	SINGLE_STEP_3,
	SINGLE_STEP_2,
	SINGLE_STEP_1,
	SINGLE_STEP_0,
	
	IDN_COMPLETE,
	UDN_COMPLETE,
	ITLB_MISS,
	ILL,  					/* illegal instruction */
	GPV,					/*general protection violation*/

	IDN_ACCESS,
	UDN_ACCESS,
	SWINT_3,				/* software interrupt*/
	SWINT_2,
	SWINT_1,

	SWINT_0,
	ILL_TRANS,				/* illegal translation */
	UNALIGN_DATA,
	DTLB_MISS,
	DTLB_ACCESS,			/* dtlb access error */

	IDN_FIREWALL,			/* idn firewall violation */
	UDN_FIREWALL,
	TILE_TIMER,
	AUX_TILE_TIMER,
	IDN_TIMER,

	UDN_TIMER,
	IDN_AVAIL,				/* idn available */
	UDN_AVAIL,
	IPI_3,					/* interprocessor interrupt 3 */
	IPI_2,

	IPI_1,
	IPI_0,
	PERF_COUNT,				/* performance counters */
	AUX_PERF_COUNT,
	INTCTRL_3,				/* interrupt control 3 */

	INTCTRL_2,
	INTCTRL_1,
	INTCTRL_0,
	BOOT_ACCESS,
	WORLD_ACCESS,

	I_ASID,					/* instruction asid */
	D_ASID,					/* data asid */
	DOUBLE_FAULT
	
} mpl_ty;

/* Tilera-specific interrupt pending bits. 
#define CPU_INTERRUPT_TIMER	CPU_INTERRUPT_TGT_EXT_0
#define CPU_INTERRUPT_SMP	CPU_INTERRUPT_TGT_EXT_1
#define CPU_INTERRUPT_MCHK	CPU_INTERRUPT_TGT_EXT_2

#define CPU_INTERRUPT_IDN_FIREWALL		
#define CPU_INTERRUPT_UDN_FIREWALL
#define CPU_INTERRUPT_TILE_TIMER
#define CPU_INTERRUPT_AUX_TILE_TIMER
#define CPU_INTERRUPT_IDN_TIMER

#define CPU_INTERRUPT_UDN_TIMER
#define CPU_INTERRUPT_IDN_AVAIL			
#define CPU_INTERRUPT_UDN_AVAIL
#define CPU_INTERRUPT_IPI_3				
#define CPU_INTERRUPT_IPI_2

#define CPU_INTERRUPT_IPI_1
#define CPU_INTERRUPT_IPI_0
#define CPU_INTERRUPT_PERF_COUNT				
#define CPU_INTERRUPT_AUX_PERF_COUNT
#define CPU_INTERRUPT_INTCTRL_3				

#define CPU_INTERRUPT_INTCTRL_2
#define CPU_INTERRUPT_INTCTRL_1
#define CPU_INTERRUPT_INTCTRL_0
#define CPU_INTERRUPT_BOOT_ACCESS
#define CPU_INTERRUPT_WORLD_ACCESS

#define CPU_INTERRUPT_I_ASID					
#define CPU_INTERRUPT_D_ASID					
#define CPU_INTERRUPT_DOUBLE_FAULT */


/* Enumerated list of special purpose registers */

typedef enum
{
	SPR_AUX_PERF_COUNT_0 ,
	SPR_AUX_PERF_COUNT_1 ,
	SPR_AUX_PERF_COUNT_CTL ,
	SPR_AUX_PERF_COUNT_STS ,
	SPR_AUX_TILE_TIMER_CONTROL ,
	SPR_MPL_AUX_TILE_TIMER ,
	SPR_MPL_AUX_TILE_TIMER_SET_0 ,
	SPR_MPL_AUX_TILE_TIMER_SET_1 ,
	SPR_MPL_AUX_TILE_TIMER_SET_2 ,
	SPR_MPL_AUX_TILE_TIMER_SET_3 ,
	SPR_DIAG_TRACE_CTL ,
	SPR_DIAG_TRACE_STS ,
	SPR_DIAG_TRACE_DATA ,
	SPR_DIAG_BCST_CTL ,
	SPR_DIAG_BCST_MASK ,
	SPR_DIAG_BCST_TRIGGER ,
	SPR_QUIESCE_CTL ,
	SPR_DIAG_MUX_CTL ,
	SPR_PERF_COUNT_PLS ,
	SPR_IDN_DEMUX_BUF_THRESH ,
	SPR_SBOX_CONFIG ,
	SPR_L1_I_PIN_WAY_0 ,
	SPR_RSHIM_COORD ,
	SPR_I_AAR ,
	SPR_PSEUDO_RANDOM_NUMBER_MODIFY ,
	SPR_CYCLE_MODIFY ,
	SPR_CBOX_MMAP_0 ,
	SPR_CBOX_MMAP_1 ,
	SPR_CBOX_MMAP_2 ,
	SPR_CBOX_MMAP_3 ,
	SPR_CBOX_HOME_MAP_ADDR ,
	SPR_CBOX_HOME_MAP_DATA ,
	SPR_CBOX_MSR ,
	SPR_CBOX_CACHEASRAM_CONFIG ,
	SPR_CBOX_CACHE_CONFIG ,
	SPR_BIG_ENDIAN_CONFIG ,
	SPR_MEM_STRIPE_CONFIG ,
	SPR_XDN_ROUND_ROBIN_ARB_CTL ,
	SPR_XDN_CORE_STARVATION_COUNT ,
	SPR_CACHE_INVALIDATION_COMPRESSION_MODE ,
	SPR_CACHE_INVALIDATION_MASK_0 ,
	SPR_CACHE_INVALIDATION_MASK_1 ,
	SPR_CACHE_INVALIDATION_MASK_2 ,
	SPR_MEM_ROUTE_ORDER ,
	SPR_MPL_BOOT_ACCESS ,
	SPR_MPL_BOOT_ACCESS_SET_0 ,
	SPR_MPL_BOOT_ACCESS_SET_1 ,
	SPR_MPL_BOOT_ACCESS_SET_2 ,
	SPR_MPL_BOOT_ACCESS_SET_3 ,
	SPR_UDN_DEMUX_BUF_THRESH ,
	SPR_D_ASID ,
	SPR_MPL_D_ASID ,
	SPR_MPL_D_ASID_SET_0 ,
	SPR_MPL_D_ASID_SET_1 ,
	SPR_MPL_D_ASID_SET_2 ,
	SPR_MPL_D_ASID_SET_3 ,
	SPR_LAST_INTERRUPT_REASON ,
	SPR_MPL_DOUBLE_FAULT ,
	SPR_MPL_DOUBLE_FAULT_SET_0 ,
	SPR_MPL_DOUBLE_FAULT_SET_1 ,
	SPR_MPL_DOUBLE_FAULT_SET_2 ,
	SPR_MPL_DOUBLE_FAULT_SET_3 ,
	SPR_MPL_DTLB_ACCESS ,
	SPR_MPL_DTLB_ACCESS_SET_0 ,
	SPR_MPL_DTLB_ACCESS_SET_1 ,
	SPR_MPL_DTLB_ACCESS_SET_2 ,
	SPR_MPL_DTLB_ACCESS_SET_3 ,
	SPR_AAR ,
	SPR_DTLB_CURRENT_VA ,
	SPR_DTLB_CURRENT_PA ,
	SPR_DTLB_CURRENT_ATTR ,
	SPR_DTLB_INDEX ,
	SPR_WIRED_DTLB ,
	SPR_CACHE_PINNED_WAYS ,
	SPR_REPLACEMENT_DTLB ,
	SPR_NUMBER_DTLB ,
	SPR_DTLB_MATCH_0 ,
	SPR_DTLB_PERF ,
	SPR_DTLB_TSB_BASE_ADDR_0 ,
	SPR_DTLB_TSB_ADDR_0 ,
	SPR_DTLB_TSB_FILL_CURRENT_ATTR ,
	SPR_DTLB_TSB_FILL_MATCH ,
	SPR_DTLB_TSB_BASE_ADDR_1 ,
	SPR_DTLB_TSB_ADDR_1 ,
	SPR_DTLB_BAD_ADDR ,
	SPR_DTLB_BAD_ADDR_REASON ,
	SPR_MPL_DTLB_MISS ,
	SPR_MPL_DTLB_MISS_SET_0 ,
	SPR_MPL_DTLB_MISS_SET_1 ,
	SPR_MPL_DTLB_MISS_SET_2 ,
	SPR_MPL_DTLB_MISS_SET_3 ,
	SPR_GPV_REASON ,
	SPR_MPL_GPV ,
	SPR_MPL_GPV_SET_0 ,
	SPR_MPL_GPV_SET_1 ,
	SPR_MPL_GPV_SET_2 ,
	SPR_MPL_GPV_SET_3 ,
	SPR_I_ASID ,
	SPR_MPL_I_ASID ,
	SPR_MPL_I_ASID_SET_0 ,
	SPR_MPL_I_ASID_SET_1 ,
	SPR_MPL_I_ASID_SET_2 ,
	SPR_MPL_I_ASID_SET_3 ,
	SPR_IDN_DATA_AVAIL ,
	SPR_IDN_DEMUX_COUNT_0 ,
	SPR_IDN_DEMUX_COUNT_1 ,
	SPR_IDN_PENDING ,
	SPR_IDN_SP_FIFO_CNT ,
	SPR_IDN_FLUSH_EGRESS ,
	SPR_IDN_ROUTE_ORDER ,
	SPR_MPL_IDN_ACCESS ,
	SPR_MPL_IDN_ACCESS_SET_0 ,
	SPR_MPL_IDN_ACCESS_SET_1 ,
	SPR_MPL_IDN_ACCESS_SET_2 ,
	SPR_MPL_IDN_ACCESS_SET_3 ,
	SPR_IDN_AVAIL_EN ,
	SPR_MPL_IDN_AVAIL ,
	SPR_MPL_IDN_AVAIL_SET_0 ,
	SPR_MPL_IDN_AVAIL_SET_1 ,
	SPR_MPL_IDN_AVAIL_SET_2 ,
	SPR_MPL_IDN_AVAIL_SET_3 ,
	SPR_IDN_COMPLETE_PENDING ,
	SPR_MPL_IDN_COMPLETE ,
	SPR_MPL_IDN_COMPLETE_SET_0 ,
	SPR_MPL_IDN_COMPLETE_SET_1 ,
	SPR_MPL_IDN_COMPLETE_SET_2 ,
	SPR_MPL_IDN_COMPLETE_SET_3 ,
	SPR_IDN_DIRECTION_PROTECT ,
	SPR_MPL_IDN_FIREWALL ,
	SPR_MPL_IDN_FIREWALL_SET_0 ,
	SPR_MPL_IDN_FIREWALL_SET_1 ,
	SPR_MPL_IDN_FIREWALL_SET_2 ,
	SPR_MPL_IDN_FIREWALL_SET_3 ,
	SPR_IDN_DEADLOCK_COUNT ,
	SPR_IDN_DEADLOCK_TIMEOUT ,
	SPR_MPL_IDN_TIMER ,
	SPR_MPL_IDN_TIMER_SET_0 ,
	SPR_MPL_IDN_TIMER_SET_1 ,
	SPR_MPL_IDN_TIMER_SET_2 ,
	SPR_MPL_IDN_TIMER_SET_3 ,
	SPR_MPL_ILL ,
	SPR_MPL_ILL_SET_0 ,
	SPR_MPL_ILL_SET_1 ,
	SPR_MPL_ILL_SET_2 ,
	SPR_MPL_ILL_SET_3 ,
	SPR_ILL_VA_PC ,
	SPR_ILL_TRANS_REASON ,
	SPR_MPL_ILL_TRANS ,
	SPR_MPL_ILL_TRANS_SET_0 ,
	SPR_MPL_ILL_TRANS_SET_1 ,
	SPR_MPL_ILL_TRANS_SET_2 ,
	SPR_MPL_ILL_TRANS_SET_3 ,
	SPR_SINGLE_STEP_EN_3_0 ,
	SPR_SINGLE_STEP_EN_2_0 ,
	SPR_SINGLE_STEP_EN_1_0 ,
	SPR_SINGLE_STEP_EN_0_0 ,
	SPR_INTCTRL_0_STATUS ,
	SPR_INTERRUPT_VECTOR_BASE_0 ,
	SPR_SYSTEM_SAVE_0_0 ,
	SPR_SYSTEM_SAVE_0_1 ,
	SPR_SYSTEM_SAVE_0_2 ,
	SPR_SYSTEM_SAVE_0_3 ,
	SPR_MPL_INTCTRL_0 ,
	SPR_MPL_INTCTRL_0_SET_0 ,
	SPR_MPL_INTCTRL_0_SET_1 ,
	SPR_MPL_INTCTRL_0_SET_2 ,
	SPR_MPL_INTCTRL_0_SET_3 ,
	SPR_EX_CONTEXT_0_0 ,
	SPR_EX_CONTEXT_0_1 ,
	SPR_INTERRUPT_MASK_0 ,
	SPR_INTERRUPT_MASK_SET_0 ,
	SPR_INTERRUPT_MASK_RESET_0 ,
	SPR_SINGLE_STEP_EN_3_1 ,
	SPR_SINGLE_STEP_EN_2_1 ,
	SPR_SINGLE_STEP_EN_1_1 ,
	SPR_SINGLE_STEP_EN_0_1 ,
	SPR_INTCTRL_1_STATUS ,
	SPR_INTERRUPT_VECTOR_BASE_1 ,
	SPR_SYSTEM_SAVE_1_0 ,
	SPR_SYSTEM_SAVE_1_1 ,
	SPR_SYSTEM_SAVE_1_2 ,
	SPR_SYSTEM_SAVE_1_3 ,
	SPR_MPL_INTCTRL_1 ,
	SPR_MPL_INTCTRL_1_SET_0 ,
	SPR_MPL_INTCTRL_1_SET_1 ,
	SPR_MPL_INTCTRL_1_SET_2 ,
	SPR_MPL_INTCTRL_1_SET_3 ,
	SPR_EX_CONTEXT_1_0 ,
	SPR_EX_CONTEXT_1_1 ,
	SPR_INTERRUPT_MASK_1 ,
	SPR_INTERRUPT_MASK_SET_1 ,
	SPR_INTERRUPT_MASK_RESET_1 ,
	SPR_SINGLE_STEP_EN_3_2 ,
	SPR_SINGLE_STEP_EN_2_2 ,
	SPR_SINGLE_STEP_EN_1_2 ,
	SPR_SINGLE_STEP_EN_0_2 ,
	SPR_INTCTRL_2_STATUS ,
	SPR_INTERRUPT_VECTOR_BASE_2 ,
	SPR_SYSTEM_SAVE_2_0 ,
	SPR_SYSTEM_SAVE_2_1 ,
	SPR_SYSTEM_SAVE_2_2 ,
	SPR_SYSTEM_SAVE_2_3 ,
	SPR_MPL_INTCTRL_2 ,
	SPR_MPL_INTCTRL_2_SET_0 ,
	SPR_MPL_INTCTRL_2_SET_1 ,
	SPR_MPL_INTCTRL_2_SET_2 ,
	SPR_MPL_INTCTRL_2_SET_3 ,
	SPR_EX_CONTEXT_2_0 ,
	SPR_EX_CONTEXT_2_1 ,
	SPR_INTERRUPT_MASK_2 ,
	SPR_INTERRUPT_MASK_SET_2 ,
	SPR_INTERRUPT_MASK_RESET_2 ,
	SPR_SINGLE_STEP_EN_3_3 ,
	SPR_SINGLE_STEP_EN_2_3 ,
	SPR_SINGLE_STEP_EN_1_3 ,
	SPR_SINGLE_STEP_EN_0_3 ,
	SPR_INTCTRL_3_STATUS ,
	SPR_INTERRUPT_VECTOR_BASE_3 ,
	SPR_SYSTEM_SAVE_3_0 ,
	SPR_SYSTEM_SAVE_3_1 ,
	SPR_SYSTEM_SAVE_3_2 ,
	SPR_SYSTEM_SAVE_3_3 ,
	SPR_MPL_INTCTRL_3 ,
	SPR_MPL_INTCTRL_3_SET_0 ,
	SPR_MPL_INTCTRL_3_SET_1 ,
	SPR_MPL_INTCTRL_3_SET_2 ,
	SPR_MPL_INTCTRL_3_SET_3 ,
	SPR_EX_CONTEXT_3_0 ,
	SPR_EX_CONTEXT_3_1 ,
	SPR_INTERRUPT_MASK_3 ,
	SPR_INTERRUPT_MASK_SET_3 ,
	SPR_INTERRUPT_MASK_RESET_3 ,
	SPR_IPI_EVENT_0 ,
	SPR_IPI_EVENT_SET_0 ,
	SPR_IPI_EVENT_RESET_0 ,
	SPR_IPI_MASK_0 ,
	SPR_IPI_MASK_SET_0 ,
	SPR_IPI_MASK_RESET_0 ,
	SPR_MPL_IPI_0 ,
	SPR_MPL_IPI_0_SET_0 ,
	SPR_MPL_IPI_0_SET_1 ,
	SPR_MPL_IPI_0_SET_2 ,
	SPR_MPL_IPI_0_SET_3 ,
	SPR_IPI_EVENT_1 ,
	SPR_IPI_EVENT_SET_1 ,
	SPR_IPI_EVENT_RESET_1 ,
	SPR_IPI_MASK_1 ,
	SPR_IPI_MASK_SET_1 ,
	SPR_IPI_MASK_RESET_1 ,
	SPR_MPL_IPI_1 ,
	SPR_MPL_IPI_1_SET_0 ,
	SPR_MPL_IPI_1_SET_1 ,
	SPR_MPL_IPI_1_SET_2 ,
	SPR_MPL_IPI_1_SET_3 ,
	SPR_IPI_EVENT_2 ,
	SPR_IPI_EVENT_SET_2 ,
	SPR_IPI_EVENT_RESET_2 ,
	SPR_IPI_MASK_2 ,
	SPR_IPI_MASK_SET_2 ,
	SPR_IPI_MASK_RESET_2 ,
	SPR_MPL_IPI_2 ,
	SPR_MPL_IPI_2_SET_0 ,
	SPR_MPL_IPI_2_SET_1 ,
	SPR_MPL_IPI_2_SET_2 ,
	SPR_MPL_IPI_2_SET_3 ,
	SPR_IPI_EVENT_3 ,
	SPR_IPI_EVENT_SET_3 ,
	SPR_IPI_EVENT_RESET_3 ,
	SPR_IPI_MASK_3 ,
	SPR_IPI_MASK_SET_3 ,
	SPR_IPI_MASK_RESET_3 ,
	SPR_MPL_IPI_3 ,
	SPR_MPL_IPI_3_SET_0 ,
	SPR_MPL_IPI_3_SET_1 ,
	SPR_MPL_IPI_3_SET_2 ,
	SPR_MPL_IPI_3_SET_3 ,
	SPR_ITLB_CURRENT_VA ,
	SPR_ITLB_CURRENT_PA ,
	SPR_ITLB_CURRENT_ATTR ,
	SPR_ITLB_INDEX ,
	SPR_WIRED_ITLB ,
	SPR_REPLACEMENT_ITLB ,
	SPR_NUMBER_ITLB ,
	SPR_ITLB_PR ,
	SPR_ITLB_MATCH_0 ,
	SPR_ITLB_PERF ,
	SPR_ITLB_TSB_BASE_ADDR_0 ,
	SPR_ITLB_TSB_ADDR_0 ,
	SPR_ITLB_TSB_FILL_CURRENT_ATTR ,
	SPR_ITLB_TSB_FILL_MATCH ,
	SPR_ITLB_TSB_BASE_ADDR_1 ,
	SPR_ITLB_TSB_ADDR_1 ,
	SPR_MPL_ITLB_MISS ,
	SPR_MPL_ITLB_MISS_SET_0 ,
	SPR_MPL_ITLB_MISS_SET_1 ,
	SPR_MPL_ITLB_MISS_SET_2 ,
	SPR_MPL_ITLB_MISS_SET_3 ,
	SPR_XDN_DEMUX_ERROR ,
	SPR_SBOX_ERROR ,
	SPR_MEM_ERROR_ENABLE ,
	SPR_MEM_ERROR_MBOX_STATUS ,
	SPR_MEM_ERROR_MBOX_ADDR ,
	SPR_MEM_ERROR_CBOX_STATUS ,
	SPR_MEM_ERROR_CBOX_ADDR ,
	SPR_MPL_MEM_ERROR ,
	SPR_MPL_MEM_ERROR_SET_0 ,
	SPR_MPL_MEM_ERROR_SET_1 ,
	SPR_MPL_MEM_ERROR_SET_2 ,
	SPR_MPL_MEM_ERROR_SET_3 ,
	SPR_PERF_COUNT_0 ,
	SPR_PERF_COUNT_1 ,
	SPR_PERF_COUNT_CTL ,
	SPR_PERF_COUNT_STS ,
	SPR_WATCH_VAL ,
	SPR_WATCH_MASK ,
	SPR_PERF_COUNT_DN_CTL ,
	SPR_MPL_PERF_COUNT ,
	SPR_MPL_PERF_COUNT_SET_0 ,
	SPR_MPL_PERF_COUNT_SET_1 ,
	SPR_MPL_PERF_COUNT_SET_2 ,
	SPR_MPL_PERF_COUNT_SET_3 ,
	SPR_SINGLE_STEP_CONTROL_0 ,
	SPR_MPL_SINGLE_STEP_0 ,
	SPR_MPL_SINGLE_STEP_0_SET_0 ,
	SPR_MPL_SINGLE_STEP_0_SET_1 ,
	SPR_MPL_SINGLE_STEP_0_SET_2 ,
	SPR_MPL_SINGLE_STEP_0_SET_3 ,
	SPR_SINGLE_STEP_CONTROL_1 ,
	SPR_MPL_SINGLE_STEP_1 ,
	SPR_MPL_SINGLE_STEP_1_SET_0 ,
	SPR_MPL_SINGLE_STEP_1_SET_1 ,
	SPR_MPL_SINGLE_STEP_1_SET_2 ,
	SPR_MPL_SINGLE_STEP_1_SET_3 ,
	SPR_SINGLE_STEP_CONTROL_2 ,
	SPR_MPL_SINGLE_STEP_2 ,
	SPR_MPL_SINGLE_STEP_2_SET_0 ,
	SPR_MPL_SINGLE_STEP_2_SET_1 ,
	SPR_MPL_SINGLE_STEP_2_SET_2 ,
	SPR_MPL_SINGLE_STEP_2_SET_3 ,
	SPR_SINGLE_STEP_CONTROL_3 ,
	SPR_MPL_SINGLE_STEP_3 ,
	SPR_MPL_SINGLE_STEP_3_SET_0 ,
	SPR_MPL_SINGLE_STEP_3_SET_1 ,
	SPR_MPL_SINGLE_STEP_3_SET_2 ,
	SPR_MPL_SINGLE_STEP_3_SET_3 ,
	SPR_MPL_SWINT_0 ,
	SPR_MPL_SWINT_0_SET_0 ,
	SPR_MPL_SWINT_0_SET_1 ,
	SPR_MPL_SWINT_0_SET_2 ,
	SPR_MPL_SWINT_0_SET_3 ,
	SPR_MPL_SWINT_1 ,
	SPR_MPL_SWINT_1_SET_0 ,
	SPR_MPL_SWINT_1_SET_1 ,
	SPR_MPL_SWINT_1_SET_2 ,
	SPR_MPL_SWINT_1_SET_3 ,
	SPR_MPL_SWINT_2 ,
	SPR_MPL_SWINT_2_SET_0 ,
	SPR_MPL_SWINT_2_SET_1 ,
	SPR_MPL_SWINT_2_SET_2 ,
	SPR_MPL_SWINT_2_SET_3 ,
	SPR_MPL_SWINT_3 ,
	SPR_MPL_SWINT_3_SET_0 ,
	SPR_MPL_SWINT_3_SET_1 ,
	SPR_MPL_SWINT_3_SET_2 ,
	SPR_MPL_SWINT_3_SET_3 ,
	SPR_TILE_TIMER_CONTROL ,
	SPR_MPL_TILE_TIMER ,
	SPR_MPL_TILE_TIMER_SET_0 ,
	SPR_MPL_TILE_TIMER_SET_1 ,
	SPR_MPL_TILE_TIMER_SET_2 ,
	SPR_MPL_TILE_TIMER_SET_3 ,
	SPR_MPL_UDN_ACCESS ,
	SPR_MPL_UDN_ACCESS_SET_0 ,
	SPR_MPL_UDN_ACCESS_SET_1 ,
	SPR_MPL_UDN_ACCESS_SET_2 ,
	SPR_MPL_UDN_ACCESS_SET_3 ,
	SPR_UDN_DATA_AVAIL ,
	SPR_UDN_DEMUX_COUNT_0 ,
	SPR_UDN_DEMUX_COUNT_1 ,
	SPR_UDN_DEMUX_COUNT_2 ,
	SPR_UDN_DEMUX_COUNT_3 ,
	SPR_UDN_PENDING ,
	SPR_UDN_SP_FIFO_CNT ,
	SPR_UDN_FLUSH_EGRESS ,
	SPR_UDN_ROUTE_ORDER ,
	SPR_MPL_UDN_AVAIL ,
	SPR_MPL_UDN_AVAIL_SET_0 ,
	SPR_MPL_UDN_AVAIL_SET_1 ,
	SPR_MPL_UDN_AVAIL_SET_2 ,
	SPR_MPL_UDN_AVAIL_SET_3 ,
	SPR_UDN_AVAIL_EN ,
	SPR_UDN_COMPLETE_PENDING ,
	SPR_MPL_UDN_COMPLETE ,
	SPR_MPL_UDN_COMPLETE_SET_0 ,
	SPR_MPL_UDN_COMPLETE_SET_1 ,
	SPR_MPL_UDN_COMPLETE_SET_2 ,
	SPR_MPL_UDN_COMPLETE_SET_3 ,
	SPR_MPL_UDN_FIREWALL ,
	SPR_MPL_UDN_FIREWALL_SET_0 ,
	SPR_MPL_UDN_FIREWALL_SET_1 ,
	SPR_MPL_UDN_FIREWALL_SET_2 ,
	SPR_MPL_UDN_FIREWALL_SET_3 ,
	SPR_UDN_DIRECTION_PROTECT ,
	SPR_MPL_UDN_TIMER ,
	SPR_MPL_UDN_TIMER_SET_0 ,
	SPR_MPL_UDN_TIMER_SET_1 ,
	SPR_MPL_UDN_TIMER_SET_2 ,
	SPR_MPL_UDN_TIMER_SET_3 ,
	SPR_UDN_DEADLOCK_COUNT ,
	SPR_UDN_DEADLOCK_TIMEOUT ,
	SPR_MPL_UNALIGN_DATA ,
	SPR_MPL_UNALIGN_DATA_SET_0 ,
	SPR_MPL_UNALIGN_DATA_SET_1 ,
	SPR_MPL_UNALIGN_DATA_SET_2 ,
	SPR_MPL_UNALIGN_DATA_SET_3 ,
	SPR_DSTREAM_PF ,
	SPR_PASS ,
	SPR_FAIL ,
	SPR_DONE ,
	SPR_CYCLE ,
	SPR_PSEUDO_RANDOM_NUMBER ,
	SPR_PROC_STATUS ,
	SPR_STATUS_SATURATE ,
	SPR_CMPEXCH_VALUE ,
	SPR_TILE_COORD ,
	SPR_TILE_RTF_HWM ,
	SPR_MPL_WORLD_ACCESS ,
	SPR_MPL_WORLD_ACCESS_SET_0 ,
	SPR_MPL_WORLD_ACCESS_SET_1 ,
	SPR_MPL_WORLD_ACCESS_SET_2 ,
	SPR_MPL_WORLD_ACCESS_SET_3 ,
	SPR_INTERRUPT_CRITICAL_SECTION ,
	SPR_SIM_SOCKET ,
	SPR_SIM_CONTROL ,
	SPR_EVENT_BEGIN ,
	SPR_EVENT_END ,
	SPR_MAX
}spr_t;

/* This array defines the minimum protection level associated with a special 
purpose register. The spr number is an index to the array. e.g. to get minimum
protection level for SPR_SIM_SOCKET use :-

mpl_ty sim_socket_mpl = spr_mpl[SPR_SIM_SOCKET];

*/

extern mpl_ty spr_mpl[SPR_MAX];

/* This array defines the address of special purpose registers in the special 
purpose register file. The spr number is an index to the array. e.g. to get address
for SPR_SIM_SOCKET use :-

uint64_t  sim_socket_addr = spr_addr [SPR_SIM_SOCKET];

*/

extern uint64_t spr_addr[SPR_MAX];

/*auxiliary performance counter-0 */
typedef struct {
	uint64_t	aux_perf_count_0	:	32;
	uint64_t	reserved 	: 32;
}aux_perf_count_0_t;

/*auxiliary performance counter-1 */
typedef struct {
	uint64_t	aux_perf_count_1	:	32;
	uint64_t	reserved 	: 32;
}aux_perf_count_1_t;

/*performance counter control */
typedef struct {
	uint64_t	count_0_sel	:	6;
	uint64_t	count_0_box	:	3	;
	uint64_t	reserved_1 	: 1;
	uint64_t	count_0_mask	:	4	;
	uint64_t	reserved_2 	: 2;
	uint64_t	count_1_sel	:	6	;
	uint64_t	count_1_box	:	3	;
	uint64_t	reserved_3 	: 1;
	uint64_t	count_1_mask	:	4	;
	uint64_t	reserved_4 	: 34;
}aux_perf_count_ctl_t;

enum {
	NONE,
	MBOX,
	CBOX,
	SBOX,
	NETWORK,
	BCST,
	TOP,
	SPCL
};

/*auxiliary performance counter status */
typedef struct {
	uint64_t	overflow	:	2;
	uint64_t	reserved_1 	: 62;
}aux_perf_count_sts_t;

/*auxiliary tile timer */
typedef struct {
	uint64_t	count	:	32;
	uint64_t	reserved_1 	: 32;
}aux_tile_timer_control_t;

/*minimum protection level for auxiliary tile timer */
typedef struct {
	uint64_t	mpl	:	2;
	uint64_t	reserved_1 	: 62;
}mpl_t;

typedef mpl_t mpl_aux_tile_timer_t;

/*mpl_aux_tile_timer set to protection level 0 */
typedef struct {
	uint64_t	set	:	1;
	uint64_t	reserved_1 	: 63;
} set_t;

typedef set_t mpl_aux_tile_timer_set_0_t;

/*mpl_aux_tile_timer set to protection level 1 */
typedef set_t	mpl_aux_tile_timer_set_1_t;

/*mpl_aux_tile_timer set to protection level 2 */
typedef set_t	mpl_aux_tile_timer_set_2_t;

/*mpl_aux_tile_timer set to protection level 3 */
typedef set_t	mpl_aux_tile_timer_set_3_t;

/*trace control */
typedef struct {
	uint64_t	sample_enable	:	1	;
	uint64_t	idn_enable	:	1	;
	uint64_t	reserved_1 	: 2;
	uint64_t	tile_y	:	4;
	uint64_t	tile_x	:	4	;
	uint64_t	length	:	7	;
	uint64_t	rid	:	1	;
	uint64_t	reserved_2	:	8	;
	uint64_t	invert_trigger	:	4	;
	uint64_t	trigger_mode	:	3	;
	uint64_t	reserved_3	:	29	;	
}diag_trace_ctl_t;

enum {
	AND4,
	OR4,
	AND22OR,
	AND3OR,
	OR22AND,
	OR3AND,
	SEQ,
	ALWAYS
};

/*trace status */
typedef struct {
	uint64_t	sample_valid	: 	1;
	uint64_t	idn_in_prog	:	1;
	uint64_t	idn_sw_error	:	1;
	uint64_t	reserved_1	:	5;
	uint64_t	overrun_count	:	8;
	uint64_t	reserved_2	:	48;
}diag_trace_sts_t;

/*trace data */
typedef	uint64_t	diag_trace_data_t;

/*broadcast control */
typedef struct {
	uint64_t	enable_0	:	1;
	uint64_t	enable_1	:	1;
	uint64_t	enable_2	:	1;
	uint64_t	enable_3	:	1;
	uint64_t	reserved_1	:	60;
}diag_bcst_ctl_t;

/*broadcast mask */
typedef struct {
	uint64_t	n	:	4	;
	uint64_t	s	:	4	;
	uint64_t	e	:	4	;
	uint64_t	w	:	4;
	uint64_t	reserved_1	:	48;
}diag_bcst_mask_t;

/*diag broadcast trigger */
typedef struct {
	uint64_t	pulse	:	4	;
	uint64_t	level	:	4;
	uint64_t	reserved_1	:	56;
}diag_bcst_trigger_t;

/*diag quiesce control */
typedef struct {
	uint64_t	ii_quiesce_request	:	1;
	uint64_t	set_ii_qr	:	1;
	uint64_t	clr_ii_qr	:	1;
	uint64_t	local_ii_qr_enable	:	1;
	uint64_t	bcst_in_ii_qr_enable	:	4;
	uint64_t	bcst_out_ii_qr_enable	:	4	;
	uint64_t	perf_event_ii_qr_enable_0	:	1;
	uint64_t	perf_event_ii_qr_enable_1	:	1;
	uint64_t	aux_perf_event_ii_qr_enable_0	:	1;
	uint64_t	aux_perf_event_ii_qr_enable_1	:	1;
	uint64_t	reserved_1	:	8;
	uint64_t	ct_quiesce_request	:	1;
	uint64_t	set_ct_qr	:	1;
	uint64_t	clr_ct_qr	:	1;
	uint64_t	local_ct_qr_enable	:	1;
	uint64_t	bcst_in_ct_qr_enable	:	4	;
	uint64_t	bcst_out_ct_qr_enable	:	4	;		
	uint64_t	perf_event_ct_qr_enable_0	:	1;
	uint64_t	perf_event_ct_qr_enable_1	:	1;
	uint64_t	aux_perf_event_ct_qr_enable_0	:	1;
	uint64_t	aux_perf_event_ct_qr_enable_1	:	1;
	uint64_t	reserved	:	16;
}quiesce_ctl_t;

/*diag mux control */
typedef struct {
	uint64_t	m_sel	:	3	;
	uint64_t	c_sel	:	3	;
	uint64_t	s_sel	:	3	;
	uint64_t	n_sel	:	3	;
	uint64_t	top_sel	:	4	;
	uint64_t	reserved_1	:	48	;
}diag_mux_ctl_t;

enum{
	/*NONE, removed because of redefinition */
	M_C_S_N=1,
	PASS_DATA,
	PASS_WRITE
};

/*performance counter protection levels */
typedef struct {
	uint64_t	mask	:	4	;
	uint64_t	aux_mask	:	4;
	uint64_t	reserved_1	:	56;
}perf_count_pls_t;

/*idn demux buffer threshold */	
typedef struct {
	uint64_t	idn_demux_buf_thresh	:	8;
	uint64_t	reserved_1	:	56;
}idn_demux_buf_thresh_t;

/*sbox configuration */
typedef struct {
	uint64_t	pf_limit	:	2;
	uint64_t	pf_l2_limit	:	2;
	uint64_t	force_drain	:	1;
	uint64_t	l1i_tag_parity_enable	:	1;
	uint64_t	l1i_data_parity_enable	:	1;
	uint64_t	reserved_1	:	4;
	uint64_t	prn_mode	:	1;
	uint64_t	dpf_enable	:	1;
	uint64_t	reserved_2	:	51;	
}sbox_config_t;

/*level 1 icache pin way 0 */
typedef uint64_t	l1_i_pin_way_0_t;

/*rshim location */
typedef struct {
	uint64_t	reserved_1	:	7;
	uint64_t	y_coord	:	4	;
	uint64_t	reserved_2	:	7;
	uint64_t	x_coord	:	4;
	uint64_t	reserved_3	:	42;
}rshim_coord_t;

/*instruction address attribute register entry */
typedef struct {
	uint64_t	physical_memory_mode	:	1;
	uint64_t	reserved_1	:	16;
	uint64_t	memory_attribute	:	2;
	uint64_t	cache_home_mapping	:	2	;
	uint64_t	no_l1d_allocation	:	1;
	uint64_t	adaptive_allocation	:	1;
	uint64_t	pin	:	1;
	uint64_t	reserved_2	:1;
	uint64_t	cache_prefetch	:	1;
	uint64_t	location_y_or_page_offset	:	11	;
	uint64_t	location_x_or_page_mask	:	11	;
	uint64_t	reserved_3	:16;	
}i_aar_t;

enum{
	HASH,
	TILE=3
};

enum{
	COHERENT,
	NONCOHERENT,
	UNCACHEABLE,
	MMIO
};

/*pseudo random number */
typedef uint64_t	pseudo_random_number_modify_t;

/*cycle counter modification */
typedef uint64_t	cycle_modify_t;

/*cbox memory controller map 0 */
typedef struct {
	uint64_t	reserved_1	:	7;
	uint64_t	y_coord	:	11;
	uint64_t	x_coord	:	11;
	uint64_t	reserved_2	:	35;	
}cbox_mmap_0_t;

/*cbox memory controller map 1 */
typedef struct {
	uint64_t	reserved_1	:	7;
	uint64_t	y_coord	:	11;
	uint64_t	x_coord	:	11;
	uint64_t	reserved_2	:	35;	
}cbox_mmap_1_t;

/*cbox memory controller map 2 */
typedef struct {
	uint64_t	reserved_1	:	7;
	uint64_t	y_coord	:	11;
	uint64_t	x_coord	:	11;
	uint64_t	reserved_2	:	35;	
}cbox_mmap_2_t;

/*cbox memory controller map 3 */
typedef struct {
	uint64_t	reserved_1	:	7;
	uint64_t	y_coord	:	11;
	uint64_t	x_coord	:	11;
	uint64_t	reserved_2	:	35;	
}cbox_mmap_3_t;

/*cbox cache home map address */
typedef struct {
	uint64_t	index	:	7;
	uint64_t	reserved_1	: 56;
	uint64_t	r	:	1;
}cbox_home_map_addr_t;

/*cbox cache home map data */
typedef struct {
	uint64_t	frac	:	7;
	uint64_t	tile_id_b	:	8;
	uint64_t	tile_id_a	:	8;
	uint64_t	reserved	:	41;
}cbox_home_map_data_t;

/*cbox msr */
typedef struct {
	uint64_t	maf_timeout	:	2;
	uint64_t	maf_timeout_disable	:	1;
	uint64_t	sdn_timeout	:	2	;
	uint64_t	sdn_timeout_disable	:	1	;
	uint64_t	rdn_qdn_timeout	:	2	;
	uint64_t	rdn_qdn_timeout_disable	:	1	;
	uint64_t	ireq_timeout	:	2	;
	uint64_t	ireq_timeout_disable	:	1	;
	uint64_t	rtf_timeout	:	2	;
	uint64_t	rtf_timeout_disable	:	1	;
	uint64_t	starvation_test	:	1;
	uint64_t	mdf_timeout	:	2	;
	uint64_t	mdf_timeout_disable	:	1	;
	uint64_t	mmio_timeout	:	2	;
	uint64_t	mmio_timeout_disable	:	1	;
	uint64_t	remote_request_wr_hwm	:	2	;
	uint64_t	qdn_request_wr_hwm	:	2	;
	uint64_t	mmio_request_wr_hwm :	2	;
	uint64_t	rtf_clock_gating_disable	:	1	;
	uint64_t	invalidate_l1d_fillpending	:	1	;
	uint64_t	xdn_attr_send_copy_disable	:	1	;
	uint64_t	full_cacheline_write_fetch	:	1	;
	uint64_t	early_fill_disable	:	1	;
	uint64_t	non_temporal_write_disable	:	1	;
	uint64_t	skf_issue_mode	:	1	;
	uint64_t	tile_rtf_hwm_min	:	4	;
	uint64_t	tile_rtf_hwm_max	:	4	;
	uint64_t	rtf_stall_sdn_hwm	:	5	;
	uint64_t	sdn_stall_control	:	2	;
	uint64_t	skf_stall_mp_hwm	:	4	;
	uint64_t	maf_stall_mp_hwm	:	4	;
	uint64_t	maf_stall_sdn_hwm	:	4	;
	uint64_t	reserved_1	:2;	
}cbox_msr_t;

/*level ii cache as ram configuration */
typedef struct {
	uint64_t	enable	:	1;
	uint64_t	enable_way	:	3;
	uint64_t	reserved_1	:	60;
}cbox_cacheasram_config_t;

/*level ii cache configuration */
typedef struct {
	uint64_t	disabled_ways	:	4;
	uint64_t	reserved_1	:	60;
}cbox_cache_config_t;

/*big endian configuration */
typedef	struct {
	uint64_t	big_endian_config : 1;
	uint64_t	reserved_1	:	63;
}big_endian_config_t;

/*memory stripe configuration */
typedef struct {
	uint64_t	mode0	:	2;
	uint64_t	mode1	:	2	;
	uint64_t	mode2	:	2	;
	uint64_t	mode3	:	2	;
	uint64_t	hash_enable	:	3	;
	uint64_t	reserved_1	:	53;	
}mem_stripe_config_t;

/*xdn round robin arbitration control */
typedef	struct {
	uint64_t	qdn_rra_en	:	1;
	uint64_t	rdn_rra_en	:	1;
	uint64_t	sdn_rra_en	:	1;
	uint64_t	idn_rra_en	:	1;
	uint64_t	udn_rra_en	:	1;
	uint64_t	reserved_1	:	59;
}xdn_round_robin_arb_ctl_t;

/*xdn core starvation count */
typedef struct {
	uint64_t	xdn_core_starvation_count	:	10;
	uint64_t	reserved_1	:	54;
}xdn_core_starvation_count_t	;
	

/*cache invalidation mask compression mode */
typedef struct {
	uint64_t	cache_invalidation_compression_mode	:	2;
	uint64_t	reserved_1	:	62;
}cache_invalidation_compression_mode_t;

enum{
	NO_COMPRESSION,
	X_COMPRESSION,
	Y_COMPRESSION,
	X_Y_COMPRESSION
};

/*cache invalidation mask register 0 */
typedef struct {
	uint64_t	entry_00	:	4	;
	uint64_t	entry_01	:	4	;
	uint64_t	entry_02	:	4	;
	uint64_t	entry_03	:	4	;
	uint64_t	entry_04	:	4	;
	uint64_t	entry_05	:	4	;
	uint64_t	entry_06	:	4	;
	uint64_t	entry_07	:	4	;
	uint64_t	entry_08	:	4	;
	uint64_t	entry_09	:	4	;
	uint64_t	entry_10	:	4	;
	uint64_t	entry_11	:	4	;
	uint64_t	entry_12	:	4	;
	uint64_t	entry_13	:	4	;
	uint64_t	entry_14	:	4	;
	uint64_t	entry_15	:	4	;
}cache_invalidation_mask_0_t	;

/*cache invalidation mask register 1 */
typedef struct {
	uint64_t	entry_16	:	4	;
	uint64_t	entry_17	:	4	;
	uint64_t	entry_18	:	4	;
	uint64_t	entry_19	:	4	;
	uint64_t	entry_20	:	4	;
	uint64_t	entry_21	:	4	;
	uint64_t	entry_22	:	4	;
	uint64_t	entry_23	:	4	;
	uint64_t	entry_24	:	4	;
	uint64_t	entry_25	:	4	;
	uint64_t	entry_26	:	4	;
	uint64_t	entry_27	:	4	;
	uint64_t	entry_28	:	4	;
	uint64_t	entry_29	:	4	;
	uint64_t	entry_30	:	4	;
	uint64_t	entry_31	:	4	;
}cache_invalidation_mask_1_t	;
		
/*cache invalidation mask register 1 */
typedef struct {
	uint64_t	entry_32	:	4	;
	uint64_t	entry_33	:	4	;
	uint64_t	entry_34	:	4	;
	uint64_t	entry_35	:	4	;
	uint64_t	reserved_1	:	48	;
}cache_invalidation_mask_2_t;

/*mem route order */
typedef	struct {
	uint64_t	qdn_route_x_first	:	1	;
	uint64_t	rdn_route_x_first	:	1	;
	uint64_t	sdn_route_x_first	:	1	;
	uint64_t	reserved_1	:	61	;
}mem_route_order_t;

/*minimum protection level for boot access */
typedef mpl_t	mpl_boot_access_t;

/*mpl_boot_access set to protection level 0 */
typedef	set_t	mpl_boot_access_set_0_t;

/*mpl_boot_access set to protection level 1 */
typedef	set_t	mpl_boot_access_set_1_t;


/*mpl_boot_access set to protection level 2 */
typedef	set_t	mpl_boot_access_set_2_t;


/*mpl_boot_access set to protection level 3 */
typedef	set_t	mpl_boot_access_set_3_t;

/*udn demux buffer threshold */	
typedef struct {
	uint64_t	udn_demux_buf_thresh	:	8 ;
	uint64_t	reserved_1	:	56	;
}udn_demux_buf_thresh_t	;
		
/*data address space identifier */
typedef struct {
	uint64_t	d_asid	:	8;
	uint64_t	reserved_1	:	56	;
}d_asid_t;

/*minimum protection level for data address space identifier access */
typedef mpl_t	mpl_d_asid_t;

/*mpl_d_asid set to protection level 0 */
typedef	set_t	mpl_d_asid_set_0_t;

/*mpl_d_asid set to protection level 1 */
typedef	set_t	mpl_d_asid_set_1_t;

/*mpl_d_asid set to protection level 2 */
typedef	set_t	mpl_d_asid_set_2_t;

/*mpl_d_asid set to protection level 3 */
typedef	set_t	mpl_d_asid_set_3_t;

/*last interrupt reason */
typedef struct {
	uint64_t	last_reason :	6	;
	uint64_t	reserved_1	:	2	;
	uint64_t	last_last_reason	:	6	;
	uint64_t	reserved_2	:	50	;
}last_interrupt_reason_t	;

/*minimum protection level for double fault interrupt */
typedef mpl_t	mpl_double_fault_t;

/*mpl_double_fault set to protection level 0 */
typedef	set_t	mpl_double_fault_set_0_t;

/*mpl_double_fault set to protection level 1 */
typedef	set_t	mpl_double_fault_set_1_t;

/*mpl_double_fault set to protection level 2 */
typedef	set_t	mpl_double_fault_set_2_t;

/*mpl_double_fault set to protection level 3 */
typedef	set_t	mpl_double_fault_set_3_t;

/*minimum protection level for dtlb access violation */
typedef mpl_t	mpl_dtlb_access_t;

/*mpl_dtlb_access set to protection level 0 */
typedef	set_t	mpl_dtlb_access_set_0_t;

/*mpl_dtlb_access set to protection level 1 */
typedef	set_t	mpl_dtlb_access_set_1_t;

/*mpl_dtlb_access set to protection level 2 */
typedef	set_t	mpl_dtlb_access_set_2_t;

/*mpl_dtlb_access set to protection level 3 */
typedef	set_t	mpl_dtlb_access_set_3_t;

/*address extension register entry 1 */
typedef struct {
	uint64_t	physical_memory_mode	:	1;
	uint64_t	reserved_1	:	16;
	uint64_t	memory_attribute	:	2;
	uint64_t	cache_home_mapping	:	2	;
	uint64_t	no_l1d_allocation	:	1	;
	uint64_t	adaptive_allocation	:	1	;
	uint64_t	pin	:	1	;
	uint64_t	reserved_2	:	1	;
	uint64_t	cache_prefetch	:	1	;
	uint64_t	location_y_or_page_offset	:	11	;
	uint64_t	location_x_or_page_mask	:	11	;
	uint64_t 	reserved_3	:	16;		
}aar_t;

/*data tlb entry va */
typedef struct {
	uint64_t	reserved_1	:	12;	
	uint64_t	vpn	:	52;
}dtlb_current_va_t;

/*data tlb entry pa */
typedef struct {
	uint64_t	reserved_1	:	12;
	uint64_t	pfn	:	28;
	uint64_t	reserved_2	:	24;
}dtlb_current_pa_t;

/*data tlb entry attr */
typedef struct {
	uint64_t	v	:	1	;
	uint64_t	w	:	1	;
	uint64_t	mpl :	2	;
	uint64_t	ps	:	4	;
	uint64_t	g	:	1	;
	uint64_t	asid	:	8	;
	uint64_t	memory_attribute	:	2	;
	uint64_t	cache_home_mapping	:	2	;
	uint64_t	no_l1d_allocation	:	1	;
	uint64_t	adaptive_allocation	:	1	;
	uint64_t	pin	:	1	;
	uint64_t	reserved_1	:	1	;
	uint64_t	cache_prefetch	:	1	;
	uint64_t	location_y_or_page_offset	:	11	;
	uint64_t	location_x_or_page_mask	:	11	;
	uint64_t 	reserved_2	:	16;
}dtlb_current_attr_t;

/* page size ps options */
enum {
	PAGE_4K,
	PAGE_16K,
	PAGE_64K,
	PAGE_256K,
	PAGE_1M,
	PAGE_4M,
	PAGE_16M,
	PAGE_256M,
	PAGE_1G,
	PAGE_4G,
	PAGE_16G,
	PAGE_64G
};

/*data tlb index */
typedef struct {
	uint64_t	index	:	12;
	uint64_t	reserved_1	:	50;
	uint64_t	l	:	1;
	uint64_t	r	:	1;
}dtlb_index_t;

/*data tlb wired entries */
typedef struct {
	uint64_t	index	:	12;
	uint64_t	reserved_1	:	52;
}wired_dtlb_t;

/*cache pinned ways */
typedef struct {
	uint64_t	mp_enable	:	8;
	uint64_t	io_enable	:	8	;
	uint64_t	l1d_enable	:	2	;
	uint64_t	reserved_1	: 46 ;
}cache_pinned_ways_t;

/*data tlb replacement index */
typedef struct {
	uint64_t	index	:	12;
	uint64_t	reserved_1	:	52;
}replacement_dtlb_t;

/*data tlb number of entries */
typedef struct {
	uint64_t	num	:	13;
	uint64_t	reserved_1	:	51;
}number_dtlb_t;

/*data tlb match result */
typedef struct {
	uint64_t	dtlb_match_0	:	32;
	uint64_t	reserved_1	:	32;
}dtlb_match_0_t;

/*data tlb performance monitor configuration */
typedef struct {
	uint64_t	match_entries	:	32;
	uint64_t	reserved_1	:	32;
}dtlb_perf_t;

/*data tlb tsb base address 0 */
typedef struct {
	uint64_t	entries :	3;
	uint64_t	reserved_1	:	1;
	uint64_t	ps	:	4	;
	uint64_t	size	:	2	;
	uint64_t	reserved_2	:	3;
	uint64_t	tsb_base_high	:	29	;
	uint64_t	base_ext	:	22	;
}dtlb_tsb_base_addr_0_t;

/*data tlb tsb address 0 */
typedef uint64_t	dtlb_tsb_addr_0_t;

/*data tlb entry attr on tsb fill match 0 */
typedef struct {
	uint64_t	v	:	1	;
	uint64_t	w	:	1	;
	uint64_t	mpl :	2	;
	uint64_t	ps	:	4	;
	uint64_t	g	:	1	;
	uint64_t	asid	:	8	;
	uint64_t	memory_attribute	:	2	;
	uint64_t	cache_home_mapping	:	2	;
	uint64_t	no_l1d_allocation	:	1	;
	uint64_t	adaptive_allocation	:	1	;
	uint64_t	pin	:	1	;
	uint64_t	reserved_1	:	1	;
	uint64_t	cache_prefetch	:	1	;
	uint64_t	location_y_or_page_offset	:	11	;
	uint64_t	location_x_or_page_mask	:	11	;
	uint64_t	dtlbv	:	1;
	uint64_t	itlbv	:	1;
	uint64_t 	reserved_2	:	14;
}dtlb_tsb_fill_current_attr_t;

/*data tlb tsb fill match 0 */
typedef	struct {
	uint64_t	match	:	1	;
	uint64_t	reserved_1	:	63	;
}dtlb_tsb_fill_match_t;

/*data tlb tsb base address 1 */
typedef struct {
	uint64_t	entries :	3;
	uint64_t	reserved_1	:	1;
	uint64_t	ps	:	4	;
	uint64_t	size	:	2	;
	uint64_t	reserved_2	:	3;
	uint64_t	tsb_base_high	:	29	;
	uint64_t	base_ext	:	22	;
}dtlb_tsb_base_addr_1_t;

/*data tlb tsb address 1 */
typedef uint64_t	dtlb_tsb_addr_1_t;

/*data tlb bad address */
typedef uint64_t	dtlb_bad_addr_t;

/*data tlb bad address reason */
typedef	struct {
	uint64_t	write	:	1	;
	uint64_t	reserved_1	:	63	;
}dtlb_bad_addr_reason_t;

/*minimum protection level for dtlb miss */
typedef mpl_t	mpl_dtlb_miss_t;

/*mpl_dtlb_miss set to protection level 0 */
typedef	set_t	mpl_dtlb_miss_set_0_t;

/*mpl_dtlb_miss set to protection level 1 */
typedef	set_t	mpl_dtlb_miss_set_1_t;

/*mpl_dtlb_miss set to protection level 2 */
typedef	set_t	mpl_dtlb_miss_set_2_t;

/*mpl_dtlb_miss set to protection level 3 */
typedef	set_t	mpl_dtlb_miss_set_3_t;

/*general protection violation reason */
typedef struct {
	uint64_t	spr_index	:	14;
	uint64_t	reserved_1	:	15;
	uint64_t	mf_error	:	1;
	uint64_t	mt_error	:	1;
	uint64_t	iret_error	:	1;
	uint64_t	reserved_2	:	32;
}gpv_reason_t;

/*minimum protection level for a general protection violation */
typedef mpl_t	mpl_gpv_t;

/*mpl_gpv set to protection level 0 */
typedef	set_t	mpl_gpv_set_0_t;

/*mpl_gpv set to protection level 1 */
typedef	set_t	mpl_gpv_set_1_t;

/*mpl_gpv set to protection level 2 */
typedef	set_t	mpl_gpv_set_2_t;

/*mpl_gpv set to protection level 3 */
typedef	set_t	mpl_gpv_set_3_t;

/*instruction address space identifier */
typedef struct {
	uint64_t	i_asid	:	8;
	uint64_t	reserved_1	:	56;
	
}i_asid_t;

/*minimum protection level for instruction address space identifier access */
typedef mpl_t	mpl_i_asid_t;

/*mpl_i_asid set to protection level 0 */
typedef	set_t	mpl_i_asid_set_0_t;

/*mpl_i_asid set to protection level 1 */
typedef	set_t	mpl_i_asid_set_1_t;

/*mpl_i_asid set to protection level 2 */
typedef	set_t	mpl_i_asid_set_2_t;

/*mpl_i_asid set to protection level 3 */
typedef	set_t	mpl_i_asid_set_3_t;

/*io dynamic network data available */
typedef	struct {
	uint64_t	avail_0	:	1	;
	uint64_t	avail_1	:	1	;
	uint64_t	eop_0	:	1	;
	uint64_t	eop_1	:	1	;
	uint64_t	reserved_1	:	60	;
}idn_data_avail_t;

/*io dynamic network demultiplexor count 0 */
typedef uint64_t	idn_demux_count_0_t;

/*io dynamic network demultiplexor count 1 */
typedef uint64_t	idn_demux_count_1_t;

/*idn words pending */
typedef struct {
	uint64_t	idn_pending	:	8;
	uint64_t	reserved_1	:	56	;
}idn_pending_t;

/*idn switch point fifo count */
typedef struct {
	uint64_t	n_fifo_cnt	:	2	;
	uint64_t	s_fifo_cnt	:	2	;
	uint64_t	e_fifo_cnt	:	2	;
	uint64_t	w_fifo_cnt	:	2	;
	uint64_t	c_fifo_cnt	:	4	;
	uint64_t	reserved_1 	:	52	;
}idn_sp_fifo_cnt_t;

/*idn flush egress */
typedef uint64_t	idn_flush_egress_t;

/*idn route order */
typedef	struct {
	uint64_t	route_x_first	:	1;
	uint64_t	reserved_1	:	63;
}idn_route_order_t;

/*minimum protection level for a i/o dynamic network access violation */
typedef mpl_t mpl_idn_access_t;

/*mpl_idn_access set to protection level 0 */
typedef	set_t	mpl_idn_access_set_0_t;

/*mpl_idn_access set to protection level 1 */
typedef	set_t	mpl_idn_access_set_1_t;

/*mpl_idn_access set to protection level 2 */
typedef	set_t	mpl_idn_access_set_2_t;

/*mpl_idn_access set to protection level 3 */
typedef	set_t	mpl_idn_access_set_3_t;

/*io dynamic network available enables */
typedef	struct {
	uint64_t	en_0	:	1;
	uint64_t	en_1	:	1;
	uint64_t	reserved_1	:	62;
}idn_avail_en_t;

/*minimum protection level for i/o dynamic network available */
typedef mpl_t mpl_idn_avail_t;

/*mpl_idn_avail set to protection level 0 */
typedef	set_t	mpl_idn_avail_set_0_t;

/*mpl_idn_avail set to protection level 1 */
typedef	set_t	mpl_idn_avail_set_1_t;

/*mpl_idn_avail set to protection level 2 */
typedef	set_t	mpl_idn_avail_set_2_t;

/*mpl_idn_avail set to protection level 3 */
typedef	set_t	mpl_idn_avail_set_3_t;

/*idn complete interrupt pending */
typedef	struct {
	uint64_t	pending	:	1;
	uint64_t	reserved_1	:	63;
}idn_complete_pending_t;

/*minimum protection level for i/o dynamic network complete interrupt */
typedef mpl_t mpl_idn_complete_t;

/*mpl_idn_complete set to protection level 0 */
typedef	set_t	mpl_idn_complete_set_0_t;

/*mpl_idn_complete set to protection level 1 */
typedef	set_t	mpl_idn_complete_set_1_t;

/*mpl_idn_complete set to protection level 2 */
typedef	set_t	mpl_idn_complete_set_2_t;

/*mpl_idn_complete set to protection level 3 */
typedef	set_t	mpl_idn_complete_set_3_t;

/*idn direction protect */
typedef	struct {
	uint64_t	n_protect	:	1;
	uint64_t	e_protect	:	1;
	uint64_t	s_protect	:	1;
	uint64_t	w_protect	:	1;
	uint64_t	reserved_1	:	12;
	uint64_t	n_src_violate	:	1;
	uint64_t	e_src_violate	:	1;
	uint64_t	s_src_violate	:	1;
	uint64_t	w_src_violate	:	1;
	uint64_t	m_src_violate	:	1;
	uint64_t	reserved_2	:	3;
	uint64_t	n_dest_violate	:	1;
	uint64_t	e_dest_violate	:	1;
	uint64_t	s_dest_violate	:	1;
	uint64_t	w_dest_violate	:	1;
	uint64_t	reserved_3	:	36;
}idn_direction_protect_t;

/*minimum protection level for i/o dynamic network firewall violation */
typedef mpl_t mpl_idn_firewall_t;

/*mpl_idn_firewall set to protection level 0 */
typedef	set_t	mpl_idn_firewall_set_0_t;

/*mpl_idn_firewall set to protection level 1 */
typedef	set_t	mpl_idn_firewall_set_1_t;

/*mpl_idn_firewall set to protection level 2 */
typedef	set_t	mpl_idn_firewall_set_2_t;

/*mpl_idn_firewall set to protection level 3 */
typedef	set_t	mpl_idn_firewall_set_3_t;

/*idn deadlock counter */
typedef struct {
	uint64_t	idn_deadlock_count	:	16;
	uint64_t	reserved_1	:	48;
}idn_deadlock_count_t;

/*idn deadlock timeout */
typedef struct {
	uint64_t	idn_deadlock_timeout	:	16;
	uint64_t	reserved_1	:	48;
}idn_deadlock_timeout_t;

/*minimum protection level for i/o dynamic network timer */
typedef mpl_t mpl_idn_timer_t;

/*mpl_idn_timer set to protection level 0 */
typedef	set_t	mpl_idn_timer_set_0_t;

/*mpl_idn_timer set to protection level 1 */
typedef	set_t	mpl_idn_timer_set_1_t;

/*mpl_idn_timer set to protection level 2 */
typedef	set_t	mpl_idn_timer_set_2_t;

/*mpl_idn_timer set to protection level 3 */
typedef	set_t	mpl_idn_timer_set_3_t;

/*minimum protection level for a illegal instruction */
typedef mpl_t mpl_ill_t;

/*mpl_ill set to protection level 0 */
typedef	set_t	mpl_ill_set_0_t;

/*mpl_ill set to protection level 1 */
typedef	set_t	mpl_ill_set_1_t;

/*mpl_ill set to protection level 2 */
typedef	set_t	mpl_ill_set_2_t;

/*mpl_ill set to protection level 3 */
typedef	set_t	mpl_ill_set_3_t;

/*illegal virtual address pc */
typedef uint64_t	ill_va_pc_t;

/*illegal translation reason */
typedef	struct {
	uint64_t	i_stream_va	:	1;
	uint64_t	d_stream_va	:	1;
	uint64_t	reserved_1	:	62;
}ill_trans_reason_t;

/*minimum protection level for a illegal translation */
typedef mpl_t mpl_ill_trans_t;

/*mpl_ill_trans set to protection level 0 */
typedef	set_t	mpl_ill_trans_set_0_t;

/*mpl_ill_trans set to protection level 1 */
typedef	set_t	mpl_ill_trans_set_1_t;

/*mpl_ill_trans set to protection level 2 */
typedef	set_t	mpl_ill_trans_set_2_t;

/*mpl_ill_trans set to protection level 3 */
typedef	set_t	mpl_ill_trans_set_3_t;

/*single step interrupt enable 3 pl 0-3 */
typedef struct {
	uint64_t	pl	:	4;
	uint64_t	reserved_1	:	60;
}single_step_en_3_x_t;

/*single step interrupt enable 2 pl 0-3 */
typedef struct {
	uint64_t	pl	:	4;
	uint64_t	reserved_1	:	60;
}single_step_en_2_x_t;

/*single step interrupt enable 1 pl 0-3 */
typedef struct {
	uint64_t	pl	:	4;
	uint64_t	reserved_1	:	60;
}single_step_en_1_x_t;

/*single step interrupt enable 0 pl 0-3 */
typedef struct {
	uint64_t	pl	:	4;
	uint64_t	reserved_1	:	60;
}single_step_en_0_x_t;

/*interrupt control 0-3 status */
typedef	struct {
	uint64_t	intctrl_0_status	:	1;
	uint64_t	reserved_1	:	63;
}intctrl_x_status_t;

/*interrupt vector base protection level 0-3 */
typedef struct {
	uint64_t	reserved_1	:	14;
	uint64_t	base	:	28;
	uint64_t	base_ext	:	22;
}interrupt_vector_base_x_t;
		
		
/*system save register level 0-3 entry 0 */
typedef uint64_t	system_save_x_0_t;

/*system save register level 0-3 entry 1 */
typedef uint64_t	system_save_x_1_t;

/*system save register level 0-3 entry 2 */
typedef uint64_t	system_save_x_2_t;

/*system save register level 0-3 entry 3 */
typedef uint64_t	system_save_x_3_t;

/*minimum protection level for interrupt control 0-3 */
typedef mpl_t	mpl_intctrl_x_t;

/*mpl_intctrl_0-3 set to protection level 0 */
typedef	set_t	mpl_intctrl_x_set_0_t;

/*mpl_intctrl_0-3 set to protection level 1 */
typedef	set_t	mpl_intctrl_x_set_1_t;

/*mpl_intctrl_0-3 set to protection level 2 */
typedef	set_t	mpl_intctrl_x_set_2_t;

/*mpl_intctrl_0-3 set to protection level 3 */
typedef	set_t	mpl_intctrl_x_set_3_t;

/*exceptional context protection level 0-3 entry 0 */
typedef struct {
	uint64_t	pc	:	42;
	uint64_t	reserved_1	:	22;

}ex_context_x_0_t;

/*exceptional context protection level 0-3 entry 1 */
typedef struct {
	uint64_t	pl	:	2;
	uint64_t	ics	:	1;
	uint64_t	reserved_1	:	61;

}ex_context_x_1_t;

/*interrupt mask protection level 0-3 */
typedef	struct {
	uint64_t	mask_0 	:	1;
	uint64_t	mask_1 	:	1;
	uint64_t	mask_2 	:	1;
	uint64_t	mask_3 	:	1;
	uint64_t	mask_4 	:	1;
	uint64_t	mask_5 	:	1;
	uint64_t	mask_6	:	1;
	uint64_t	reserved_1 :	13;
	uint64_t	mask_20	:	1;
	uint64_t	mask_21	:	1;
	uint64_t	mask_22	:	1;
	uint64_t	mask_23	:	1;
	uint64_t	mask_24	:	1;
	uint64_t	mask_25	:	1;
	uint64_t	mask_26	:	1;
	uint64_t	mask_27	:	1;
	uint64_t	reserved_2 :	36;	
}interrupt_mask_x_t;

/*interrupt mask protection level 0-3 */
typedef	interrupt_mask_x_t	interrupt_mask_set_x_t;

/*interrupt mask protection level 0-3 */
typedef	interrupt_mask_x_t	interrupt_mask_reset_x_t;

/*interprocessor interrupt event 0-3 */
typedef struct {
	uint64_t	event	:	32;
	uint64_t	reserved_1	:32;
}ipi_event_x_t;

/*interprocessor interrupt event set 0-3 */
typedef ipi_event_x_t ipi_event_set_x_t;

/*interprocessor interrupt event reset 0-3 */
typedef ipi_event_x_t ipi_event_reset_x_t;

/*interprocessor interrupt mask 0-3 */	
typedef struct {
	uint64_t	mask	:	32;
	uint64_t	reserved_1	:	32;
}ipi_mask_x_t	;
		
/*interprocessor interrupt mask set 0-3 */
typedef ipi_mask_x_t ipi_mask_set_x_t;

/*interprocessor interrupt mask reset 0-3 */	
typedef ipi_mask_x_t ipi_mask_reset_x_t;
	
/*minimum protection level for interprocessor interrupt 0-3 */
typedef mpl_t	mpl_ipi_x_t;

/*mpl_ipi_0-3 set to protection level 0 */
typedef	set_t	mpl_ipi_x_set_0_t;

/*mpl_ipi_0-3 set to protection level 1 */
typedef	set_t	mpl_ipi_x_set_1_t;

/*mpl_ipi_0-3 set to protection level 2 */
typedef	set_t	mpl_ipi_x_set_2_t;

/*mpl_ipi_0-3 set to protection level 3 */
typedef	set_t	mpl_ipi_x_set_3_t;

/*instruction tlb entry va */
typedef dtlb_current_va_t itlb_current_va_t;

/*instruction tlb entry pa */
typedef dtlb_current_pa_t itlb_current_pa_t;

/*instruction tlb entry attr */
typedef dtlb_current_attr_t itlb_current_attr_t;

/*instruction tlb index */
typedef dtlb_index_t itlb_index_t;

/*instruction tlb wired entries */
typedef wired_dtlb_t wired_itlb_t;

/*instruction tlb replacement index */
typedef replacement_dtlb_t replacement_itlb_t;

/*instruction tlb number of entries */
typedef number_dtlb_t number_itlb_t;

/*instruction tlb probe address */
typedef uint64_t	itlb_pr_t;

/*instruction tlb match result */
typedef struct {
	uint64_t	itlb_match_0	:	32;
	uint64_t	reserved_1	:	32;
}itlb_match_0_t;

/*instruction tlb performance monitor configuration */
typedef dtlb_perf_t itlb_perf_t;

/*instruction tlb tsb base address 0 */
typedef dtlb_tsb_base_addr_0_t	itlb_tsb_base_addr_0_t;

/*instruction tlb tsb address 0 */
typedef uint64_t	itlb_tsb_addr_0_t;

/*instruction tlb entry attr on tsb fill match */
typedef dtlb_tsb_fill_current_attr_t	itlb_tsb_fill_current_attr_t;

/*instruction tlb tsb fill match */
typedef	uint64_t	itlb_tsb_fill_match_t;

/*instruction tlb tsb base address 1 */
typedef dtlb_tsb_base_addr_1_t itlb_tsb_base_addr_1_t;

/*instruction tlb tsb address 1 */
typedef uint64_t	itlb_tsb_addr_1_t;

/*minimum protection level for itlb miss */
typedef mpl_t	mpl_itlb_miss_t;

/*mpl_itlb_miss set to protection level 0 */
typedef	set_t	mpl_itlb_miss_set_0_t;

/*mpl_itlb_miss set to protection level 1 */
typedef	set_t	mpl_itlb_miss_set_1_t;

/*mpl_itlb_miss set to protection level 2 */
typedef	set_t	mpl_itlb_miss_set_2_t;

/*mpl_itlb_miss set to protection level 3 */
typedef	set_t	mpl_itlb_miss_set_3_t;

/*xdn demultiplexor error */
typedef struct {
	uint64_t	error_entry	:	8;
	uint64_t	reserved_1	:	22;
	uint64_t	xdn_demux_error_overflow	:	1;
	uint64_t	xdn_demux_error_pending	:	1;
	uint64_t	reserved_2	:	32;
}xdn_demux_error_t;

/*sbox error */
typedef struct {
	uint64_t	index	:	11;
	uint64_t	reserved_1	:	14;
	uint64_t	data_way_0_parity	:	1;
	uint64_t	data_way_1_parity	:	1;
	uint64_t	tag_way_0_parity	:	1;
	uint64_t	tag_way_1_parity	:	1;
	uint64_t	itlb_multi_match	:	1;
	uint64_t	illegal_itlb_entry	:	1;
	uint64_t	reserved_2	:	31;
	uint64_t	overflow	:	1;
	uint64_t	l1_i	:	1;
}sbox_error_t;

/*memory error enable */
typedef struct {
	uint64_t	xdn_demux	:	1;
	uint64_t	l1_i	:	1;
	uint64_t	itlb_multi_match	:	1;
	uint64_t	illegal_itlb_entry	:	1;
	uint64_t	l1_d_data	:	1;
	uint64_t	l1_d_tag	:	1;
	uint64_t	dtlb_multi_match	:	1;
	uint64_t	illegal_dtlb_entry	:	1;
	uint64_t	illegal_atomic_attribute	:	1;
	uint64_t	illegal_opcode_attribute	:	1;
	uint64_t	l2_data_corrected	:	1;
	uint64_t	l2_data_fatal	:	1;
	uint64_t	l2_tag	:	1;
	uint64_t	l2_state	:	1;
	uint64_t	l2_rdn_write	:	1;
	uint64_t	l2_maf_timeout	:	1;
	uint64_t	share_invalidation	:	1;
	uint64_t	reserved_1	:	46;	
}mem_error_enable_t;

/*mbox memory error status */
typedef	struct  {
	uint64_t	valid	:	1;
	uint64_t	l1_d_data	:	1;
	uint64_t	l1_d_tag	:	1;
	uint64_t	dtlb_multi_match	:	1;
	uint64_t	illegal_dtlb_entry	:	1;
	uint64_t	illegal_atomic_attribute	:	1;
	uint64_t	illegal_opcode_attribute	:	1;
	uint64_t	int_l1_d_data	:	1;
	uint64_t	int_l1_d_tag	:	1;
	uint64_t	int_dtlb_multi_match	:	1;
	uint64_t	int_illegal_dtlb_entry	:	1;
	uint64_t	int_illegal_atomic_attribute	:	1;
	uint64_t	int_illegal_opcode_attribute	:	1;
	uint64_t	overflow	:	1;
	uint64_t	way0_error	:	1;
	uint64_t	way1_error	:	1;
	uint64_t	reserved_1	:	48;
} mem_error_mbox_status_t;

/*mbox memory error address */
typedef struct {
	uint64_t	mem_error_mbox_addr	:	40;
	uint64_t	reserved_1	:	24;
}mem_error_mbox_addr_t;

/*cbox memory error status */
typedef struct {
	uint64_t	valid	:	1;
	uint64_t	l2_data_corrected	:	1;
	uint64_t	l2_data_fatal	:	1;
	uint64_t	l2_tag	:	1;
	uint64_t	l2_state	:	1;
	uint64_t	l2_rdn_write	:	1;
	uint64_t	l2_rdn_read	:	1;
	uint64_t	l2_maf_timeout	:	1;
	uint64_t	share_invalidation	:	1;
	uint64_t	int_l2_data_corrected	:	1;
	uint64_t	int_l2_data_fatal	:	1;
	uint64_t	int_l2_tag	:	1;
	uint64_t	int_l2_state	:	1;
	uint64_t	int_l2_rdn_write	:	1;
	uint64_t	int_l2_rdn_read	:	1;
	uint64_t	int_l2_maf_timeout	:	1;
	uint64_t	int_share_invalidation	:	1;
	uint64_t	overflow	:	1;
	uint64_t	info_way	:	3;
	uint64_t	info	:	16	;
	uint64_t	reserved_1	:	27;	
}mem_error_cbox_status_t;

/*cbox memory error address */
typedef struct {
	uint64_t	mem_error_cbox_addr	:	40;
	uint64_t	reserved_1	:	24;
}mem_error_cbox_addr_t;

/*minimum protection level for a memory error interrupt */
typedef mpl_t	mpl_mem_error_t;

/*mpl_mem_error set to protection level 0 */
typedef	set_t	mpl_mem_error_set_0_t;

/*mpl_mem_error set to protection level 1 */
typedef	set_t	mpl_mem_error_set_1_t;

/*mpl_mem_error set to protection level 2 */
typedef	set_t	mpl_mem_error_set_2_t;

/*mpl_mem_error set to protection level 3 */
typedef	set_t	mpl_mem_error_set_3_t;

/*performance counter-0 */
typedef struct {
	uint64_t	perf_count_0	:	32;
	uint64_t	reserved_1	:	32;
}perf_count_0_t;

/*performance counter-1 */
typedef struct {
	uint64_t	perf_count_1	:	32;
	uint64_t	reserved_1	:	32;
}perf_count_1_t;

/*performance counter control */
typedef struct {
	uint64_t	count_0_sel :	6;
	uint64_t	count_0_box	:	3	;
	uint64_t	reserved_1	:	1;
	uint64_t	count_0_mask	:	4	;
	uint64_t	reserved_2	:	2;
	uint64_t	count_1_sel	:	6	;
	uint64_t	count_1_box :	3	;
	uint64_t	reserved_3	:	1;
	uint64_t	count_1_mask	:	4	;
	uint64_t	reserved_4	:	34;
}perf_count_ctl_t;

/*performance counter status */
typedef struct {
	uint64_t	overflow	:	2;
	uint64_t	reserved_1	:	62;
}perf_count_sts_t;

/*watch value register */
typedef uint64_t	watch_val_t;

/*watch mask register */
typedef uint64_t	watch_mask_t;

/*performance counter control */
typedef struct {
	uint64_t	udn_evt_port_sel	:	5;
	uint64_t	idn_evt_port_sel	:	5	;
	uint64_t	qdn_evt_port_sel	:	5	;
	uint64_t	sdn_evt_port_sel	:	5	;
	uint64_t	rdn_evt_port_sel	:	5	;
	uint64_t	reserved_1	:	39;
}perf_count_dn_ctl_t;

/*minimum protection level for performance counters */
typedef mpl_t	mpl_perf_count_t;

/*mpl_perf_count set to protection level 0 */
typedef	set_t	mpl_perf_count_set_0_t;

/*mpl_perf_count set to protection level 1 */
typedef	set_t	mpl_perf_count_set_1_t;

/*mpl_perf_count set to protection level 2 */
typedef	set_t	mpl_perf_count_set_2_t;

/*mpl_perf_count set to protection level 3 */
typedef	set_t	mpl_perf_count_set_3_t;

/*single step interrupt control 0-3 */
typedef	struct {
	uint64_t	canceled	:	1;
	uint64_t	inhibit	:	1;
	uint64_t	reserved_1	:	62;
}single_step_control_x_t;

/*minimum protection level for single step interrupt 0-3 */
typedef mpl_t	mpl_single_step_x_t;

/*mpl_single_step_0-3 set to protection level 0 */
typedef	set_t	mpl_single_step_x_set_0_t;

/*mpl_single_step_0-3 set to protection level 1 */
typedef	set_t	mpl_single_step_x_set_1_t;

/*mpl_single_step_0-3 set to protection level 2 */
typedef	set_t	mpl_single_step_x_set_2_t;

/*mpl_single_step_0-3 set to protection level 3 */
typedef	set_t	mpl_single_step_x_set_3_t;

/*minimum protection level for software interrupt 0 */
typedef mpl_t	mpl_swint_x_t;

/*mpl_swint_0-3 set to protection level 0 */
typedef	set_t	mpl_swint_x_set_0_t;

/*mpl_swint_0-3 set to protection level 1 */
typedef	set_t	mpl_swint_x_set_1_t;

/*mpl_swint_0-3 set to protection level 2 */
typedef	set_t	mpl_swint_x_set_2_t;

/*mpl_swint_0-3 set to protection level 3 */
typedef	set_t	mpl_swint_x_set_3_t;

/*tile timer control */
typedef struct {
	uint64_t	count	:	32;
	uint64_t	reserved_1	:	30;
	uint64_t	disable	:	1;
	uint64_t	underflow	:	1;
}tile_timer_control_t;

/*minimum protection level for tile timer */
typedef mpl_t	mpl_tile_timer_t;

/*mpl_tile_timer set to protection level 0 */
typedef	set_t	mpl_tile_timer_set_0_t;

/*mpl_tile_timer set to protection level 1 */
typedef	set_t	mpl_tile_timer_set_1_t;

/*mpl_tile_timer set to protection level 2 */
typedef	set_t	mpl_tile_timer_set_2_t;

/*mpl_tile_timer set to protection level 3 */
typedef	set_t	mpl_tile_timer_set_3_t;

/*minimum protection level for a user dynamic network access violation */
typedef mpl_t	mpl_udn_access_t;

/*mpl_udn_access set to protection level 0 */
typedef	set_t	mpl_udn_access_set_0_t;

/*mpl_udn_access set to protection level 1 */
typedef	set_t	mpl_udn_access_set_1_t;

/*mpl_udn_access set to protection level 2 */
typedef	set_t	mpl_udn_access_set_2_t;

/*mpl_udn_access set to protection level 3 */
typedef	set_t	mpl_udn_access_set_3_t;

/*user dynamic network data available */
typedef	struct {
	uint64_t	avail_0	:	1;
	uint64_t	avail_1	:	1;
	uint64_t	avail_2	:	1;
	uint64_t	avail_3	:	1;
	uint64_t	eop_0	:	1;
	uint64_t	eop_1	:	1;
	uint64_t	eop_2	:	1;
	uint64_t	eop_3	:	1;
	uint64_t	reserved_1	:	56;
}udn_data_avail_t;

/*user dynamic network demultiplexor count 0 */
typedef uint64_t	udn_demux_count_0_t;

/*user dynamic network demultiplexor count 1 */
typedef uint64_t	udn_demux_count_1_t;

/*user dynamic network demultiplexor count 2 */
typedef uint64_t	udn_demux_count_2_t;

/*user dynamic network demultiplexor count 3 */
typedef uint64_t	udn_demux_count_3_t;

/*udn words pending */
typedef idn_pending_t udn_pending_t;

/*udn switch point fifo count */
typedef idn_sp_fifo_cnt_t udn_sp_fifo_cnt_t;

/*udn flush egress */
typedef idn_flush_egress_t	udn_flush_egress_t;

/*udn route order */
typedef	idn_route_order_t	udn_route_order_t;

/*minimum protection level for user dynamic network available */
typedef mpl_t	mpl_udn_avail_t;

/*mpl_udn_avail set to protection level 0 */
typedef	set_t	mpl_udn_avail_set_0_t;

/*mpl_udn_avail set to protection level 1 */
typedef	set_t	mpl_udn_avail_set_1_t;

/*mpl_udn_avail set to protection level 2 */
typedef	set_t	mpl_udn_avail_set_2_t;

/*mpl_udn_avail set to protection level 3 */
typedef	set_t	mpl_udn_avail_set_3_t;

/*user dynamic network available enables */
typedef	struct {
	uint64_t	en_0	:	1;
	uint64_t	en_1	:	1;
	uint64_t	en_2	:	1;
	uint64_t	en_3	:	1;
	uint64_t	reserved_t	:	60;
}udn_avail_en_t;

/*udn complete interrupt pending */
typedef	idn_complete_pending_t	udn_complete_pending_t;

/*minimum protection level for user dynamic network complete interrupt */
typedef mpl_t	mpl_udn_complete_t;

/*mpl_udn_complete set to protection level 0 */
typedef	set_t	mpl_udn_complete_set_0_t;

/*mpl_udn_complete set to protection level 1 */
typedef	set_t	mpl_udn_complete_set_1_t;

/*mpl_udn_complete set to protection level 2 */
typedef	set_t	mpl_udn_complete_set_2_t;

/*mpl_udn_complete set to protection level 3 */
typedef	set_t	mpl_udn_complete_set_3_t;

/*minimum protection level for user dynamic network firewall violation */
typedef mpl_t	mpl_udn_firewall_t;

/*mpl_udn_firewall set to protection level 0 */
typedef	set_t	mpl_udn_firewall_set_0_t;

/*mpl_udn_firewall set to protection level 1 */
typedef	set_t	mpl_udn_firewall_set_1_t;

/*mpl_udn_firewall set to protection level 2 */
typedef	set_t	mpl_udn_firewall_set_2_t;

/*mpl_udn_firewall set to protection level 3 */
typedef	set_t	mpl_udn_firewall_set_3_t;

/*udn direction protect */
typedef	idn_direction_protect_t	udn_direction_protect_t;

/*minimum protection level for user dynamic network timer */
typedef mpl_t	mpl_udn_timer_t;

/*mpl_udn_timer set to protection level 0 */
typedef	set_t	mpl_udn_timer_set_0_t;

/*mpl_udn_timer set to protection level 1 */
typedef	set_t	mpl_udn_timer_set_1_t;

/*mpl_udn_timer set to protection level 2 */
typedef	set_t	mpl_udn_timer_set_2_t;

/*mpl_udn_timer set to protection level 3 */
typedef	set_t	mpl_udn_timer_set_3_t;

/*udn deadlock counter */
typedef idn_deadlock_count_t udn_deadlock_count_t;

/*udn deadlock timeout */
typedef idn_deadlock_timeout_t udn_deadlock_timeout_t;

/*minimum protection level for unaligned data reference interrupt */
typedef mpl_t	mpl_unalign_data_t;

/*mpl_unalign_data set to protection level 0 */
typedef	set_t	mpl_unalign_data_set_0_t;

/*mpl_unalign_data set to protection level 1 */
typedef	set_t	mpl_unalign_data_set_1_t;

/*mpl_unalign_data set to protection level 2 */
typedef	set_t	mpl_unalign_data_set_2_t;

/*mpl_unalign_data set to protection level 3 */
typedef	set_t	mpl_unalign_data_set_3_t;

/*data stream prefetch control */
typedef struct {
	uint64_t	reserved_1	:	6;
	uint64_t	stride	:	7;
	uint64_t	reserved_2	:	3;
	uint64_t	level	:	2	;
	uint64_t	reserved_3	:	45;
}dstream_pf_t;

/*pass magic register */
typedef uint64_t	pass_t;

/*fail magic register */
typedef uint64_t	fail_t;

/*done magic register */
typedef uint64_t	done_t;

/*cycle counter */
typedef uint64_t	cycle_t;

/*pseudo random number modification */
typedef uint64_t pseudo_random_number_t;

/*processor status bits */
typedef	struct {
	uint64_t	saturate	:	1;
	uint64_t	reserved_1	:	63;
}proc_status_t;

/*saturate status bits */
typedef	struct {
	uint64_t	status_saturate	:	1;
	uint64_t	reserved_1	:	63;
}	status_saturate_t;

/*atomic compare and exchange value */
typedef uint64_t	cmpexch_value_t;

/*tile coordinate */
typedef rshim_coord_t	tile_coord_t;

/*tile cache retry fifo hwm */
typedef struct {
	uint64_t	tile_rtf_hwm	:	4;
	uint64_t	reserved_1	:	60;
}tile_rtf_hwm_t;

/*minimum protection level for world access */
typedef mpl_t	mpl_world_access_t;

/*mpl_world_access set to protection level 0 */
typedef	set_t	mpl_world_access_set_0_t;

/*mpl_world_access set to protection level 1 */
typedef	set_t	mpl_world_access_set_1_t;

/*mpl_world_access set to protection level 2 */
typedef	set_t	mpl_world_access_set_2_t;

/*mpl_world_access set to protection level 3 */
typedef	set_t	mpl_world_access_set_3_t;

/*interrupt critical section */
typedef	uint64_t	interrupt_critical_section_t;

/*simulator socket pseudo register */
typedef struct {
	uint64_t	sim_socket	:	32;
	uint64_t	reserved_1	:	32;
}sim_socket_t;

/*simulator control pseudo register */
typedef struct {
	uint64_t	operator	:	8;
	uint64_t	param	:	24	;
	uint64_t	reserved_1	:	32;	
}sim_control_t;

/*event_begin magic register */
typedef struct {
	uint64_t	event_begin	:	32;
	uint64_t	reserved_1	:	32;
}event_begin_t;

/*event_end magic register */
typedef struct {
	uint64_t	event_end	:	32;
	uint64_t	reserved_1	:	32;
}event_end_t;

#endif

/*
 *  Tilera emulation cpu definitions for qemu.
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

#if !defined (__CPU_TILERA_H__)
#define __CPU_TILERA_H__

#include "config.h"
#include "qemu-common.h"
#include "spr.h"

#define CPUArchState struct CPUTileraState

#include "softfloat.h"

#define TARGET_HAS_ICE 1



#define ICACHE_LINE_SIZE 64
#define DCACHE_LINE_SIZE 32

/* Tilera major type */
enum {
    TILERA_GX  = 1,
    TILERA_PRO  = 2,
    TILERA_64 = 3
};

/* GX minor type */
enum {
    TILERA_GX_8072 = 0,
    TILERA_GX_8036 = 1,
    TILERA_GX_8016 = 2,
    TILERA_GX_8009 = 3
};

#define MASK16 0xffff

#ifdef __tilepro__	/* Tile64 and TilePro  */

#define ELF_MACHINE     EM_TILERA32

#define TARGET_PHYS_ADDR_SPACE_BITS  36
#define TARGET_VIRT_ADDR_SPACE_BITS  32
#define TARGET_LONG_BITS 32
#define TARGET_PAGE_BITS 16				/*Tilera traditionally supports 64 kbyte pages*/

#else
#define ELF_MACHINE     EM_TILERA64

#define TARGET_PHYS_ADDR_SPACE_BITS  64
#define TARGET_VIRT_ADDR_SPACE_BITS  32
#define TARGET_LONG_BITS 64
#define TARGET_PAGE_BITS  16				/*Tilera traditionally supports 64 kbyte pages*/

#define WORD_SIZE 64 					/*The size of a machine word in bits. The 
							TILE-Gx Processor is a 64-bit machine.*/
#endif



/* PRO minor type */
enum {
    TILERA_PRO64 = 1, 
    TILERA_PRO36 = 2, 
};


/* MMU definitions */

#ifndef __tilepro__

#if !defined(CONFIG_USER_ONLY)
	/* Software TLB cache */

	typedef struct  {
	    uint64_t itlbe[16];
		itlb_current_attr_t itlbe_attr[16];
		uint64_t dtlbe[32];
		dtlb_current_attr_t dtlbe_attr[32];
	} tilegx_tlb_t;

extern	tilegx_tlb_t tilegx_tlb;

#endif
#endif
#include "cpu-defs.h"


enum {
	TILE_REG_FP=52,
	TILE_REG_TP=53,
	TILE_REG_SP=54,
	TILE_REG_LR=55
};
	
typedef struct {
#ifndef __tilepro__
	aux_perf_count_0_t	aux_perf_count_0 ;
	aux_perf_count_1_t	aux_perf_count_1 ;
	aux_perf_count_ctl_t	aux_perf_count_ctl ;
	aux_perf_count_sts_t	aux_perf_count_sts ;
	aux_tile_timer_control_t	aux_tile_timer_control ;
	mpl_aux_tile_timer_t	mpl_aux_tile_timer ;
	mpl_aux_tile_timer_set_0_t	mpl_aux_tile_timer_set_0 ;
	mpl_aux_tile_timer_set_1_t	mpl_aux_tile_timer_set_1 ;
	mpl_aux_tile_timer_set_2_t	mpl_aux_tile_timer_set_2 ;
	mpl_aux_tile_timer_set_3_t	mpl_aux_tile_timer_set_3 ;
	diag_trace_ctl_t	diag_trace_ctl ;
	diag_trace_sts_t	diag_trace_sts ;
	diag_trace_data_t	diag_trace_data ;
	diag_bcst_ctl_t diag_bcst_ctl ;
	diag_bcst_mask_t	diag_bcst_mask ;
	diag_bcst_trigger_t diag_bcst_trigger ;
	quiesce_ctl_t	quiesce_ctl ;
	diag_mux_ctl_t	diag_mux_ctl ;
	perf_count_pls_t	perf_count_pls ;
	idn_demux_buf_thresh_t	idn_demux_buf_thresh ;
	sbox_config_t	sbox_config ;
	l1_i_pin_way_0_t	l1_i_pin_way_0 ;
	rshim_coord_t	rshim_coord ;
	i_aar_t i_aar ;
	pseudo_random_number_modify_t	pseudo_random_number_modify ;
	cycle_modify_t	cycle_modify ;
	cbox_mmap_0_t	cbox_mmap_0 ;
	cbox_mmap_1_t	cbox_mmap_1 ;
	cbox_mmap_2_t	cbox_mmap_2 ;
	cbox_mmap_3_t	cbox_mmap_3 ;
	cbox_home_map_addr_t	cbox_home_map_addr ;
	cbox_home_map_data_t	cbox_home_map_data ;
	cbox_msr_t	cbox_msr ;
	cbox_cacheasram_config_t	cbox_cacheasram_config ;
	cbox_cache_config_t cbox_cache_config ;
	big_endian_config_t big_endian_config ;
	mem_stripe_config_t mem_stripe_config ;
	xdn_round_robin_arb_ctl_t	xdn_round_robin_arb_ctl ;
	xdn_core_starvation_count_t xdn_core_starvation_count ;
	cache_invalidation_compression_mode_t	cache_invalidation_compression_mode ;
	cache_invalidation_mask_0_t cache_invalidation_mask_0 ;
	cache_invalidation_mask_1_t cache_invalidation_mask_1 ;
	cache_invalidation_mask_2_t cache_invalidation_mask_2 ;
	mem_route_order_t	mem_route_order ;
	mpl_boot_access_t	mpl_boot_access ;
	mpl_boot_access_set_0_t mpl_boot_access_set_0 ;
	mpl_boot_access_set_1_t mpl_boot_access_set_1 ;
	mpl_boot_access_set_2_t mpl_boot_access_set_2 ;
	mpl_boot_access_set_3_t mpl_boot_access_set_3 ;
	udn_demux_buf_thresh_t	udn_demux_buf_thresh ;
	d_asid_t	d_asid ;
	mpl_d_asid_t	mpl_d_asid ;
	mpl_d_asid_set_0_t	mpl_d_asid_set_0 ;
	mpl_d_asid_set_1_t	mpl_d_asid_set_1 ;
	mpl_d_asid_set_2_t	mpl_d_asid_set_2 ;
	mpl_d_asid_set_3_t	mpl_d_asid_set_3 ;
	last_interrupt_reason_t last_interrupt_reason ;
	mpl_double_fault_t	mpl_double_fault ;
	mpl_double_fault_set_0_t	mpl_double_fault_set_0 ;
	mpl_double_fault_set_1_t	mpl_double_fault_set_1 ;
	mpl_double_fault_set_2_t	mpl_double_fault_set_2 ;
	mpl_double_fault_set_3_t	mpl_double_fault_set_3 ;
	mpl_dtlb_access_t	mpl_dtlb_access ;
	mpl_dtlb_access_set_0_t mpl_dtlb_access_set_0 ;
	mpl_dtlb_access_set_1_t mpl_dtlb_access_set_1 ;
	mpl_dtlb_access_set_2_t mpl_dtlb_access_set_2 ;
	mpl_dtlb_access_set_3_t mpl_dtlb_access_set_3 ;
	aar_t	aar ;
	dtlb_current_va_t	dtlb_current_va ;
	dtlb_current_pa_t	dtlb_current_pa ;
	dtlb_current_attr_t dtlb_current_attr ;
	dtlb_index_t	dtlb_index ;
	wired_dtlb_t	wired_dtlb ;
	cache_pinned_ways_t cache_pinned_ways ;
	replacement_dtlb_t	replacement_dtlb ;
	number_dtlb_t	number_dtlb ;
	dtlb_match_0_t	dtlb_match_0 ;
	dtlb_perf_t dtlb_perf ;
	dtlb_tsb_base_addr_0_t	dtlb_tsb_base_addr_0 ;
	dtlb_tsb_addr_0_t	dtlb_tsb_addr_0 ;
	dtlb_tsb_fill_current_attr_t	dtlb_tsb_fill_current_attr ;
	dtlb_tsb_fill_match_t	dtlb_tsb_fill_match ;
	dtlb_tsb_base_addr_1_t	dtlb_tsb_base_addr_1 ;
	dtlb_tsb_addr_1_t	dtlb_tsb_addr_1 ;
	dtlb_bad_addr_t dtlb_bad_addr ;
	dtlb_bad_addr_reason_t	dtlb_bad_addr_reason ;
	mpl_dtlb_miss_t mpl_dtlb_miss ;
	mpl_dtlb_miss_set_0_t	mpl_dtlb_miss_set_0 ;
	mpl_dtlb_miss_set_1_t	mpl_dtlb_miss_set_1 ;
	mpl_dtlb_miss_set_2_t	mpl_dtlb_miss_set_2 ;
	mpl_dtlb_miss_set_3_t	mpl_dtlb_miss_set_3 ;
	gpv_reason_t	gpv_reason ;
	mpl_gpv_t	mpl_gpv ;
	mpl_gpv_set_0_t mpl_gpv_set_0 ;
	mpl_gpv_set_1_t mpl_gpv_set_1 ;
	mpl_gpv_set_2_t mpl_gpv_set_2 ;
	mpl_gpv_set_3_t mpl_gpv_set_3 ;
	i_asid_t	i_asid ;
	mpl_i_asid_t	mpl_i_asid ;
	mpl_i_asid_set_0_t	mpl_i_asid_set_0 ;
	mpl_i_asid_set_1_t	mpl_i_asid_set_1 ;
	mpl_i_asid_set_2_t	mpl_i_asid_set_2 ;
	mpl_i_asid_set_3_t	mpl_i_asid_set_3 ;
	idn_data_avail_t	idn_data_avail ;
	idn_demux_count_0_t idn_demux_count_0 ;
	idn_demux_count_1_t idn_demux_count_1 ;
	idn_pending_t	idn_pending ;
	idn_sp_fifo_cnt_t	idn_sp_fifo_cnt ;
	idn_flush_egress_t	idn_flush_egress ;
	idn_route_order_t	idn_route_order ;
	mpl_idn_access_t	mpl_idn_access ;
	mpl_idn_access_set_0_t	mpl_idn_access_set_0 ;
	mpl_idn_access_set_1_t	mpl_idn_access_set_1 ;
	mpl_idn_access_set_2_t	mpl_idn_access_set_2 ;
	mpl_idn_access_set_3_t	mpl_idn_access_set_3 ;
	idn_avail_en_t	idn_avail_en ;
	mpl_idn_avail_t mpl_idn_avail ;
	mpl_idn_avail_set_0_t	mpl_idn_avail_set_0 ;
	mpl_idn_avail_set_1_t	mpl_idn_avail_set_1 ;
	mpl_idn_avail_set_2_t	mpl_idn_avail_set_2 ;
	mpl_idn_avail_set_3_t	mpl_idn_avail_set_3 ;
	idn_complete_pending_t	idn_complete_pending ;
	mpl_idn_complete_t	mpl_idn_complete ;
	mpl_idn_complete_set_0_t	mpl_idn_complete_set_0 ;
	mpl_idn_complete_set_1_t	mpl_idn_complete_set_1 ;
	mpl_idn_complete_set_2_t	mpl_idn_complete_set_2 ;
	mpl_idn_complete_set_3_t	mpl_idn_complete_set_3 ;
	idn_direction_protect_t idn_direction_protect ;
	mpl_idn_firewall_t	mpl_idn_firewall ;
	mpl_idn_firewall_set_0_t	mpl_idn_firewall_set_0 ;
	mpl_idn_firewall_set_1_t	mpl_idn_firewall_set_1 ;
	mpl_idn_firewall_set_2_t	mpl_idn_firewall_set_2 ;
	mpl_idn_firewall_set_3_t	mpl_idn_firewall_set_3 ;
	idn_deadlock_count_t	idn_deadlock_count ;
	idn_deadlock_timeout_t	idn_deadlock_timeout ;
	mpl_idn_timer_t mpl_idn_timer ;
	mpl_idn_timer_set_0_t	mpl_idn_timer_set_0 ;
	mpl_idn_timer_set_1_t	mpl_idn_timer_set_1 ;
	mpl_idn_timer_set_2_t	mpl_idn_timer_set_2 ;
	mpl_idn_timer_set_3_t	mpl_idn_timer_set_3 ;
	mpl_ill_t	mpl_ill ;
	mpl_ill_set_0_t mpl_ill_set_0 ;
	mpl_ill_set_1_t mpl_ill_set_1 ;
	mpl_ill_set_2_t mpl_ill_set_2 ;
	mpl_ill_set_3_t mpl_ill_set_3 ;
	ill_va_pc_t ill_va_pc ;
	ill_trans_reason_t	ill_trans_reason ;
	mpl_ill_trans_t mpl_ill_trans ;
	mpl_ill_trans_set_0_t	mpl_ill_trans_set_0 ;
	mpl_ill_trans_set_1_t	mpl_ill_trans_set_1 ;
	mpl_ill_trans_set_2_t	mpl_ill_trans_set_2 ;
	mpl_ill_trans_set_3_t	mpl_ill_trans_set_3 ;
	single_step_en_3_x_t	single_step_en_3_0 ;
	single_step_en_2_x_t	single_step_en_2_0 ;
	single_step_en_1_x_t	single_step_en_1_0 ;
	single_step_en_0_x_t	single_step_en_0_0 ;
	intctrl_x_status_t	intctrl_0_status ;
	interrupt_vector_base_x_t	interrupt_vector_base_0 ;
	system_save_x_0_t	system_save_0_0 ;
	system_save_x_1_t	system_save_0_1 ;
	system_save_x_2_t	system_save_0_2 ;
	system_save_x_3_t	system_save_0_3 ;
	mpl_intctrl_x_t mpl_intctrl_0 ;
	mpl_intctrl_x_set_0_t	mpl_intctrl_0_set_0 ;
	mpl_intctrl_x_set_1_t	mpl_intctrl_0_set_1 ;
	mpl_intctrl_x_set_2_t	mpl_intctrl_0_set_2 ;
	mpl_intctrl_x_set_3_t	mpl_intctrl_0_set_3 ;
	ex_context_x_0_t	ex_context_0_0 ;
	ex_context_x_1_t	ex_context_0_1 ;
	interrupt_mask_x_t	interrupt_mask_0 ;
	interrupt_mask_set_x_t	interrupt_mask_set_0 ;
	interrupt_mask_reset_x_t	interrupt_mask_reset_0 ;
	single_step_en_3_x_t	single_step_en_3_1 ;
	single_step_en_2_x_t	single_step_en_2_1 ;
	single_step_en_1_x_t	single_step_en_1_1 ;
	single_step_en_0_x_t	single_step_en_0_1 ;
	intctrl_x_status_t	intctrl_1_status ;
	interrupt_vector_base_x_t	interrupt_vector_base_1 ;
	system_save_x_0_t	system_save_1_0 ;
	system_save_x_1_t	system_save_1_1 ;
	system_save_x_2_t	system_save_1_2 ;
	system_save_x_3_t	system_save_1_3 ;
	mpl_intctrl_x_t mpl_intctrl_1 ;
	mpl_intctrl_x_set_0_t	mpl_intctrl_1_set_0 ;
	mpl_intctrl_x_set_1_t	mpl_intctrl_1_set_1 ;
	mpl_intctrl_x_set_2_t	mpl_intctrl_1_set_2 ;
	mpl_intctrl_x_set_3_t	mpl_intctrl_1_set_3 ;
	ex_context_x_0_t	ex_context_1_0 ;
	ex_context_x_1_t	ex_context_1_1 ;
	interrupt_mask_x_t	interrupt_mask_1 ;
	interrupt_mask_set_x_t	interrupt_mask_set_1 ;
	interrupt_mask_reset_x_t	interrupt_mask_reset_1 ;
	single_step_en_3_x_t	single_step_en_3_2 ;
	single_step_en_2_x_t	single_step_en_2_2 ;
	single_step_en_1_x_t	single_step_en_1_2 ;
	single_step_en_0_x_t	single_step_en_0_2 ;
	intctrl_x_status_t	intctrl_2_status ;
	interrupt_vector_base_x_t	interrupt_vector_base_2 ;
	system_save_x_0_t	system_save_2_0 ;
	system_save_x_1_t	system_save_2_1 ;
	system_save_x_2_t	system_save_2_2 ;
	system_save_x_3_t	system_save_2_3 ;
	mpl_intctrl_x_t mpl_intctrl_2 ;
	mpl_intctrl_x_set_0_t	mpl_intctrl_2_set_0 ;
	mpl_intctrl_x_set_1_t	mpl_intctrl_2_set_1 ;
	mpl_intctrl_x_set_2_t	mpl_intctrl_2_set_2 ;
	mpl_intctrl_x_set_3_t	mpl_intctrl_2_set_3 ;
	ex_context_x_0_t	ex_context_2_0 ;
	ex_context_x_1_t	ex_context_2_1 ;
	interrupt_mask_x_t	interrupt_mask_2 ;
	interrupt_mask_set_x_t	interrupt_mask_set_2 ;
	interrupt_mask_reset_x_t	interrupt_mask_reset_2 ;
	single_step_en_3_x_t	single_step_en_3_3 ;
	single_step_en_2_x_t	single_step_en_2_3 ;
	single_step_en_1_x_t	single_step_en_1_3 ;
	single_step_en_0_x_t	single_step_en_0_3 ;
	intctrl_x_status_t	intctrl_3_status ;
	interrupt_vector_base_x_t	interrupt_vector_base_3 ;
	system_save_x_0_t	system_save_3_0 ;
	system_save_x_1_t	system_save_3_1 ;
	system_save_x_2_t	system_save_3_2 ;
	system_save_x_3_t	system_save_3_3 ;
	mpl_intctrl_x_t mpl_intctrl_3 ;
	mpl_intctrl_x_set_0_t	mpl_intctrl_3_set_0 ;
	mpl_intctrl_x_set_1_t	mpl_intctrl_3_set_1 ;
	mpl_intctrl_x_set_2_t	mpl_intctrl_3_set_2 ;
	mpl_intctrl_x_set_3_t	mpl_intctrl_3_set_3 ;
	ex_context_x_0_t	ex_context_3_0 ;
	ex_context_x_1_t	ex_context_3_1 ;
	interrupt_mask_x_t	interrupt_mask_3 ;
	interrupt_mask_set_x_t	interrupt_mask_set_3 ;
	interrupt_mask_reset_x_t	interrupt_mask_reset_3 ;
	ipi_event_x_t	ipi_event_0 ;
	ipi_event_set_x_t	ipi_event_set_0 ;
	ipi_event_reset_x_t ipi_event_reset_0 ;
	ipi_mask_x_t	ipi_mask_0 ;
	ipi_mask_set_x_t	ipi_mask_set_0 ;
	ipi_mask_reset_x_t	ipi_mask_reset_0 ;
	mpl_ipi_x_t mpl_ipi_0 ;
	mpl_ipi_x_set_0_t	mpl_ipi_0_set_0 ;
	mpl_ipi_x_set_1_t	mpl_ipi_0_set_1 ;
	mpl_ipi_x_set_2_t	mpl_ipi_0_set_2 ;
	mpl_ipi_x_set_3_t	mpl_ipi_0_set_3 ;
	ipi_event_x_t	ipi_event_1 ;
	ipi_event_set_x_t	ipi_event_set_1 ;
	ipi_event_reset_x_t ipi_event_reset_1 ;
	ipi_mask_x_t	ipi_mask_1 ;
	ipi_mask_set_x_t	ipi_mask_set_1 ;
	ipi_mask_reset_x_t	ipi_mask_reset_1 ;
	mpl_ipi_x_t mpl_ipi_1 ;
	mpl_ipi_x_set_0_t	mpl_ipi_1_set_0 ;
	mpl_ipi_x_set_1_t	mpl_ipi_1_set_1 ;
	mpl_ipi_x_set_2_t	mpl_ipi_1_set_2 ;
	mpl_ipi_x_set_3_t	mpl_ipi_1_set_3 ;
	ipi_event_x_t	ipi_event_2 ;
	ipi_event_set_x_t	ipi_event_set_2 ;
	ipi_event_reset_x_t ipi_event_reset_2 ;
	ipi_mask_x_t	ipi_mask_2 ;
	ipi_mask_set_x_t	ipi_mask_set_2 ;
	ipi_mask_reset_x_t	ipi_mask_reset_2 ;
	mpl_ipi_x_t mpl_ipi_2 ;
	mpl_ipi_x_set_0_t	mpl_ipi_2_set_0 ;
	mpl_ipi_x_set_1_t	mpl_ipi_2_set_1 ;
	mpl_ipi_x_set_2_t	mpl_ipi_2_set_2 ;
	mpl_ipi_x_set_3_t	mpl_ipi_2_set_3 ;
	ipi_event_x_t	ipi_event_3 ;
	ipi_event_set_x_t	ipi_event_set_3 ;
	ipi_event_reset_x_t ipi_event_reset_3 ;
	ipi_mask_x_t	ipi_mask_3 ;
	ipi_mask_set_x_t	ipi_mask_set_3 ;
	ipi_mask_reset_x_t	ipi_mask_reset_3 ;
	mpl_ipi_x_t mpl_ipi_3 ;
	mpl_ipi_x_set_0_t	mpl_ipi_3_set_0 ;
	mpl_ipi_x_set_1_t	mpl_ipi_3_set_1 ;
	mpl_ipi_x_set_2_t	mpl_ipi_3_set_2 ;
	mpl_ipi_x_set_3_t	mpl_ipi_3_set_3 ;
	itlb_current_va_t	itlb_current_va ;
	itlb_current_pa_t	itlb_current_pa ;
	itlb_current_attr_t itlb_current_attr ;
	itlb_index_t	itlb_index ;
	wired_itlb_t	wired_itlb ;
	replacement_itlb_t	replacement_itlb ;
	number_itlb_t	number_itlb ;
	itlb_pr_t	itlb_pr ;
	itlb_match_0_t	itlb_match_0 ;
	itlb_perf_t itlb_perf ;
	itlb_tsb_base_addr_0_t	itlb_tsb_base_addr_0 ;
	itlb_tsb_addr_0_t	itlb_tsb_addr_0 ;
	itlb_tsb_fill_current_attr_t	itlb_tsb_fill_current_attr ;
	itlb_tsb_fill_match_t	itlb_tsb_fill_match ;
	itlb_tsb_base_addr_1_t	itlb_tsb_base_addr_1 ;
	itlb_tsb_addr_1_t	itlb_tsb_addr_1 ;
	mpl_itlb_miss_t mpl_itlb_miss ;
	mpl_itlb_miss_set_0_t	mpl_itlb_miss_set_0 ;
	mpl_itlb_miss_set_1_t	mpl_itlb_miss_set_1 ;
	mpl_itlb_miss_set_2_t	mpl_itlb_miss_set_2 ;
	mpl_itlb_miss_set_3_t	mpl_itlb_miss_set_3 ;
	xdn_demux_error_t	xdn_demux_error ;
	sbox_error_t	sbox_error ;
	mem_error_enable_t	mem_error_enable ;
	mem_error_mbox_status_t mem_error_mbox_status ;
	mem_error_mbox_addr_t	mem_error_mbox_addr ;
	mem_error_cbox_status_t mem_error_cbox_status ;
	mem_error_cbox_addr_t	mem_error_cbox_addr ;
	mpl_mem_error_t mpl_mem_error ;
	mpl_mem_error_set_0_t	mpl_mem_error_set_0 ;
	mpl_mem_error_set_1_t	mpl_mem_error_set_1 ;
	mpl_mem_error_set_2_t	mpl_mem_error_set_2 ;
	mpl_mem_error_set_3_t	mpl_mem_error_set_3 ;
	perf_count_0_t	perf_count_0 ;
	perf_count_1_t	perf_count_1 ;
	perf_count_ctl_t	perf_count_ctl ;
	perf_count_sts_t	perf_count_sts ;
	watch_val_t watch_val ;
	watch_mask_t	watch_mask ;
	perf_count_dn_ctl_t perf_count_dn_ctl ;
	mpl_perf_count_t	mpl_perf_count ;
	mpl_perf_count_set_0_t	mpl_perf_count_set_0 ;
	mpl_perf_count_set_1_t	mpl_perf_count_set_1 ;
	mpl_perf_count_set_2_t	mpl_perf_count_set_2 ;
	mpl_perf_count_set_3_t	mpl_perf_count_set_3 ;
	single_step_control_x_t single_step_control_0 ;
	mpl_single_step_x_t mpl_single_step_0 ;
	mpl_single_step_x_set_0_t	mpl_single_step_0_set_0 ;
	mpl_single_step_x_set_1_t	mpl_single_step_0_set_1 ;
	mpl_single_step_x_set_2_t	mpl_single_step_0_set_2 ;
	mpl_single_step_x_set_3_t	mpl_single_step_0_set_3 ;
	single_step_control_x_t single_step_control_1 ;
	mpl_single_step_x_t mpl_single_step_1 ;
	mpl_single_step_x_set_0_t	mpl_single_step_1_set_0 ;
	mpl_single_step_x_set_1_t	mpl_single_step_1_set_1 ;
	mpl_single_step_x_set_2_t	mpl_single_step_1_set_2 ;
	mpl_single_step_x_set_3_t	mpl_single_step_1_set_3 ;
	single_step_control_x_t single_step_control_2 ;
	mpl_single_step_x_t mpl_single_step_2 ;
	mpl_single_step_x_set_0_t	mpl_single_step_2_set_0 ;
	mpl_single_step_x_set_1_t	mpl_single_step_2_set_1 ;
	mpl_single_step_x_set_2_t	mpl_single_step_2_set_2 ;
	mpl_single_step_x_set_3_t	mpl_single_step_2_set_3 ;
	single_step_control_x_t single_step_control_3 ;
	mpl_single_step_x_t mpl_single_step_3 ;
	mpl_single_step_x_set_0_t	mpl_single_step_3_set_0 ;
	mpl_single_step_x_set_1_t	mpl_single_step_3_set_1 ;
	mpl_single_step_x_set_2_t	mpl_single_step_3_set_2 ;
	mpl_single_step_x_set_3_t	mpl_single_step_3_set_3 ;
	mpl_swint_x_t	mpl_swint_0 ;
	mpl_swint_x_set_0_t mpl_swint_0_set_0 ;
	mpl_swint_x_set_1_t mpl_swint_0_set_1 ;
	mpl_swint_x_set_2_t mpl_swint_0_set_2 ;
	mpl_swint_x_set_3_t mpl_swint_0_set_3 ;
	mpl_swint_x_t	mpl_swint_1 ;
	mpl_swint_x_set_0_t mpl_swint_1_set_0 ;
	mpl_swint_x_set_1_t mpl_swint_1_set_1 ;
	mpl_swint_x_set_2_t mpl_swint_1_set_2 ;
	mpl_swint_x_set_3_t mpl_swint_1_set_3 ;
	mpl_swint_x_t	mpl_swint_2 ;
	mpl_swint_x_set_0_t mpl_swint_2_set_0 ;
	mpl_swint_x_set_1_t mpl_swint_2_set_1 ;
	mpl_swint_x_set_2_t mpl_swint_2_set_2 ;
	mpl_swint_x_set_3_t mpl_swint_2_set_3 ;
	mpl_swint_x_t	mpl_swint_3 ;
	mpl_swint_x_set_0_t mpl_swint_3_set_0 ;
	mpl_swint_x_set_1_t mpl_swint_3_set_1 ;
	mpl_swint_x_set_2_t mpl_swint_3_set_2 ;
	mpl_swint_x_set_3_t mpl_swint_3_set_3 ;
	tile_timer_control_t	tile_timer_control ;
	mpl_tile_timer_t	mpl_tile_timer ;
	mpl_tile_timer_set_0_t	mpl_tile_timer_set_0 ;
	mpl_tile_timer_set_1_t	mpl_tile_timer_set_1 ;
	mpl_tile_timer_set_2_t	mpl_tile_timer_set_2 ;
	mpl_tile_timer_set_3_t	mpl_tile_timer_set_3 ;
	mpl_udn_access_t	mpl_udn_access ;
	mpl_udn_access_set_0_t	mpl_udn_access_set_0 ;
	mpl_udn_access_set_1_t	mpl_udn_access_set_1 ;
	mpl_udn_access_set_2_t	mpl_udn_access_set_2 ;
	mpl_udn_access_set_3_t	mpl_udn_access_set_3 ;
	udn_data_avail_t	udn_data_avail ;
	udn_demux_count_0_t udn_demux_count_0 ;
	udn_demux_count_1_t udn_demux_count_1 ;
	udn_demux_count_2_t udn_demux_count_2 ;
	udn_demux_count_3_t udn_demux_count_3 ;
	udn_pending_t	udn_pending ;
	udn_sp_fifo_cnt_t	udn_sp_fifo_cnt ;
	udn_flush_egress_t	udn_flush_egress ;
	udn_route_order_t	udn_route_order ;
	mpl_udn_avail_t mpl_udn_avail ;
	mpl_udn_avail_set_0_t	mpl_udn_avail_set_0 ;
	mpl_udn_avail_set_1_t	mpl_udn_avail_set_1 ;
	mpl_udn_avail_set_2_t	mpl_udn_avail_set_2 ;
	mpl_udn_avail_set_3_t	mpl_udn_avail_set_3 ;
	udn_avail_en_t	udn_avail_en ;
	udn_complete_pending_t	udn_complete_pending ;
	mpl_udn_complete_t	mpl_udn_complete ;
	mpl_udn_complete_set_0_t	mpl_udn_complete_set_0 ;
	mpl_udn_complete_set_1_t	mpl_udn_complete_set_1 ;
	mpl_udn_complete_set_2_t	mpl_udn_complete_set_2 ;
	mpl_udn_complete_set_3_t	mpl_udn_complete_set_3 ;
	mpl_udn_firewall_t	mpl_udn_firewall ;
	mpl_udn_firewall_set_0_t	mpl_udn_firewall_set_0 ;
	mpl_udn_firewall_set_1_t	mpl_udn_firewall_set_1 ;
	mpl_udn_firewall_set_2_t	mpl_udn_firewall_set_2 ;
	mpl_udn_firewall_set_3_t	mpl_udn_firewall_set_3 ;
	udn_direction_protect_t udn_direction_protect ;
	mpl_udn_timer_t mpl_udn_timer ;
	mpl_udn_timer_set_0_t	mpl_udn_timer_set_0 ;
	mpl_udn_timer_set_1_t	mpl_udn_timer_set_1 ;
	mpl_udn_timer_set_2_t	mpl_udn_timer_set_2 ;
	mpl_udn_timer_set_3_t	mpl_udn_timer_set_3 ;
	udn_deadlock_count_t	udn_deadlock_count ;
	udn_deadlock_timeout_t	udn_deadlock_timeout ;
	mpl_unalign_data_t	mpl_unalign_data ;
	mpl_unalign_data_set_0_t	mpl_unalign_data_set_0 ;
	mpl_unalign_data_set_1_t	mpl_unalign_data_set_1 ;
	mpl_unalign_data_set_2_t	mpl_unalign_data_set_2 ;
	mpl_unalign_data_set_3_t	mpl_unalign_data_set_3 ;
	dstream_pf_t	dstream_pf ;
	pass_t	pass ;
	fail_t	fail ;
	done_t	done ;
	cycle_t cycle ;
	pseudo_random_number_t	pseudo_random_number ;
	proc_status_t	proc_status ;
	status_saturate_t	status_saturate ;
	cmpexch_value_t cmpexch_value ;
	tile_coord_t	tile_coord ;
	tile_rtf_hwm_t	tile_rtf_hwm ;
	mpl_world_access_t	mpl_world_access ;
	mpl_world_access_set_0_t	mpl_world_access_set_0 ;
	mpl_world_access_set_1_t	mpl_world_access_set_1 ;
	mpl_world_access_set_2_t	mpl_world_access_set_2 ;
	mpl_world_access_set_3_t	mpl_world_access_set_3 ;
	interrupt_critical_section_t	interrupt_critical_section ;
	sim_socket_t	sim_socket ;
	sim_control_t	sim_control ;
	event_begin_t	event_begin ;
	event_end_t event_end ;
#else
	snctl_t snctl ;
	sn_fifo_sel_t	sn_fifo_sel ;
	sniostate_t sniostate ;
	snstatic_t	snstatic ;
	sn_data_avail_t sn_data_avail ;
	sn_static_ctl_t sn_static_ctl ;
	sn_static_fifo_sel_t	sn_static_fifo_sel ;
	sn_static_istate_t	sn_static_istate ;
	sn_static_static_t	sn_static_static ;
	sn_static_data_avail_t	sn_static_data_avail ;
	udn_demux_ca_count_t	udn_demux_ca_count ;
	udn_demux_count_t	udn_demux_count ;
	udn_demux_ctl_t udn_demux_ctl ;
	udn_demux_curr_tag_t	udn_demux_curr_tag ;
	udn_demux_queue_sel_t	udn_demux_queue_sel ;
	udn_demux_write_fifo_t	udn_demux_write_fifo ;
	udn_demux_status_t	udn_demux_status ;
	udn_demux_write_queue_t udn_demux_write_queue ;
	udn_pending_t	udn_pending ;
	udn_sp_fifo_data_t	udn_sp_fifo_data ;
	udn_sp_fifo_sel_t	udn_sp_fifo_sel ;
	udn_sp_freeze_t udn_sp_freeze ;
	udn_sp_state_t	udn_sp_state ;
	udn_tag_t	udn_tag ;
	udn_tag_valid_t udn_tag_valid ;
	udn_tile_coord_t	udn_tile_coord ;
	udn_ca_data_t	udn_ca_data ;
	udn_ca_rem_t	udn_ca_rem ;
	udn_ca_tag_t	udn_ca_tag ;
	udn_data_avail_t	udn_data_avail ;
	udn_refill_en_t udn_refill_en ;
	udn_remaining_t udn_remaining ;
	udn_avail_en_t	udn_avail_en ;
	udn_deadlock_count_t	udn_deadlock_count ;
	udn_deadlock_timeout
	udn_deadlock_timeout_t	udn_deadlock_timeout ;
	cycle_high_t	cycle_high ;
	cycle_low_t cycle_low ;
	done_t	done ;
	fail_t	fail ;
	pass_t	pass ;
	interrupt_critical_section_t	interrupt_critical_section ;
	ex_context_0_0_t	ex_context_0_0 ;
	ex_context_0_1_t	ex_context_0_1 ;
	intctrl_n_status_t	intctrl_n_status ;
	interrupt_mask_set_reset_0_0_t	interrupt_mask_set_reset_0_0 ;
	interrupt_mask_set_reset_0_1_t	interrupt_mask_set_reset_0_1 ;
	system_save_t	system_save ;
	mpl_tile_timer_t	mpl_tile_timer ;
	dma_byte_t	dma_byte ;
	dma_chunk_size_t	dma_chunk_size ;
	dma_ctr_t	dma_ctr ;
	dma_dst_addr_t	dma_dst_addr ;
	dma_chunk_addr_t	dma_chunk_addr ;
	dma_src_addr_t	dma_src_addr ;
	dma_src_chunk_addr_t	dma_src_chunk_addr ;
	dma_stride_t	dma_stride ;
	dma_user_status_t	dma_user_status ;
#endif
} tile_sprf;

/* MMU modes definitions */
#define NB_MMU_MODES 3
#define MMU_MODE0_SUFFIX _kernel
#define MMU_MODE1_SUFFIX _user
#define MMU_MODE2_SUFFIX _hypv
#define MMU_KERNEL_IDX 0
#define MMU_USER_IDX 1
#define MMU_HYPV_IDX 2

typedef struct CPUTileraState CPUTileraState;

struct CPUTileraState {
#ifndef __tilepro__

	uint64_t	gpr[56];
	/*uint64_t	fp;
	uint64_t	tp;
	uint64_t	sp;
	uint64_t	lr;*/
	uint64_t	sn;
	uint64_t	idn0;
	uint64_t	idn1;
	uint64_t	udn0;
	uint64_t	udn1;
	uint64_t	udn2;
	uint64_t	udn3;
	uint64_t	zero;

	uint64_t	pc;

	/*special purpose registers */
	union {
	uint64_t spr[SPR_MAX];
	tile_sprf spr_names;
	};

	
#else	

	uint32_t	gpr[55];
	/*uint32_t	fp;
	uint32_t	tp;
	uint32_t	sp;*/
	uint32_t	lr;
	uint32_t	sn;
	uint32_t	idn0;
	uint32_t	idn1;
	uint32_t	udn0;
	uint32_t	udn1;
	uint32_t	udn2;
	uint32_t	udn3;
	uint32_t	zero;

	uint32_t	pc;

	/*special purpose registers */
	union {
	uint32_t spr[SPR_MAX];
	tile_sprf spr_names;
	};

#endif
	/* These pass data from the exception logic in the translator and
	helpers to the OS entry point.  This is used for both system
	emulation and user-mode.  */
	uint64_t trap_arg0;
	uint64_t trap_arg1;
	uint64_t trap_arg2;

	uint64_t lock_addr;
	uint64_t lock_st_addr;
	uint64_t lock_value;

	/* This alarm doesn't exist in real hardware */
	struct QEMUTimer *alarm_timer;
	uint64_t alarm_expire;

	/* Those resources are used only in QEMU core */
	CPU_COMMON

	int mmu_idx;
	int error_code;
};

#define cpu_init cpu_tilera_init
#define cpu_exec cpu_tilera_exec
#define cpu_gen_code cpu_tilera_gen_code
#define cpu_signal_handler cpu_tilera_signal_handler

#include "cpu-all.h"
#include "cpu-qom.h"


static inline int cpu_mmu_index (CPUTileraState *env)
{
    return env->mmu_idx;
}


CPUTileraState * cpu_tilera_init (const char *cpu_model);
int cpu_tilera_exec (CPUTileraState *s);
/* you can call this signal handler from your SIGBUS and SIGSEGV
   signal handlers to inform the virtual CPU of exceptions. non zero
   is returned if the signal was handled by the virtual CPU.  */
int cpu_tilera_signal_handler(int host_signum, void *pinfo,
                             void *puc);
int cpu_tilera_handle_mmu_fault (CPUTileraState *env, uint64_t address, int rw,
                                int mmu_idx);
#define cpu_handle_mmu_fault cpu_tilera_handle_mmu_fault
void do_interrupt (CPUTileraState *env);
void do_restore_state(CPUTileraState *env, uintptr_t retaddr);

static inline void cpu_get_tb_cpu_state(CPUTileraState *env, target_ulong *pc,
                                        target_ulong *cs_base, int *pflags)
{
    int flags = 0;

    *pc = env->pc;
    *cs_base = 0;

    *pflags = flags;
}

#if defined(CONFIG_USER_ONLY)
/* this function sets the value of stack pointer and clears the first gpr ?*/
static inline void cpu_clone_regs(CPUTileraState *env, target_ulong newsp)
{
    if (newsp)
        env->gpr[TILE_REG_SP] = newsp;
    env->gpr[0] = 0;
}

static inline void cpu_set_tls(CPUTileraState *env, target_ulong newtls)
{
    env->gpr[TILE_REG_TP] = newtls;
}
#endif

static inline bool cpu_has_work(CPUState *cpu)
{
    CPUTileraState *env = &TILERA_CPU(cpu)->env;

    return env->interrupt_request & CPU_INTERRUPT_HARD;
}

#include "exec-all.h"

static inline void cpu_pc_from_tb(CPUTileraState *env, TranslationBlock *tb)
{
    env->pc = tb->pc;
}

/* prototypes of auxiliary functions in helper.c */
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

inline int64_t set_4_byte(int64_t word, int num, int64_t val);

inline int little_endian (CPUTileraState *env);

void QEMU_NORETURN dynamic_excp(CPUTileraState *env, uintptr_t retaddr,
                                int excp, int error);

#endif /* !defined (__CPU_TILERA_H__) */

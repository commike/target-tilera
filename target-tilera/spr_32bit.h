/*
 *  Special purpose registers definitions for 32 bit tilera processors 
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

#if !defined (__CPU_TILERA_SPR_32_H__)
#define __CPU_TILERA_SPR_32_H__

enum {
	SN_ACCESS,
	SN_STATIC_ACCESS,
	UDN_ACCESS,
	UDN_REFILL,
	UDN_COMPLETE,
	UDN_AVAIL,
	UDN_TIMER,
	WORLD_ACCESS,
	INTCTRL_0,
	TILE_TIMER,
	DMA_NOTIFY
} mpl_ty;

enum {
	SPR_SNCTL ,
	SPR_SNFIFO_DATA ,
	SPR_SNFIFO_SEL ,
	SPR_SNISTATE ,
	SPR_SNOSTATE ,
	SPR_SNSTATIC ,
	SPR_SN_DATA_AVAIL ,

	/* user dynamic network registers */
	SPR_UDN_DEMUX_CA_COUNT ,
	SPR_UDN_DEMUX_COUNT_0 ,
	SPR_UDN_DEMUX_COUNT_1 ,
	SPR_UDN_DEMUX_COUNT_2 ,
	SPR_UDN_DEMUX_COUNT_3 ,
	SPR_UDN_DEMUX_CTL ,
	SPR_UDN_DEMUX_CURR_TAG ,
	SPR_UDN_DEMUX_QUEUE_SEL ,
	SPR_UDN_DEMUX_STATUS ,
	SPR_UDN_DEMUX_FIFO ,
	SPR_UDN_DEMUX_WRITE_QUEQUE ,
	SPR_UDN_PENDING ,
	SPR_UDN_SP_FIFO_DATA0 ,
	SPR_UDN_SP_FIFO_DATA1 ,
	SPR_UDN_SP_FREEZE ,
	SPR_UDN_SP_STATE ,
	SPR_UDN_TAG_0 ,
	SPR_UDN_TAG_1 ,
	SPR_UDN_TAG_2 ,
	SPR_UDN_TAG_3 ,
	SPR_UDN_TAG_VALID ,
	SPR_UDN_TILE_COORD ,
	SPR_UDN_CA_DATA ,
	SPR_UDN_CA_REM ,
	SPR_UDN_CA_DATA ,
	SPR_UDN_CA_AVAIL ,

	SPR_UDN_REFILL_EN ,
	SPR_UDN_REMAINING ,
	SPR_UDN_AVAIL_EN ,

	SPR_UDN_DEADLOCK_COUNT ,
	SPR_UDN_DEADLOCK_TIMEOUT ,

	/* world accessible registers*/
	SPR_CYCLE_HIGH ,
	SPR_CYCLE_LOW ,
	SPR_DONE ,
	SPR_FAIL ,
	SPR_INTERRUPT_CRITICAL_SECTION ,
	SPR_PASS ,

	/* interrupt control 0 registers*/
	SPR_EX_CONTEXT_0_0 ,
	SPR_EX_CONTEXT_0_1 ,
	SPR_INTCTRL_N_STATUS ,
	SPR_INTERRUPT_MASK_0_0 ,
	SPR_INTERRUPT_MASK_0_1 ,
	SPR_INTERRUPT_MASK_RESET_0_0 ,
	SPR_INTERRUPT_MASK_RESET_0_1 ,
	SPR_INTERRUPT_MASK_SET_0_1 ,
	SPR_INTERRUPT_MASK_SET_0_1 ,
	SPR_SYSTEM_SAVE_0_0 ,
	SPR_SYSTEM_SAVE_0_1 ,
	SPR_SYSTEM_SAVE_0_2 ,
	SPR_SYSTEM_SAVE_0_3 ,

	/* tile timer register */
	SPR_MPL_TILE_TIMER ,

	/* DMA registers */
	SPR_DMA_BYTE ,
	SPR_DMA_CHUNK_SIZE ,
	SPR_DMA_CTR ,
	SPR_DMA_DST_ADDR ,
	SPR_DMA_DST_CHUNK_ADDR ,
	SPR_DMA_SRC_ADDR ,
	SPR_DMA_SRC_CHUNK_ADDR ,
	SPR_DMA_STRIDE ,
	SPR_DMA_USER_STATUS ,
	
#ifdef TARGET_TILEPRO
	SPR_SN_STATIC_CTL ,
	SPR_SN_STATIC_FIFO_DATA ,
	SPR_SN_STATIC_FIFO_SEL ,
	SPR_SN_STATIC_ISTATE ,
	SPR_SN_STATIC_OSTATE ,
	SPR_SN_STATIC_STATIC ,
	SPR_SN_STATIC_DATA_AVAIL ,
#endif
		
	SPR_MAX
};

/* This array defines the minimum protection level associated with a special 
purpose register. The spr number is an index to the array. e.g. to get minimum
protection level for SPR_DMA_CTR use :-

mpl_ty sim_socket_mpl = spr_mpl(SPR_DMA_CTR);

*/

extern mpl_ty spr_mpl [SPR_MAX];

/* This array defines the address (index) of special purpose registers in the special 
purpose register file. The spr number is an index to the array. e.g. to get address
for SPR_DMA_BYTE use :-

uint32_t  sim_socket_addr = spr_index(SPR_DMA_BYTE);

*/

extern uint32_t spr_addr [SPR_MAX];
		
/* Tilera Special Purpose Register Definitions 
--------------------------------------- */

/* static network control register SNCTL bits*/
typedef struct
{
	uint32_t frzfabric : 1;
#ifdef TARGET_TILE64
	uint32_t frzproc : 1;
	uint32_t reserved : 30;
#else
	uint32_t reserved :31;
#endif
} snctl_t ;

/* static network fifo select register SNFIFO_SEL */
enum 
{
	NORTH_INPUT_FIFO,
	EAST_INPUT_FIFO,
	SOUTH_INPUT_FIFO,
	WEST_INPUT_FIFO,
	PROCESSOR_INPUT_FIFO,
	PROCESSOR_OUTPUT_FIFO	
};

typedef struct
{
	uint32_t snfifo_sel : 3;
	uint32_t reserved : 29;
	
} sn_fifo_sel_t ;

/* static network input/output state register type*/
typedef struct
{
#ifdef TARGET_TILE_64
	uint32_t n : 2;
	uint32_t reserved1 : 2;
	uint32_t e : 2;
	uint32_t reserved2 : 2;
	uint32_t s : 2;
	uint32_t reserved3 : 2;
	uint32_t w : 2;
	uint32_t reserved4 : 2;
	uint32_t m : 2;
	uint32_t reserved5 : 12;
#else
	uint32_t n : 4;
	uint32_t e : 4;
	uint32_t s : 4;
	uint32_t w : 4;
	uint32_t m : 4;
	uint32_t reserved : 12;
#endif
} sniostate_t ;

/* static network static route*/
enum
{
	SN_NORTH=1,
	SN_EAST,
	SN_SOUTH,
	SN_WEST,
	SN_MAIN_PROCESSOR
};
typedef struct
{
	uint32_t n : 3;
	uint32_t e : 3;
	uint32_t s : 3;
	uint32_t w : 3;
	uint32_t m : 3;
	uint32_t reserved : 17;
} snstatic_t ;

/* static network data available */
typedef struct
{
	uint32_t avail : 1;
	uint32_t reserved : 31;
} sn_data_avail_t ;

#ifdef TARGET_TILEPRO
/* static network control */
typedef struct
{
	uint32_t frzfabric : 1;
	uint32_t reserved : 31;
} sn_static_ctl_t ;

/* static network fifo select */
typedef struct
{
	uint32_t sn_static_fifo_sel : 3;
	uint32_t reserved : 29 ;
} sn_static_fifo_sel_t;

/* static network input/output state */
typedef struct
{
	uint32_t n : 4;
	uint32_t e : 4;
	uint32_t s : 4;
	uint32_t w : 4;
	uint32_t m : 4;
	uint32_t reserved : 12;
} sn_static_istate_t ;

/* static network static route */
typedef struct
{
	uint32_t n : 3;
	uint32_t e : 3;
	uint32_t s : 3;
	uint32_t w : 3;
	uint32_t m : 3;
	uint32_t reserved : 17;
} sn_static_static_t ;

/* static network data available */
typedef struct
{
	uint32_t avail : 1;
	uint32_t reserved : 31;
} sn_static_data_avail_t ;

#endif

/* user dynamic network (UDN) catch-all demultiplexor count register */
typedef struct
{
#ifdef TARGET_TILE64
	uint32_t udn_demux_ca_count : 7;
	uint32_t reserved : 25;
#else
	uint32_t udn_demux_ca_count ;
#endif
} udn_demux_ca_count_t ;

/* UDN demux count register type */
typedef struct
{
	uint32_t udn_demux_count : 7;
	uint32_t reserved : 25;
} udn_demux_count_t ;

/* UDN demux control register ?? */
typedef uint32_t udn_demux_ctl_t;

/* UDN demux current tag  ?? */
typedef uint32_t udn_demux_curr_tag_t;

/* UDN demux queue select register */
typedef struct
{
	uint32_t udn_demux_queue_sel : 2;
	uint32_t reserved : 30;
} udn_demux_queue_sel_t ;

/* UDN demux fifo */
typedef uint32_t udn_demux_write_fifo_t;

/* UDN demux status */
typedef struct
{
	uint32_t curr_rem : 8;
	uint32_t wait_tag : 1;
	uint32_t space_avail : 1;
	uint32_t rcv_fifo_cnt : 2;
	uint32_t reserved : 20;
} udn_demux_status_t ;

/* UDN demux write queue */
typedef uint32_t udn_demux_write_queue_t;

/* UDN words pending */
typedef struct
{
	uint32_t udn_pending : 8;
	uint32_t reserved : 24;
} udn_pending_t;

/* UDN fifo data */
typedef uint32_t udn_sp_fifo_data_t;

/* UDN fifo select */
enum
{
	UDN_NORTH,
	UDN_SOUTH,
	UDN_EAST,
	UDN_WEST,
	UDN_cORE
};

typedef struct
{
	uint32_t udn_sp_fifo_sel : 3;
	uint32_t reserved : 29;
} udn_sp_fifo_sel_t;

/* UDN freeze */
typedef struct
{
	uint32_t sp_frz : 1;
	uint32_t demux_frz : 1; 
	uint32_t non_dest_ext : 1;
	uint32_t reserved : 29;
} udn_sp_freeze_t;

/* UDN port state */
typedef struct
{
	uint32_t fcnt : 4;
	uint32_t ip_words_rem : 7;
	uint32_t ip_eop : 1;
	uint32_t ip_sop : 1;
	uint32_t op_mux_sel : 4;
	uint32_t op_locked : 1;
	uint32_t op_credit : 2;
	uint32_t reserved : 12;
} udn_sp_state_t ;

/* UDN tag type */
typedef uint32_t udn_tag_t ;

/* UDN tag valid */
typedef struct
{
	uint32_t vld : 4;
	uint32_t reserved1 : 4;
	uint32_t rf : 4;
	uint32_t reserved2 : 20;
} udn_tag_valid_t;

/* UDN tile coordinates */
typedef struct
{
#ifdef TARGET_TILEPRO
	uint32_t route_order : 1;
	uint32_t reserved1 : 6;
#else 
	uint32_t reserved1 : 7;
#endif
	uint32_t yloc : 11;
	uint32_t xloc : 11;
	uint32_t reserved2 : 1;
	uint32_t edge : 2;	
} udn_tile_coord_t;

/* UDN catch-all data */
typedef uint32_t udn_ca_data_t ;

/* UDN catch-all remaining words */
typedef struct
{
	uint32_t udn_ca_rem : 7;
	uint32_t reserved : 25;
} udn_ca_rem_t;

/* UDN catch-all tag type */
typedef uint32_t udn_ca_tag_t ;


/* UDN data available */
typedef struct
{
	uint32_t avail_0 : 1;
	uint32_t avail_1 : 1;
	uint32_t avail_2 : 1;
	uint32_t avail_3 : 1;
	uint32_t avail_ca : 1;
	uint32_t reserved : 27;
} udn_data_avail_t;

/* UDN refill available enable */
typedef struct
{
	uint32_t en_0 : 1;
	uint32_t en_1 : 1;
	uint32_t en_2 : 1;
	uint32_t en_3 : 1;
	uint32_t reserved : 28;
} udn_refill_en_t;

/* UDN remaining words */
typedef struct
{
	uint32_t words : 8;
	uint32_t reserved : 24;
} udn_remaining_t;

/* UDN available enables */
typedef struct
{
	uint32_t en_0 : 1;
	uint32_t en_1 : 1;
	uint32_t en_2 : 1;
	uint32_t en_3 : 1;
	uint32_t reserved : 28;
} udn_avail_en_t;

/* UDN deadlock counter */
typedef struct
{
	uint32_t udn_deadlock_count : 16;
	uint32_t reserved : 16;
} udn_deadlock_count_t;

/* UDN deadlock timeout */
typedef struct
{
	uint32_t udn_deadlock_timeout : 16;
	uint32_t reserved : 16;
} udn_deadlock_timeout_t;

/* cycle counter high*/
typedef uint32_t cycle_high_t;

/* cycle counter low*/
typedef uint32_t cycle_low_t;

/* done magic register*/
typedef uint32_t done_t;

/* fail magic register*/
typedef uint32_t fail_t;

/* pass magic register */
typedef uint32_t pass_t;

/* interrupt critical section */
typedef struct
{
	uint32_t ics : 1;
	uint32_t reserved : 31;
} interrupt_critical_section_t;

/* exceptional context protection level 0 entry 0 */ 
typedef struct
{
	uint32_t  pc;
} ex_context_0_0_t;

/* exceptional context protection level 0 entry 1 */
typedef struct
{
	uint32_t pl : 2;
	uint32_t ics : 1;
	uint32_t reserved : 29;
} ex_context_0_1_t;

/* interrupt control n register */
typedef struct
{
	uint32_t intctrl_n_status : 1;
	uint32_t reserved : 31;
} intctrl_n_status_t;

/* common type for interrupt mask, interrupt mask reset and interrupt mask set 
protection level 0 entry 0 registers */
typedef struct
{
	uint32_t reserved1 : 1;
	uint32_t mask_1 : 1;
	uint32_t reserved2 : 5;
	uint32_t mask_7 : 1;
	uint32_t mask_8 : 1;
	uint32_t mask_9 : 1;
	uint32_t mask_10 : 1;
	uint32_t reserved3 : 7;
	uint32_t mask_18 : 1;
	uint32_t mask_19 : 1;
	uint32_t mask_20 : 1;
	uint32_t mask_21 : 1;
	uint32_t mask_22 : 1;
	uint32_t mask_23 : 1;
	uint32_t mask_24 : 1;
	uint32_t mask_25 : 1;
	uint32_t mask_26 : 1;
	uint32_t mask_27 : 1;
	uint32_t mask_28 : 1;
	uint32_t mask_29 : 1;
	uint32_t mask_30 : 1;
	uint32_t mask_31 : 1;	
} interrupt_mask_set_reset_0_0_t;

/* common type for interrupt mask, interrupt mask reset and interrupt mask set 
protection level 0 entry 1 registers */
typedef struct
{
	uint32_t mask_32 : 1;
	uint32_t mask_33 : 1;
	uint32_t mask_34 : 1;
	uint32_t mask_35 : 1;
	uint32_t mask_36 : 1;
	uint32_t mask_37 : 1;
#ifdef TARGET_TILEPRO
	uint32_t reserved1 : 9;
	uint32_t mask_47 : 1;
	uint32_t mask_48 : 1;
	uint32_t reserved2 : 15;
#else
	uint32_t reserved : 26;
#endif
} interrupt_mask_set_reset_0_1_t;

/* common type for system save registers level 0 entry 0-3 */
typedef uint32_t system_save_t;

/* minimum protection level for tile timer */
typedef struct
{
	uint32_t mpl : 2;
	uint32_t reserved : 30;
} mpl_tile_timer_t;

/* dma byte register*/
typedef struct
{
	uint32_t size : 20;
	uint32_t chunk_number : 12;
} dma_byte_t;

/* dma chunk size register */
typedef struct
{
	uint32_t dma_chunk_size : 20;
	uint32_t reserved : 12;
} dma_chunk_size_t;

/* dma control register */
typedef struct
{
	uint32_t request : 1;
	uint32_t suspend : 1;
	uint32_t reserved : 30;
} dma_ctr_t;

/* dma destination address */
typedef uint32_t dma_dst_addr_t;

/* dma destination chunk address */
typedef uint32_t dma_chunk_addr_t;

/* dma source address */
typedef uint32_t dma_src_addr_t;

/* dma source chunk address */
typedef uint32_t dma_src_chunk_addr_t;

/* dma source and destination strides */
typedef struct
{
	uint32_t load : 16;
	uint32_t store : 16;
} dma_stride_t;

/* dma user status */
typedef struct
{
	uint32_t done : 1;
	uint32_t busy : 1;
	uint32_t reserved1 : 2;
	uint32_t running : 1;
	uint32_t reserved2 : 1;
	uint32_t error : 1;
	uint32_t reserved3 : 25;
} dma_user_status_t;

#endif

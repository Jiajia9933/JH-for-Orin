/*
 * Copyright (c) 2018-2023, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef INCLUDED_OSI_COMMON_H
#define INCLUDED_OSI_COMMON_H

#include <nvethernet_type.h>

/**
 * @addtogroup FC Flow Control Threshold Macros
 *
 * @brief These bits control the threshold (fill-level of Rx queue) at which
 * the flow control is asserted or de-asserted
 * @{
 */
#define FULL_MINUS_1_5K		((nveu32_t)1)
#define FULL_MINUS_16_K		((nveu32_t)30)
#define FULL_MINUS_32_K		((nveu32_t)62)
/** @} */

/**
 * @addtogroup OSI-Helper OSI Helper MACROS
 * @{
 */
#define OSI_UNLOCKED		0x0U
#define OSI_LOCKED		0x1U
#define OSI_NSEC_PER_SEC	1000000000ULL

#ifndef OSI_STRIPPED_LIB
#define OSI_MAX_RX_COALESCE_USEC	1020U
#define OSI_EQOS_MIN_RX_COALESCE_USEC	5U
#define OSI_MGBE_MIN_RX_COALESCE_USEC	6U
#define OSI_MIN_RX_COALESCE_FRAMES	1U
#define OSI_MAX_TX_COALESCE_USEC	1020U
#define OSI_MIN_TX_COALESCE_USEC	32U
#define OSI_MIN_TX_COALESCE_FRAMES	1U
#define OSI_PAUSE_FRAMES_DISABLE	0U
#define OSI_PAUSE_FRAMES_ENABLE		1U
#endif /* !OSI_STRIPPED_LIB */

/* Compiler hints for branch prediction */
#define osi_unlikely(x)			__builtin_expect(!!(x), 0)
/** @} */

/**
 * @addtogroup Helper MACROS
 *
 * @brief EQOS generic helper MACROS.
 * @{
 */
#define OSI_MAX_24BITS			0xFFFFFFU
#define OSI_MAX_28BITS			0xFFFFFFFU
#define OSI_MAX_32BITS			0xFFFFFFFFU
#define OSI_MASK_16BITS			0xFFFFU
#define OSI_MASK_20BITS			0xFFFFFU
#define OSI_MASK_24BITS			0xFFFFFFU
#define OSI_GCL_SIZE_64			64U
#define OSI_GCL_SIZE_128		128U
#define OSI_GCL_SIZE_512		512U
#define OSI_GCL_SIZE_1024		1024U
/** @} */

#ifndef OSI_STRIPPED_LIB
/**
 * @addtogroup Helper MACROS
 *
 * @brief EQOS generic helper MACROS.
 * @{
 */
#define OSI_PTP_REQ_CLK_FREQ		250000000U
#define OSI_FLOW_CTRL_DISABLE		0U
#define OSI_ADDRESS_32BIT		0
#define OSI_ADDRESS_40BIT		1
#define OSI_ADDRESS_48BIT		2
/** @ } */

/**
 * @addtogroup - LPI-Timers LPI configuration macros
 *
 * @brief LPI timers and config register field masks.
 * @{
 */
/* LPI LS timer - minimum time (in milliseconds) for which the link status from
 * PHY should be up before the LPI pattern can be transmitted to the PHY.
 * Default 1sec.
 */
#define OSI_DEFAULT_LPI_LS_TIMER	(nveu32_t)1000
#define OSI_LPI_LS_TIMER_MASK		0x3FFU
#define OSI_LPI_LS_TIMER_SHIFT		16U
/* LPI TW timer - minimum time (in microseconds) for which MAC wait after it
 * stops transmitting LPI pattern before resuming normal tx.
 * Default 21us
 */
#define OSI_DEFAULT_LPI_TW_TIMER	0x15U
#define OSI_LPI_TW_TIMER_MASK		0xFFFFU
/* LPI entry timer - Time in microseconds that MAC will wait to enter LPI mode
 * after all tx is complete.
 * Default 1sec.
 */
#define OSI_LPI_ENTRY_TIMER_MASK	0xFFFF8U

/* LPI entry timer - Time in microseconds that MAC will wait to enter LPI mode
 * after all tx is complete. Default 1sec.
 */
#define OSI_DEFAULT_TX_LPI_TIMER	0xF4240U

/* Max Tx LPI timer (in usecs) based on the timer value field length in HW
 * MAC_LPI_ENTRY_TIMER register */
#define OSI_MAX_TX_LPI_TIMER		0xFFFF8U

/* Min Tx LPI timer (in usecs) based on the timer value field length in HW
 * MAC_LPI_ENTRY_TIMER register */
#define OSI_MIN_TX_LPI_TIMER		0x8U

/* Time in 1 microseconds tic counter used as reference for all LPI timers.
 * It is clock rate of CSR slave port (APB clock[eqos_pclk] in eqos) minus 1
 * Current eqos_pclk is 204MHz
 */
#define OSI_LPI_1US_TIC_COUNTER_DEFAULT	0xCBU
#define OSI_LPI_1US_TIC_COUNTER_MASK	0xFFFU
/** @} */
#endif /* !OSI_STRIPPED_LIB */

#define OSI_POLL_COUNT			1000U
#ifndef UINT_MAX
#define UINT_MAX			(~0U)
#endif
#ifndef INT_MAX
#define INT_MAX				(0x7FFFFFFF)
#ifndef OSI_LLONG_MAX
#define OSI_LLONG_MAX			(0x7FFFFFFFFFFFFFFF)
#endif
#endif
/** @} */

/**
 * @addtogroup Generic helper MACROS
 *
 * @brief These are Generic helper macros used at various places.
 * @{
 */
#define OSI_UCHAR_MAX			(0xFFU)

/* Logging defines */
/* log levels */

#define OSI_LOG_INFO			1U
#ifndef OSI_STRIPPED_LIB
#define OSI_LOG_WARN			2U
#endif /* OSI_STRIPPED_LIB */
#define OSI_LOG_ERR			3U
/* Error types */
#define OSI_LOG_ARG_OUTOFBOUND          1U
#define OSI_LOG_ARG_INVALID		2U
#define OSI_LOG_ARG_HW_FAIL		4U
#define OSI_LOG_ARG_OPNOTSUPP		3U
/* Default maximum Giant Packet Size Limit is 16K */
#define OSI_MAX_MTU_SIZE	16383U

#ifdef UPDATED_PAD_CAL
/* MAC Tx/Rx Idle retry and delay count */
#define OSI_TXRX_IDLE_RETRY	5000U
#define OSI_DELAY_COUNT		10U
#endif

#define EQOS_DMA_CHX_STATUS(x)		((0x0080U * (x)) + 0x1160U)
#define MGBE_DMA_CHX_STATUS(x)		((0x0080U * (x)) + 0x3160U)
#define EQOS_DMA_CHX_IER(x)		((0x0080U * (x)) + 0x1134U)
#define MGBE_MTL_CHX_TX_OP_MODE(x)	((0x0080U * (x)) + 0x1100U)

/* FIXME add logic based on HW version */
#define OSI_EQOS_MAX_NUM_CHANS		8U
#define OSI_EQOS_MAX_NUM_QUEUES		8U
#define OSI_MGBE_MAX_L3_L4_FILTER	8U
#define OSI_MGBE_MAX_NUM_CHANS		10U
#define OSI_MGBE_MAX_NUM_QUEUES		10U
#define OSI_EQOS_XP_MAX_CHANS		4U

/* MACSEC max SC's supported 16*/
#define OSI_MACSEC_SC_INDEX_MAX		16

#ifndef OSI_STRIPPED_LIB
/* HW supports 8 Hash table regs, but eqos_validate_core_regs only checks 4 */
#define OSI_EQOS_MAX_HASH_REGS		4U
#endif /* OSI_STRIPPED_LIB */

#define MAC_VERSION		0x110
#define MAC_VERSION_SNVER_MASK	0x7FU

#define OSI_MAC_HW_EQOS		0U
#define OSI_MAC_HW_MGBE		1U
#define OSI_MAX_VM_IRQS		5U

#define OSI_NULL                ((void *)0)
#define OSI_ENABLE		1U
#define OSI_NONE		0U
#define OSI_NONE_SIGNED		0
#define OSI_DISABLE		0U
#define OSI_H_DISABLE		0x10101010U
#define OSI_H_ENABLE		(~OSI_H_DISABLE)

#define OSI_BIT(nr)             ((nveu32_t)1 << (nr))

#ifndef OSI_STRIPPED_LIB
#define OSI_MGBE_MAC_3_00	0x30U
#define OSI_EQOS_MAC_4_10       0x41U
#define OSI_EQOS_MAC_5_10       0x51U
#define OSI_MGBE_MAC_4_00	0x40U
#endif /* OSI_STRIPPED_LIB */

#define OSI_EQOS_MAC_5_00       0x50U
#define OSI_EQOS_MAC_5_30       0x53U
#define OSI_MGBE_MAC_3_10	0x31U

#define OSI_MAX_VM_IRQS              5U

#ifndef OSI_STRIPPED_LIB
#define OSI_HASH_FILTER_MODE	1U
#define OSI_L4_FILTER_TCP	0U
#define OSI_L4_FILTER_UDP	1U
#define OSI_PERFECT_FILTER_MODE	0U

#define OSI_INVALID_CHAN_NUM    0xFFU
#endif /* OSI_STRIPPED_LIB */
/** @} */

/**
 * @addtogroup OSI-DEBUG helper macros
 *
 * @brief OSI debug type macros
 * @{
 */
#ifdef OSI_DEBUG
#define OSI_DEBUG_TYPE_DESC		1U
#define OSI_DEBUG_TYPE_REG		2U
#define OSI_DEBUG_TYPE_STRUCTS		3U
#endif /* OSI_DEBUG */
/** @} */

/**
 * @brief unused function attribute
 */
#define OSI_UNUSED  __attribute__((__unused__))

/**
 * @brief osi_update_stats_counter - update value by increment passed
 *	as parameter
 * @note
 * Algorithm:
 *  - Check for boundary and return sum
 *
 * @param[in] last_value: last value of stat counter
 * @param[in] incr: increment value
 *
 * @note Input parameter should be only nveu64_t type
 *
 * @note
 * API Group:
 * - Initialization: No
 * - Run time: Yes
 * - De-initialization: No
 *
 * @retval 0 on sucess
 * @retval -1 on failure
 */
static inline nveu64_t osi_update_stats_counter(nveu64_t last_value,
						nveu64_t incr)
{
	nveu64_t temp = last_value + incr;

	if (temp < last_value) {
		/* Stats overflow, so reset it to zero */
		temp = 0UL;
	}

	return temp;
}
#endif /* OSI_COMMON_H */

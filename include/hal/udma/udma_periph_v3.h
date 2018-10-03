/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __HAL_UDMA_UDMA_PERIPH_V3_H__
#define __HAL_UDMA_UDMA_PERIPH_V3_H__

#include <hal/pulp.h>
#include <stdint.h>
#include "archi/udma/udma_periph_v3.h"

///////////////////////////////////////////////////
// macro definition to help construction of data field
// of periph register access functions
///////////////////////////////////////////////////
#define PLP_SET_FIELD(reg, data, fieldmask) \
    reg = (reg & ~fieldmask) | (data)

// TODO maybe shift it right by the offset here with new param
#define PLP_GET_FIELD(reg, fieldmask) \
    ((reg & fieldmask))

// only for fields of 1 bit
#define PLP_BITSET_FIELD(reg, mask) \
    reg |= mask

// only for fields of 1 bit
#define PLP_BITCLR_FIELD(reg, mask) \
    reg &= (~mask)
///////////////////////////////////////////////////

#define ARCHI_SOC_EVENT_UDMA_RX(periph)   ((periph)*2)
#define ARCHI_SOC_EVENT_UDMA_TX(periph)   ((periph)*2 + 1)


#ifdef ARCHI_UDMA_HAS_I2C

#define UDMA_I2C_OFFSET(id)           UDMA_PERIPH_OFFSET(ARCHI_UDMA_I2C_ID(id))

#define UDMA_I2C_DATA_ADDR(id)        (ARCHI_UDMA_ADDR + UDMA_I2C_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_I2C_CMD_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_I2C_OFFSET(id) + UDMA_CHANNEL_TX_OFFSET)
#define UDMA_I2C_CUSTOM_ADDR(id)      (ARCHI_UDMA_ADDR + UDMA_I2C_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)
#endif



/*
 * SPIM
 */
#ifdef ARCHI_UDMA_HAS_SPIM

#define UDMA_SPIM_OFFSET(id)          UDMA_PERIPH_OFFSET(ARCHI_UDMA_SPIM_ID(id))
#define UDMA_SPIM_CMD_ADDR(id)        (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + 0x20)
#define UDMA_SPIM_RX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + 0x00)
#define UDMA_SPIM_TX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + 0x10)
#define UDMA_SPIM_CUSTOM_ADDR(id)     (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)

#endif

/*
 * TCDM
 */
#ifdef ARCHI_UDMA_HAS_TCDM

#define UDMA_TCDM_OFFSET              UDMA_PERIPH_OFFSET(ARCHI_UDMA_TCDM_ID(id))

#define UDMA_TCDM_RX_ADDR             (ARCHI_UDMA_ADDR + UDMA_TCDM_OFFSET + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_TCDM_TX_ADDR             (ARCHI_UDMA_ADDR + UDMA_TCDM_OFFSET + UDMA_CHANNEL_TX_OFFSET)
#define UDMA_TCDM_CUSTOM_ADDR         (ARCHI_UDMA_ADDR + UDMA_TCDM_OFFSET + UDMA_CHANNEL_CUSTOM_OFFSET)

static inline void plp_tcdm_setDst(unsigned int addr)
{
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_TCDM_OFFSET + UDMA_CHANNEL_CUSTOM_OFFSET + TCDM_T_DST_SADDR, addr);
}

static inline void plp_tcdm_setSrc(unsigned int addr)
{
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_TCDM_OFFSET + UDMA_CHANNEL_CUSTOM_OFFSET + TCDM_T_SRC_SADDR, addr);
}

static inline void plp_udma_enqueue_toTcdm(unsigned int l2Addr, unsigned int fcTcdmAddr, unsigned int size, unsigned int cfg)
{
  plp_tcdm_setDst(fcTcdmAddr);
  plp_udma_enqueue(UDMA_TCDM_TX_ADDR, l2Addr, size, cfg);
}

static inline void plp_udma_enqueue_fromTcdm(unsigned int l2Addr, unsigned int fcTcdmAddr, unsigned int size, unsigned int cfg)
{
  plp_tcdm_setSrc(fcTcdmAddr);
  plp_udma_enqueue(UDMA_TCDM_RX_ADDR, l2Addr, size, cfg);
}


#endif

/*
 * CSI2
 */
#ifdef ARCHI_UDMA_HAS_CSI2

#define UDMA_CSI2_OFFSET(id)           UDMA_PERIPH_OFFSET(ARCHI_UDMA_CSI2_ID(id))

#define UDMA_CSI2_RX_ADDR(id)          (ARCHI_UDMA_ADDR + UDMA_CSI2_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_CSI2_CUSTOM_ADDR(id)      (ARCHI_UDMA_ADDR + UDMA_CSI2_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)
// NOTE defined in archi/vega/properties.h
// #define ARCHI_SOC_EVENT_CSI2_RX(id)    ARCHI_SOC_EVENT_UDMA_RX(ARCHI_UDMA_CSI2_ID(id))

// CSI2 HAL Registers Structure
struct plpUdmaCsi2_struct_t {
    plpUdmaRxTxChHandle_t udma_rxtx_ch_handle;
    uint32_t unused;
    uint32_t cfg_clk;
};
typedef volatile struct plpUdmaCsi2_struct_t plpUdmaCsi2Handle_t;

// CSI2 HAL Handle
// located in "archi/chip/vega/memory_map.h"

// CSI2 HAL functions prototype
static inline void plpUdmaCsi2ClkCfgSet (plpUdmaCsi2Handle_t * handle, uint32_t data);
static inline uint32_t plpUdmaCsi2ClkCfgGet (plpUdmaCsi2Handle_t * handle);

// CSI2 HAL functions definition
static inline void plpUdmaCsi2ClkCfgSet (plpUdmaCsi2Handle_t * handle, uint32_t data) {
    handle->cfg_clk = data;
}

static inline uint32_t plpUdmaCsi2ClkCfgGet (plpUdmaCsi2Handle_t * handle) {
    return handle->cfg_clk;
}

#endif // ARCHI_UDMA_HAS_CSI2

/*
 * CAM
 */
#ifdef ARCHI_UDMA_HAS_CAM

#define UDMA_CAM_OFFSET(id)           UDMA_PERIPH_OFFSET(ARCHI_UDMA_CAM_ID(id))

#define UDMA_CAM_RX_ADDR(id)          (ARCHI_UDMA_ADDR + UDMA_CAM_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_CAM_CUSTOM_ADDR(id)      (ARCHI_UDMA_ADDR + UDMA_CAM_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)
#define ARCHI_SOC_EVENT_CAM_RX(id)    ARCHI_SOC_EVENT_UDMA_RX(ARCHI_UDMA_CAM_ID(id))

// CAM HAL Registers Structure
typedef struct {
    plpUdmaRxTxChHandle_t udma_rxtx_ch_handle;
    uint32_t cfg_glob;
    uint32_t cfg_ll;
    uint32_t cfg_ur;
    uint32_t cfg_size;
    uint32_t cfg_filter;
} plpUdmaCameraHandle_t;

// CAM HAL Register Fields Structure
typedef struct {
    uint32_t framedrop_enable:1;
    uint32_t framedrop_value:6;
    uint32_t frameslice_enable:1;
    uint32_t format:3;
    uint32_t shift:4;
    uint32_t unused:16;
    uint32_t enable:1;
} __attribute__ ((packed)) plpUdmaCamCfgGlob_t;

typedef struct {
    uint32_t frameslice_llx:16;
    uint32_t frameslice_lly:16;
} __attribute__ ((packed)) plpUdmaCamCfgLl_t;

typedef struct {
    uint32_t frameslice_urx:16;
    uint32_t frameslice_ury:16;
} __attribute__ ((packed)) plpUdmaCamCfgUr_t;

typedef struct {
    uint32_t unused:16;
    uint32_t row_length:16;
} __attribute__ ((packed)) plpUdmaCamCfgSize_t;

typedef struct {
    uint32_t b_coeff:8;
    uint32_t g_coeff:8;
    uint32_t r_coeff:8;
    uint32_t unused:8;
} __attribute__ ((packed)) plpUdmaCamCfgFilter_t;

// CAM HAL Register Union definition
typedef union {
    plpUdmaCamCfgGlob_t cfg_glob;
    plpUdmaCamCfgLl_t cfg_ll;
    plpUdmaCamCfgUr_t cfg_ur;
    plpUdmaCamCfgSize_t cfg_size;
    plpUdmaCamCfgFilter_t cfg_filter;
    uint32_t raw;
} __attribute__ ((packed)) plpUdmaCamCustom_u;

static inline void hal_cpi_ll_set(int cpi, unsigned int value){
  pulp_write32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_LL_OFFSET, value);
}
static inline unsigned int hal_cpi_ll_read(int cpi){
  return pulp_read32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_LL_OFFSET);
}
static inline void hal_cpi_ur_set(int cpi, unsigned int value){
  pulp_write32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_UR_OFFSET, value);
}
static inline unsigned int hal_cpi_ur_read(int cpi){
  return  pulp_read32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_UR_OFFSET);
}
static inline void hal_cpi_size_set(int cpi, unsigned int value){
  pulp_write32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_SIZE_OFFSET, value);
}
static inline unsigned int hal_cpi_size_read(int cpi){
  return pulp_read32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_SIZE_OFFSET);
}
static inline void hal_cpi_glob_set(int cpi, unsigned int value){
  pulp_write32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_GLOB_OFFSET, value);
}
static inline unsigned int hal_cpi_glob_read(int cpi){
  return pulp_read32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_GLOB_OFFSET);
}
static inline void hal_cpi_filter_set(int cpi, unsigned int value){
  pulp_write32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_FILTER_OFFSET, value);
}
static inline unsigned int hal_cpi_filter_read(int cpi){
  return pulp_read32(UDMA_CAM_CUSTOM_ADDR(cpi) + CAM_FILTER_OFFSET);
}

///////////////////////////////////////////////////
// TODO Obsolete : to be removed cause deprecated
static inline void plp_cam_glob_set(int enabled, int frameDropEn, int nbFrameDrop, int frameSliceEn, int format, int shift)
{
  unsigned int value = (enabled << CAM_CFG_GLOB_EN_BIT) | (frameDropEn << CAM_CFG_GLOB_FRAMEDROP_EN_BIT) | (nbFrameDrop << CAM_CFG_GLOB_FRAMEDROP_VAL_BIT) | (format << CAM_CFG_GLOB_FORMAT_BIT) | (shift << CAM_CFG_GLOB_SHIFT_BIT);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_CAM_OFFSET(0) + UDMA_CHANNEL_CUSTOM_OFFSET + CAM_GLOB_OFFSET, value);
}

// CAM HAL Handle
#define CAM_HANDLE(id) ((plpUdmaCameraHandle_t *)(ARCHI_UDMA_ADDR + UDMA_CAM_OFFSET(id)))

// CAM HAL functions prototype
static inline void plpUdmaCamGlobSet (plpUdmaCameraHandle_t * const handle, uint32_t data);
static inline uint32_t plpUdmaCamGlobGet (plpUdmaCameraHandle_t * const handle);
static inline void plpUdmaCamLlSet (plpUdmaCameraHandle_t * const handle, uint32_t data);
static inline uint32_t plpUdmaCamLlGet (plpUdmaCameraHandle_t * const handle);
static inline void plpUdmaCamUrSet (plpUdmaCameraHandle_t * const handle, uint32_t data);
static inline uint32_t plpUdmaCamUrGet (plpUdmaCameraHandle_t * const handle);
static inline void plpUdmaCamSizeSet (plpUdmaCameraHandle_t * const handle, uint32_t data);
static inline uint32_t plpUdmaCamSizeGet (plpUdmaCameraHandle_t * const handle);
static inline void plpUdmaCamFilterSet (plpUdmaCameraHandle_t * const handle, uint32_t data);
static inline uint32_t plpUdmaCamFilterGet (plpUdmaCameraHandle_t * const handle);

// CAM HAL functions definition
static inline void plpUdmaCamGlobSet (plpUdmaCameraHandle_t * const handle, uint32_t data) {
    handle->cfg_glob = data;
}

static inline uint32_t plpUdmaCamGlobGet (plpUdmaCameraHandle_t * const handle) {
    return handle->cfg_glob;
}

static inline void plpUdmaCamLlSet (plpUdmaCameraHandle_t * const handle, uint32_t data) {
    handle->cfg_ll = data;
}

static inline uint32_t plpUdmaCamLlGet (plpUdmaCameraHandle_t * const handle) {
    return handle->cfg_ll;
}

static inline void plpUdmaCamUrSet (plpUdmaCameraHandle_t * const handle, uint32_t data) {
    handle->cfg_ur = data;
}

static inline uint32_t plpUdmaCamUrGet (plpUdmaCameraHandle_t * const handle) {
    return handle->cfg_ur;
}

static inline void plpUdmaCamSizeSet (plpUdmaCameraHandle_t * const handle, uint32_t data) {
    handle->cfg_size = data;
}

static inline uint32_t plpUdmaCamSizeGet (plpUdmaCameraHandle_t * const handle) {
    return handle->cfg_size;
}

static inline void plpUdmaCamFilterSet (plpUdmaCameraHandle_t * const handle, uint32_t data) {
    handle->cfg_filter = data;
}

static inline uint32_t plpUdmaCamFilterGet (plpUdmaCameraHandle_t * const handle) {
    return handle->cfg_filter;
}

///////////////////////////////////////////////////

#endif

/*
 * I2S
 */

#ifdef ARCHI_UDMA_HAS_I2S

#define UDMA_I2S_OFFSET(id)           UDMA_PERIPH_OFFSET(ARCHI_UDMA_I2S_ID(id))

#define UDMA_I2S_ADDR(id)             (ARCHI_UDMA_ADDR + UDMA_I2S_OFFSET(id)) //FIXME shall be removed because deprecated
#define UDMA_I2S_RX0_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_I2S_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_I2S_RX1_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_I2S_OFFSET(id) + UDMA_CHANNEL_TX_OFFSET)
#define UDMA_I2S_CUSTOM_ADDR(id)      (ARCHI_UDMA_ADDR + UDMA_I2S_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)

#include "hal/udma/udma_periph_i2s_v1.h"

#endif

/*
 * UART
 */

#ifdef ARCHI_UDMA_HAS_UART

#define UDMA_UART_OFFSET(id)          UDMA_PERIPH_OFFSET(ARCHI_UDMA_UART_ID(id))
#define UDMA_UART_RX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_UART_TX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(id) + UDMA_CHANNEL_TX_OFFSET)
#define UDMA_UART_CUSTOM_ADDR(id)     (ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)
#define ARCHI_SOC_EVENT_UART_RX(id)    (ARCHI_SOC_EVENT_PERIPH_FIRST_EVT(ARCHI_UDMA_UART_ID(id)) + ARCHI_UDMA_UART_RX_EVT)
#define ARCHI_SOC_EVENT_UART_TX(id)    (ARCHI_SOC_EVENT_PERIPH_FIRST_EVT(ARCHI_UDMA_UART_ID(id)) + ARCHI_UDMA_UART_TX_EVT)

// UART HAL Registers Structure
typedef struct {
    plpUdmaRxTxChHandle_t udma_rxtx_ch_handle;
    uint32_t status;
    uint32_t setup;
} plpUdmaUartHandle_t;

// UART HAL Register Fields Structure
typedef struct {
    uint32_t tx_oe:1;
    uint32_t rx_oe:1;
    uint32_t rx_pe:1;
    uint32_t unused:29;
} __attribute__ ((packed)) plpUdmaUartStatus_t;

typedef struct {
    uint32_t parity_en:1;
    uint32_t char_length:2;
    uint32_t stop_bit_length:1;
    uint32_t unused0:4;
    uint32_t tx_en:1;
    uint32_t rx_en:1;
    uint32_t unused1:6;
    uint32_t clkdiv:16;
} __attribute__ ((packed)) plpUdmaUartSetup_t;

// UART HAL Register Union definition
typedef union {
    plpUdmaUartStatus_t  status;
    plpUdmaUartSetup_t	 setup;
    uint32_t raw;
} __attribute__ ((packed)) plpUdmaUartCustom_u;

// UART HAL Handle
#define UART_HANDLE(id) ((plpUdmaUartHandle_t *)(ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(id)))

// UART HAL functions prototype
static inline void plpUdmaUartSetupSet (plpUdmaUartHandle_t * const handle, uint32_t data);
static inline uint32_t plpUdmaUartSetupGet (plpUdmaUartHandle_t * const handle);
static inline uint32_t plpUdmaUartStatusGet (plpUdmaUartHandle_t * const handle);

// UART HAL functions definition
static inline void plpUdmaUartSetupSet (plpUdmaUartHandle_t * const handle, uint32_t data) {
    handle->setup = data;
}

static inline uint32_t plpUdmaUartSetupGet (plpUdmaUartHandle_t * const handle) {
    return handle->setup;
}

static inline uint32_t plpUdmaUartStatusGet (plpUdmaUartHandle_t * const handle) {
    return handle->status;
}

///////////////////////////////////////////////////
// TODO Obsolete : to be removed cause deprecated
/**
 * Setup UART. The UART defaults to 8 bit character mode with 1 stop bit.
 *
 * parity       Enable/disable parity mode
 * clk_counter  Clock counter value that is used to derive the UART clock.
 *              There is a prescaler in place that already divides the SoC
 *              clock by 16.  Since this is a counter, a value of 0 means that
 *              the SoC clock divided by 16 is used. A value of 31 would mean
 *              that we use the SoC clock divided by 16*32 = 512.
 */

static inline void plp_uart_setup(int channel, int parity, uint16_t clk_counter)
{

  // [31:16]: clock divider (from SoC clock)
  // [9]: RX enable
  // [8]: TX enable
  // [3]: stop bits  0 = 1 stop bit
  //                 1 = 2 stop bits
  // [2:1]: bits     00 = 5 bits
  //                 01 = 6 bits
  //                 10 = 7 bits
  //                 11 = 8 bits
  // [0]: parity

  unsigned int val = 0x0306 | parity; // both tx and rx enabled; 8N1 configuration; 1 stop bits

  val |= ((clk_counter) << 16);

  pulp_write32(ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + UART_SETUP_OFFSET, val);
}

static inline void plp_uart_disable(int channel) {
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + UART_SETUP_OFFSET, 0x00500006);
}

static inline int plp_uart_tx_busy(int channel) {
  return pulp_read32(ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + UART_STATUS_OFFSET) & 1;
}

static inline int plp_uart_rx_busy(int channel) {
  return (pulp_read32(ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + UART_STATUS_OFFSET) >> 1) & 1;
}

static inline unsigned int plp_uart_reg_read(int channel, unsigned int addr)
{ //adr is an offset, expected UART_STATUS_OFFSET or UART_SETUP_OFFSET
  return pulp_read32(ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + addr);
}

static inline void plp_uart_reg_write(int channel, unsigned int addr, unsigned int cfg)
{ //adr is an offset, expected UART_STATUS_OFFSET or UART_SETUP_OFFSET
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_UART_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + addr, cfg);
}

#endif

/*
 * Hyperbus
 */

#ifdef ARCHI_UDMA_HAS_HYPER

#define UDMA_HYPER_OFFSET(id)         UDMA_PERIPH_OFFSET(ARCHI_UDMA_HYPER_ID(id))
#define UDMA_HYPER_RX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_HYPER_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_HYPER_TX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_HYPER_OFFSET(id) + UDMA_CHANNEL_TX_OFFSET)
#define UDMA_HYPER_CUSTOM_ADDR(id)     (ARCHI_UDMA_ADDR + UDMA_HYPER_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)
#define ARCHI_SOC_EVENT_HYPER_RX(id)    (ARCHI_SOC_EVENT_PERIPH_FIRST_EVT(ARCHI_UDMA_HYPER_ID(id)) + ARCHI_UDMA_HYPER_RX_EVT)
#define ARCHI_SOC_EVENT_HYPER_TX(id)    (ARCHI_SOC_EVENT_PERIPH_FIRST_EVT(ARCHI_UDMA_HYPER_ID(id)) + ARCHI_UDMA_HYPER_TX_EVT)

#include "archi/udma/hyper.h"
#include "hal/udma/udma_periph_hyper_v1.h"

static inline void hal_hyper_ext_addr_set(unsigned int value)
{
    pulp_write32(ARCHI_UDMA_ADDR + HYPER_EXT_ADDR_OFFSET, value);
}

static inline void hal_hyper_ram_ext_addr_set(unsigned int value) {return hal_hyper_ext_addr_set(REG_MBR0 | value);}

static inline void hal_hyper_flash_ext_addr_set(unsigned int value) {return hal_hyper_ext_addr_set(REG_MBR1 | value);}

static inline void hal_hyper_enqueue(unsigned channelBase, unsigned int l2Addr, unsigned int ext_addr, unsigned int size, unsigned int cfg) {
  hal_hyper_ext_addr_set(ext_addr);
  plp_udma_enqueue(channelBase, l2Addr, size, cfg);
}

static inline void hal_hyper_ram_enqueue(unsigned channelBase, unsigned int l2Addr, unsigned int ext_addr, unsigned int size, unsigned int cfg) {
  hal_hyper_ram_ext_addr_set(ext_addr);
  plp_udma_enqueue(channelBase, l2Addr, size, cfg);
}

static inline void hal_hyper_flash_enqueue(unsigned channelBase, unsigned int l2Addr, unsigned int ext_addr, unsigned int size, unsigned int cfg) {
  hal_hyper_flash_ext_addr_set(ext_addr);
  plp_udma_enqueue(channelBase, l2Addr, size, cfg);
}

#endif

#endif

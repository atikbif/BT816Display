/**
  **************************************************************************
  * @file     at32_emac.h
  * @version  v2.0.6
  * @date     2022-03-11
  * @brief    header file of emac config program.
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to 
  * download from Artery official website is the copyrighted work of Artery. 
  * Artery authorizes customers to use, copy, and distribute the BSP 
  * software and its related documentation for the purpose of design and 
  * development in conjunction with Artery microcontrollers. Use of the 
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
  
/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32_EMAC_H
#define __AT32_EMAC_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* includes ------------------------------------------------------------------*/                                                                               
#include "at32f435_437.h"
#include "netif.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */
  
/** @addtogroup 437_EMAC_tcp_server
  * @{
  */


#define ETH_MDC_GPIO_CRM_CLK            CRM_GPIOC_PERIPH_CLOCK
#define ETH_MDC_GPIO					GPIOC
#define ETH_MDC_GPIO_PIN				GPIO_PINS_1
#define ETH_MDC_GPIO_PIN_SOURCE			GPIO_PINS_SOURCE1
#define ETH_MDC_GPIO_MUX				GPIO_MUX_11

#define ETH_MDIO_GPIO_CRM_CLK           CRM_GPIOA_PERIPH_CLOCK
#define ETH_MDIO_GPIO					GPIOA
#define ETH_MDIO_GPIO_PIN				GPIO_PINS_2
#define ETH_MDIO_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE2
#define ETH_MDIO_GPIO_MUX				GPIO_MUX_11

#define ETH_TXD0_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK
#define ETH_TXD0_GPIO					GPIOB
#define ETH_TXD0_GPIO_PIN				GPIO_PINS_12
#define ETH_TXD0_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE12
#define ETH_TXD0_GPIO_MUX				GPIO_MUX_11

#define ETH_TXD1_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK
#define ETH_TXD1_GPIO					GPIOB
#define ETH_TXD1_GPIO_PIN				GPIO_PINS_13
#define ETH_TXD1_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE13
#define ETH_TXD1_GPIO_MUX				GPIO_MUX_11

#define ETH_TXD2_GPIO_CRM_CLK           CRM_GPIOC_PERIPH_CLOCK
#define ETH_TXD2_GPIO					GPIOC
#define ETH_TXD2_GPIO_PIN				GPIO_PINS_2
#define ETH_TXD2_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE2
#define ETH_TXD2_GPIO_MUX				GPIO_MUX_11

#define ETH_TXD3_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK
#define ETH_TXD3_GPIO					GPIOB
#define ETH_TXD3_GPIO_PIN				GPIO_PINS_8
#define ETH_TXD3_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE8
#define ETH_TXD3_GPIO_MUX				GPIO_MUX_11

#define ETH_TXCLK_GPIO_CRM_CLK          CRM_GPIOC_PERIPH_CLOCK
#define ETH_TXCLK_GPIO					GPIOC
#define ETH_TXCLK_GPIO_PIN				GPIO_PINS_3
#define ETH_TXCLK_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE3
#define ETH_TXCLK_GPIO_MUX				GPIO_MUX_11

#define ETH_RXD0_GPIO_CRM_CLK           CRM_GPIOC_PERIPH_CLOCK
#define ETH_RXD0_GPIO					GPIOC
#define ETH_RXD0_GPIO_PIN				GPIO_PINS_4
#define ETH_RXD0_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE4
#define ETH_RXD0_GPIO_MUX				GPIO_MUX_11

#define ETH_RXD1_GPIO_CRM_CLK           CRM_GPIOC_PERIPH_CLOCK
#define ETH_RXD1_GPIO					GPIOC
#define ETH_RXD1_GPIO_PIN				GPIO_PINS_5
#define ETH_RXD1_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE5
#define ETH_RXD1_GPIO_MUX				GPIO_MUX_11

#define ETH_RXD2_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK
#define ETH_RXD2_GPIO					GPIOB
#define ETH_RXD2_GPIO_PIN				GPIO_PINS_0
#define ETH_RXD2_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE0
#define ETH_RXD2_GPIO_MUX				GPIO_MUX_11

#define ETH_RXD3_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK
#define ETH_RXD3_GPIO					GPIOB
#define ETH_RXD3_GPIO_PIN				GPIO_PINS_1
#define ETH_RXD3_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE1
#define ETH_RXD3_GPIO_MUX				GPIO_MUX_11

#define ETH_RXCLK_GPIO_CRM_CLK          CRM_GPIOA_PERIPH_CLOCK
#define ETH_RXCLK_GPIO					GPIOA
#define ETH_RXCLK_GPIO_PIN				GPIO_PINS_1
#define ETH_RXCLK_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE1
#define ETH_RXCLK_GPIO_MUX				GPIO_MUX_11

#define ETH_RXDV_GPIO_CRM_CLK           CRM_GPIOA_PERIPH_CLOCK
#define ETH_RXDV_GPIO					GPIOA
#define ETH_RXDV_GPIO_PIN				GPIO_PINS_7
#define ETH_RXDV_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE7
#define ETH_RXDV_GPIO_MUX				GPIO_MUX_11

#define ETH_TXEN_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK
#define ETH_TXEN_GPIO					GPIOB
#define ETH_TXEN_GPIO_PIN				GPIO_PINS_11
#define ETH_TXEN_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE11
#define ETH_TXEN_GPIO_MUX				GPIO_MUX_11

#define ETH_COL_GPIO_CRM_CLK            CRM_GPIOA_PERIPH_CLOCK
#define ETH_COL_GPIO					GPIOA
#define ETH_COL_GPIO_PIN				GPIO_PINS_3
#define ETH_COL_GPIO_PIN_SOURCE			GPIO_PINS_SOURCE3
#define ETH_COL_GPIO_MUX				GPIO_MUX_11

#define ETH_CRS_GPIO_CRM_CLK            CRM_GPIOA_PERIPH_CLOCK
#define ETH_CRS_GPIO					GPIOA
#define ETH_CRS_GPIO_PIN				GPIO_PINS_0
#define ETH_CRS_GPIO_PIN_SOURCE			GPIO_PINS_SOURCE0
#define ETH_CRS_GPIO_MUX				GPIO_MUX_11

#define ETH_RXERR_GPIO_CRM_CLK          CRM_GPIOB_PERIPH_CLOCK
#define ETH_RXERR_GPIO					GPIOB
#define ETH_RXERR_GPIO_PIN				GPIO_PINS_10
#define ETH_RXERR_GPIO_PIN_SOURCE		GPIO_PINS_SOURCE10
#define ETH_RXERR_GPIO_MUX				GPIO_MUX_11

#define ETH_RST_GPIO_CRM_CLK          	CRM_GPIOC_PERIPH_CLOCK
#define ETH_RST_GPIO					GPIOC
#define ETH_RST_GPIO_PIN				GPIO_PINS_0


#define PHY_ADDRESS                      (0x01)       /*!< relative to at32 board */
#define PHY_CONTROL_REG                  (0x00)       /*!< basic mode control register */
#define PHY_STATUS_REG                   (0x01)       /*!< basic mode status register */
#define PHY_SPECIFIED_CS_REG             (0x1F)       /*!< phy status register */

#define PHY_AUTO_NEGOTIATION_BIT         (0x1000)     /*!< enable auto negotiation */
#define PHY_LOOPBACK_BIT                 (0x4000)     /*!< enable loopback */
#define PHY_RESET_BIT                    (0x8000)     /*!< reset phy */

#define PHY_LINKED_STATUS_BIT            (0x0004)     /*!< link status */
#define PHY_NEGO_COMPLETE_BIT            (0x0020)     /*!< auto negotiation complete */

#define PHY_DUPLEX_MODE                  (0x0010)     /*!< full duplex mode */
#define PHY_SPEED_MODE                   (0x0004)     /*!< 10 mbps */


  
error_status emac_system_init(void);
void emac_nvic_configuration(void);
void emac_pins_configuration(void);
error_status emac_layer2_configuration(void);
void static reset_phy(void);
error_status emac_phy_register_reset(void);
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed);
error_status emac_phy_init(emac_control_config_type *control_para);

uint16_t link_update(void);
void ethernetif_set_link(void const *argument);
void ethernetif_notify_conn_changed(struct netif *netif);
void ethernetif_update_config(struct netif *netif);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

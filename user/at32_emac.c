/**
  **************************************************************************
  * @file     at32_emac.c
  * @version  v2.0.6
  * @date     2022-03-11
  * @brief    emac config program
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

/* includes ------------------------------------------------------------------*/                                                                               
#include "at32f435_437_board.h"
#include "lwip/dhcp.h"
#include "at32_emac.h"
#include "FreeRTOS.h"
#include "task.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */
  
/** @addtogroup 437_EMAC_tcp_server
  * @{
  */

emac_control_config_type mac_control_para;
  
/**
  * @brief  enable emac clock and gpio clock
  * @param  none
  * @retval success or error
  */
error_status emac_system_init(void)
{
  error_status status;
 
  emac_nvic_configuration();
  
  /* emac periph clock enable */
  crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_EMACTX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_EMACRX_PERIPH_CLOCK, TRUE);
  
  emac_pins_configuration();
  status = emac_layer2_configuration();
  
  return status;
}

/**
  * @brief  configures emac irq channel.
  * @param  none
  * @retval none
  */
void emac_nvic_configuration(void)
{
  nvic_irq_enable(EMAC_IRQn, 4, 0);
}

/**
  * @brief  configures emac required pins.
  * @param  none
  * @retval none
  */
void emac_pins_configuration(void)
{
	gpio_init_type gpio_init_struct = {0};

	crm_periph_clock_enable(ETH_MDC_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_MDIO_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_TXD0_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_TXD1_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_TXD2_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_TXD3_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_TXCLK_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_RXD0_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_RXD1_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_RXD2_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_RXD3_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_RXCLK_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_RXDV_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_TXEN_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_COL_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_CRS_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(ETH_RXERR_GPIO_CRM_CLK, TRUE);

	gpio_pin_mux_config(ETH_MDC_GPIO, ETH_MDC_GPIO_PIN_SOURCE, ETH_MDC_GPIO_MUX);
	gpio_pin_mux_config(ETH_MDIO_GPIO, ETH_MDIO_GPIO_PIN_SOURCE, ETH_MDIO_GPIO_MUX);
	gpio_pin_mux_config(ETH_TXD0_GPIO, ETH_TXD0_GPIO_PIN_SOURCE, ETH_TXD0_GPIO_MUX);
	gpio_pin_mux_config(ETH_TXD1_GPIO, ETH_TXD1_GPIO_PIN_SOURCE, ETH_TXD1_GPIO_MUX);
	gpio_pin_mux_config(ETH_TXD2_GPIO, ETH_TXD2_GPIO_PIN_SOURCE, ETH_TXD2_GPIO_MUX);
	gpio_pin_mux_config(ETH_TXD3_GPIO, ETH_TXD3_GPIO_PIN_SOURCE, ETH_TXD3_GPIO_MUX);
	gpio_pin_mux_config(ETH_TXCLK_GPIO, ETH_TXCLK_GPIO_PIN_SOURCE, ETH_TXCLK_GPIO_MUX);
	gpio_pin_mux_config(ETH_RXD0_GPIO, ETH_RXD0_GPIO_PIN_SOURCE, ETH_RXD0_GPIO_MUX);
	gpio_pin_mux_config(ETH_RXD1_GPIO, ETH_RXD1_GPIO_PIN_SOURCE, ETH_RXD1_GPIO_MUX);
	gpio_pin_mux_config(ETH_RXD2_GPIO, ETH_RXD2_GPIO_PIN_SOURCE, ETH_RXD2_GPIO_MUX);
	gpio_pin_mux_config(ETH_RXD3_GPIO, ETH_RXD3_GPIO_PIN_SOURCE, ETH_RXD3_GPIO_MUX);
	gpio_pin_mux_config(ETH_RXCLK_GPIO, ETH_RXCLK_GPIO_PIN_SOURCE, ETH_RXCLK_GPIO_MUX);
	gpio_pin_mux_config(ETH_RXDV_GPIO, ETH_RXDV_GPIO_PIN_SOURCE, ETH_RXDV_GPIO_MUX);
	gpio_pin_mux_config(ETH_TXEN_GPIO, ETH_TXEN_GPIO_PIN_SOURCE, ETH_TXEN_GPIO_MUX);
	gpio_pin_mux_config(ETH_COL_GPIO, ETH_COL_GPIO_PIN_SOURCE, ETH_COL_GPIO_MUX);
	gpio_pin_mux_config(ETH_CRS_GPIO, ETH_CRS_GPIO_PIN_SOURCE, ETH_CRS_GPIO_MUX);
	gpio_pin_mux_config(ETH_RXERR_GPIO, ETH_RXERR_GPIO_PIN_SOURCE, ETH_RXERR_GPIO_MUX);


	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

	gpio_init_struct.gpio_pins = ETH_MDC_GPIO_PIN;
	gpio_init(ETH_MDC_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_MDIO_GPIO_PIN;
	gpio_init(ETH_MDIO_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_TXD0_GPIO_PIN;
	gpio_init(ETH_TXD0_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_TXD1_GPIO_PIN;
	gpio_init(ETH_TXD1_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_TXD2_GPIO_PIN;
	gpio_init(ETH_TXD2_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_TXD3_GPIO_PIN;
	gpio_init(ETH_TXD3_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_TXCLK_GPIO_PIN;
	gpio_init(ETH_TXCLK_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_RXD0_GPIO_PIN;
	gpio_init(ETH_RXD0_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_RXD1_GPIO_PIN;
	gpio_init(ETH_RXD1_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_RXD2_GPIO_PIN;
	gpio_init(ETH_RXD2_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_RXD3_GPIO_PIN;
	gpio_init(ETH_RXD3_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_RXCLK_GPIO_PIN;
	gpio_init(ETH_RXCLK_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_RXDV_GPIO_PIN;
	gpio_init(ETH_RXDV_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_TXEN_GPIO_PIN;
	gpio_init(ETH_TXEN_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_COL_GPIO_PIN;
	gpio_init(ETH_COL_GPIO, &gpio_init_struct);

	gpio_init_struct.gpio_pins = ETH_CRS_GPIO_PIN;
	gpio_init(ETH_CRS_GPIO, &gpio_init_struct);
}

/**
  * @brief  configures emac layer2
  * @param  none
  * @retval error or success
  */
error_status emac_layer2_configuration(void)
{
	emac_dma_config_type dma_control_para;

	crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
	scfg_emac_interface_set(SCFG_EMAC_SELECT_MII);

	/* reset phy */
	reset_phy();
	/* reset emac ahb bus */
	emac_reset();

	/* software reset emac dma */
	emac_dma_software_reset_set();

	while(emac_dma_software_reset_get() == SET);

	emac_control_para_init(&mac_control_para);

	mac_control_para.auto_nego = EMAC_AUTO_NEGOTIATION_ON;

	if(emac_phy_init(&mac_control_para) == ERROR)
	{
		return ERROR;
	}

	emac_dma_para_init(&dma_control_para);

	dma_control_para.rsf_enable = TRUE;
	dma_control_para.tsf_enable = TRUE;
	dma_control_para.osf_enable = TRUE;
	dma_control_para.aab_enable = TRUE;
	dma_control_para.usp_enable = TRUE;
	dma_control_para.fb_enable = TRUE;
	dma_control_para.flush_rx_disable = TRUE;
	dma_control_para.rx_dma_pal = EMAC_DMA_PBL_32;
	dma_control_para.tx_dma_pal = EMAC_DMA_PBL_32;
	dma_control_para.priority_ratio = EMAC_DMA_1_RX_1_TX;

	emac_dma_config(&dma_control_para);
	emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_NORMAL_SUMMARY, TRUE);
	emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_RX, TRUE);

	return SUCCESS;
}

/**
  * @brief  reset layer 1
  * @param  none
  * @retval none
  */
void static reset_phy(void)
{
	gpio_init_type gpio_init_struct = {0};
	crm_periph_clock_enable(ETH_RST_GPIO_CRM_CLK, TRUE);

	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = ETH_RST_GPIO_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(ETH_RST_GPIO, &gpio_init_struct);
	gpio_bits_reset(ETH_RST_GPIO, ETH_RST_GPIO_PIN);
	vTaskDelay(2);
	gpio_bits_set(ETH_RST_GPIO, ETH_RST_GPIO_PIN);
	vTaskDelay(2);
}

/**
  * @brief  reset phy register
  * @param  none
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_register_reset(void)
{
  uint16_t data = 0;
  uint32_t timeout = 0;
  uint32_t i = 0;

  if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_RESET_BIT) == ERROR)
  {
    return ERROR;
  }

  for(i = 0; i < 0x000FFFFF; i++);

  do
  {
    timeout++;
    if(emac_phy_register_read(PHY_ADDRESS, PHY_CONTROL_REG, &data) == ERROR)
    {
      return ERROR;
    }
  } while((data & PHY_RESET_BIT) && (timeout < PHY_TIMEOUT));

  for(i = 0; i < 0x00FFFFF; i++);
  if(timeout == PHY_TIMEOUT)
  {
    return ERROR;
  }
  return SUCCESS;
}

/**
  * @brief  set mac speed related parameters
  * @param  nego: auto negotiation on or off.
  *         this parameter can be one of the following values:
  *         - EMAC_AUTO_NEGOTIATION_OFF
  *         - EMAC_AUTO_NEGOTIATION_ON.
  * @param  mode: half-duplex or full-duplex.
  *         this parameter can be one of the following values:
  *         - EMAC_HALF_DUPLEX
  *         - EMAC_FULL_DUPLEX.
  * @param  speed: 10 mbps or 100 mbps
  *         this parameter can be one of the following values:
  *         - EMAC_SPEED_10MBPS
  *         - EMAC_SPEED_100MBPS.
  * @retval none
  */
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed)
{
  uint16_t data = 0;
  uint32_t timeout = 0;
  if(nego == EMAC_AUTO_NEGOTIATION_ON)
  {
    do
    {
      timeout++;
      if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
      {
    	  return ERROR;
      }
    } while(!(data & PHY_LINKED_STATUS_BIT) && (timeout < PHY_TIMEOUT));

    if(timeout == PHY_TIMEOUT)
    {
    	return ERROR;
    }

    timeout = 0;

    if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_AUTO_NEGOTIATION_BIT) == ERROR)
    {
    	return ERROR;
    }


    do
    {
      timeout++;
      if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
      {
    	  return ERROR;
      }
    } while(!(data & PHY_NEGO_COMPLETE_BIT) && (timeout < PHY_TIMEOUT));

    if(timeout == PHY_TIMEOUT)
    {
    	return ERROR;
    }

    if(emac_phy_register_read(PHY_ADDRESS, PHY_SPECIFIED_CS_REG, &data) == ERROR)
    {
    	return ERROR;
    }
    if(data & PHY_DUPLEX_MODE)
    {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else
    {
    	emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
    if(data & PHY_SPEED_MODE)
    {
    	emac_fast_speed_set(EMAC_SPEED_10MBPS);
    }
    else
    {
    	emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }
  }
  else
  {
    if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, (uint16_t)((mode << 8) | (speed << 13))) == ERROR)
    {
      return ERROR;
    }
    if(speed == EMAC_SPEED_100MBPS)
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }
    else
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
    }
    if(mode == EMAC_FULL_DUPLEX)
    {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else
    {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
  }

  return SUCCESS;
}

/**
  * @brief  initialize emac phy
  * @param  none
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_init(emac_control_config_type *control_para)
{
  emac_clock_range_set();
  if(emac_phy_register_reset() == ERROR)
  {
    return ERROR;
  }

  emac_control_config(control_para);
  return SUCCESS;
}

/**
  * @brief  updates the link states
  * @param  none
  * @retval link state 0: disconnect, 1: connection
  */
uint16_t link_update(void)
{
  uint16_t link_data, link_state;
  if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &link_data) == ERROR)
  {
    return ERROR;
  }
  
  link_state = (link_data & PHY_LINKED_STATUS_BIT)>>2;
  return link_state;
}

/**
  * @brief  this function sets the netif link status.
  * @param  netif: the network interface
  * @retval none
  */  
void ethernetif_set_link(void const *argument)
{
	uint16_t regvalue = 0;
	struct netif *netif = (struct netif *)argument;

	for(;;) {

		/* read phy_bsr*/
		regvalue = link_update();


		/* check whether the netif link down and the phy link is up */
		if(!netif_is_link_up(netif) && (regvalue))
		{
		/* network cable is connected */
		netif_set_link_up(netif);
		}
		else if(netif_is_link_up(netif) && (!regvalue))
		{
		/* network cable is dis-connected */
		netif_set_link_down(netif);
		}
		vTaskDelay(200);
	}
}

/**
  * @brief  this function notify user about link status changement.
  * @param  netif: the network interface
  * @retval none
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
  /* note : this is function could be implemented in user file 
            when the callback is needed,
  */

  if (netif_is_link_up(netif)) {
    netif_set_up(netif);
  }
  else
    netif_set_down(netif);
}

/**
  * @brief  link callback function, this function is called on change of link status
  *         to update low level driver configuration.
  * @param  netif: the network interface
  * @retval none
  */
void ethernetif_update_config(struct netif *netif)
{
  if(netif_is_link_up(netif))
  { 
    emac_speed_config(mac_control_para.auto_nego, mac_control_para.duplex_mode, mac_control_para.fast_ethernet_speed);
    
    vTaskDelay(300);
    /* enable mac and dma transmission and reception */
    emac_start();
  }
  else
  {
    /* disable mac and dma transmission and reception */
    emac_stop(); 
  }

  ethernetif_notify_conn_changed(netif);
}

/**
  * @}
  */ 

/**
  * @}
  */ 

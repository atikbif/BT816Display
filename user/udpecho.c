#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"

#include <string.h>
#include "bt816_cmd.h"
#include "prog_menu.h"
#include "menu_list.h"
#include "state_menu.h"


#define UDPECHO_THREAD_PRIO  	( tskIDLE_PRIORITY + 5 )
#define UDPECHO_STK_SIZE		512

#define UDP_BUF_LENGTH		1200
#define BLOCK_SIZE			4096
#define PACKET_DATA_SIZE	1024

TaskHandle_t Udpecho_Handler;

static struct netconn *conn;
static struct netbuf *rx_net_buf;
static struct netbuf *tx_net_buf;
static ip_addr_t *addr;
static unsigned short port;

extern uint8_t prog_mode_flag;
extern uint8_t rst_flag;
extern uint16_t rst_tmr;

extern menu_list_t current_menu;

__attribute__((aligned(4))) volatile uint8_t fl_buf[BLOCK_SIZE];



uint8_t udp_tx[UDP_BUF_LENGTH];

static const char dev_name[] = "BT Flash Loader 1.0";

static uint16_t udp_answer(uint8_t *rx, uint16_t rx_cnt) {
	uint16_t res = 0;
	uint16_t tmp = 0;
	uint16_t offset = 0;
	uint16_t cnt = 0;
	uint16_t i=0;
	uint32_t addr = 0;
	uint8_t protocol_version = rx[0];
	if(protocol_version>=1) {
		if((rx[1]=='B')&&(rx[2]=='T')) { // protocol id
			udp_tx[0] = 0x01; // protocol version
			udp_tx[1] = 'B';
			udp_tx[2] = 'T';
			if(rx[3]==1) {	// 1 - request, 0 -answer
				uint16_t id = ((uint16_t)rx[4]<<8) | rx[5];		// packet id
				uint16_t cmd = ((uint16_t)rx[6]<<8) | rx[7];	// cmd code
				udp_tx[3] = 0x00;	// answer
				udp_tx[4] = rx[4];	// repeat packet_id
				udp_tx[5] = rx[5];
				udp_tx[6] = rx[6];	// repeat cmd
				udp_tx[7] = rx[7];
				switch(cmd) {
					case 0x00:	// device name
						tmp = strlen(dev_name);
						memcpy(&udp_tx[8],dev_name,tmp);
						res = 8 + tmp;
						break;
					case 0x01:	// update flash

						addr = rx[8];addr = addr<<8;
						addr |= rx[9];addr = addr<<8;
						addr |= rx[10];addr = addr<<8;
						addr |= rx[11];
						tmp = rx[12]; // subpacket num - 0..BLOCK_SIZE/PACKET_DATA_SIZE-1

						if(prog_mode_flag) {
							rst_flag = 1;
							rst_tmr = 2000;
							if(addr%BLOCK_SIZE==0 && (tmp<BLOCK_SIZE/PACKET_DATA_SIZE) && (rx_cnt==13+PACKET_DATA_SIZE)) {
								if(tmp==0) for(i=0;i<BLOCK_SIZE;i++) fl_buf[offset+i] = 0;
								offset = PACKET_DATA_SIZE*tmp;
								for(i=0;i<PACKET_DATA_SIZE;i++) fl_buf[offset+i] = rx[13+i];
								if(tmp==BLOCK_SIZE/PACKET_DATA_SIZE-1) {
									bt816_cmd_memwrite(0,BLOCK_SIZE,(uint8_t*)fl_buf);
									bt816_cmd_flashupdate(addr,0,BLOCK_SIZE);
									vTaskDelay(5);
								}
								udp_tx[8] = 1; // success
								res = 9;
							}
						}else {
							// access denied
						}


						break;
					case 0x02:	// read flash
						addr = rx[8];addr = addr<<8;
						addr |= rx[9];addr = addr<<8;
						addr |= rx[10];addr = addr<<8;
						addr |= rx[11];
						cnt = ((uint16_t)rx[12]<<8) | rx[13];	// length
						if(prog_mode_flag) {
							bt816_cmd_flashread(0, addr, cnt);
							vTaskDelay(5);
							udp_tx[8] = 1; // success
							for(i=0;i<cnt;i++) {
								udp_tx[9+i] = bt816_mem_read8(i);
							}
							res = 9 + cnt;
						}else {
							// access denied
						}

						break;
					case 0x03:	// set program mode
						prog_mode_flag = 1;
						init_prog_menu();
						current_menu = MENU_PROG;
						udp_tx[8] = 0x01;
						res = 9;
						break;
					case 0x04:
						prog_mode_flag = 0;
						init_state_menu();
						current_menu = MENU_STATE;
						udp_tx[8] = 0x01;
						res = 9;
						break;
				}
			}

		}
	}

	return res;
}

/*-----------------------------------------------------------------------------------*/
static void udpecho_thread(void *arg)
{
  err_t err, recv_err;
  struct pbuf *txBuf;

  LWIP_UNUSED_ARG(arg);

  conn = netconn_new(NETCONN_UDP);
  if (conn != NULL)
  {
    err = netconn_bind(conn, IP_ADDR_ANY, 7);
    if (err == ERR_OK)
    {
      while (1)
      {
        recv_err = netconn_recv(conn, &rx_net_buf);
        if (recv_err == ERR_OK)
        {
          addr = netbuf_fromaddr(rx_net_buf);
          port = netbuf_fromport(rx_net_buf);

          uint16_t cnt = udp_answer(rx_net_buf->p->payload, rx_net_buf->p->len);
          if(cnt) {
        	  netconn_connect(conn, addr, port);
        	  tx_net_buf = netbuf_new();
        	  uint8_t *data = netbuf_alloc(tx_net_buf, cnt);
        	  memcpy(data,udp_tx,cnt);
        	  netconn_send(conn, tx_net_buf);
        	  netbuf_delete(tx_net_buf);
        	  netconn_disconnect(conn);
          }
          netbuf_delete(rx_net_buf);
        }
      }
    }
    else {
      netconn_delete(conn);
    }
  }
}
/*-----------------------------------------------------------------------------------*/
void udpecho_init(void)
{
  sys_thread_new("udpecho_thread", udpecho_thread, NULL, UDPECHO_STK_SIZE,UDPECHO_THREAD_PRIO );	
}

#endif /* LWIP_NETCONN */

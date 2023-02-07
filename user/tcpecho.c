#include "lwip/opt.h"

//#define  tx_size  20000
//u8_t tx_buf_t[tx_size] = {0};

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"
/*-----------------------------------------------------------------------------------*/
#define TCPECHO_THREAD_PRIO  ( tskIDLE_PRIORITY + 5 )



static void tcpecho_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
  void *data;
  u16_t len;
//	int i;
  err_t recv_err;
  LWIP_UNUSED_ARG(arg);
//	
//	for(i=0;i<tx_size;i++)
//	{
//		tx_buf_t[i] = 't';
//	}

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!=NULL)
  {  
    /* Bind connection to well known port number 7. */
    err = netconn_bind(conn, IP_ADDR_ANY, 8088);
    
    if (err == ERR_OK)
    {
      /* Tell connection to go into listening mode. */
      netconn_listen(conn);
      while (1) 
      {
        /* Grab new connection. */
         accept_err = netconn_accept(conn, &newconn);
    
        /* Process the new connection. */
        if (accept_err == ERR_OK) 
        {
							recv_err = netconn_recv(newconn, &buf);					
					
					while(recv_err == ERR_OK)
					{
						
						do 
            {
              netbuf_data(buf, &data, &len);
              netconn_write(newconn, data, len, NETCONN_COPY);
          
            } 
            while (netbuf_next(buf) >= 0);
          
            netbuf_delete(buf);
						recv_err = netconn_recv(newconn, &buf);

							
//							while ( recv_err == ERR_OK) 
//							{
//								do 
//								{
//									netbuf_data(buf, &data, &len);
//									netconn_write(newconn, data, len, NETCONN_COPY);
//									//netconn_write(newconn, tx_buf_t, tx_size, NETCONN_COPY);
//							
//								} 
//								while (netbuf_next(buf) >= 0);
//							
//								netbuf_delete(buf);
//								recv_err = netconn_recv(newconn, &buf);
//							}
						}
        
          /* Close connection and discard connection identifier. */
          netconn_close(newconn);
          netconn_delete(newconn);
        }
      }
    }
    else
    {
      netconn_delete(newconn);
      printf(" can not bind TCP netconn");
    }
  }
  else
  {
    printf("can not create TCP netconn");
  }
}
/*-----------------------------------------------------------------------------------*/

void tcpecho_init(void)
{
  sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCPECHO_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/
















#endif


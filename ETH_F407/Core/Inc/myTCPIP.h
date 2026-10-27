/*
 * myTCPIP.h
 *
 *  Created on: Oct 27, 2026
 *      Author: TAMRD
 */

#ifndef INC_MYTCPIP_H_
#define INC_MYTCPIP_H_

#include "tcp.h"

/*  protocol states */
enum tcp_server_states
{
  ES_NONE = 0,
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING
};

/* structure for maintaining connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_server_struct
{
  u8_t state;             /* current connection state */
  u8_t retries;
  struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
  struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
};

void tcp_server_init(void);

#endif /* INC_MYTCPIP_H_ */

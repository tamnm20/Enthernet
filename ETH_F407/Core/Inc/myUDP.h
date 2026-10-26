/*
 * myUDP.h
 *
 *  Created on: Oct 23, 2026
 *      Author: TAMRD
 */

#ifndef INC_MYUDP_H_
#define INC_MYUDP_H_
#include "udp.h"

void udp_receive_callback(void *arg, struct udp_pcb *upcb,
                           struct pbuf *p, const ip_addr_t *addr, u16_t port);
void udpServer_init(void);
void udpClient_connect(void);
void udp_receive_callback_from_server(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void udpClient_send(void);

#endif /* INC_MYUDP_H_ */

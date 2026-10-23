/*
 * myUDP.c
 *
 *  Created on: Oct 23, 2026
 *      Author: TAMRD
 */
#include "myUDP.h"

void udp_receive_callback(void *arg, struct udp_pcb *upcb,
                           struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    struct pbuf *txBuf;

    /* Build the reply string using the received payload */
    char buf[100];
    int len = sprintf(buf, "Hello %s From UDP SERVER\n", (char *)p->payload);

    /* Allocate a pbuf for the reply */
    txBuf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

    /* Copy the reply data into the pbuf */
    pbuf_take(txBuf, buf, len);

    /* Set the client as the destination and send */
    udp_connect(upcb, addr, port);
    udp_send(upcb, txBuf);

    /* Disconnect so the server is ready for the next client */
    udp_disconnect(upcb);

    /* Free both buffers — never skip this */
    pbuf_free(txBuf);
    pbuf_free(p);
}

void udpServer_init(void)
{
    struct udp_pcb *upcb;
    err_t err;

    /* 1. Create a new UDP control block */
    upcb = udp_new();

    /* 2. Bind the upcb to the STM32 static IP and port 7 */
    ip_addr_t myIPADDR;
    IP_ADDR4(&myIPADDR, 192, 168, 0, 4);

    err = udp_bind(upcb, &myIPADDR, 7);

    /* 3. Register the receive callback if bind succeeded */
    if(err == ERR_OK)
    {
        udp_recv(upcb, udp_receive_callback, NULL);
    }
    else
    {
        udp_remove(upcb);
    }
}

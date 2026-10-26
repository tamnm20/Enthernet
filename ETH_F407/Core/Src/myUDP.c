/*
 * myUDP.c
 *
 *  Created on: Oct 23, 2026
 *      Author: TAMRD
 */
#include "myUDP.h"
#include<string.h>
int counter =0;
char buffer[100];
struct udp_pcb *upcb;

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

void udp_receive_callback_from_server(void *arg, struct udp_pcb *upcb, struct pbuf *p,
                           const ip_addr_t *addr, u16_t port)
{
    /* Copy the data from the pbuf */
    strncpy(buffer, (char *)p->payload, p->len);

    /* increment message count */
    counter++;

    /* Free receive pbuf */
    pbuf_free(p);
}

void udpClient_connect(void)
{
    err_t err;

    /* 1. Create a new UDP control block  */
    upcb = udp_new();

    /* Bind the block to module's IP and port */
    ip_addr_t myIPaddr;
    IP_ADDR4(&myIPaddr, 192, 168, 0, 4);
    udp_bind(upcb, &myIPaddr, 8);

    /* configure destination IP address and port */
    ip_addr_t DestIPaddr;
    IP_ADDR4(&DestIPaddr, 192, 168, 0, 1);
    err = udp_connect(upcb, &DestIPaddr, 7);

    if (err == ERR_OK)
    {
        /* 2. Send message to server */
        udpClient_send();

        /* 3. Set a receive callback for the upcb */
        udp_recv(upcb, udp_receive_callback_from_server, NULL);
    }
}

void udpClient_send(void)
{
    struct pbuf *txBuf;
    char data[100];

    int len = sprintf(data, "sending UDP client message %d\n", counter);

    /* allocate pbuf from pool */
    txBuf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

    if (txBuf != NULL)
    {
        /* copy data to pbuf */
        pbuf_take(txBuf, data, len);

        /* send udp data */
        udp_send(upcb, txBuf);

        /* free pbuf */
        pbuf_free(txBuf);
    }
}

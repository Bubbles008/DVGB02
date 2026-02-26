#include "Sim_Engine.h"
#include <string.h>

#define END     "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

#ifndef A
#define A 0
#endif

#ifndef B
#define B 1
#endif

#define Wait_layer5 0
#define Wait_Ack 1

int A_seqnr, state;
struct pkt last_packet;

int checksum(struct pkt p)
{
  int sum = 0;

  sum += p.seqnum;
  sum += p.acknum;

  for(int i = 0; i < 20; i++)
  {
    sum += (unsigned char)p.payload[i];
  }
  return sum;
};


/* Called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message) {
  if(state == Wait_Ack)
  {
    printf(YELLOW "Still waiting for ACK.\n" END);
    return;
  } 
  struct pkt packet;

  packet.seqnum = A_seqnr;
  packet.acknum = 0;

  memcpy(packet.payload, message.data, 20);
  packet.checksum = checksum(packet);
  
  last_packet = packet;

  tolayer3(A, packet);

  starttimer(A, 15.0);
  state = Wait_Ack;
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
  if(packet.checksum != checksum(packet))
  {
    printf(RED "ACK is corrupted!\n" END);
    return;
  }
  
  if(packet.acknum == A_seqnr)
  {
    stoptimer(A);
    A_seqnr = 1 - A_seqnr;
    state = Wait_layer5;
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  printf(MAGENTA "Timeout." END CYAN" Packet is being resend.\n" END);
  tolayer3(A, last_packet);
  starttimer(A, 15.0);
}  

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  A_seqnr = 0;
  state = Wait_layer5;
}

#include "Sim_Engine.h"
#include <string.h> 

#ifndef A
#define A 0
#endif
#ifndef B
#define B 1
#endif


static int expected_seq;
static struct pkt last_ack_pkt; 

/*Checksum-functions*/


static int compute_checksum(struct pkt packet){
  int sum = 0;
  sum += packet.seqnum;
  sum += packet.acknum;

  for(int i = 0; i < 20; i++){
    sum += (unsigned char)packet.payload[i];
  }
  return sum;

}

static int is_corrupt(struct pkt packet){
  return packet.checksum != compute_checksum(packet);
}

static struct pkt make_ack(int acknum){
  struct pkt ack;
  memset(&ack, 0, sizeof(ack));

  ack.acknum = acknum;
  ack.checksum = compute_checksum(ack);
  return ack;
}


/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet) {

  if(is_corrupt(packet)){
    tolayer3(B, last_ack_pkt);
    return;
}
        //////////////////////////////////seqnum???
if(packet.seqnum == expected_seq) {
  tolayer5(B, packet.payload);

  last_ack_pkt = make_ack(expected_seq);
  tolayer3(B, last_ack_pkt);

  expected_seq = 1 - expected_seq;
  return;

}

tolayer3(B, last_ack_pkt);
}


/* Called when B's timer goes off */
void B_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init() {

  expected_seq = 0;

  last_ack_pkt = make_ack(1);

}

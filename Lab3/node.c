#include "node.h"

#define INF 999

void rtinit(struct distance_table *table, int node) {

	int mincost[4];
	for( int i = 0; i < 4; i++){
		mincost[i] = table->costs[node][i];
	}

	
	/*Create packet*/
	struct rtpkt pkt;
	pkt.sourceid = node; 

	/*Copy minimum cost to packet*/
	for(int i= 0; i < 4; i++){
		pkt.mincost[i] = mincost[i];
	}

	/*Send to neighbors*/
	for(int i = 0; i < 4; i++){
		if (is_neighbor(node, i) && i != node) {
			pkt.destid = i;
			tolayer2(pkt);
		}
	}

	printdt(table, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
	printdt(table, node);
}


#include "node.h"

#define INF 999

void rtinit(struct distance_table *table, int node) {
	for( int i = 0; i < 4; i++){
		for(int j = 0; j < 4 ; j++){
			table->costs[i][j] = INF; 
		}
	}

	if (node == 0){
		table->costs[0][0] = 0;
		table->costs[1][0] = 1;
		table->costs[2][0] = 3;
		table->costs[3][0] = 7;
	}
	else if(node == 1){
		table->costs[1][1] = 0;
		table->costs[0][1] = 1;
		table->costs[2][1] = 1;
	}
	else if(node == 2){
		table->costs[2][2] = 0;
		table->costs[0][2] = 3;
		table->costs[1][2] = 1;
		table->costs[3][2] = 2;
	}
	else if(node == 3){
		table->costs[3][3] = 0;
		table->costs[0][3] = 7;
		table->costs[2][3] = 2;
	}

	/*Create minimum cost array*/
	int mincost[4];

	for(int i = 0; i < 4;i++){
		mincost[i] = table->costs[i][node];
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
		if (table->costs[i][node] != INF && i != node) {
			pkt.destid = i;
			tolayer2(pkt);
		}
	}

	printdt(table, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
	printdt(table, node);
}


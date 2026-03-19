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

    int changeDetected = 0;
    int src = pkt->sourceid;


    int costToSrc = table->costs[src][node];


    for(int i = 0; i < 4; i++)  // i = destination
    {
        int newcost = costToSrc + pkt->mincost[i];

        if(newcost < table->costs[i][src])
        {
            table->costs[i][src] = newcost;
            changeDetected = 1;
        }
    }

    if(changeDetected)
    {
        struct rtpkt outpkt;
        outpkt.sourceid = node;

        int best[4];

        for(int d = 0; d < 4; d++)
        {
            int min = table->costs[d][0];

            for(int v = 1; v < 4; v++)
            {
                if(table->costs[d][v] < min)
                    min = table->costs[d][v];
            }

            best[d] = min;
            outpkt.mincost[d] = min;
        }

        for(int n = 0; n < 4; n++)
        {
            if(is_neighbor(node, n))
            {
                outpkt.destid = n;
                tolayer2(outpkt);
            }
        }
    }

    printdt(table, node);
}



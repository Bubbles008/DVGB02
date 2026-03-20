#include "node.h"
#include "sim_engine.h"
#include <stdio.h>

#define INF 999


static int link_cost[4][4];

void rtinit(struct distance_table *table, int node) {
    for (int i = 0; i < 4; i++)
        link_cost[node][i] = table->costs[node][i];

    for (int neighbor = 0; neighbor < 4; neighbor++) {
        if (neighbor == node) continue;
        if (!is_neighbor(node, neighbor)) continue;
        struct rtpkt pkt;
        creatertpkt(&pkt, node, neighbor, table->costs[node]);
        tolayer2(pkt);
    }

    printdt(table, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
    int sender = pkt->sourceid;
    int improved = 0;

    for (int dest = 0; dest < 4; dest++)
        table->costs[sender][dest] = pkt->mincost[dest];

    for (int dest = 0; dest < 4; dest++) {
        int best = INF;
        for (int neighbor = 0; neighbor < 4; neighbor++) {
            if (!is_neighbor(node, neighbor)) continue;
            int cost = link_cost[node][neighbor] + table->costs[neighbor][dest];
            if (cost > INF) cost = INF;
            if (cost < best) best = cost;
        }
        if (best < table->costs[node][dest]) {
            table->costs[node][dest] = best;
            improved = 1;
        }
    }

    if (improved) {
        for (int neighbor = 0; neighbor < 4; neighbor++) {
            if (neighbor == node) continue;
            if (!is_neighbor(node, neighbor)) continue;
            struct rtpkt out_pkt;
            creatertpkt(&out_pkt, node, neighbor, table->costs[node]);
            tolayer2(out_pkt);
        }
    }

    printdt(table, node);
}

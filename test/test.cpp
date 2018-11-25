#include "../src/KGGGP.h"
#include <cstdio>

KGGGP Partitioner;

int main(){
	freopen("test_input.in","r",stdin);
	
	int n, m; // # of Vertex ,# of Edge
	scanf("%d%d", &n, &m);
	Partitioner.init(n,4); // 4-ways Partition
	while(m--){
		int u,v;
		scanf("%d%d", &u, &v);
		Partitioner.add_edge(u-1, v-1);
	}
	Partitioner.solve();
	for(int u=0; u<n; ++u) printf("%d ", Partitioner.partition[u]);
	puts("");
	Partitioner.clear(); //remove all data in Partitioner
	return 0;
}
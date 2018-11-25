// https://hal.inria.fr/hal-01277392/document
// Implement without Initial Fixed Vertices

#include<utility>
#include<vector>

using std::vector;
using std::pair;

class KGGGP{
	long long N, P; // base-0
	vector<vector<int>> G;
	
	int maxVertex;
	
	vector<int> partition_size;
	
	enum setName{
		HREG, HNBC, HNCC, setNameSize
	};
	
	struct node{
		int data;
		int p, u;
		setName type;
		node *l,*r;
		node(int d=0,int p=0,int u=0,setName t=HREG)
			:data(d),p(p),u(u),type(t),l(nullptr),r(nullptr){}
	}*Set[setNameSize], *mem;
	int mem_cnt;
	
	vector<vector<node**>> PU;
	
	node *new_node(int d,int p,int u,setName type){
		return &(mem[mem_cnt++]=node(d,p,u,type));
	}
	
	void set_node_point(node *&o){
		if(o) PU[o->p][o->u] = &o;
	}
	
	node *merge(node *a,node *b){
		if(!a||!b)return a?a:b;
		if(a->data<b->data) return merge(b,a);
		node *t = a->r;
		a->r = a->l;
		set_node_point(a->r);
		a->l = merge(b,t);
		set_node_point(a->l);
		return a;
	}
	
	void push(setName st,node *nd){
		Set[st]=merge(Set[st],nd);
		set_node_point(Set[st]);
	}
	
	void eraseVoid(node *&o){
		o = merge(o->l,o->r);
		set_node_point(o);
	}
	
	node* erase(node *&o){
		node *res = o;
		eraseVoid(o);
		res->l = nullptr;
		res->r = nullptr;
		return res;
	}
	
	node* pop(setName st){
		return erase(Set[st]);
	}
	
public:
	vector<int> partition;
	~KGGGP(){
		delete[] mem;
	}
	void init(int _n, int _p, double ratio = 1.1){
		clear();
		N = _n;
		P = _p;
		maxVertex = (N/P+(N%P!=0))*ratio;
		G.resize(N);
		mem = new node[N*P];
	}
	void clear(){
		G.clear();
		partition.clear();
		partition_size.clear();
		mem_cnt = 0;
		delete[] mem;
		mem = nullptr;
	}
	void add_edge(int u,int v){
		if(u==v) return;
		G[u].emplace_back(v);
		G[v].emplace_back(u);
	}
	
	void solve(){
		partition.resize(N,-1);
		PU.resize(P,vector<node**>(N));
		partition_size.resize(P,0);
		
		Set[HREG] = nullptr;
		for(int p=0; p<P; ++p){
			for(int u=0; u<N; ++u){
				push(HREG,new_node(-int(G[u].size()),p,u,HREG));
			}
		}
		Set[HNBC] = nullptr;
		Set[HNCC] = nullptr;
		while(Set[HREG]||Set[HNBC]||Set[HNCC]){
			int u, p;
			for(;;){
				if(Set[HREG]){
					node *now = pop(HREG);
					u = now->u;
					p = now->p;
					if(partition_size[p]==0) break;
					if(partition_size[p]>=maxVertex){
						now->type = HNBC;
						push(HNBC,now);
					}else{
						bool OK = false;
						for(int v:G[u]){
							if(partition[v]==p){
								OK = true;
								break;
							}
						}
						if(OK) break;
						now->type = HNCC;
						push(HNCC,now);
					}
				}else if(Set[HNCC]){
					node *now = pop(HNCC);
					u = now->u;
					p = now->p;
					if(partition_size[p]>=maxVertex){
						now->type = HNBC;
						push(HNBC,now);
					}else break;
				}else if(Set[HNBC]){
					node *now = pop(HNBC);
					u = now->u;
					p = now->p;
					break;
				}
			}
			
			partition[u] = p;
			++partition_size[p];
			PU[p][u] = nullptr;
			for(int i=0; i<P; ++i){
				if(p==i) continue;
				eraseVoid(*PU[i][u]);
				PU[i][u] = nullptr;
			}
			for(int v:G[u]){
				if(partition[v]!=-1) continue;
				for(int p2=0; p2<P; ++p2){
					node *tmp = erase(*PU[p2][v]);
					PU[p2][v] = nullptr;
					
					if(p==p2) tmp->data += 2;
					else tmp->data -= 2;
					
					if(tmp->type==HNCC&&p==p2){
						tmp->type = HREG;
						push(HREG,tmp);
					}else{
						push(tmp->type,tmp);
					}
				}
			}
		}
	}
};
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<vector>
using namespace std;
const int N = 160;
const int INF = INT_MAX>>1;
const int city1 = 95050;
const int city2 = 104729;
struct Dinic{
    int n,s,t;
    struct Edge{
        int v,capacity,inv;
        Edge(int _v,int _capacity, int _inv):v(_v),capacity(_capacity),inv(_inv){}
    };
    vector<vector<Edge> > edges;
    vector<int> q;
    vector<int> work;
    vector<int> dist;
    Dinic(int n,int s,int t){
        edges.resize(n);
        q.resize(n);
        work.resize(n);
        dist.resize(n);
        this->n = n, this->s = s, this->t = t;
        
    }
    bool bfs(){
        fill(dist.begin(),dist.end(),-1);
        dist[s] = 0;
        int tail = 0;
        q[tail++] = s;
        int head = 0;
        while(head<tail){
            int u = q[head];
            int v = 0;
            for(int i=0;i<edges[u].size();i++){
                v = edges[u][i].v;
                if(edges[u][i].capacity>0 && dist[v]==-1)
                    dist[v] = dist[u]+1, q[tail++] = v;
            }
            head++;
        }
        return dist[t]!=-1;
    }
    int dfs(int u,int lim){
        if(u==t) return lim;
        int f = 0;
        for(int &i=work[u];i<edges[u].size();i++){
            int v = edges[u][i].v;
            if(edges[u][i].capacity<=0 || dist[v]!=dist[u]+1)
                continue;
            int nf=dfs(v,min(lim,edges[u][i].capacity));
            if(nf>0)
            {
                edges[u][i].capacity -= nf;
                edges[v][edges[u][i].inv].capacity += nf;
                f += nf;
                lim -= nf;
                if(lim==0) break;
            }
        }
        return f;
    }
    void addEdge(int u,int v,int capacity){
        edges[u].push_back(Edge(v,capacity,edges[v].size()));
        edges[v].push_back(Edge(u,capacity,edges[u].size()-1));
    }
    int maxflow(){
        int result = 0;
        while(bfs()){
            fill(work.begin(),work.end(),0);
            result += dfs(s,INF);
        }
        return result;
    }
    
};


int n,m,e;
int g[N][N];
int dist[N],rdist[N];
inline void dijkstra(int s,int* d){
    int vis[N];
    for(int i=0;i<n;i++) d[i] = INF,vis[i] = 0;
    d[s] = 0;
    for(int k=0;k<n;k++){
        int best = INF, id = -1;
        for(int i=0;i<n;i++)
            if(!vis[i] && d[i]<=best) best = d[i],id = i;
        vis[id] = 1;
        for(int i=0;i<n;i++)
            if(!vis[i] && d[id]+g[id][i] < d[i]) d[i] = d[id]+g[id][i];
    }
}
inline bool check(int T){
    //maxflow <= m
    Dinic dinic(n,n-2,n-1);
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++){
            if(g[i][j]==INF) continue;
            int x = min(dist[i],dist[j]);
            int y = min(rdist[i],rdist[j]);
            if(x>T || y>T){
                dinic.addEdge(i,j,1000);
                continue;
            }
            if(dist[i]<=T && rdist[i]<=T || dist[j]<=T && rdist[j]<=T){
                dinic.addEdge(i,j,1);
                continue;
            }
            if(x+y+g[i][j]>2*T) dinic.addEdge(i,j,1000);
            else dinic.addEdge(i,j,1);
        }
    int flow = dinic.maxflow();
    //cout<<"flow "<<flow<<endl;
    return flow<=m;
}
int main()
{
    while(scanf("%d",&n),n){
        int u,v,w;
        scanf("%d%d",&m,&e);
        n+=2;//n-2 city 1, n-1 city 2
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++) g[i][j] = INF;
        for(int i=0;i<e;i++){
            scanf("%d%d%d",&u,&v,&w);
            if(u==city1) u = n-2;
            if(u==city2) u = n-1;
            if(v==city1) v = n-2;
            if(v==city2) v = n-1;
            g[u][v] = g[v][u] = 2*w;
        }
        dijkstra(n-2,dist);
        dijkstra(n-1,rdist);
        if(dist[n-1]==INF){
            printf("Impossible\n"); // cannot reach city
            continue;
        }
        //run max_flow if > m, impossible
        Dinic dinic(n,n-2,n-1);
        for(int i=0;i<n;i++)
            for(int j=i+1;j<n;j++)
                if(g[i][j]!=INF) dinic.addEdge(i,j,1);
        if(dinic.maxflow() > m){
            printf("Impossible\n");
            continue;
        }
        
        //binary search on T (mobilizing time)
        int l = 0, r = dist[n-1];
        int mid = 0;
        while(l+1<r){
            mid = (l+r)/2;
            if(check(mid)) r = mid;
            else l = mid;
        }
        printf("%.1f\n",1.0*r/2.0);
    }
    return 0;
}

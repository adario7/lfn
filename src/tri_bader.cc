#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

/*
 * see: "Fast Triangle Counting" (Bader, 2023)
*/

using namespace std;

vector<int> BFS_v_levels(const vector<unordered_set<int>>& graph, int StartNode = 0)
{
    vector<int> levels(graph.size(), -1);
    levels[StartNode] = 0;
    queue<int> q;
    q.push(StartNode);

    while(!q.empty())
    {
        int node = q.front();
        q.pop();

        for(int neighbor : graph[node])
        {
            if(levels[neighbor] == -1)
            {
                levels[neighbor] = levels[node] + 1;
                q.push(neighbor);
            }
        }
    }

    return levels; 
}

int TC_forward_hashed(int n, const vector<unordered_set<int>>& graph)
{
    int T = 0;
    vector<unordered_set<int>> A(graph.size());
	vector<bool> hash(n);

    for(int u = 0; u < graph.size(); u++)
    {
        for(int v : graph[u])
        {
            if(u < v)
            {
                for(int w:A[u])
                    hash[w] = true;

                for(int w:A[v])
                    if(hash[w])
                        T++;

                for(int w:A[u])
                    hash[w] = false;

                A[v].insert(u);
            }
        }
    }

    return T;
}

int main()
{
    ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

    int n, m;
    cin >> n >> m;
    
    vector<unordered_set<int>> graph(n), G0(n), G1(n);

    for(int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        if(u == v)
            continue;

        //graph[u].insert(v);
        //graph[v].insert(u);
		int a = min(u, v), b = max(u, v);
		graph[a].insert(b);
    }

    vector<int> levels = BFS_v_levels(graph, 0);
    
    for(int u = 0; u < graph.size(); u++)
    {
        for(int v : graph[u])
        {
            if(levels[u] == levels[v])
            {
                G0[u].insert(v);
                G0[v].insert(u);
            }else
            {
                G1[u].insert(v);
                G1[v].insert(u);
            }
        }
    }

    int T = TC_forward_hashed(n, G0);

    vector<bool> hash(n);
    for(int u=0; u < G1.size(); u++)
    {
        for(int v:G1[u])
            hash[v] = true;

        for(int v:G0[u])
            if(u < v)
                for(int w:G1[v])
                    if(hash[w])
                        T++;

        for(int v:G1[u])
            hash[v] = false;
    }
    cout<<T<<endl;
}

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>

using namespace std;

vector<int> BFS_v_levels(const vector<set<int>>& graph, int StartNode = 0)
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

int TC_forward_hashed(const vector<set<int>>& graph)
{
    int T = 0;
    vector<unordered_set<int>> A(graph.size());
    unordered_map<int, bool> hash;

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
    //int n=25; 
    vector<set<int>> graph(n), G0(n), G1(n);

    for(int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        if(u == v)
            continue;

        graph[u].insert(v);
        graph[v].insert(u);
    }
   /*for (int i = 0; i + 2 < n; i += 3) {
        // Collega i nodi i, i+1, i+2
        graph[i].insert(i + 1);
        graph[i].insert(i + 2);

        graph[i + 1].insert(i);
        graph[i + 1].insert(i + 2);

        graph[i + 2].insert(i);
        graph[i + 2].insert(i + 1);
    }

    // Gestione dei nodi rimanenti
    int remaining = n % 3;
    if (remaining == 1) {
        // Collega l'ultimo nodo al penultimo
        graph[n - 1].insert(n - 2);
        graph[n - 2].insert(n - 1);
    } else if (remaining == 2) {
        // Collega gli ultimi due nodi al terzultimo
        graph[n - 3].insert(n - 2);
        graph[n - 3].insert(n - 1);

        graph[n - 2].insert(n - 3);
        graph[n - 2].insert(n - 1);

        graph[n - 1].insert(n - 3);
        graph[n - 1].insert(n - 2);
    }*/

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

    int T = TC_forward_hashed(G0);

    unordered_map<int, bool> hash;
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
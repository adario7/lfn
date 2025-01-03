#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <unordered_set>
#include <random>


using namespace std;

vector<int> random_color(int n, int k)
{
    srand(time(0));
    mt19937 gen(time(0));
    uniform_int_distribution<> dist(0, k - 1); 
    vector<int> colors(n);
    /*for(int i = 0; i < n; i++)
         colors[i] = dist(gen);*/
    colors = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};
    return colors;
}


int count_colorful(const vector<unordered_set<int>> &graph, const vector<int> &colors, int node, int bitmask, int k, int depth, vector<vector<int>> &dp, int prev)
{
    //cout << "Node: " << node << ", Bitmask: " << bitmask << ", Color: " << (1 << colors[node]) <<  ", Depth " << depth << endl;
    //passi base
    if(dp[node][bitmask] != -1)
        return dp[node][bitmask];
    
    if (!(bitmask & (1 << colors[node])))
    {
        dp[node][bitmask] = 0;
        return dp[node][bitmask];
    }
    
    if(depth == k)
    {
        dp[node][bitmask] = (bitmask == (1 << colors[node])) ? 1 : 0;
        return dp[node][bitmask];
    }
    //passo ricorsivo

    int result = 0;
    for(int neighbor : graph[node])
    {
        if (neighbor == prev)
            continue;
        result += count_colorful(graph, colors, neighbor, bitmask & ~(1 << colors[node]), k, depth + 1, dp, node);
        //cout << "Result: " << result << endl;
    }
    dp[node][bitmask] = result;
    return result;
}


int main()
{
    ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

    int n, m;
    //cin >> n >> m;

    n=10;
    
    vector<unordered_set<int>> graph(n);
    graph[0].insert(1); graph[1].insert(0); // Arco tra 0 e 1
    graph[0].insert(2); graph[2].insert(0); // Arco tra 0 e 2
    graph[0].insert(3); graph[3].insert(0); // Arco tra 0 e 3
    graph[1].insert(4); graph[4].insert(1); // Arco tra 1 e 4
    graph[1].insert(5); graph[5].insert(1); // Arco tra 1 e 5
    graph[2].insert(6); graph[6].insert(2); // Arco tra 2 e 6
    graph[2].insert(7); graph[7].insert(2); // Arco tra 2 e 7
    graph[3].insert(8); graph[8].insert(3); // Arco tra 3 e 8
    graph[4].insert(9); graph[9].insert(4); // Arco tra 4 e 9
    graph[5].insert(8); graph[8].insert(5); // Arco tra 5 e 8
    graph[6].insert(9); graph[9].insert(6); // Arco tra 6 e 9
    graph[7].insert(0); graph[0].insert(7); // Arco tra 7 e 0
    graph[8].insert(2); graph[2].insert(8); // Arco tra 8 e 2
    graph[9].insert(7); graph[7].insert(9); // Arco tra 9 e 7

    /*for(int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        if(u == v)
            continue;
        graph[u].insert(v);
        graph[v].insert(u);
    }*/



    vector<int> colors;
    int count = 0;
    int t = 100;
    int k = 3;


    for(int i = 0; i<t; i++)
    {
        colors = random_color(n,k);
        vector<vector<int>> dp(n, vector<int>(1 << k, -1));
        for(int j=0; j<n; j++)
            count = count + count_colorful(graph, colors, j, 7, k-1, 0, dp,-1);
        //cout << count << endl;
    }

    double graphlets = (1.0 / t) * static_cast<double>(count) * (pow(k, k) / tgamma(k + 1)); 
    cout << graphlets << endl;

} 
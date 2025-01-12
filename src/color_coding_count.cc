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
    for(int i = 0; i < n; i++)
         colors[i] = dist(gen);
    return colors;
}


int count_colorful(const vector<unordered_set<int>> &graph, const vector<int> &colors, int node, int bitmask, int k, int depth, vector<vector<int>> &dp, int prev)
{
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


    int result = 0;
    for(int neighbor : graph[node])
    {
        if (neighbor == prev)
            continue;
        result += count_colorful(graph, colors, neighbor, bitmask & ~(1 << colors[node]), k, depth + 1, dp, node);
    }
    dp[node][bitmask] = result;
    return result;
}


int main()
{
    ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

    int n, m;
    cin >> n >> m;
    
    vector<unordered_set<int>> graph(n);

    for(int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        if(u == v)
            continue;
        graph[u].insert(v);
        graph[v].insert(u);
    }

    vector<int> colors;
    int count = 0;
    int t = 10;
    int k = 3;


    for(int i = 0; i<t; i++)
    {
        colors = random_color(n,k);
        vector<vector<int>> dp(n, vector<int>(1 << k, -1));
        for(int j=0; j<n; j++)
            count = count + count_colorful(graph, colors, j, 7, k-1, 0, dp,-1);
    }

    double graphlets = (1.0 / t) * static_cast<double>(count) * (pow(k, k) / tgamma(k + 1)); 
    cout << graphlets << endl;
} 
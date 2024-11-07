#include <bits/stdc++.h>
using namespace std;
#define ll long long 

/*
    The algorithm searches for a txt file "paths" which contains the paths to each dataset, one by line.
    Nodes are 0 based.
    It assumes that the graph is simple and undirected, multiple edges are automatically discarded.
*/

vector<string> paths;
set<pair<int,int>> edge_set;

ll optimized(vector<vector<int>> &adj_list){

    ll result = 0;

    vector<pair<int,int>> degree_node;

    int n = adj_list.size();

    for(int u = 0; u < n; u++){
        degree_node.push_back({adj_list[u].size(), u});
    }

    sort(degree_node.begin(), degree_node.end());

    vector<int> el(n, 0);

    for(int i = 0; i < n; i++){
        int u = degree_node[i].second;

        vector<int> remained;
        for(auto v: adj_list[u])
            if(!el[v])
                remained.push_back(v);

        int siz = remained.size();

        for(int i = 0; i < siz; i++){
            int v = remained[i];
            for(int j = i+1; j < siz; j++){
                
                int w = remained[j];
                
                result += edge_set.count({v, w});


            }
        }

        el[u] = 1;
    }


    return result;
}

int main(){

    ifstream in("paths.txt");

    string path;
    while(getline(in, path)){
        paths.push_back(path);
    }

    in.close();

    clock_t start;
    double seconds_passed;

    cout << fixed << setprecision(3);

    for(int i_data = 0; i_data < (int)paths.size(); i_data++){
        
        cout << "Dataset " << i_data << ":\n";
        cout << "Path of the dataset: " + paths[i_data] + "\n"; 

        start = clock();

        ifstream in(paths[i_data]);

        //input format: 
        //first line: number of nodes and number of edges
        //following lines: one line per edge with the 0 based indexes of the nodes connected by that edge

        int n, m;

        in >> n >> m;

        cout << n << " " << m << endl;

        vector<vector<int>> adj_list(n, vector<int>());

        for(int i = 0; i < m; i++){
            int u, v;
            in >> u >> v;

            if((u < 0) || (u >= n) || (v < 0) || (v >= n)){
                cerr << "Error on input line " << i + 1 << endl << "The readed nodes are " << u << " " << v << endl; 
                return 0;
            }

            if(u == v || edge_set.count({u, v})){
                continue;
            }

            adj_list[u].push_back(v);
            adj_list[v].push_back(u);

            edge_set.insert({u,v});
            edge_set.insert({v,u});
        }

        for(int u = 0; u < n; u++){
            sort(adj_list[u].begin(), adj_list[u].end());
        }

        seconds_passed = (clock() - start) / (double) CLOCKS_PER_SEC;
        cout << "The input was taken in " << seconds_passed << " seconds.\n";

        start = clock();

        int n_triangles_opt = optimized(adj_list);

        seconds_passed = (clock() - start) / (double) CLOCKS_PER_SEC;
        cout << "The optimized algorithm took " << seconds_passed << " seconds.\n";
        cout << "The number of triangles found is " << n_triangles_opt << ".\n";

        adj_list.clear();
        edge_set.clear();
    }

}
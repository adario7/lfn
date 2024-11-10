#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/* idea: for each node,
 * look at each pair of neighbors and see if the triangle is closed
 * start from low degree nodes and go up
 * after we check a node, we mark it as eliminated
 * we don't need to check eliminated neighbors when we check following nodes
*/

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

	int n, m;
	cin >> n >> m;

	vector<set<int>> adj_list(n);
	for(int i = 0; i < m; i++){
		int u, v;
		cin >> u >> v;

		if((u < 0) || (u >= n) || (v < 0) || (v >= n)){
			cerr << "Error on input line " << i + 1 << endl << "The readed nodes are " << u << " " << v << endl; 
			return 0;
		}

		if(u == v){
			continue;
		}

		adj_list[u].insert(v);
		adj_list[v].insert(u);
	}

    ll result = 0;
    vector<int> degree_node(n);
    for(int u = 0; u < n; u++) degree_node[u] = u;
    sort(degree_node.begin(), degree_node.end(), [&](int a, int b) {
			return adj_list[a].size() < adj_list[b].size();
		});
    vector<int> elim(n, 0);

    for(int i = 0; i < n; i++){
        int u = degree_node[i];
		const auto& neigh = adj_list[u];
		for (auto j = neigh.begin(); j != neigh.end(); j++) {
            int v = *j;
			if (elim[v]) continue;
			auto k = j;
			for (++k; k != neigh.end(); k++) {
                int w = *k;
				if (elim[w]) continue;
                result += adj_list[v].count(w);
            }
        }
        elim[u] = 1;
    }

	cout << result << endl;
}

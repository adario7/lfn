#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

/* idea: for each edge,
 * compare the two adjacency lists and look for intersections using binary search
 */

int main(int argc, char** argv) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

	int N, M;
	cin >> N >> M;

	vector<unordered_set<int>> neighbours(N);
	vector<int> degree(N), indegree(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		if (x == y) continue;
		degree[x]++, degree[y]++;
		int a = min(x, y), b = max(x, y);
		neighbours[a].insert(b);
		indegree[b]++;
	}

	vector<long long> G(9);
	G[0] = M;

	for (int u = 0; u < N; u++) {
		int du = degree[u];
		// G4: u is the central vertex, look for all possible triplets: Bin(du, 3)
		if (du >= 3) {
			G[4] += du * (du - 1) * (du - 2) / 6;
		}
	}
	for (int u = 0; u < N; u++) for (int v : neighbours[u]) {
		int idu = indegree[u], odv = neighbours[v].size();

		// G1: (u, v) is the first edge, look for a second one
		G[1] += odv;

		// G3: (u, v) is the central edge, count possible extensions before and after this one
		G[3] += idu * odv;

		// G5: look if any of the G3s is closed
		for (int w : neighbours[v]) for (int z : neighbours[w]) {
			if (u == z) continue;
			if (neighbours[u].count(z)) {
				G[5]++;
			}
		}

		// G2: look for triangles on this edge
		vector<int> tris;
		for (int w : neighbours[v]) if (neighbours[u].count(w)) {
			// here u, v, w is a triangle
			G[2]++;
			tris.push_back(w);
			// G6: look for all possible extensions of the triangle
			G[6] += (degree[u] - 2) + (degree[v] - 2) + (degree[w] - 2);
		}

		// G7: two triangles sharing an edge
		int n_tris = tris.size();
		if (n_tris >= 2) {
			G[7] += n_tris * (n_tris - 1) / 2;
		}

		// G8: look for G7s with connected endpoints
		sort(tris.begin(), tris.end());
		for (int i = 0; i < n_tris; i++) {
			for (int j = i + 1; j < n_tris; j++) {
				int w = tris[i], z = tris[j];
				if (neighbours[w].count(z))
					G[8]++;
			}
		}
	}

	for (int g = 0; g <= 8; g++) {
		cout << G[g] << endl;
	}
}

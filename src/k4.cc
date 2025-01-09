#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <chrono>

using namespace std;

using ll = long long;
using Clock = chrono::high_resolution_clock;

int N, M;
vector<unordered_set<int>> neighbours;
vector<int> degree, indegree;

int get(const map<int, int>& m, int x) {
	auto it = m.find(x);
	return it == m.end() ? 0 : it->second;
}

template<bool C1, bool C2, bool C3, bool C4, bool C5, bool C6, bool C7, bool C8>
vector<ll> count() {
	vector<ll> G(9);
	G[0] = M;

	for (int u = 0; u < N; u++) {
		int du = degree[u];
		// G1: u is the central vertex, look for pairs
		if (C1) if (du >= 2) {
			G[1] += du * (du - 1) / 2;
		}
		// G4: u is the central vertex, look for all possible triplets: Bin(du, 3)
		if (C4) if (du >= 3) {
			G[4] += du * (du - 1) * (du - 2) / 6;
		}
	}

	vector<map<int, int>> div_paths, dir_paths;
	if (C5) div_paths.resize(N), dir_paths.resize(N);
	for (int u = 0; u < N; u++) for (int v : neighbours[u]) {
		int idu = indegree[u], odv = neighbours[v].size();

		// G3: (u, v) is the central edge, count possible extensions before and after this one
		if (C3)
			G[3] += (degree[u] - 1) * (degree[v] - 1);

		if (C5) {
			for (int w : neighbours[v]) {
				// path u -> v -> w, u < v < w
				dir_paths[u][w]++;
			}
			for (int z : neighbours[u]) if (v < z) { 
				// path v <- u -> z, v < z, u < v, u < z
				div_paths[v][z]++;
			}
		}

		// G2: look for triangles on this edge
		int n_tris = 0;
		vector<int> tris;
		if (C2 || C6 || C7 || C8) {
			for (int w : neighbours[v]) if (neighbours[u].count(w)) {
				// here u, v, w is a triangle
				if (C2)
					G[2]++;
				if (C8)
					tris.push_back(w);
				n_tris++;
				// G6: look for all possible extensions of the triangle
				if (C6)
					G[6] += (degree[u] - 2) + (degree[v] - 2) + (degree[w] - 2);
			}
		}

		// G7: two triangles sharing an edge
		if (C7) if (n_tris >= 2) {
			G[7] += n_tris * (n_tris - 1) / 2;
		}

		// G8: look for G7s with connected endpoints
		if (C8) {
			sort(tris.begin(), tris.end());
			for (int i = 0; i < n_tris; i++) {
				for (int j = i + 1; j < n_tris; j++) {
					int w = tris[i], z = tris[j];
					if (neighbours[w].count(z))
						G[8]++;
				}
			}
		}
	}

	if (C5) {
		for (int u = 0; u < N; u++) {
			// type 1 square: one pair has same direction, other opposite
			for (auto [z, c] : div_paths[u]) {
				int c_dir = get(dir_paths[u], z);
				//if (c_dir) cerr << "G5_1: u=" << u << " z=" << z << " c=" << (c*c_dir) << endl;
				G[5] += c * c_dir;
			}
			// type 2 square: both pairs have same direction
			for (auto [z, c] : dir_paths[u]) {
				//if (c >= 2) cerr << "G5_2: u=" << u << " z=" << z << " c=" << (c*(c-1)/2) << endl;
				G[5] += c * (c - 1) / 2;
			}
			// type 3 square: both pairs have opposite directions
			for (auto [z, c] : div_paths[u]) {
				//if (c >= 2) cerr << "G5_3: u=" << u << " z=" << z << " c=" << (c*(c-1)/2) << endl;
				G[5] += c * (c - 1) / 2;
			}
		}
	}

	return G;
}

template<bool C1, bool C2, bool C3, bool C4, bool C5, bool C6, bool C7, bool C8>
int time() {
	auto start = Clock::now();
	count<C1, C2, C3, C4, C5, C6, C7, C8>();
	auto end = Clock::now();
	return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

template<size_t g>
constexpr void static_for(int baseline) {
    if constexpr (g <= 8) {
		int add = time<
            g == 1, g == 2, g == 3, g == 4, g == 5, g == 6, g == 7, g == 8
        >();
		int diff = baseline - time<
            g != 1, g != 2, g != 3, g != 4, g != 5, g != 6, g != 7, g != 8
		>();
		cout << "G" << g << " -> \t" << add << "ms / \t" << diff << "ms" << endl;
        static_for<g + 1>(baseline);
    }
}

int main(int argc, char** argv) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

	cin >> N >> M;
	neighbours.resize(N);
	degree.resize(N);
	indegree.resize(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		if (x == y) continue;
		degree[x]++, degree[y]++;
		int a = min(x, y), b = max(x, y);
		neighbours[a].insert(b);
		indegree[b]++;
	}
	
	if (argc == 1) {
		vector<long long> G = count<1, 1, 1, 1, 1, 1, 1, 1>();
		for (int g = 0; g <= 8; g++) {
			cout << G[g] << endl;
		}
	} else {
		int baseline = time<1, 1, 1, 1, 1, 1, 1, 1>();
		cout << "all -> \t" << baseline << "ms" << endl;
		static_for<1>(baseline);
	}
	
	return 0;
}

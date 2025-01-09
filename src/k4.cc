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
vector<unordered_set<int>> neighbours, inv_neighbours;
vector<int> degree, indegree;

int get(const map<int, int>& m, int x) {
	auto it = m.find(x);
	return it == m.end() ? 0 : it->second;
}

int set_intersection(const unordered_set<int>& a, const unordered_set<int>& b, int u, int v, int w) {
	const auto& small = a.size() < b.size() ? a : b;
	const auto& large = a.size() < b.size() ? b : a;
	int count = 0;
	for (int x : small) {
		if (x == u || x == v || x == w) continue;
		if (large.count(x)) count++;
	}
	return count;
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

	for (int u = 0; u < N; u++) for (int v : neighbours[u]) {
		int idu = indegree[u], odv = neighbours[v].size();

		// G3: (u, v) is the central edge, count possible extensions before and after this one
		if (C3)
			G[3] += (degree[u] - 1) * (degree[v] - 1);

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
			for (int v : neighbours[u]) {
				for (int w : neighbours[u]) if (v < w) {
					// type 1 square: one pair has same direction, other opposite (not used)
					int c1 = set_intersection(inv_neighbours[v], neighbours[w], u, v, w);
					//if (c1) cerr << "G5_1: u=" << u << " v=" << v << " w=" << w << " c=" << c1 << endl;
					G[5] += c1;
					// type 2 square: both pairs have same direction
					int c2 = set_intersection(neighbours[v], neighbours[w], u, v, w);
					G[5] += c2;
					//if (c2) cerr << "G5_2: u=" << u << " v=" << v << " w=" << w << " c=" << c2 << endl;
					// type 3 square: both pairs have opposite directions
					int c3 = set_intersection(inv_neighbours[v], inv_neighbours[w], u, v, w);
					G[5] += c3;
					//if (c3) cerr << "G5_3: u=" << u << " v=" << v << " w=" << w << " c=" << c3 << endl;
				}
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
	inv_neighbours.resize(N);
	degree.resize(N);
	indegree.resize(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		assert(0 <= x && x < N);
		assert(0 <= y && y < N);
		if (x == y) continue;
		degree[x]++, degree[y]++;
		int a = min(x, y), b = max(x, y);
		neighbours[a].insert(b);
		inv_neighbours[b].insert(a);
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

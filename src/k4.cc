#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <numeric>
#include <unordered_set>
#include <utility>
#include <vector>
#include <chrono>

using namespace std;

using ll = long long;
using ii = pair<int, int>;
using Clock = chrono::high_resolution_clock;

int N, M;
vector<unordered_set<int>> neighbours, inv_neighbours;
vector<int> degree;

int bin2(int n) { return n * (n - 1) / 2; }

int get(const map<int, int>& m, int x) {
	auto it = m.find(x);
	return it == m.end() ? 0 : it->second;
}

#define set_intersection(_a, _b, condition, d11, d12, d21, d22) do { \
	const auto& _small = _a.size() < _b.size() ? _a : _b; \
	const auto& _large = _a.size() < _b.size() ? _b : _a; \
	for (int z : _small) { \
		if (!(condition)) continue; \
		if (z == u || z == v || z == w) continue; \
		if (_large.count(z)) { \
			if (C5) G[5]++; \
			/* G7: look for sqaures with a diagonal */ \
			if (C7) if (neighbours[d11].count(d12)) G[7]++; \
			if (C7) if (neighbours[d21].count(d22)) G[7]++; \
		} \
	} \
} while (0)

template<bool C1, bool C2, bool C3, bool C4, bool C5, bool C6, bool C7, bool C8>
vector<ll> count() {
	vector<ll> G(9);
	G[0] = M;

	for (int u = 0; u < N; u++) {
		int du = degree[u];
		// G1: u is the central vertex, look for pairs
		if (C1) if (du >= 2) {
			G[1] += bin2(du);
		}
		// G4: u is the central vertex, look for all possible triplets: Bin(du, 3)
		if (C4) if (du >= 3) {
			G[4] += ll(du) * (du - 1) * (du - 2) / 6;
		}
	}

	for (int u = 0; u < N; u++) for (int v : neighbours[u]) {
		// G3: (u, v) is the central edge, count possible extensions before and after this one
		// this will also count triangles as G3s, corrected when converting the count to induced
		if (C3)
			G[3] += (degree[u] - 1) * (degree[v] - 1);

		// G2: look for triangles on this edge
		int n_tris = 0;
		vector<int> tris;
		if (C2 || C6 || C8) {
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

	if (C5 || C7) {
		// G5: look for the intersection of two orented paths of length 2
		for (int u = 0; u < N; u++) {
			int n_div = 2*bin2(neighbours[u].size());
			int n_dir = 0, n_inv = 0;
			for (int v : neighbours[u]) {
				n_dir += neighbours[v].size();
				n_inv += inv_neighbours[v].size();
			}
			int n_min  = min(n_div, min(n_dir, n_inv));

			// type 1 square: one pair has same direction, other opposite
			if (n_div == n_min) {
				for (int v : neighbours[u]) {
					for (int w : neighbours[u]) if (v < w) {
						set_intersection(neighbours[v], inv_neighbours[w], 1, u, z, v, w);
					}}
			} else if (n_dir == n_min) {
				for (int v : neighbours[u]) {
					for (int w : neighbours[v]) {
						set_intersection(neighbours[u], neighbours[w], 1, u, w, v, z);
					}}
			} else { // n_inv == n_min
				for (int v : neighbours[u]) {
					for (int w : inv_neighbours[v]) if (u < w) {
						set_intersection(neighbours[u], inv_neighbours[w], 1, u, w, z, v);
					}}
			}

			// type 2 square: both pairs have same direction
			if (n_div < n_dir) {
				for (int v : neighbours[u]) {
					for (int w : neighbours[u]) if (v < w) {
						set_intersection(neighbours[v], neighbours[w], 1, u, z, v, w);
				}}
			} else {
				for (int v : neighbours[u]) {
					for (int w : neighbours[v]) {
						set_intersection(neighbours[u], inv_neighbours[w], v < z, u, w, v, z);
					}}
			}

			// type 3 square: both pairs have opposite directions
			if (n_div < n_inv) {
				for (int v : neighbours[u]) {
					for (int w : neighbours[u]) if (v < w) {
						set_intersection(inv_neighbours[v], inv_neighbours[w], u < z, u, z, v, w);
					}}
			} else {
				for (int v : neighbours[u]) {
					for (int w : inv_neighbours[v]) if (u < w) {
						set_intersection(neighbours[u], neighbours[w], v < z, u, w, v, z);
					}}
			}
		}
	}

	return G;
}

vector<ll> count_induced(const vector<ll> &NI) {
	vector<ll> I(NI.size());
	// k2
	I[0] = NI[0];
	// k3
	I[2] = NI[2];
	I[1] = NI[1] - 3 * NI[2];
	// k4
	I[8] = NI[8];
	I[7] = NI[7] - 6 * I[8];
	I[6] = NI[6] - 4 * I[7] - 12 * I[8];
	I[5] = NI[5] - I[7] - 3 * I[8];
	I[4] = NI[4] - I[6] - 2 * I[7] - 4 * I[8];
	I[3] = NI[3] - 2 * I[6] - 4 * I[5] - 6 * I[7] - 12 * I[8] - 3 * I[2];
	return I;
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

	constexpr int SORT = 0;

	cin >> N >> M;
	vector<ii> edges;
	neighbours.resize(N);
	inv_neighbours.resize(N);
	degree.resize(N);
	edges.reserve(M * !!SORT);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		assert(0 <= x && x < N);
		assert(0 <= y && y < N);
		if (x == y) continue; // skip self-loops
		int a = min(x, y), b = max(x, y);
		bool inserted = neighbours[a].insert(b).second;
		if (!inserted) continue; // skip multi-edges
		degree[x]++, degree[y]++;
		if (SORT) edges.push_back({x, y});
		else inv_neighbours[b].insert(a);
	}

	if (SORT) {
		vector<int> sorted(N), perm(N);
		iota(sorted.begin(), sorted.end(), 0);
		sort(sorted.begin(), sorted.end(), [&](int u, int v) {
			int du = SORT*degree[u], dv = SORT*degree[v];
			return du < dv || (du == dv && u < v);
		});
		for (int i = 0; i < N; i++) perm[sorted[i]] = i;

		for (int i = 0; i < N; i++)
			neighbours[i].clear(), inv_neighbours[i].clear();
		for (auto [i, j] : edges) {
			int x = perm[i], y = perm[j];
			int a = min(x, y), b = max(x, y);
			neighbours[a].insert(b);
			inv_neighbours[b].insert(a);
		}
		for (int i = 0; i < N; i++)
			degree[i] = neighbours[i].size() + inv_neighbours[i].size();

		for (int i = 0; i < N; i++)
			for (int j : neighbours[i]) 
				assert(i < j && SORT*degree[i] <= SORT*degree[j]);
	}

	
	if (argc == 1) {
		vector<long long> G = count<1, 1, 1, 1, 1, 1, 1, 1>();
		G = count_induced(G);
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

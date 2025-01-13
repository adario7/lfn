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
vector<int> degree;

int get(const map<int, int>& m, int x) {
	auto it = m.find(x);
	return it == m.end() ? 0 : it->second;
}

#define set_intersection(_a, _b, condition, code) do { \
	const auto& _small = _a.size() < _b.size() ? _a : _b; \
	const auto& _large = _a.size() < _b.size() ? _b : _a; \
	for (int z : _small) { \
		if (!(condition)) continue; \
		if (z == v || z == w) continue; \
		if (_large.count(z)) { \
			if (C5) G[5]++; \
			/* G7: look for sqaures with a diagonal */ \
			if (C7) if (neighbours[u].count(z)) G[7]++; \
			if (C7) if (neighbours[v].count(w)) G[7]++; \
			code; \
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
			G[1] += du * (du - 1) / 2;
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
			for (int v : neighbours[u]) {
				for (int w : neighbours[u]) if (v < w) {
					// type 1 square: one pair has same direction, other opposite
					set_intersection(neighbours[v], inv_neighbours[w], 1, {
						//cerr << "G5_1: u=" << u << " v=" << v << " w=" << w << " z=" << z << endl;
					});
					// type 2 square: both pairs have same direction
					set_intersection(neighbours[v], neighbours[w], 1, {
						//cerr << "G5_2: u=" << u << " v=" << v << " w=" << w << " z=" << z << endl;
					});
					// type 3 square: both pairs have opposite directions
					set_intersection(inv_neighbours[v], inv_neighbours[w], u < z, {
						//cerr << "G5_3: u=" << u << " v=" << v << " w=" << w << " z=" << z << endl;
					});
				}
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

	cin >> N >> M;
	neighbours.resize(N);
	inv_neighbours.resize(N);
	degree.resize(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		assert(0 <= x && x < N);
		assert(0 <= y && y < N);
		if (x == y) continue; // skip self-loops
		int a = min(x, y), b = max(x, y);
		bool inserted = neighbours[a].insert(b).second;
		if (!inserted) continue; // skip multi-edges
		inv_neighbours[b].insert(a);
		degree[x]++, degree[y]++;
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

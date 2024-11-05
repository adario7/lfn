#include <cassert>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

	int N, M;
	cin >> N >> M;

	vector<set<int>> neighbors(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		if (x == y) continue;
		int a = min(x, y), b = max(x, y);
		neighbors[a].insert(b);
	}

	// look at each edge and look for neighbour list intersections
	vector<int> t(N);
	for (int u = 0; u < N; u++) for (auto itr = neighbors[u].begin(); itr != neighbors[u].end(); itr++) {
		int v = *itr;
		assert(u < v);
		const auto &nu = neighbors[u], &nv = neighbors[v];
		auto iu = itr, iv = nv.begin();
		iu++;
		while (iu != nu.end() && iv != nv.end()) {
			int wu = *iu, wy = *iv;
			if (wu < wy) {
				iu++;
			} else if (wu > wy) {
				iv++;
			} else {
				t[u]++;
				t[v]++;
				t[wu]++;
				iu++, iv++;
			}
		}
	}

	int sum = 0;
	for (int i = 0; i < N; i++) {
		cout << t[i] << endl;
		sum += t[i];
	}
	cout << sum/3 << endl;
}

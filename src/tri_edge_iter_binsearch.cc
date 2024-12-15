#include <cassert>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

/* idea: for each edge,
 * compare the two adjacency lists and look for intersections using binary search
 */

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

	vector<int> t(N);
	for (int u = 0; u < N; u++) for (auto v : neighbors[u]) {
		const auto& small = neighbors[u].size() < neighbors[v].size() ? neighbors[u] : neighbors[v];
		const auto& large = neighbors[u].size() < neighbors[v].size() ? neighbors[v] : neighbors[u];
		for (auto w : small) {
			if (large.count(w)) {
				t[u]++;
				t[v]++;
				t[w]++;
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

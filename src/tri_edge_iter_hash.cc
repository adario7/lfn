#include <cassert>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

/* idea: for each edge,
 * compare the two adjacency lists and look for intersections using hash
 */

int main(int argc, char** argv) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

	int N, M;
	cin >> N >> M;

	vector<unordered_set<int>> neighbors(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		if (x == y) continue;
		int a = min(x, y), b = max(x, y);
		neighbors[a].insert(b);
	}

	int op = 0;
	vector<int> t(N);
	for (int u = 0; u < N; u++) for (auto v : neighbors[u]) {
		// note: not checking which neighbourhood is smaller has at least a x10 imact on performance
		const auto& small = neighbors[u].size() < neighbors[v].size() ? neighbors[u] : neighbors[v];
		const auto& large = neighbors[u].size() < neighbors[v].size() ? neighbors[v] : neighbors[u];
		for (auto w : small) {
			op++;
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

	if (argc > 1) cerr << "op = " << op << endl;
}

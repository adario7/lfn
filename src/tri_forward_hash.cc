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

	vector<vector<int>> nlist(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		if (x == y) continue;
		int u = min(x, y), v = max(x, y);
		nlist[u].push_back(v);
	}

	int op = 0;
	vector<int> t(N);
	vector<unordered_set<int>> nset(N);
	for (int u = 0; u < N; u++) for (int v : nlist[u]) {
		const auto& small = nset[u].size() < nset[v].size() ? nset[u] : nset[v];
		const auto& large = nset[u].size() < nset[v].size() ? nset[v] : nset[u];
		for (int w : small) {
			op++;
			if (large.count(w)) {
				t[u]++;
				t[v]++;
				t[w]++;
			}
		}

		nset[v].insert(u);
	}

	int sum = 0;
	for (int i = 0; i < N; i++) {
		cout << t[i] << endl;
		sum += t[i];
	}
	cout << sum/3 << endl;

	if (argc > 1) cerr << "op = " << op << endl;
}

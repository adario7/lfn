#include <cassert>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

/* idea: for each node,
 * look at each neighbor neighbors and see if the triangle is closed
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
	for (int u = 0; u < N; u++) for (int v : neighbors[u]) for (int w : neighbors[v]) {
		assert(u < v && v < w);
		if (neighbors[u].find(w) != neighbors[u].end()) {
			t[u]++;
			t[v]++;
			t[w]++;
		}
	}

	int sum = 0;
	for (int i = 0; i < N; i++) {
		cout << t[i] << endl;
		sum += t[i];
	}
	cout << sum/3 << endl;
}

#include <cassert>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <cassert>

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

	vector<int> degree(N);
	vector<ii> edges;
	edges.reserve(M);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		if (x == y) continue;
		edges.push_back({ x, y });
		degree[x]++;
		degree[y]++;
	}
	M = edges.size();

	auto sort_pair = [&degree](int u, int v) {
		int du = degree[u], dv = degree[v];
		int a = du < dv || (du == dv && u < v) ? u : v, b = u + v - a;
		return ii{ a, b };
	};

	vector<unordered_set<int>> neighbours(N);
	for (auto [u, v] : edges) {
		auto [a, b] = sort_pair(u, v);
		neighbours[a].insert(b);
	}

	vector<int> t(N);
	for (int u = 0; u < N; u++) for (int v : neighbours[u]) for (int w : neighbours[v]) {
		assert(order_pair(u, v) == ii(u, w));
		if (neighbours[u].count(w)) {
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

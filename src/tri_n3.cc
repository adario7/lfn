#include <cassert>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

/* idea: take all possible nodes triplets and check if all edges are in the graph.
 */

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

	int N, M;
	cin >> N >> M;

	set<ii> E;
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		assert(x != y);
		E.insert(ii(min(x, y), max(x, y)));
	}

	vector<int> t(N);
	for (int u = 0; u < N; u++) for (int v = u + 1; v < N; v++) for (int w = v + 1; w < N; w++) {
		assert(u < v && v < w);
		if (E.find(ii(u, v)) != E.end() && E.find(ii(v, w)) != E.end() && E.find(ii(u, w)) != E.end()) {
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

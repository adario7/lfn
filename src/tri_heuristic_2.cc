#include <algorithm>
#include <cassert>
#include <ios>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

/* idea: similar to heauristic_1;
 * instead of marking nodes as eliminated, we sort the nodes by degree
 * and keep on the adjecency list only nodes of higher degree:
 * this way we never consider nodes that are have already been visited
 */

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);

	int N, M;
	cin >> N >> M;

	vector<pair<int, int>> edges;
	vector<int> degree(N);
	for (int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		if (x == y) continue;
		assert(0 <= x && x < N && 0 <= y && y < N);
		edges.push_back({ x,y });
		degree[x]++;
		degree[y]++;
	}
	M = edges.size();

	// sort verticies by degree
	vector<int> order(N);
	for (int i = 0; i < N; i++) order[i] = i;
	sort(order.begin(), order.end(), [&](int a, int b) {
		return degree[a] < degree[b];
	});
	vector<int> order_of(N); // inverse of `order`
	for (int i = 0; i < N; i++)
		order_of[order[i]] = i;

	// list of neighbours next in the order
	vector<set<int>> neighbors(N);
	for (auto [x, y] : edges) {
		int ox = order_of[x], oy = order_of[y];
		int a = ox < oy ? x : y, b = x + y - a;
		assert(ox != oy);
		assert(order_of[a] < order_of[b]);
		neighbors[a].insert(b);
	}

	// look at each neighbor neighbors and see if the triangle is closed
	vector<int> t(N);
	for (int ou = 0; ou < N; ou++) {
		int u = order[ou];
		for (int v : neighbors[u]) for (int w : neighbors[v]) {
			assert(order_of[u] < order_of[v] && order_of[v] < order_of[w]);
			if (neighbors[u].find(w) != neighbors[u].end()) {
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

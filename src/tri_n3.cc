#include <iostream>
#include <set>
#include <vector>

using namespace std;

using ii = pair<int, int>;

int main() {
	int N, M;
	cin >> N >> M;

	set<ii> E;
	for (int i = 0; i < N; i++) {
		int x, y;
		cin >> x >> y;
		E.insert(ii(x, y));
	}

	vector<int> t(N);
	for (int u = 0; u < N; u++) for (int v = u + 1; v < N; v++) for (int w = v + 1; w < N; w++) {
		if (u == v || v == w || u == w) continue;
		if (E.find(ii(u, v)) != E.end() && E.find(ii(v, w)) != E.end() && E.find(ii(u, w)) != E.end()) {
			t[u]++;
			t[v]++;
			t[w]++;
		}
	}

	int sum = 0;
	for (int i = 0; i < N; i++) {
		cout << 2*t[i] << endl;
		sum += 2*t[i];
	}
	cout << sum << endl;
}

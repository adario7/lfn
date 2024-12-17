#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <chrono>

using namespace std;

using ii = pair<int, int>;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    using namespace std::chrono;

    // Start measuring time
    auto start = high_resolution_clock::now();

    // Reading input
    int N, M;
    cin >> N >> M;

    vector<int> degree(N);
    vector<ii> edges;
    edges.reserve(M);
    vector<unordered_set<int>> nset(N);

    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        if (x == y) continue;
        edges.push_back({x, y});
        degree[x]++;
        degree[y]++;
    }
    auto input_end = high_resolution_clock::now();

    // Sorting pair elements
    auto sort_pair = [&degree](int u, int v) {
        int du = degree[u], dv = degree[v];
        int a = du < dv || (du == dv && u < v) ? u : v, b = u + v - a;
        return ii{a, b};
    };
    for (ii& e : edges) {
        e = sort_pair(e.first, e.second);
    }
    auto pair_sort_end = high_resolution_clock::now();

    // Sorting the list of edges
    sort(edges.begin(), edges.end(), [&degree](ii e, ii f) {
		auto [eu, ev] = e;
		int deu = degree[eu], dev = degree[ev];
		auto [fu, fv] = f;
		int dfu = degree[eu], dfv = degree[fv];
		// sort first by starting node, then by ending node; nodes are sorted first by degree then by index
		if (deu != dfu) return deu < dfu;
		if (eu != fu) return eu < fu;
		if (dev != dfv) return dev < dfv;
		return ev < fv;
    });
    auto edge_sort_end = high_resolution_clock::now();

    // Counting triangles
    vector<int> t(N);
    for (auto [u, v] : edges) {
        const auto& small = nset[u].size() < nset[v].size() ? nset[u] : nset[v];
        const auto& large = nset[u].size() < nset[v].size() ? nset[v] : nset[u];
        for (int w : small) {
            if (large.count(w)) {
                t[u]++;
                t[v]++;
                t[w]++;
            }
        }
        nset[v].insert(u);
        nset[u].insert(v);
    }
    auto triangle_count_end = high_resolution_clock::now();

    // Printing output
    int sum = 0;
    for (int i = 0; i < N; i++) {
        cout << t[i] << endl;
        sum += t[i];
    }
    cout << sum / 3 << endl;
    auto output_end = high_resolution_clock::now();

	if (argc > 1) {
		auto input_duration = duration_cast<milliseconds>(input_end - start).count();
		auto pair_sort_duration = duration_cast<milliseconds>(pair_sort_end - input_end).count();
		auto edge_sort_duration = duration_cast<milliseconds>(edge_sort_end - pair_sort_end).count();
		auto triangle_count_duration = duration_cast<milliseconds>(triangle_count_end - edge_sort_end).count();
		auto output_duration = duration_cast<milliseconds>(output_end - triangle_count_end).count();
		cerr << "Input Reading Time: " << input_duration << " ms\n";
		cerr << "Pair Sorting Time: " << pair_sort_duration << " ms\n";
		cerr << "Edge Sorting Time: " << edge_sort_duration << " ms\n";
		cerr << "Triangle Counting Time: " << triangle_count_duration << " ms\n";
		cerr << "Output Printing Time: " << output_duration << " ms\n";
	}
    return 0;
}

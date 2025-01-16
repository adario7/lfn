#include <iostream>
#include "Snap.h"

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr), std::cout.tie(nullptr);


    int N, M;
    std::cin >> N >> M;

    PUNGraph Graph = TUNGraph::New();
    for (int i = 0; i < N; i++) {
        Graph->AddNode(i);
    }

    for (int i = 0; i < M; i++) {
        int u, v;
        std::cin >> u >> v;
        Graph->AddEdge(u, v);
    }

    TIntTrV TriadVec;
    TSnap::GetTriads(Graph, TriadVec);
    int triangleCount = 0;
    for (int i = 0; i < TriadVec.Len(); i++) {
        triangleCount += TriadVec[i].GetVal2();
    }

    std::cout << triangleCount / 3 << std::endl;

    return 0;
}

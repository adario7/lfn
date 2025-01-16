import snap
import sys

G = snap.TUNGraph.New()

i = 0

for line in sys.stdin:

    if i == 0:
        n = line.split()

        for u in range(n[0]):
            G.AddNode(u)

    else:

        nodes = line.split()

        G.AddEdge(nodes[0], nodes[1])


triangles = snap.GetTriads(G)
print(triangles)


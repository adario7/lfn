import networkx as nx
import sys

i = 0
G = nx.Graph()

for line in sys.stdin:

    if i != 0:
        nodes = line.split()
        
        G.add_edge(nodes[0], nodes[1])
    
    i = 1

print(sum(nx.triangles(G).values())//3)

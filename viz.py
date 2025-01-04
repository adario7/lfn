import sys
import networkx as nx
import matplotlib.pyplot as plt
from itertools import combinations

# Read input
lines = sys.stdin.read().strip().splitlines()
lines = [line for line in lines if line]
num_vertices, num_edges = map(int, lines[0].split())
edges = [tuple(map(int, line.split())) for line in lines[1:]]

# Build graph
G = nx.Graph()
G.add_nodes_from(range(num_vertices))
G.add_edges_from(edges)

# Count triangles using NetworkX's built-in function
triangle_count = sum(nx.triangles(G).values()) // 3
print("Number of triangles:", triangle_count)

# Visualize the graph
nx.draw(G, with_labels=True, node_color='skyblue', node_size=700, edge_color='gray')
plt.show()

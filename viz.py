import sys
import networkx as nx
import matplotlib.pyplot as plt

# Read input
lines = sys.stdin.read().strip().splitlines()
lines = [line for line in lines if line]
num_vertices, num_edges = map(int, lines[0].split())
edges = [tuple(map(int, line.split())) for line in lines[1:]]

# Determine graph type
is_digraph = len(sys.argv) > 1

# Build graph
G = nx.DiGraph() if is_digraph else nx.Graph()
G.add_nodes_from(range(num_vertices))
if is_digraph:
    G.add_edges_from((min(u, v), max(u, v)) for u, v in edges)
else:
    G.add_edges_from(edges)

# Count triangles if undirected
if not is_digraph:
    triangle_count = sum(nx.triangles(G).values()) // 3
    print("Number of triangles:", triangle_count)

# Visualize the graph
pos = nx.spring_layout(nx.Graph(G))
nx.draw(G, pos, with_labels=True, node_color='skyblue', node_size=700, edge_color='gray', arrows=is_digraph)
plt.show()

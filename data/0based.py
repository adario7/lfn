import sys

def read_and_convert_edges():
    edges = []
    max_vertex_id = 0
    
    for line in sys.stdin:
        u, v = map(int, line.strip().split())
        # Convert to 0-based indexing
        u -= 1
        v -= 1
        edges.append((u, v))
        # Track the maximum vertex id for node count
        max_vertex_id = max(max_vertex_id, u + 1, v + 1)

    # Print header with the number of nodes and edges
    num_nodes = max_vertex_id  # maximum 1-based vertex ID
    num_edges = len(edges)
    print(f"{num_nodes} {num_edges}")

    # Print the edges in 0-based indexing
    for u, v in edges:
        print(u, v)

# Run the function
read_and_convert_edges()

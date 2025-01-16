import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df= pd.read_csv('final_report.csv')

algorithms = [
    "color_coding_count",
    "tri_heuristic_1",
    "tri_heuristic_2",
    "tri_edge_iter_sorted",
    "tri_edge_iter_merge",
    "tri_edge_iter_hash",
    "tri_edge_iter_binsearch",
    "tri_forward_sorted",
    "tri_forward_merge",
    "tri_forward_hash",
    "tri_forward_binsearch",
    "tri_vert_iter_sorted",
    "tri_vert_iter_hash",
    "tri_vert_iter_binsearch",
    "tri_bader",
    "tri_n3"
]

print(df)

fig, ax = plt.subplots(figsize=(12, 6))
x = np.arange(len(df["Graph"]))
width = 0.8 / len(algorithms)

for i, algo in enumerate(algorithms):
    ax.bar(x + i * width, df[algo + " CPU time"], width, label=algo)

ax.set_xlabel("Graph", fontsize=12)
ax.set_ylabel("Execution Time (s)", fontsize=12)
ax.set_title("Execution Times by Algorithm and Graph", fontsize=14)
ax.set_xticks(x + width * (len(algorithms) - 1) / 2)
ax.set_xticklabels(df["Graph"], rotation=45, ha="right")
ax.legend(title="Algorithm", bbox_to_anchor=(1.05, 1), loc='upper left')

plt.ylim(0, 400)
plt.tight_layout()
plt.show()






#This class executes the input executable program five times for each graph in the "data" folder.
#It generates a report that includes:
#The mean of the outputs produced.
#The average execution time for all graphs  


import sys
import numpy as np
import time
import subprocess
import pandas as pd
from pathlib import Path

if len(sys.argv)>1:
    exe_name = sys.argv[1]
else:
    exe_name = input("Enter the name of the .exe file: ")

data_test_path = Path(__file__).resolve().parent / "data" 

def stopwatch(exe_path, test_graph_path):
    print(exe_path, test_graph_path)
    with open(test_graph_path, "r") as infile:
        start_time = time.time()
        try:
            graphlets = subprocess.run(exe_path, stdin=infile, capture_output=True,text=True,timeout=3600)
        except subprocess.TimeoutExpired:
            print("timer out")
            return ["-1", 3600]
        end_time = time.time()
    cpu_time = end_time - start_time
    print(cpu_time)
    return[graphlets.stdout, cpu_time]

def test(exe_path, n_tests = 5):
    report = []
    for graph in data_test_path.glob('*'):
        results = []
        graph_name = graph.name
        try:
            with open(str(graph), "r") as infile:
                V, E = map(int, infile.readline().strip().split()[:2])
        except Exception:
            continue
        if E >= 100000:
            results.append([-1.0, 3600])
        else:
            for i in range(n_tests):
                result = stopwatch(exe_path, str(graph))
                if result[0] is None or result[0]=='':
                    continue
                float_result = [float(result[0].strip().split("\n")[-1]), result[1]]
                results.append(float_result)
        results = np.array(results)
        if len(results) != 0:
            report.append([graph_name,V,E]+list(results.mean(axis=0)))
    return report


exe_path = str(Path(__file__).resolve().parent / "build" / exe_name)
tests = test(exe_path)
exe_path = Path(exe_path)
df = pd.DataFrame(tests, columns=[ "Graph", "|V|", "|E|" , exe_path.stem + " #graphlets", exe_path.stem + " CPU time" ])
print(df)

df_csv = pd.read_csv('final_report.csv')
df_csv[exe_path.stem + " #graphlets"] = df[exe_path.stem + " #graphlets"]
df_csv[exe_path.stem + " CPU time"] = df[exe_path.stem + " CPU time"]
df_csv.to_csv("final_report.csv", index=False)

import winsound
winsound.MessageBeep()
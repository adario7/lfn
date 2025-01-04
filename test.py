
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
import pickle

if len(sys.argv)>1:
    exe_names = sys.argv[1:]
else:
    #You can specify the names of the algorithms to be ran in torun.txt, one per line including the .exe
    f = open("build/torun.txt")

    exe_names = [str(name.split()[0]) for name in f]

    f.close()

data_test_path = Path(__file__).resolve().parent / "data" #"C:\Users\leona\OneDrive\Desktop\lfn\data"

def stopwatch(exe_path, test_graph_path):
    
    with open(test_graph_path, "r") as infile:
        start_time = time.time()
        graphlets = subprocess.run(exe_path, stdin=infile, capture_output=True,text=True)
        end_time = time.time()
    cpu_time = end_time - start_time
    return[graphlets.stdout, cpu_time]

def test(exe_path, n_tests = 5):
    report = []
    for graph in data_test_path.glob('*'):
        results = []
        graph_name = graph.name
        with open(str(graph), "r") as infile:
            V, E = map(int, infile.readline().strip().split()[:2])
        
        print(str(graph))
        for i in range(n_tests):
            result = stopwatch(exe_path, str(graph))
            if result[0] is None or result[0]=='':
                continue
            int_result = [int(result[0].strip().split("\n")[-1]), result[1]]
            results.append(int_result)
        results = np.array(results)
        if len(results) != 0:
            report.append([graph_name,V,E]+list(results.mean(axis=0)))
    return report

#The results are saved in a dictionary that is stored safely via pickle
#To load the dictionary in another script use:
#with open("saved.pkl", "rb") as pkl:
    #reports = pickle.load(pkl)

def multiple_tests(exe_names, n_tests = 5):
    reports = {}
    with open("saved.pkl", "wb") as pkl:
        pickle.dump(reports, pkl)
    
    for exe_name in exe_names:
        
        print("Now testing: " + exe_name + "\n")

        exe_path = str(Path(__file__).resolve().parent / "build" / exe_name)
        print(exe_path)
        reports[exe_name] = test(exe_path, n_tests)

        with open("saved.pkl", "wb") as pkl:
            pickle.dump(reports, pkl)
    
    return reports

#exe_name = "tri_edge_iter_binsearch.exe"

#exe_path = str(Path(__file__).resolve().parent / "build" / exe_name)
#exe_path = "C:\\Users\\leona\\OneDrive\\Desktop\\lfn\\src\\tri_heuristic_1.exe"
#tests = test(exe_path)

#exe_path = "C:\\Users\\leona\\OneDrive\\Desktop\\lfn\\src\\tri_heuristic_1.exe"
tests = multiple_tests(exe_names)
#df = pd.DataFrame(tests, columns=[ "Graph", "|V|", "|E|" , "mean #triangles", "mean CPU time" ])
#print(df)


#   This program generates scatter plots from .csv files data. 
#   Run this from within the folder containing the .csv files to 
#   convert them all into .pdf plots.
#
#   Make sure to change the title to reflect your data.
#
#   Copyright (c) Dario Sanfilippo 2021

import sys
import glob
import numpy as np
import scipy as sp
import pandas as pd
import matplotlib.pyplot as plt

sr = int(sys.argv[1])

for filename in glob.glob('*.csv'):

    error = []
    temp = []
    
    name = []
    name = filename
    name = name + ".pdf"
    
    data = pd.read_csv(filename)
    tmp = data.to_numpy()
    
    l = len(data)
    
    for i in range(l):
        error = np.append(error, tmp[i][0])
    
    avg = np.mean(error)
    
    plt.xlabel('Test run')
    plt.ylabel('Error rate')
    
    plt.axhline(y = avg,linewidth = 1, color = 'b', label = "Average error")
    plt.plot(error, linewidth = .5, color = 'black', marker = ".", label = "Error rate")
    
    plt.title("Adder operator")

    handles, labels = plt.gca().get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    plt.legend(by_label.values(), by_label.keys())

    plt.grid(True)
    
    plt.savefig(name)
    plt.show()

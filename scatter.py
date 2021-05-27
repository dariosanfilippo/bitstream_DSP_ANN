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

for filename in glob.glob('*.csv'):

    freq = []
    error = []
    tmp = []
    
    name = []
    name = filename
    name = name + ".pdf"
    
    data = pd.read_csv(filename)
    tmp = data.to_numpy()
    
    l = len(data)
    
    for i in range(l):
        freq = np.append(freq, tmp[i][0])
        error = np.append(error, tmp[i][1])
    
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Error (target-ANN ratio)')
    
    plt.axhline(y = 1,linewidth = 1, color = 'r', label = "Best fit")
    plt.scatter(freq, error, linewidth = .5, color = 'black', label = "ANN output")
    
    plt.xscale('log')
    
    plt.title("Frequency estimation")

    handles, labels = plt.gca().get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    plt.legend(by_label.values(), by_label.keys())

    plt.grid(True)
    
    plt.savefig(name)

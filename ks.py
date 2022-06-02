import numpy as np
from scipy import stats

fe = np.loadtxt("fe.txt")
gel = np.loadtxt("gel.txt")

ks = stats.ks_2samp(fe, gel, mode='exact')

print(ks)

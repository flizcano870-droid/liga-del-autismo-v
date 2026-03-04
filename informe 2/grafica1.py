import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos1.xlsx')

x=df.iloc[:, 0]
y=df.iloc[:, 1]

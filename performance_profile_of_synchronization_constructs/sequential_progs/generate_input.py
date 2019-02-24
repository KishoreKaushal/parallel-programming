import numpy as np
NUM_QUERIES = 10000
arr = np.random.randint(low=1, high=4, size=(NUM_QUERIES))
print(*arr , sep=" ", end="\n")
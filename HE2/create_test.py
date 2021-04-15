import numpy as np

arr = np.zeros((6, 6))
arr[:, 2:4] = 30
# arr = np.zeros((8, 8))
# arr[:, :4] = 10
print('arr')
print(arr)

weights = np.zeros((3, 3))
weights[0] = 2
weights[1] = 1
weights[2] = -2
# weights[:, 0] = 1
# weights[:, 1] = 0
# weights[:, 2] = -1
print('weights')
print(weights)

# out = np.zeros((6, 6))
out = np.zeros((4, 4))
for i in range(out.shape[0]):
    for j in range(out.shape[1]):
        out[i, j] = np.sum(arr[i:i+3, j:j+3]*weights)

print('out')
print(out)

with open('SNN2_2D', 'r') as infile:
    table2D = infile.readlines()

with open('SNN2_CRS', 'r') as infile:
    CRS = infile.readlines()

length = 2*88234 + 2
print(len(CRS), length)

assert len(table2D) == length
assert len(CRS) == length

for e1, e2 in zip(table2D, CRS):
    assert e1 == e2

print('All good!')

with open('test', 'r') as infile:
    lines = infile.readlines()
assert len(lines) == 7
row_ptr = lines[5].split()
assert len(row_ptr) == 4039+1
col_idx = lines[6].split()
assert len(col_idx) == 2*88234
basic = row_ptr + col_idx

with open('test_alt', 'r') as infile:
    lines = infile.readlines()
assert len(lines) == 7
row_ptr = lines[5].split()
assert len(row_ptr) == 4039+1
col_idx = lines[6].split()
assert len(col_idx) == 2*88234
fast = row_ptr + col_idx

for e1, e2 in zip(basic, fast):
    assert e1 == e2

print('All good!')

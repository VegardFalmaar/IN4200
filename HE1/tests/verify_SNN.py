with open('SNN1', 'r') as infile:
    basic = infile.readlines()[1:]

with open('SNN1_O', 'r') as infile:
    fast = infile.readlines()[1:]

length = 4039**2

assert len(basic) == length
assert len(fast) == length

for e1, e2 in zip(basic, fast):
    assert e1 == e2

print('All good!')

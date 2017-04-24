with open('ex5_7_mutates_answer.txt', 'r') as f:
    l = f.readlines()

pattern = l[0].split('pattern: ')[1]

d = {
    0: 0,
    1: 0,
    2: 0,
    3: 0,
    4: 0,
    5: 0,
    6: 0,
    7: 0
}

for i in l[1:]:
    if len(i) > 1:
        if i[0] == 'S':
            print('================')
        elif i[2] == '(':
            diff = 0
            print(i[3:18])
            for idx, k in enumerate(i[3:18]):
                if pattern[idx] != k:
                    diff += 1
            if i[0] == '*':
                d[diff] += 1
                print('*', diff)
            else:
                print(diff)

print(d)

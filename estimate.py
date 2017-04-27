with open('ex3_5_mutates_answer.txt', 'r') as f:
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
            pass
            # print('================')
        elif i[2] == '(':
            diff = 0
            # print(i[3:18])
            for idx, k in enumerate(i[3:18]):
                if pattern[idx] != k:
                    diff += 1
            if i[0] == '*':
                d[diff] += 1
                # print('*', diff)
            # else:
                # print(diff)


# d = {0: 0, 1: 4, 2: 9, 3: 10, 4: 10, 5: 17, 6: 0, 7: 0}
print(d)
avg = (10*d[0]+2*d[1]+1*d[2]+1*d[3]+2*d[4]+10*d[5])/50
print(avg)
l2_sum = 0
l2_sum += d[0]*((10-avg)**2)
l2_sum += d[1]*((2-avg)**2)
l2_sum += d[2]*((1-avg)**2)
l2_sum += d[3]*((1-avg)**2)
l2_sum += d[4]*((2-avg)**2)
l2_sum += d[5]*((10-avg)**2)
print(l2_sum)

d = {0: 0, 1: 8, 2: 15, 3: 15, 4: 8, 5: 4, 6: 0, 7: 0}
# print(d)
avg = (2*d[1]+1*d[2]+1*d[3]+2*d[4]+10*d[5])/50
print(avg)
l2_sum = 0
for i in range(6):
    l2_sum += (i-avg)**2
print(l2_sum)

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
            # pass
            print('================')
        elif i[2] == '(':
            diff = 0
            # print(i[3:18])
            for idx, k in enumerate(i[3:18]):
                if pattern[idx] != k:
                    diff += 1
            if i[0] == '*':
                # d[diff] += 1
                print('*', diff)
            else:
                print(diff)
            d[diff] += 1

# d = {0: 0, 1: 0, 2: 5, 3: 9, 4: 17, 5: 12, 6: 6, 7: 1}
print(d)

arr = d

# l2_sum = 0;
# l2_avg = (105*arr[0]+15*arr[1]+5*arr[2]+3*arr[3]+3*arr[4]+5*arr[5]+15*arr[6]+105*arr[7])/50;

# print(105+15+5+3+3+5+15+105)
# print(l2_avg)

# l2_sum += arr[0]*((105-l2_avg)*(105-l2_avg));
# l2_sum += arr[1]*((15-l2_avg)*(15-l2_avg));
# l2_sum += arr[2]*((5-l2_avg)*(5-l2_avg));
# l2_sum += arr[3]*((3-l2_avg)*(3-l2_avg));
# l2_sum += arr[4]*((3-l2_avg)*(3-l2_avg));
# l2_sum += arr[5]*((5-l2_avg)*(5-l2_avg));
# l2_sum += arr[6]*((15-l2_avg)*(15-l2_avg));
# l2_sum += arr[7]*((105-l2_avg)*(105-l2_avg));

# print(l2_sum)

from itertools import combinations
import time
import resource

rsrc = resource.RLIMIT_DATA
soft, hard = resource.getrlimit(rsrc)
resource.setrlimit(rsrc, (8000, hard))
soft, hard = resource.getrlimit(rsrc)

CS_FILE = 'q2.data'
GENOME_FILE = 'genome.data'

control_sequence_list = []

genome_list = []

with open(CS_FILE, 'r') as f:
     control_sequence_list = f.readlines()

with open(GENOME_FILE, 'r') as f:
     genome_list = f.readlines()

def cs_to_int(cs):
    cs_int = 0
    for idx, c in enumerate(reversed(cs)):
        cs_int += 'ATCG'.find(c) << 2*idx
    return cs_int

def int_to_cs(cs_int):
    cs = ''
    for i in range(15):
        cs += 'ATCG'[cs_int & 3]
        cs_int = cs_int >> 2
    return cs[::-1]

def get_diff(s1, s2):
    diff = s1 ^ s2
    res = 0
    for i in range(15):
        res += 1 if (diff & 3) else 0
        diff = diff >> 2
    return res

def all_combinations(start, end):
    all_c = []
    for i in range(start, end+1):
        all_c += list(combinations(range(15), i))
    return all_c


def get_replace_list(seq, pos):
    res = []
    g = seq & (3 << pos)
    for i in range(4):
        tmp = seq
        if g != i:
            res.append((seq & (0b111111111111111111111111111111 ^ (1 << pos*2) ^ (1 << pos*2+1))) | (i << pos*2))
    return res


def gen_degen(seq, ac):
    degen_list = []
    for change_pos in ac:
        tmp = [seq]
        for pos in change_pos:
            ttmp = []
            for s in tmp:
                ttmp += get_replace_list(s, pos)
            tmp = ttmp
        degen_list += tmp
    return degen_list



ac = all_combinations(1, 1)

cs = []


for c in control_sequence_list:
    tmp = []
    for ptr in range(986):
            tmp.append(cs_to_int(c[ptr:ptr+15]))
    cs.append(tmp)

cs0 = list(set(cs[0]))
print(len(cs0))

first_degen = []

for i in cs:
    for k in i:
        first_degen += gen_degen(k, ac)

first_degen = list(set(first_degen))

for d in first_degen:
    print(len(first_degen))

    for i in cs:
        flag = False
        for k in i:
            # print(d, k)
            if get_diff(d, k) <= 5:
                flag = True
        if not flag:
            try:
                first_degen.remove(d)
            except:
                pass

print(len(first_degen))

# for idx, seq in enumerate(cs[1:]):
#     print(idx, len(cs0))
#     for c0 in cs0:
#         flag = False
#         for s in seq:
#             # print(get_diff(s, c0))
#             if get_diff(s, c0) <= 6:
#                 flag = True
#         if not flag:
#             # print('gggggg')
#             cs0.remove(c0)
# print(len(cs0))


# cnt = 0
# for i in cs1:
#     for k in cs2:
#         diff = get_diff(i, k)
#         if diff < 10:
#             cnt += 1

# print(cnt)

# print(len(d))
# print(time.time()-start)
# cs_chunk = {}
# seq = set()

# for cs in control_sequence_list:
#     for ptr in range(986):
#         cs_int = cs_to_int(cs[ptr:ptr+14])
#         seq.add(cs_int)
#         if cs_chunk.get(cs_int):
#             cs_chunk[cs_int] += 1
#         else:
#             cs_chunk[cs_int] = 1
#
# for s1 in seq:
#     for s2 in seq:
#         get_diff()

# 826987577 17 ATTACCGTTAAGCT
#  86724839 50 TTACCGTTAAGCTG


# for c in sorted(cs_chunk, key=cs_chunk.get):
    # print(c, cs_chunk[c], int_to_cs(c))

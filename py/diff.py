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
# a = [67436544,391451652,118559833,294405123,87151113,284974102,470616650,510772713,870273590,160685310,401519118,652995269,1053179883,1067941626,721338104,716172271,717205438]

# ans = cs_to_int('CATACTGTAGCTACG')

print(int_to_cs(22298705))

# for i in a:
    # print(get_diff(ans, i))

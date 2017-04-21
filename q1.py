CS_FILE = 'q1.data'
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
    for i in range(14):
        cs += 'ATCG'[cs_int & 3]
        cs_int = cs_int >> 2
    return cs[::-1]



cs_chunk = {}

for cs in control_sequence_list:
    for ptr in range(986):
        cs_int = cs_to_int(cs[ptr:ptr+15])
        if cs_chunk.get(cs_int):
            cs_chunk[cs_int] += 1
        else:
            cs_chunk[cs_int] = 1

for cs in genome_list:
    for ptr in range(986):
        cs_int = cs_to_int(cs[ptr:ptr+15])
        if cs_chunk.get(cs_int):
            cs_chunk[cs_int] -= 1

for c in sorted(cs_chunk, key=cs_chunk.get):
  print(c, cs_chunk[c], int_to_cs(c))

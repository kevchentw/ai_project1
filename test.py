from skbio.sequence import DNA

CS_FILE = 'q2.data'
GENOME_FILE = 'genome.data'

control_sequence_list = []

genome_list = []

with open(CS_FILE, 'r') as f:
     control_sequence_list = f.readlines()

with open(GENOME_FILE, 'r') as f:
     genome_list = f.readlines()

a = DNA(control_sequence_list[0][:-1])
print(a)

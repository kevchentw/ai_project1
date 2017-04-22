#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef pair<int, int> PAIR;
int cmp(const PAIR& x, const PAIR& y)
{
    return x.second < y.second;
}

int seq_to_int(string seq) {
    int iseq = 0;
    for(int i=0; i<15;i++){
        switch(seq[i]) {
            case 'T':
                iseq += 1 << (2 * (14-i));
                break;
            case 'C':
                iseq += 2 << (2 * (14-i));
                break;
            case 'G':
                iseq += 3 << (2 * (14-i));
                break;
            default:
                continue;
        }
    }
    return iseq;
}

string int_to_seq(int iseq) {
    string s;
    for(int i=0; i<15; i++){
        s = "ATCG"[iseq & 3] + s;
        iseq = iseq >> 2;
    }
    return s;
}

int main() {
    string line;
    ifstream file_q1("q1.data");
    ifstream file_genome("genome.data");
    string data[50];
    string genome[1000];
    int c = 0;
    while (getline(file_q1, line))
    {
        data[c++] = line;
    }
    c = 0;
    while (getline(file_genome, line))
    {
        genome[c++] = line;
    }


    unordered_map<int, int> m;
    unordered_map<int, int>::iterator it;

    for(int i=0; i<50; i++){
        for(int pos=0; pos<986; pos++){
            int iseq = seq_to_int(data[i].substr(pos, pos+15));
            it = m.find(iseq);
            if (it != m.end()) {
                it->second += 1;
            }
            else{
                m.insert(make_pair(iseq, 1));
            }
        }
    }

    for(int i=0; i<1000; i++){
        for(int pos=0; pos<986; pos++){
            int iseq = seq_to_int(genome[i].substr(pos, pos+15));
            it = m.find(iseq);
            if (it != m.end()) {
                it->second -= 1;
            }
        }
    }

    vector<pair<int, int> > vec;
    for (it = m.begin(); it != m.end(); ++it)
    {
        vec.push_back(make_pair(it->first, it->second));
    }

    sort(vec.begin(), vec.end(), cmp);
    vector<pair<int, int> >::iterator vit;
    for (vit = vec.begin(); vit!=vec.end(); vit++){
        cout << vit->first << " " << vit->second << endl;
    }



    return 0;
}
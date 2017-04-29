#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ctime>

using namespace std;

typedef pair<int, int> PAIR;
int cmp(const PAIR& x, const PAIR& y)
{
    return x.second > y.second;
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
    const clock_t begin_time = clock();
    string line;
    ifstream file_q1("./dataset/q1.data");
    ifstream file_genome("./dataset/genome.data");
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

    vector<vector<int> > all_cs;
    for(int i=0; i<50; i++){
        vector<int> each_cs;
        for(int pos=0; pos<986; pos++){
            int iseq = seq_to_int(data[i].substr(pos, pos+15));
            each_cs.push_back(iseq);
            it = m.find(iseq);
            if (it != m.end()) {
                it->second += 1;
            }
            else{
                m.insert(make_pair(iseq, 1));
            }
        }
        all_cs.push_back(each_cs);
    }

    vector<pair<int, int> > vec;
    for (it = m.begin(); it != m.end(); ++it)
    {
        vec.push_back(make_pair(it->first, it->second));
    }

    sort(vec.begin(), vec.end(), cmp);


    int world_appear = 0;
    for(int i=0; i<1000; i++){
        for(int pos=0; pos<986; pos++){
            int iseq = seq_to_int(genome[i].substr(pos, pos+15));
            if(vec.begin()->first == iseq){
                world_appear ++;
            }
        }
    }

    for(int i=0; i<all_cs.size(); i++){
        for(int j=0; j<all_cs[i].size(); j++){
            if(all_cs[i][j]==vec.begin()->first){
                cout << "S" << i+1 << ":{(" << int_to_seq(all_cs[i][j]) << "," << j+1 << ")}" << endl;
            }
        }
    }


    cout << "best control sequences candidate: " << int_to_seq(vec.begin()->first) << endl;
    cout << "appear in q1.data " << vec.begin()->second << " times" << endl;
    cout << "appear in world " << world_appear << " time" << endl;
    cout << "time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    return 0;
}

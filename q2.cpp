#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ctime>

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

int get_diff(int s1, int s2) {
    int x = s1 ^ s2;
    int diff = 0;
    while(x > 0){
        if(x & 3){
            diff++;
        }
        x = x >> 2;
    }
    return diff;
}

vector<vector<int> > comb(int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    vector<vector<int> > all_combinations;

    do {
        vector<int> each_comb;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i])
                each_comb.push_back(i);
        }
        all_combinations.push_back(each_comb);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    return all_combinations;
}

unordered_set<int> get_replaced_seq(int seq, int pos){
    unordered_set<int> v;
    int pos_gen = seq & (3 << pos);
    for(int i=0; i<4; i++){
            int tmp = seq;
            if(pos_gen != i){
                v.insert((seq &
                    (0b111111111111111111111111111111
                        ^ (1 << pos*2)
                        ^ (1 << (pos*2+1))))
                        | (i << pos*2)
                    );
            }
    }
    return v;
}

unordered_set<int> gen_degen(int seq, vector<vector<int> > all_combinations){
    unordered_set<int> v;
    for (auto i : all_combinations){ // access by const reference
        unordered_set<int> tmp1;
        tmp1.insert(seq);
        for (auto pos : i){
            unordered_set<int> tmp2;
            for(auto j : tmp1){
                unordered_set<int> replaced_seq = get_replaced_seq(j, pos);
                tmp2.insert(replaced_seq.begin(), replaced_seq.end());
            }
            tmp1 = tmp2;
        }
        v.insert(tmp1.begin(), tmp1.end());
    }
    return v;
}

int main() {
    const clock_t begin_time = clock();
    string line;
    ifstream file_q1("ex1_5_mutates.data");
    ifstream file_genome("genome.data");
    string data[50];
    string genome[1000];
    // cout << get_diff(1017623764, 848998258) << endl;
    // return 1;
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

    vector<vector<int> > all_cs;
    for(int i=0; i<50; i++){
        vector<int> each_cs;
        for(int pos=0; pos<986; pos++){
            each_cs.push_back(seq_to_int(data[i].substr(pos, pos+15)));
        }
        all_cs.push_back(each_cs);
    }

    vector<vector<int> > c1= comb(15, 1);

    unordered_set<int> degen;

    int cnt = 1;

    for(auto SEQ_LIST: all_cs){
        cout << "Gene: " << cnt++ << " Start" << endl;
        unordered_set<int> degen_set;

        for(auto SEQ: SEQ_LIST){
            unordered_set<int> tmp = gen_degen(SEQ, c1);
            degen_set.insert(tmp.begin(), tmp.end());
        }
        cout << "Degen set size: " << degen_set.size() << endl;

        for(auto seq_list: all_cs){
            unordered_set<int> copy_degen(degen_set);
            // cout << "size: " << degen_set.size() << endl;
            for(auto d: degen_set){
                int flag = 0;
                for(auto seq: seq_list){
                    if(get_diff(d, seq)<=5){
                        flag = 1;
                        break;
                    }
                }
                if(!flag){
                    copy_degen.erase(d);
                }
            }
            degen_set = copy_degen;
            // for(auto r: remove_set){
            //     degen_set.erase(r);
            // }
        }
        cout << "Left:" << degen_set.size() << endl;
        if(degen_set.size()){
            for(auto d: degen_set){
                cout << d << endl;
                cout << int_to_seq(d) << endl;
            }
        }
        cout << "time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    }

    return 0;
}

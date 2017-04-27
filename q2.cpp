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
    x = ((x & 0x55555555) | ((x >> 1) & 0x55555555));
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
    return x;
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

vector<int> get_replaced_seq(int seq, int pos){
    vector<int> v;
    int pos_gen = seq & (3 << pos);
    for(int i=0; i<4; i++){
            int tmp = seq;
            if(pos_gen != i){
                v.push_back((seq &
                    (0b111111111111111111111111111111
                        ^ (1 << pos*2)
                        ^ (1 << (pos*2+1))))
                        | (i << pos*2)
                    );
            }
    }
    return v;
}

vector<int> gen_degen(int seq, vector<vector<int> > all_combinations){
    vector<int> v;
    for (auto i : all_combinations){ // access by const reference
        vector<int> tmp1;
        tmp1.push_back(seq);
        for (auto pos : i){
            vector<int> tmp2;
            for(auto j : tmp1){
                vector<int> replaced_seq = get_replaced_seq(j, pos);
                tmp2.reserve(tmp2.size()+replaced_seq.size());
                tmp2.insert(tmp2.end(), replaced_seq.begin(), replaced_seq.end());
            }
            tmp1 = tmp2;
        }
        v.reserve(v.size()+tmp1.size());
        v.insert(v.end(), tmp1.begin(), tmp1.end());
    }
    return v;
}

int estimate_function(int candidate, vector<vector<int> > all_cs) {
    int estimate = 0;
    int arr[6] = {};
    int avg = 0;
    int sum = 0;
    int weighted_sum = 0;
    int median = 0;
    for(auto seq_list: all_cs){
        int min = 16;
        int diff;
        for(auto seq: seq_list){
            diff = get_diff(seq, candidate);
            if(diff<min){
                min = diff;
            }
        }
        arr[min] += 1;
    }
    float l2_sum = 0;
    float l2_avg = (float)(2*arr[1]+1*arr[2]+1*arr[3]+2*arr[4]+10*arr[5])/50;
    for(int i=1;i<=5;i++){
        weighted_sum += i*arr[i];
        sum += arr[i];
        // l2_sum += (l2_avg-(float)i)*(l2_avg-(float)i);
        if(!median && sum >= 25){
            median = i;
        }
    }
    l2_sum += arr[0]*((10-avg));
    l2_sum += arr[1]*((2-avg));
    l2_sum += arr[2]*((1-avg));
    l2_sum += arr[3]*((1-avg));
    l2_sum += arr[4]*((2-avg));
    l2_sum += arr[5]*((10-avg));
    cout << "l2: " << l2_sum << endl;

    avg = weighted_sum/5;
    cout << "avg: " << avg << " median: " << median << endl;
    return l2_sum;
}

int main() {
    const clock_t begin_time = clock();
    string line;
    ifstream file_q1("ex1_5_mutates.data");
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

    vector<vector<int> > all_cs;
    for(int i=0; i<50; i++){
        vector<int> each_cs;
        for(int pos=0; pos<986; pos++){
            int sint = seq_to_int(data[i].substr(pos, pos+15));
            each_cs.push_back(sint);
        }
        all_cs.push_back(each_cs);
    }

    vector<vector<int> > c1= comb(15, 1);


    int cnt = 1;
    int ans;

    for(auto SEQ_LIST: all_cs){
        cout << "Gene: " << cnt++ << " Start" << endl;
        int total = 0;
        unordered_set<int> degen_set;
        for(auto SEQ: SEQ_LIST){
            vector<int> tmp = gen_degen(SEQ, c1);
            degen_set.insert(tmp.begin(), tmp.end());
        }
        cout << "Degen set size: " << degen_set.size() << endl;
        for(int i=0; i<50; i++){
            unordered_set<int> next_degen;
            for(auto d: degen_set){
                int flag = 0;
                for(auto seq: all_cs.at(i)){
                    if(get_diff(d, seq)<=5){
                        flag = 1;
                        break;
                    }
                }
                if(flag){
                    next_degen.insert(d);
                }
            }
            degen_set = next_degen;
            next_degen.clear();
        }
        cout << "Left:" << degen_set.size() << endl;
        if(degen_set.size()){
            ans = *degen_set.begin();
            for(auto d: degen_set){
                cout << d << endl;
                cout << int_to_seq(d) << endl;
                estimate_function(d, all_cs);
            }
        }
        cout << "time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
        cout << "=========" << endl;

    }

    return 0;
}

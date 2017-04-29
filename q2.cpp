#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <cmath>

using namespace std;

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
    int arr[6] = {0};
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
    float l2_avg = (float)(10*arr[0]+2*arr[1]+1*arr[2]+1*arr[3]+2*arr[4]+10*arr[5])/6;

    l2_sum += abs(10*arr[0]-l2_avg);
    l2_sum += abs(2*arr[1]-l2_avg);
    l2_sum += abs(1*arr[2]-l2_avg);
    l2_sum += abs(1*arr[3]-l2_avg);
    l2_sum += abs(2*arr[4]-l2_avg);
    l2_sum += abs(10*arr[5]-l2_avg);
    return l2_sum;
}

int main() {
    const clock_t begin_time = clock();
    string line;
    ifstream file_q1("./dataset/q2.data");
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

    vector<pair<int, int> > candidate_v;

    for(auto SEQ_LIST: all_cs){
        // cout << "Gene: " << cnt++ << " Start" << endl;
        int total = 0;
        unordered_set<int> degen_set;
        for(auto SEQ: SEQ_LIST){
            vector<int> tmp = gen_degen(SEQ, c1);
            degen_set.insert(tmp.begin(), tmp.end());
        }
        // cout << "Degen set size: " << degen_set.size() << endl;
        vector<int> degen_v(degen_set.size());
        copy(degen_set.begin(), degen_set.end(), degen_v.begin());

        for(int i=0; i<50; i++){
            vector<int> next_degen;
            for(auto d: degen_v){
                int flag = 0;
                for(auto seq: all_cs.at(i)){
                    if(get_diff(d, seq)<=5){
                        flag = 1;
                        break;
                    }
                }
                if(flag){
                    next_degen.push_back(d);
                }
            }
            degen_v = next_degen;
            next_degen.clear();
        }
        if(degen_v.size()){
            for(auto d: degen_v){
                // cout << d << endl;
                // cout << int_to_seq(d) << endl;
                candidate_v.push_back(make_pair(estimate_function(d, all_cs), d));
            }
        }
    }
    sort(candidate_v.begin(), candidate_v.end());
    // int world_appear[5] = {0};
    // for(int i=0; i<1000; i++){
    //     for(int pos=0; pos<986; pos++){
    //         int iseq = seq_to_int(genome[i].substr(pos, pos+15));
    //         for(int cnt_c=0; cnt_c<5 && cnt_c<candidate_v.size(); cnt_c++){
    //             if(get_diff(candidate_v.at(cnt_c).second, iseq)<=5){
    //                 world_appear[cnt_c] ++;
    //             }
    //         }
    //     }
    // }
    //
    // for(int cnt_c=0; cnt_c<5 && cnt_c<candidate_v.size(); cnt_c++){
    //     cout << int_to_seq(candidate_v.at(cnt_c).second) << endl;
    //     cout << "cost:" << candidate_v.at(cnt_c).first << endl;
    //     cout << "world_appear:" << world_appear[cnt_c] << endl;
    // }



    for(int i=0; i<all_cs.size(); i++){
        int min = 16;
        vector<int> ans_seq;
        vector<int> ans_diff;
        vector<int> ans_pos;
        for(int j=0; j<all_cs[i].size(); j++){
            int diff = get_diff(all_cs[i][j], candidate_v.begin()->second);
            if(diff<=5){
                if(diff<min){
                    min = diff;
                }
                ans_seq.push_back(all_cs[i][j]);
                ans_diff.push_back(diff);
                ans_pos.push_back(j+1);
            }
        }
        int sig = 0;
        cout << "S" << i+1 <<  ":" << endl;
        for(int cnt_ans = 0; cnt_ans<ans_seq.size(); cnt_ans++){
            if(!sig && ans_diff[cnt_ans]==min){
                sig = 1;
                cout << "   *{(" << int_to_seq(ans_seq[cnt_ans]) << "," << ans_pos[cnt_ans]<< ")}" << endl;
            }
            else{
                cout << "    {(" << int_to_seq(ans_seq[cnt_ans]) << "," << ans_pos[cnt_ans]<< ")}" << endl;
            }
        }
    }
    cout << "best control sequences candidate:" << int_to_seq(candidate_v.begin()->second) << endl;
    cout << "second best control sequences candidate:" << int_to_seq(next(candidate_v.begin())->second) << endl;
    cout << "time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    return 0;
}

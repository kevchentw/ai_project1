#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <stdlib.h>


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

int seq_cg_total(string seq) {
    int cg_total = 0;
    for(int i=0; i<15;i++){
        if(seq[i]=='C' || seq[i]=='G'){
            cg_total++;
        }
    }
    return cg_total;
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
    int arr[16] = {0};
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
    float l2_avg = (float)(105*arr[0]+15*arr[1]+5*arr[2]+3*arr[3]+3*arr[4]+5*arr[5]+15*arr[6]+105*arr[7])/8;
    l2_sum += abs(105*arr[0]-l2_avg);
    l2_sum += abs(15*arr[1]-l2_avg);
    l2_sum += abs(5*arr[2]-l2_avg);
    l2_sum += abs(3*arr[3]-l2_avg);
    l2_sum += abs(3*arr[4]-l2_avg);
    l2_sum += abs(5*arr[5]-l2_avg);
    l2_sum += abs(15*arr[6]-l2_avg);
    l2_sum += abs(105*arr[7]-l2_avg);

    return l2_sum;
}

int main() {
    srand (time(NULL));
    const clock_t begin_time = clock();
    string line;
    ifstream file_q1("./dataset/q3.data");
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

    vector<vector<int> > cg_v;

    for(int i=0; i<80; i++){
        vector<int> tmp;
        cg_v.push_back(tmp);
    }

    for(int i=0; i<50; i++){
        vector<int> each_cs;
        for(int pos=0; pos<986; pos++){
            int sint = seq_to_int(data[i].substr(pos, pos+15));
            int cg_total = seq_cg_total(data[i].substr(pos, pos+15));
            // version 1
            cg_v[cg_total].push_back(sint);
            // version 2
            // if(cg_total>=7 && cg_total<=8){
            //     cg_v[rand() % (cg_total+9)].push_back(sint);
            // }
            // if(cg_total<=7)
            //     cg_v[rand() % (cg_total+9)].push_back(sint);
            // else
            //     cg_v[rand() % (24-cg_total)].push_back(sint);
            //version 3
            // cout << 10*cg_total + (rand()%cg_total) << endl;
            // if(cg_total<=7)
            //     cg_v[10*cg_total + (rand()%(cg_total+1))].push_back(sint);
            // else
            //     cg_v[10*(15-cg_total) + (rand() % (16-cg_total))].push_back(sint);
            each_cs.push_back(sint);
        }
        all_cs.push_back(each_cs);
    }

    // for(int i=0; i<cg_v.size(); i++){
    //     cout << i << ": " << cg_v.at(i).size() << endl;
    // }

    vector<int> candidate_v;

    vector<vector<int> > all_candidate;

    for(auto cg : cg_v){
        vector<pair<int,int> > estimate_v;
        for(auto seq:cg){
            estimate_v.push_back(make_pair(estimate_function(seq, all_cs), seq));
        }

        sort(estimate_v.begin(), estimate_v.end());
        if(estimate_v.size()){
            candidate_v.push_back(estimate_v.at(0).second);
        }
    }

    vector<pair<int,int> > ans_v;
    vector<vector<int> > c2= comb(15, 2);
    for(auto candidate: candidate_v){
        vector<pair<int,int> > possible_ans_v;
        vector<int> degen_set = gen_degen(candidate, c2);
        for(auto d:degen_set){
            possible_ans_v.push_back(make_pair(estimate_function(d, all_cs), d));
        }
        sort(possible_ans_v.begin(), possible_ans_v.end());
        ans_v.push_back(make_pair(possible_ans_v[0].first,possible_ans_v[0].second));
        ans_v.push_back(make_pair(possible_ans_v[1].first,possible_ans_v[1].second));
        // cout << "ans0:" << int_to_seq(possible_ans_v[0].second) << ":" << possible_ans_v[0].first << endl;
        // cout << "ans1:" << int_to_seq(possible_ans_v[1].second) << ":" << possible_ans_v[1].first << endl;
        // cout << possible_ans_v[0].second << ":" << possible_ans_v[0].first << endl;
    }

    sort(ans_v.begin(), ans_v.end());

    for(int i=0; i<all_cs.size(); i++){
        int min = 16;
        vector<int> ans_seq;
        vector<int> ans_diff;
        vector<int> ans_pos;
        for(int j=0; j<all_cs[i].size(); j++){
            int diff = get_diff(all_cs[i][j], ans_v[0].second);
            if(diff<=7){
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

    cout << "best control sequences candidate:" << int_to_seq(ans_v[0].second) << endl;
    int show_ans_cnt = 0;
    while(ans_v[0].second==ans_v[show_ans_cnt++].second);
    cout << "second best control sequences candidate::" << int_to_seq(ans_v[show_ans_cnt].second) << endl;
    cout << "time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;


    return 0;

}

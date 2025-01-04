#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector> 
#include <iostream> 
#include <fstream> 
#include <cassert>
#include <cstdint>

using namespace std;

long prune (long secret_no) {
	const long  mod = (1 << 24)-1;
	assert ((secret_no & mod) >= 0);
	return secret_no & mod;
}

long mix (long secret_no, long value) {
	assert ((secret_no ^ value) >= 0);
	return secret_no ^ value;
}

long next_secret (long secret_no) {
	long val_to_mix1 = secret_no << 6;
	assert (val_to_mix1 >=0);
	secret_no = mix(secret_no,val_to_mix1);
	secret_no = prune(secret_no);

	long val_to_mix2 = secret_no >> 5;
	assert (val_to_mix2 >=0);
	secret_no = mix(secret_no,val_to_mix2);
	secret_no = prune(secret_no);

	long val_to_mix3 = secret_no << 11;
	assert (val_to_mix3 >=0);
	secret_no = mix(secret_no,val_to_mix3);
	secret_no = prune(secret_no);

	return secret_no;
}

pair<long,vector<vector<int8_t>>> parse_file(string filename) {
	long result=0;
	vector<vector<int8_t>> prices_vec;
	string line;
	ifstream file(filename);
	while (getline(file,line)) {
		long secret_no = stol(line);
		vector<int8_t> prices;
		int8_t price_start = secret_no%10;
		prices.push_back(price_start);
		for (int i=0;i<2000; ++i) {
			secret_no = next_secret(secret_no);
			int8_t price = secret_no%10;
			prices.push_back(price);
		}
		prices_vec.push_back(prices);
		result+=secret_no;
	}
	return {result,prices_vec};
}

int get_result_p2 (vector<vector<int8_t>> prices_ts) {
	unordered_map<string,int> profits; //(profit, sequence)
	for (auto prices:prices_ts) {
		string sequence = "";
		unordered_set<string> visited;
		for (int i=1;i<5;++i) sequence += to_string((prices[i]-prices[i-1]))+",";
		for (int i=5; i<prices.size();++i) {
			// how to handle keys which have already appeared?
			if (!visited.contains(sequence)) {
				int price_curr = prices[i-1];
				profits[sequence]+= price_curr;
				visited.insert(sequence);
			}
			sequence = sequence.substr(sequence.find_first_of(',')+1)+to_string((prices[i]-prices[i-1]))+",";
		}
	}
	auto max_it = max_element(profits.begin(), profits.end(),
			[](const auto& p1,const auto& p2){
				return p1.second < p2.second;
			});
	cout << "Max_sequence: " << max_it->first << " ,Value: " << max_it->second << '\n';
	return max_it->second;
}
//Note: 1923 is too low and 1929 is too high. Where am I going wrong?
int main () {
	long test = parse_file("d22t.txt").first;
	assert(test==37327623);
	long result1 = parse_file("d22.txt").first;
	vector<vector<int8_t>> p2_test = parse_file("d22tp2.txt").second;
	int test_p2 = get_result_p2(p2_test);
	assert(test_p2==23);
	vector<vector<int8_t>> p2_input = parse_file("d22.txt").second;
	int result_p2 = get_result_p2(p2_input);
	cout << "Result 1: " << result1 <<'\n';
	cout << "Result 2: " << result_p2 << '\n';
	return 0;
}


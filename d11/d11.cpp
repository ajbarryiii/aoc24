#include <iostream> 
#include <fstream> 
#include <sstream>
#include <string> 
#include <unordered_map>
#include <vector> 
#include <cassert>

using namespace std;

vector<string> parse_file(string filename) {
	ifstream file(filename);
	vector<string> result;
	if (!file.is_open()) {
		cerr << "Err: could not open file \n";
	}
	string line;
	while(getline(file,line)) {
		stringstream ss(line);
		string num;
		while(ss>>num) {
			result.push_back(num);
		}
	}
	return result;
}

vector<string> blink(vector<string> stones){
	vector<string> result; 
	for(int i = 0; i < stones.size(); ++i) {
		if(stones[i] == "0"){
			result.push_back("1");
		}
		else if (stones[i].size()%2==0) {
			// can the leading digit be a zero for the first half?
			result.push_back(stones[i].substr(0, stones[i].size()/2));
			string back = stones[i].substr(stones[i].size()/2);
			// strip leading zeros from back
			if(back.size()==1){
				;
			}
			else if  (back.find_first_not_of('0')!= string::npos){
				back = back.substr(back.find_first_not_of('0'));
			}
			else {
				back = "0";
			}
			result.push_back(back);
			
		}
		else {
			long num = stol(stones[i])*2024;
			assert(num>0);
			result.push_back(to_string(num));
		}
	}
	return result;
}
// memorization?
unordered_map<string, pair<long, vector<string>>> blinkv2( unordered_map<string, pair<long, vector<string>>> stones){
	unordered_map<string, pair<long, vector<string>>>  result;
	for(auto num: stones) {
		if (!num.second.second.empty()){
			result[num.first].first += 0;
			result[num.first].second = num.second.second;
			for(int i = 0; i<num.second.second.size();++i) {
				result[num.second.second[i]].first += num.second.first;
			}
		}
		else if(num.first == "0"){
			result["1"].first += num.second.first;
			result["0"].second = {"1"};
		}
		else if (num.first.size()%2==0) {
			// can the leading digit be a zero for the first half?
			string curr = num.first.substr(0, num.first.size()/2);
			string back = num.first.substr(num.first.size()/2);
			// strip leading zeros from back
			if(back.size()==1){
				;
			}
			else if  (back.find_first_not_of('0')!= string::npos){
				back = back.substr(back.find_first_not_of('0'));
			}
			else {
				back = "0";
			}
			result[num.first].second = {curr, back};
			result[curr].first += num.second.first;
			result[back].first += num.second.first;
		}
		else {
			long curr_num = stol(num.first)*2024;
			assert(curr_num>0);
			result[to_string(curr_num)].first += num.second.first;
			result[num.first].second = {to_string(curr_num)};
		}
	}
	return result;
}

int get_result(const int blinks, vector<string> stones) {
	for (int i = 0; i< blinks; ++i){
		stones = blink(stones);
		//cout << "final result: ";
		//for (auto i : stones) cout << i<< " ";
		//cout << '\n';
		//cout << "Iteration: " << i << ", Size: " << stones.size() << '\n';
	}
	return stones.size();
}

unordered_map<string, pair<long, vector<string>>> get_result_p2(const int blinks, vector<string> stones) {
	unordered_map<string, pair<long, vector<string>>>  result;
	for (int i = 0; i<stones.size(); ++i) {
		result[stones[i]].first+=1;
	}
	for (int i = 0; i< blinks; ++i){
		result = blinkv2(result);
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename += ".txt";
	vector<string> stones = parse_file(filename);
	cout << "Starting stones: ";
	for (auto i : stones) cout << i << " ";
	cout << '\n';
	int result1 = get_result(25, stones);
	unordered_map<string, pair<long, vector<string>>> result2_map = get_result_p2(75, stones);
	long result2 = 0;
	for(auto i: result2_map){
		assert(i.second.first >= 0);
		result2 += i.second.first;
		assert(result2>=0);
		//cout << i.first << " , " << i.second.first <<'\n';
		//cout << "mapping: ";
		//for (auto j : i.second.second) cout << j << " ";
		//cout << '\n';
	}
	cout << '\n';
	cout << "Result 1: " << result1 << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}


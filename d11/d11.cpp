#include <iostream> 
#include <fstream> 
#include <sstream>
#include <string> 
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
int get_result(const int blinks, vector<string> stones) {
	for (int i = 0; i< blinks; ++i){
		stones = blink(stones);
		//cout << "final result: ";
		//for (auto i : stones) cout << i<< " ";
		//cout << '\n';
		cout << "Iteration: " << i << ", Size: " << stones.size() << '\n';
	}
	return stones.size();
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
	int result1 = get_result(75, stones);
	//int result2 = get_result2(file);
	cout << "Result 1: " << result1 << '\n';
	//cout << "Result 2: " << result2 << '\n';
	return 0;
}


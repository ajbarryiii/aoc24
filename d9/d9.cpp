#include <iostream> 
#include <fstream>
#include <string> 
#include <vector>
#include <cassert>
#include <deque>
#include <cstdint>

using namespace std;

pair<deque<pair<int,int>>, vector<int>> parse_file(string filename) {
	ifstream file(filename);

	deque<pair<int,int>> files_lens; // file_no, len
	vector<int> free_spaces;

	assert(file.is_open());
	
	string line;
	getline(file, line);
	for (int i = 0; i < line.size(); ++i) {
		if (i%2 == 0) files_lens.push_back(pair<int,int> (i/2,static_cast<int>(line[i])));
		else free_spaces.push_back(static_cast<int>(line[i]));
	}
	return pair<deque<pair<int,int>>,vector<int>>(files_lens, free_spaces);
}

vector<int> simulate (pair<deque<pair<int,int>>,vector<int>> input) {
	vector<int> result;
	deque<pair<int,int>> files = input.first;
	vector<int> free_spaces = input.second;
	int i ;
	while (!files.empty() && i< free_spaces.size()) {
		pair<int,int> curr_file = files.front();
		files.pop_front();
		vector<int> curr_file_vec(curr_file.second, curr_file.first);
		result.insert(result.end(),curr_file_vec.begin(), curr_file_vec.end()); 
		int j = 0;
		while (j < free_spaces[i] && !files.empty()) {
			if ( files.back().second > 0) {
				result.push_back(files.back().first);
				--files.back().second;
				++j;
			}
			else {
				files.pop_back();
			}
		}
		++i;
	}
	return result;
}

uint64_t get_result(vector<int> output) {
	uint64_t result = 0;
	for(uint64_t i = 0; i < output.size(); ++i) {
		result += output[i]*i;
		assert(result>0);
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename += ".txt";
	pair<deque<pair<int,int>>,vector<int>> file = parse_file(filename);
	vector<int> sim_output = simulate(file);
	uint64_t result1 = get_result(sim_output);
	cout << "Result 1: " << result1 << '\n';
	return 0;
}

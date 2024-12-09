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
		int char_val = line[i] - '0';
		if (i%2 == 0) {
			files_lens.push_back(pair<int,int> (i/2,char_val));
			assert(char_val < 10);
		}
		else {
			assert(char_val < 10);
			free_spaces.push_back(char_val);
		}
	}
	return pair<deque<pair<int,int>>,vector<int>>(files_lens, free_spaces);
}

deque<pair<int,int>> parse_file_p2(string filename) {
	ifstream file(filename);
	deque<pair<int,int>> spaces; //file_no (with -1 indicating free space) , len
	assert(file.is_open());
	string line;
	getline(file, line);
	int pos = 0;
	for (int i = 0; i < line.size(); ++i) {
		int char_val = line[i] - '0';
		if (i%2 == 0) {
			spaces.push_back(pair<int,int>( i/2,  char_val));
			assert(char_val < 10);
		}
		else {
			assert(char_val < 10);
			spaces.push_back(pair<int,int>( -1 ,  char_val));
		}
		pos += char_val;
	}
	return spaces;
}

vector<int> simulate (pair<deque<pair<int,int>>,vector<int>> input) {
	vector<int> result;
	deque<pair<int,int>> files = input.first;
	vector<int> free_spaces = input.second;
	int i = 0;
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

vector<int> simulate_p2 (deque<pair<int,int>> files) {
	vector<int> result;
	while (!files.empty()) {
		pair<int,int> curr_file = files.front();
		files.pop_front();
		if (curr_file.first != -1) {
			vector<int> curr_file_vec(curr_file.second, curr_file.first);
			result.insert(result.end(),curr_file_vec.begin(), curr_file_vec.end()); 
		}
		else {
			int j = curr_file.second;
			while (j > 0) {
				int back_idx = files.size()-1;
				while ( back_idx >= 0 ) {
					if (files[back_idx].first >= 0 && files[back_idx].second <= j) break;
					--back_idx;
				}
				if (back_idx >=0) {
					vector<int> file_to_insert(files[back_idx].second, files[back_idx].first);
					//cout << "moving the following vec: ";
					//for (int i = 0; i < file_to_insert.size(); ++i) cout << file_to_insert[i];
					//cout << '\n';
					result.insert(result.end(),file_to_insert.begin(), file_to_insert.end()); 
					files[back_idx].first = -1;
					j-= file_to_insert.size();
					//cout << "remaining j " << j << '\n';
				}
				else {
					//cout << "moving the following vec: ";
					for (int k = 0; k <= j; ++k){
						//cout << 0;
						result.push_back(0);
						--j;
					}
					//cout << '\n';
				}
			}
		}
	}
	return result;
}

uint64_t get_result(vector<int> output) {
	uint64_t result = 0;
	for(uint64_t i = 0; i < output.size(); ++i) {
		result += output[i]*i;
		assert(result>=0);
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
	deque<pair<int,int>> file_p2 = parse_file_p2(filename);
	vector<int> sim_output_2 = simulate_p2(file_p2);
	//cout << "result 2: ";
	//for (int i = 0; i < sim_output_2.size(); ++i) cout << sim_output_2[i];
	//cout << '\n';
	
	uint64_t result2 = get_result(sim_output_2);
	cout << "Result 1: " << result1 << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}

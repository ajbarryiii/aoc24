#include <functional>
#include <string>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream> 
#include <algorithm>
#include <cassert>

using namespace std;

//what if we hardcode the shortest way to get to each character from each other character?;
//will sorted substrings always have the shortest size in level of abstraction n+1?
unordered_map<string,unordered_set<string>> dirpad_next() {
	// get the shortest sequence of presses that one can have using a dirpad to get to the next dir on another dirpad
	unordered_map<string,unordered_set<string>> next;
	unordered_map<char,pair<int,int>> dirs = {{'<',{0,-1}},{'>',{0,1}},{'v',{1,0}},{'^',{-1,0}}};
	unordered_map<char,pair<int,int>> char_positions = {{'^',{0,1}},{'A',{0,2}},{'<',{1,0}},{'v',{1,1}},{'>',{1,2}}};
	vector<vector<char>> pad = {{'-','^','A'},{'<','v','>'}};
	for (auto chars: char_positions) {
		deque<pair<pair<int,int>,string>> q;
		q.push_back({chars.second,""});
		string itself(2,chars.first);
		next[itself].insert("A");
		while (!q.empty()) {
			pair<int,int> curr = q.front().first;
			string curr_sequence = q.front().second;
			q.pop_front();
			for (auto dir:dirs) {
				int next_y = curr.first+dir.second.first, next_x = curr.second+dir.second.second;
				if (0<=next_y && next_y<pad.size() && 0<=next_x && next_x<pad[0].size()) {
					if (pad[next_y][next_x]!='-') {
						string s = "";
						s+=chars.first;
						s+=pad[next_y][next_x];
						string seq = curr_sequence+dir.first;
						if (next.contains(s)) {
							if (seq.size()<=(*next[s].begin()).size()-1){
								next[s].insert(seq+'A');
								q.push_back({{next_y,next_x}, seq});
							}
						}
						else {
							next[s].insert(seq+'A');
							q.push_back({{next_y,next_x}, seq});
						}
					}
				}
			}
		}
	}
	return next;
}

unordered_map<string,unordered_set<string>> numpad_next() {
	// get the shortest sequence of presses that one can have using a dirpad to get to the next dir on another dirpad
	unordered_map<string,unordered_set<string>> next;
	unordered_map<char,pair<int,int>> dirs = {{'v',{1,0}},{'^',{-1,0}},{'<',{0,-1}},{'>',{0,1}}};
	unordered_map<char,pair<int,int>> char_positions;
	vector<vector<char>> pad = {{'7','8','9'},{'4','5','6'},{'1','2','3'},{'-','0','A'}};
	//init char_positions;
	for (int i=0; i<pad.size();++i) {
		for (int j=0; j<pad[0].size();++j) {
			if (pad[i][j]!='-') {
				char_positions[pad[i][j]] = {i,j};
			}
		}
	}
	for (auto chars: char_positions) {
		deque<pair<pair<int,int>,string>> q;
		q.push_back({chars.second,""});
		string itself(2,chars.first);
		next[itself].insert("A");
		while (!q.empty()) {
			pair<int,int> curr = q.front().first;
			string curr_sequence = q.front().second;
			q.pop_front();
			for (auto dir:dirs) {
				int next_y = curr.first+dir.second.first, next_x = curr.second+dir.second.second;
				if (0<=next_y && next_y<pad.size() && 0<=next_x && next_x<pad[0].size()) {
					if (pad[next_y][next_x]!='-') {
						string s = "";
						s+=chars.first;
						s+=pad[next_y][next_x];
						string seq = curr_sequence+dir.first;
						if (next.contains(s)) {
							if (seq.size()<= (*next[s].begin()).size()-1) {
								next[s].insert(seq+'A');
								q.push_back({{next_y,next_x}, seq});
							}
						}
						else {
							next[s].insert(seq+'A');
							q.push_back({{next_y,next_x}, seq});
						}
					}
				}
			}
		}
	}
	return next;
}

//TODO: for each code, iterate through to find the shortest code which is possible
//for part 2 we can prune these down to the codes which output the shortest sequence?
long get_result (vector<string> codes) {
	long result=0;
	unordered_map<string,unordered_set<string>> dirs_map = dirpad_next(), nums_map = numpad_next();
	for (auto code:codes) {
		long numeric_part = stol(code.substr(0,code.size()-1));
		code = "A"+code;
		// num -> dir
		unordered_set<string> code_i_sequences;
		//init code_i_sequences
		for (auto out_sequence:nums_map[code.substr(0,2)]) {
			code_i_sequences.insert(out_sequence);
		}
		for (int i=1;i<code.size()-1;++i) {
			string curr_substr = code.substr(i,2);
			auto it = code_i_sequences.begin();
			unordered_set<string> out_sequences;
			for (auto partial_sequence:code_i_sequences) {
				for (auto out_sequence:nums_map[curr_substr]){
					out_sequences.insert(partial_sequence+out_sequence);
				}
			}
			code_i_sequences = out_sequences;
		}
		cout << "door->dir complete," << code_i_sequences.size()<<"\n";
		// dir->dir
		unordered_set<string> dir_to_dir;
		//init door_to_dir
		for (auto door_to_dir:code_i_sequences) {
			string curr_sequence = "A";
			curr_sequence+=door_to_dir[0];
			for (auto next: dirs_map[curr_sequence]) {
				dir_to_dir.insert(next);
			}
		}
		for (auto d_to_d: code_i_sequences) {
			for (int i=1;i<d_to_d.size()-1;++i) {
				string curr_substr = d_to_d.substr(i,2);
				unordered_set<string> out_sequences;
				for (auto partial_sequence:dir_to_dir) {
					for (auto out_sequence:dirs_map[curr_substr]){
						out_sequences.insert(partial_sequence+out_sequence);
					}
				}
				dir_to_dir = out_sequences;
			}
		}
		code_i_sequences = dir_to_dir;
		cout << "dir->dir complete," << code_i_sequences.size()<<"\n";
		dir_to_dir.clear();
		for (auto door_to_dir:code_i_sequences) {
			string curr_sequence = "A";
			curr_sequence+=door_to_dir[0];
			for (auto next: dirs_map[curr_sequence]) {
				dir_to_dir.insert(next);
			}
		}
		for (auto d_to_d: code_i_sequences) {
			for (int i=1;i<d_to_d.size()-1;++i) {
				string curr_substr = d_to_d.substr(i,2);
				unordered_set<string> out_sequences;
				for (auto partial_sequence:dir_to_dir) {
					for (auto out_sequence:dirs_map[curr_substr]){
						out_sequences.insert(partial_sequence+out_sequence);
					}
				}
				dir_to_dir = out_sequences;
			}
		}
		code_i_sequences = dir_to_dir;
		cout << "dir->human complete," << code_i_sequences.size()<<"\n";
		long min_len_sequence_size = 10000000;
		string min_len_sequence = "";
		for (auto i:code_i_sequences) {
			if (i.size()<min_len_sequence_size) {
				min_len_sequence = i;
				min_len_sequence_size = i.size();
			}
		}
		cout << min_len_sequence << ": size: " << min_len_sequence.size() << '\n';
		result+=(min_len_sequence_size*numeric_part);
	}
	return result;
}

vector<string> parse_file (string filename) {
	ifstream file(filename);
	vector<string> out;
	assert(file.is_open());
	string line;
	while (getline(file,line)) {
		out.push_back(line);
	}
	return out;
}

int main () {
	string filename;
	cout << "Enter the filename:\n";
	cin >> filename;
	filename += ".txt";
	vector<string> test = parse_file(filename);
	long result1 = get_result(test);
	cout << "Result 1: " << result1 << "\n";
	return 0;
}

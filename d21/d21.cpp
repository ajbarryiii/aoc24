#include <string>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include <fstream> 
#include <cassert>

using namespace std;

enum class PadType {
	NUMPAD,
	DIRPAD
};

struct Pad {
	PadType pad_t; 
	vector<vector<char>> pad;
	int y ;
	int x ;
	Pad *child;
	string history;
	Pad(PadType type, Pad *child_pad): pad_t(type), child(nullptr) {
		if (pad_t == PadType::NUMPAD) {
			pad = {{'7','8','9'},{'4','5','6'},{'1','2','3'},{'-','0','A'}};
			y = 3;
			x = 2;
		}
		else if (pad_t == PadType::DIRPAD) {
			pad = {{'-','^','A'},{'<','v','>'}};
			y = 0;
			x = 2;
		}
	}
	Pad(PadType type, Pad *child_pad, int y, int x, string history): pad_t(type), child(nullptr), y(y), x(x), history(history) {
		if (pad_t == PadType::NUMPAD) {
			pad = {{'7','8','9'},{'4','5','6'},{'1','2','3'},{'-','0','A'}};
		}
		else if (pad_t == PadType::DIRPAD) {
			pad = {{'-','^','A'},{'<','v','>'}};
		}
	}
	bool valid_pos(int y_,int x_) {
		bool y_valid = (0<=y_ && y_<pad.size());
		bool x_valid = (0<=x_ && x_<pad[0].size());
		return ((pad[y_][x_]!= '-') && (y_valid && x_valid));
	}
};

//what if we hardcode the shortest way to get to each character from each other character?;
//note we probably dont actually need the history;
unordered_map<string,string> dirpad_next() {
	// get the shortest sequence of presses that one can have using a dirpad to get to the next dir on another dirpad
	unordered_map<string,string> next;
	unordered_map<char,pair<int,int>> dirs = {{'v',{1,0}},{'^',{-1,0}},{'<',{0,-1}},{'>',{0,1}}};
	unordered_map<char,pair<int,int>> char_positions = {{'^',{0,1}},{'A',{0,2}},{'<',{1,0}},{'v',{1,1}},{'>',{1,2}}};
	vector<vector<char>> pad = {{'-','^','A'},{'<','v','>'}};
	for (auto chars: char_positions) {
		deque<pair<pair<int,int>,string>> q;
		q.push_back({chars.second,""});
		string itself(2,chars.first);
		next[itself] = "A";
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
						if (!next.contains((s))) {
							next[s]=(curr_sequence+dir.first+'A');
							q.push_back({{next_y,next_x}, curr_sequence+dir.first});
						}
					}
				}
			}
		}
	}
	return next;
}

unordered_map<string,string> numpad_next() {
	// get the shortest sequence of presses that one can have using a dirpad to get to the next dir on another dirpad
	unordered_map<string,string> next;
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
		next[itself] = "A";
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
						if (!next.contains((s))) {
							next[s]=(curr_sequence+dir.first+'A');
							q.push_back({{next_y,next_x}, curr_sequence+dir.first});
						}
					}
				}
			}
		}
	}
	return next;
}

long get_result (vector<string> codes) {
	long result=0;
	unordered_map<string,string> dirs_map = dirpad_next(), nums_map = numpad_next();
	for (auto key:dirs_map) {
		cout << "Target: " << key.first[1] << " Curr: " << key.first[0] << " sequence: \"" << key.second << "\"\n";
	}

	cout << "sequence for \'Av<<\': \""<<dirs_map["Av"] + dirs_map["v<"] + dirs_map[">>"]<<"\"\n";
	cout << "sequence for \'A<v<\': \""<<dirs_map["A<"] + dirs_map["<v"] + dirs_map["v>"] <<"\"\n";


	for (auto code:codes) {
		long numeric_part = stol(code.substr(0,code.size()-1));
		long num_moves = 0;
		code = "A"+code;
		// dirpad->numpad (ie robot1)
		string l2;
		for (int i=0;i<code.size()-1;++i){
			string num_substr = code.substr(i,2);
			string num_to_dir = nums_map[num_substr];
			l2+=nums_map[num_substr];
		}
		cout << "size of l2: " << l2.size() << " L2: \"" << l2<< "\"\n";
		l2 = "A"+l2;
		// dirpad->dirpad (ie robot2->robot1)
		string l3;
		for (int i=0;i<l2.size()-1;++i){
			string num_substr = l2.substr(i,2);
			string num_to_dir = dirs_map[num_substr];
			l3+=dirs_map[num_substr];
		}
		cout << "size of l3: " << l3.size() << " L3: \"" << l3<< "\"\n";
		l3 = "A"+l3;
		// you->robot2
		string l4;
		for (int i=0;i<l3.size()-1;++i){
			string num_substr = l3.substr(i,2);
			string num_to_dir = dirs_map[num_substr];
			l4+=dirs_map[num_substr];
		}
		cout << "size of l4: " << l4.size() << " L4: \"" << l4<< "\"\n";
		result += numeric_part*num_moves;
		cout << "Numeric part: "<< numeric_part << ", num moves: " << num_moves << '\n';
	}
	return result;
}

void parse_file (string filename) {
	ifstream file(filename);
	assert(file.is_open());
}

int main () {
	vector<string> test = {"029A"};
	long result1 = get_result(test);
	cout << "Result 1: " << result1 << "\n";
	return 0;
}

#include <functional>
#include <array>
#include <istream>
#include <sstream>
#include <string>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include <fstream> 
#include <algorithm>
#include <cassert>

using namespace std;

void sort_chars (string &s, array<int,4> priority_vec) {
	unordered_map<char,int> priority = {{'<',priority_vec[0]},{'^',priority_vec[1]}, {'v',priority_vec[2]},{'>',priority_vec[3]}};
	sort(s.begin(),s.end(), 
			[&priority](char c1, char c2) {
				return priority[c1]<priority[c2];
			});
}

//what if we hardcode the shortest way to get to each character from each other character?;
//note we probably dont actually need the history;
unordered_map<string,string> dirpad_next(array<int,4> priority_vec) {
	// get the shortest sequence of presses that one can have using a dirpad to get to the next dir on another dirpad
	unordered_map<string,string> next;
	unordered_map<char,pair<int,int>> dirs = {{'<',{0,-1}},{'>',{0,1}},{'v',{1,0}},{'^',{-1,0}}};
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
							string seq = curr_sequence+dir.first;
							if ((pad[next_y][next_x]=='A'||pad[next_y][next_x]=='^') && (chars.first=='<')) {
								sort(seq.begin(),seq.end());
							}
							else if ((chars.first=='A'||chars.first)&&(pad[next_y][next_x]=='<')){
								sort(seq.rbegin(),seq.rend());
							}
							else {
								sort_chars(seq, priority_vec);
							}
							next[s]=(seq+'A');
							q.push_back({{next_y,next_x}, seq});
						}
					}
				}
			}
		}
	}
	return next;
}

unordered_map<string,string> numpad_next(array<int,4> priority_vec) {
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
							string seq = curr_sequence+dir.first;
							if ((pad[next_y][next_x]=='A'||pad[next_y][next_x]=='0')&&(chars.first=='1'||chars.first=='4'||chars.first=='7')) {
								sort(seq.begin(),seq.end());
							}
							else if ((pad[next_y][next_x]=='1'||pad[next_y][next_x]=='4'||pad[next_y][next_x]=='7')&&(chars.first=='A'||chars.first=='0')) {
								sort(seq.rbegin(),seq.rend());
							}
							else {
								sort_chars(seq,  priority_vec);
							}
							next[s]=(seq+'A');
							q.push_back({{next_y,next_x}, seq});
						}
					}
				}
			}
		}
	}
	return next;
}

long get_result (vector<string> codes, vector<array<int,4>> priority_vec) {
	long result=0;
	unordered_map<string,string> dirs_map = dirpad_next(priority_vec[0]), nums_map = numpad_next(priority_vec[1]);
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
		l2 = "A"+l2;
		// dirpad->dirpad (ie robot2->robot1)
		string l3;
		for (int i=0;i<l2.size()-1;++i){
			string num_substr = l2.substr(i,2);
			string num_to_dir = dirs_map[num_substr];
			l3+=dirs_map[num_substr];
		}
		//cout << "L3: \"" << l3 << "\"\n";
		l3 = "A"+l3;
		// you->robot2
		string l4;
		for (int i=0;i<l3.size()-1;++i){
			string num_substr = l3.substr(i,2);
			string num_to_dir = dirs_map[num_substr];
			l4+=dirs_map[num_substr];
		}
		//cout << "L4: \"" << l4 << "\"\n";
		//cout << "Numeric part: " << numeric_part << " , Size: " << l4.size() << '\n';
		result += numeric_part*l4.size();
	}
	return result;
}

struct pair_hash {
    template <class T1, class T2>
    size_t operator () (const pair<T1,T2>& p) const {
        auto h1 = hash<T1>{}(p.first);
        auto h2 = hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

string process_depth_first(
    const string& input,
    int depth,
    const unordered_map<string, string>& dirs_map,
    unordered_map<pair<string, int>, string, pair_hash>& memo
) {
    // Base case: no more transformation needed
    if (depth == 0) {
        return input;
    }

    // Check the memo first
    pair<string, int> key = {input, depth};
    if (memo.contains(key)) {
        return memo[key];
    }

    // Build the new transformed string
    ostringstream oss;
    string augmented = "A" + input;

    // Transform each 2-character substring recursively
    for (size_t i = 0; i < augmented.size() - 1; ++i) {
        // Safe if guaranteed to be in dirs_map
        const string& curr_substr = augmented.substr(i, 2);

        // Get the mapped value and recurse
        const string& mapped_val = dirs_map.at(curr_substr);
        string transformed = process_depth_first(mapped_val, depth - 1, dirs_map, memo);
        oss << transformed;
    }

    // Store in memo before returning
    string result = oss.str();
    memo[key] = result;
    return result;
}

long get_result_p2 (vector<string> codes,int num_robots, vector<array<int,4>> priority_vec) {
	long result=0;
	unordered_map<string,string> dirs_map = dirpad_next(priority_vec[0]), nums_map = numpad_next(priority_vec[1]);
	for (auto code:codes) {
		long numeric_part = stol(code.substr(0,code.size()-1));
		long num_moves = 0;
		code = "A"+code;
		// numpad->dirpad (ie robot1)
		string l2;
		for (int i=0;i<code.size()-1;++i){
			string num_substr = code.substr(i,2);
			string num_to_dir = nums_map[num_substr];
			l2+=nums_map[num_substr];
		}
		unordered_map<pair<string, int>, string, pair_hash> memo;
		string final_result = process_depth_first(l2, num_robots , dirs_map, memo);
		cout << final_result.size()<<'\n';
		result += numeric_part * final_result.size();
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

vector<array<int,4>> generate_ordering () {
	vector<array<int,4>> result;
	array<int,4> p = {0,1,2,3};
	do {
		result.push_back(p);
	} while (next_permutation(p.begin(),p.end()));
	return result;
}

vector<vector<array<int,4>>> valid_orderings(vector<string> test) {
	vector<array<int,4>> orderings = generate_ordering();
	vector<vector<array<int,4>>> valid_orders;
	for (int i=0;i<orderings.size();++i) {
		for (int j=0;j<orderings.size();++j) {
			vector<array<int,4>> valid_ordering = {orderings[i],orderings[j]};
			if (get_result(test, valid_ordering)==126384) {
				valid_orders.push_back(valid_ordering);
			}
		}
	}
	valid_orders.size();
	return valid_orders;
}

int main () {
	string filename;
	cout << "Enter the filename:\n";
	cin >> filename;
	filename += ".txt";
	vector<string> test = parse_file(filename);
	vector<vector<array<int,4>>> valid_orders = valid_orderings(test);
	vector<array<int,4>> ordering = valid_orders[0];
	vector<string> p1 = parse_file("d21.txt");
	long result2 = get_result_p2(p1,25, ordering);
	cout << "result p2: " << result2;
	return 0;
}

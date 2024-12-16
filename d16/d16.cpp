#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <queue> 
#include <cassert>

using namespace std;

struct pair_hash {
   size_t operator()(const pair<int,int>& p) const {
       return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
   }
};

struct Compare {
	bool operator()(const pair<int,pair<int,int>>& a, const pair<int,pair<int,int>>& b) {
		return a.first>b.first;
	}
};

pair<pair<vector<vector<char>>,unordered_map<pair<int,int>,int, pair_hash>>, pair<pair<int,int>,pair<int,int>>> parse_file ( string filename ){
	vector<vector<char>> map;
	pair<int,int> start_pos;
	pair<int,int> end_pos;
	unordered_map<pair<int,int>,int, pair_hash> distances;
	string line;
	ifstream file(filename);
	int line_no = 0; 
	int line_len;
	while (getline(file, line)) {
		if (line_no == 0) {
			line_len = line.size();
		}
		else {
			assert(line.size() == line_len);
		}
		vector<char> curr_line;
		for(int i = 0; i<line.size(); ++i) {
			curr_line.push_back(line[i]);
			if (line[i] == 'S') {
				start_pos = {line_no,i};
			}
			else if (line[i] == 'E') {
				end_pos = {line_no,i};
			}
			if (line[i] != '#'){
				distances[{line_no,i}] = INT_MAX;
			}
		}
		map.push_back(curr_line);
		++line_no;
	}

	return {{map,distances}, {start_pos,end_pos}};
}

int maze_score (pair<pair<vector<vector<char>>,unordered_map<pair<int,int>,int, pair_hash>>, pair<pair<int,int>,pair<int,int>>> input){
	const vector<vector<char>> map = input.first.first;
	unordered_map<pair<int,int>,int, pair_hash> distances = input.first.second;
	unordered_set<pair<int,int>, pair_hash> visited;
	pair<int,int> start_pos = input.second.first;
	pair<int,int> end_pos = input.second.second;
	priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>, Compare> heap;
	heap.push({0,start_pos});
	vector<int> dir = {-1,0};

	// use rotation matrices for computation of next;
}

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
	bool operator()(const pair<int,pair<pair<int,int>,vector<int>>>& a, const pair<int,pair<pair<int,int>,vector<int>>>& b) {
		return a.first>b.first;
	}
};

pair<pair<vector<vector<char>>,unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash>>, pair<pair<int,int>,pair<int,int>>> parse_file ( string filename ){
	vector<vector<char>> map;
	pair<int,int> start_pos;
	pair<int,int> end_pos;
	unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash> distances;
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
				distances[{line_no,i}] = {INT_MAX,{0,0}};
			}
		}
		map.push_back(curr_line);
		++line_no;
	}

	return {{map,distances}, {start_pos,end_pos}};
}

int maze_score (pair<pair<vector<vector<char>>,unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash>>, pair<pair<int,int>,pair<int,int>>> input){
	const vector<vector<char>> map = input.first.first;
	// point: {distance, dir} -> need the dir so that we can find cost of next move
	unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash> distances = input.first.second;
	unordered_set<pair<int,int>, pair_hash> visited;
	pair<int,int> start_pos = input.second.first;
	pair<int,int> end_pos = input.second.second;
	// do we need the dir in the heap?
	priority_queue<pair<int,pair<pair<int,int>,vector<int>>>,vector<pair<int,pair<pair<int,int>,vector<int>>>>, Compare> heap;
	heap.push({0,{start_pos,{0,1}}});
	distances[start_pos] = {0,{0,1}};
	// use rotation matrices for computation of next;
	while (!heap.empty()) { 
		pair<int,pair<pair<int,int>,vector<int>>> curr = heap.top();
		int dist = curr.first;
		vector<int> dir = curr.second.second;
		pair<int,int> pos = curr.second.first;
		heap.pop();
		if (!visited.contains(pos)) {
			visited.insert(pos);
		}
		// 90 deg rotations;
		vector<int> left90 = vector<int> {-1*dir[1], dir[0]};
		vector<int> right90 = vector<int> {dir[1], -1*dir[0]}; 
		if (map[pos.first+dir[0]][pos.second+dir[1]] != '#'){
			if ( distances[{pos.first+dir[0],pos.second+dir[1]}].first > dist+1) { // should this line be >=?
				distances[{pos.first+dir[0],pos.second+dir[1]}] = {dist+1, dir};
				heap.push({dist+1, {{pos.first+dir[0],pos.second+dir[1]}, dir}});
			}
		}
		if (map[pos.first+left90[0]][pos.second+left90[1]] != '#'){
			if ( distances[{pos.first+left90[0],pos.second+left90[1]}].first > dist+1001) { // should this line be >=?
				distances[{pos.first+left90[0],pos.second+left90[1]}] = {dist+1001, left90};
				heap.push({dist+1001, {{pos.first+left90[0],pos.second+left90[1]}, left90}});
			}
		}
		if (map[pos.first+right90[0]][pos.second+right90[1]] != '#'){
			if ( distances[{pos.first+right90[0],pos.second+right90[1]}].first > dist+1001) { // should this line be >=?
				distances[{pos.first+right90[0],pos.second+right90[1]}] = {dist+1001, right90};
				heap.push({dist+1001, {{pos.first+right90[0],pos.second+right90[1]}, right90}});
			}
		}
	}
	return distances[end_pos].first;
}

int main() {
	string filename;
	cout << "input the filename:";
	cin >> filename;
	filename += ".txt";
	pair<pair<vector<vector<char>>,unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash>>, pair<pair<int,int>,pair<int,int>>> input = parse_file(filename);
	int score1 = maze_score(input);
	cout << "result 1: " << score1 << '\n';
}

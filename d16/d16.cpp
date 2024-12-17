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

int maze_score_p2 (pair<pair<vector<vector<char>>,unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash>>, pair<pair<int,int>,pair<int,int>>> input){
	const vector<vector<char>> map = input.first.first;
	// point: {distance, dir} -> need the dir so that we can find cost of next move
	unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash> distances = input.first.second;
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
	// is this a problem with the way I calculated the distances?;
	vector<vector<int>> directions = {{-1,0}, {1,0},{0,-1}, {0,1}};
	unordered_set<pair<int,int>, pair_hash> optimal_path;
	vector<pair<int,int>> q;
	optimal_path.insert(end_pos);
	q.push_back(end_pos);
	// code is not properly handling the case where we reach a turning (branch) point. what if we add a direction component, check 
	while(!q.empty()){
		pair<int,int> curr = q.back();
		int curr_dist = distances[curr].first;
		//cout << "current: (" << curr.first << ',' << curr.second <<"), curr_dist = " << curr_dist << '\n';
		q.pop_back();
		for (int i = 0; i < directions.size(); ++i){
			pair<int,int> next = {curr.first+directions[i][0], curr.second+directions[i][1]};
			if (distances.contains(next)) {
				//cout << "next: (" << next.first << ',' << next.second <<"), dist = " << distances[next].first << ", direction (og): " << distances[next].second[0] << ',' << distances[next].second[1]<< '\n';
				if (distances[next].first < curr_dist) {
					q.push_back(next);
					optimal_path.insert(next);
				}
				if (distances[curr].second != distances[next].second ) {
					//cout << "case you are working on\n";
					if (distances.contains({next.first-distances[curr].second[0],next.second-distances[curr].second[1]})) {
						if (distances[{next.first-distances[curr].second[0],next.second-distances[curr].second[1]}].first == curr_dist-2) {
							q.push_back({next.first-distances[curr].second[0],next.second-distances[curr].second[1]});
							optimal_path.insert({next.first-distances[curr].second[0],next.second-distances[curr].second[1]});
						}
					}
				}
			}
		}
		//cout <<'\n';
	}
	//for (int i = 0; i < map.size(); ++i) {
	//	for (int j = 0; j<map[0].size(); ++j) {
	//		if (optimal_path.contains({i,j})){
	//			cout <<'O';
	//		}
	//		else {
	//			cout << map[i][j];
	//		}
	//	}
	//	cout <<' '<< i <<'\n';
	//}
	return optimal_path.size();
}

int main() {
	
	string filename;
	cout << "input the filename:";
	cin >> filename;
	filename += ".txt";
	pair<pair<vector<vector<char>>,unordered_map<pair<int,int>,pair<int,vector<int>>, pair_hash>>, pair<pair<int,int>,pair<int,int>>> input = parse_file(filename);
	int score1 = maze_score(input);
	int score2 = maze_score_p2(input);
	// difficulty of p2 is that there can be multiple best paths through the maze, what happens if we just assume otherwise?
	cout << "result 1: " << score1 << '\n';
	cout << "result 2: " << score2 << '\n';
}

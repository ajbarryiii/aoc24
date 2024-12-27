#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <cassert>
#include <string>
#include <utility>
#include <functional>

using namespace std;

pair<vector<vector<bool>>,pair<pair<int,int>,pair<int,int>>> parse_file(string filename) {
	vector<vector<bool>> map;
	pair<int,int> start, end;
	ifstream file(filename);
	string line;
	int line_no=0;
	int line_size;
	assert(file.is_open());
	while (getline(file,line)) {
		if (line_no==0) {
			line_size=line.size();
		}
		else  {
			if (line_size!=line.size()) {
				cout << "line " << line_no << " has the wrong size\n";
				assert(line_size==line.size());
			}
		}
		vector<bool> line_vec;
		for (int i=0; i<line.size();++i) {
			if (line[i]=='#') {
				line_vec.push_back(false);
			}
			else {
				line_vec.push_back(true);
				if (line[i]=='S') {
					start = {line_no,i};
				}
				else if (line[i] =='E') {
					end = {line_no,i};
				}
			}
		}
		line_no++;
		map.push_back(line_vec);
	}
	return {map,{start,end}};
}

template <typename T, typename U>
struct pair_hash {
	size_t operator()(const pair<T,U>& p) const {
		return hash<T>()(p.first)^hash<U>()(p.second);
	}
};

unordered_map<int,int> bfs(pair<vector<vector<bool>>,pair<pair<int,int>,pair<int,int>>> input){
	const vector<vector<bool>> map = input.first;
	unordered_map<pair<int,int>,int,pair_hash<int,int>> times; 
	unordered_map<int,int> cheat_time_counter;
	deque<pair<int,int>> q;
	q.push_back(input.second.first);
	vector<pair<int,int>> dirs {{-1,0},{0,-1},{1,0},{0,1}};
	vector<pair<int,int>> cheat_dirs {{-2,0},{-1,1},{-1,-1},{1,-1},{1,1},{2,0},{0,-2},{0,2}};
	int t = 0;
	while (!q.empty()) {
		int q_size = q.size();
		for (int i=0; i<q_size;++i) {
			pair<int,int> curr = q.front();
			q.pop_front();
			times[curr] = t;
			//identify possible cheats
			for (int dir=0; dir<cheat_dirs.size();++dir){
				int next_y = curr.first+cheat_dirs[dir].first, next_x = curr.second+cheat_dirs[dir].second;
				if (times.contains({next_y,next_x})) {
					if (times[{next_y,next_x}] < t-2) {
						cheat_time_counter[t-2-times[{next_y,next_x}]]++;
					}
				}
			}
			//append to the queue
			for (int dir=0; dir<dirs.size();++dir){
				int next_y = curr.first+dirs[dir].first, next_x = curr.second+dirs[dir].second;
				if (map[next_y][next_x] && !times.contains({next_y,next_x})) {
					q.push_back({next_y,next_x});
				}
			}
		}
		++t;
	}
	return cheat_time_counter;
}
unordered_map<int,int> bfs2 (pair<vector<vector<bool>>,pair<pair<int,int>,pair<int,int>>> input){
	const vector<vector<bool>> map = input.first;
	unordered_map<pair<int,int>,int,pair_hash<int,int>> times; 
	unordered_map<int,int> cheat_time_counter;
	deque<pair<int,int>> q;
	unordered_map<pair<int,int>,int,pair_hash<int,int>> cheat_set;
	vector<pair<int,int>> dirs {{-1,0},{0,-1},{1,0},{0,1}};
	// init cheat set 
	int steps=0;
	vector<pair<int,int>> next;
	vector<pair<int,int>> processing;
	processing.push_back({0,0});
	while (steps<20) {
		for (auto step_i:processing) {
			for(auto dir:dirs) {
				if (!cheat_set.contains({step_i.first+dir.first, step_i.second+dir.second})) {
					cheat_set[{step_i.first+dir.first, step_i.second+dir.second}]=steps+1;
					next.push_back({step_i.first+dir.first, step_i.second+dir.second});
				}
			}
		}
		++steps;
		processing = next;
	}
	cout << cheat_set.size() << '\n';

	q.push_back(input.second.first);
	int t = 0;
	while (!q.empty()) {
		int q_size = q.size();
		for (int i=0; i<q_size;++i) {
			pair<int,int> curr = q.front();
			q.pop_front();
			times[curr] = t;
			//identify possible cheats
			for (auto dir:cheat_set){
				int next_y = curr.first+dir.first.first,next_x = curr.second+dir.first.second;
				if (times.contains({next_y,next_x})) {
					if (times[{next_y,next_x}] < t-dir.second) {
						cheat_time_counter[t-dir.second-times[{next_y,next_x}]]++;
					}
				}
			}
			//append to the queue
			for (int dir=0; dir<dirs.size();++dir){
				int next_y = curr.first+dirs[dir].first, next_x = curr.second+dirs[dir].second;
				if (map[next_y][next_x] && !times.contains({next_y,next_x})) {
					q.push_back({next_y,next_x});
				}
			}
		}
		++t;
	}
	return cheat_time_counter;
}

int main() {
	string filename;
	cout << "Enter the filename:\n";
	cin >> filename;
	filename+=".txt";
	pair<vector<vector<bool>>,pair<pair<int,int>,pair<int,int>>> input = parse_file(filename);
	//for (int i=0; i<input.first.size(); ++i) {
	//	for (int j=0; j<input.first[0].size(); ++j) {
	//		if (input.first[i][j]) {
	//			cout << '.';
	//		}
	//		else {
	//			cout << '#';
	//		}
	//	}
	//	cout<< '\n';
	//}
	unordered_map<int,int> cheat_count = bfs(input);
	unordered_map<int,int> cheat_count_2 = bfs2(input);
	int result1 = 0;
	for (auto cheat: cheat_count) {
		if (cheat.first>=100) {
			result1 += cheat.second;
		}
	}
	int result2 = 0;
	for (auto cheat: cheat_count_2) {
		//cout << "there are " << cheat.second << " cheats which save " << cheat.first << " picoseconds\n";
		if (cheat.first>=100) {
			result2 += cheat.second;
		}
	}
	cout << "result 1: " << result1 << '\n';
	cout << "result 2: " << result2 << '\n';
	return 0;
}

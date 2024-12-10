#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <cassert>
#include <unordered_set>

using namespace std;

struct pair_hash {
	size_t operator()(const pair<int, int>& p) const {
		return hash<int>()(p.first) ^ hash<int>()(p.second);
	}
};

pair<vector<vector<int>>, vector<pair<int,int>>> parse_file(string filename) {
	vector<vector<int>> map;
	vector<pair<int,int>> zeros;
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Err: could not open file \n";
	}
	string line;
	int i = 0;
	int width;
	while(getline(file,line)) {
		if (i == 0) {
			width = line.size();
		}
		else {
			assert(width == line.size());
		}
		vector<int> curr_line;
		for (int j = 0; j< line.size(); ++j) {
			int curr = line[j] - '0';
			assert(0 <= curr && curr < 10);
			curr_line.push_back(curr);
			if (curr == 0) {
				zeros.push_back(pair<int,int>(i,j));
			}
		}
		map.push_back(curr_line);
		++i;
	}
	return pair<vector<vector<int>>,vector<pair<int,int>>>(map, zeros);
}

// possible edge case where trail splits and rejoins before 9, 
int dfs(pair<int,int> start, vector<vector<int>> map) {
	const int y = map.size(), x = map[0].size();
	vector<pair<int,int>> stack;
	stack.push_back(start);
	unordered_set<pair<int,int>, pair_hash> summits;
	vector<pair<int,int>> dirs = {{0,1},{0,-1},{1,0},{-1,0}};
	while (!stack.empty()) {
		pair<int,int> curr = stack.back();
		stack.pop_back();
		assert(0<= curr.first && curr.first < y && 0 <= curr.second && curr.second < x);
		int curr_val = map[curr.first][curr.second];
		for (int i = 0; i<dirs.size(); ++i){
			int dy = dirs[i].first, dx = dirs[i].second;
			if (0<= curr.first + dy && curr.first + dy < y && 0 <= curr.second+dx && curr.second +dx < x) {
				if (curr_val == map[curr.first+dy][curr.second+dx]-1) {
						if (map[curr.first][curr.second] == 8) {
							summits.insert(pair<int,int>(curr.first+dy, curr.second+dx));
						}
						else {
							stack.push_back(pair<int,int>(curr.first+dy, curr.second+dx));
						}
				}
			}
		}
	}
	return summits.size();
}

int dfs2(pair<int,int> start, vector<vector<int>> map) {
	int result=0;
	const int y = map.size(), x = map[0].size();
	vector<pair<int,int>> stack;
	stack.push_back(start);
	vector<pair<int,int>> dirs = {{0,1},{0,-1},{1,0},{-1,0}};
	while (!stack.empty()) {
		pair<int,int> curr = stack.back();
		stack.pop_back();
		assert(0<= curr.first && curr.first < y && 0 <= curr.second && curr.second < x);
		int curr_val = map[curr.first][curr.second];
		for (int i = 0; i<dirs.size(); ++i){
			int dy = dirs[i].first, dx = dirs[i].second;
			if (0<= curr.first + dy && curr.first + dy < y && 0 <= curr.second+dx && curr.second +dx < x) {
				if (curr_val == map[curr.first+dy][curr.second+dx]-1) {
						if (map[curr.first][curr.second] == 8) {
							result +=1;
						}
						else {
							stack.push_back(pair<int,int>(curr.first+dy, curr.second+dx));
						}
				}
			}
		}
	}
	return result;
}


int get_result (pair<vector<vector<int>>, vector<pair<int,int>>> input) {
	int result = 0;
	const vector<vector<int>> map = input.first;
	const vector<pair<int,int>> zeros = input.second;
	for(int i = 0; i<zeros.size(); ++i) {
		result += dfs(zeros[i], map);
	}
	return result;
}

int get_result2 (pair<vector<vector<int>>, vector<pair<int,int>>> input) {
	int result = 0;
	const vector<vector<int>> map = input.first;
	const vector<pair<int,int>> zeros = input.second;
	for(int i = 0; i<zeros.size(); ++i) {
		result += dfs2(zeros[i], map);
	}
	return result;
}


int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename += ".txt";
	pair<vector<vector<int>>, vector<pair<int,int>>> file = parse_file(filename);
	int result1 = get_result(file);
	int result2 = get_result2(file);
	cout << "Result 1: " << result1 << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>

using namespace std;

vector<vector<int>> parse_file (string filename) {
	ifstream file(filename);
	assert(file.is_open());
	string line;
	vector<vector<int>> corruptions;
	while(getline(file,line)) {
		int split_pos=line.find_first_of(',');
		vector<int> next;
		next.push_back(stoi(line.substr(split_pos+1)));
		next.push_back(stoi(line.substr(0, split_pos)));
		corruptions.push_back(next);
	}
	return corruptions;
}

struct pair_hash {
	size_t operator()(const vector<int>& p) const {
		assert(p.size()>=2);
		return hash<int>()(p[0]) ^ (hash<int>()(p[0]) << 1);
	}
};

int get_result (vector<vector<int>> corruptions, const int dim, int start_time) {
	vector<vector<bool>> map(dim,vector<bool>(dim, true));
	//init map
	for(int i = 0; i<start_time; ++i) {
		map[corruptions[i][0]][corruptions[i][1]] = false;
	}
	// use bfs for to get solution;
	deque<vector<int>> q;
	unordered_map<vector<int>,int, pair_hash> visited;
	vector<vector<int>> dirs = {{0,1},{0,-1},{-1,0},{1,0}};
	q.push_back({0,0,0});// y,x,distance
	while (!q.empty()) {
		vector<int> curr = q.front();
		q.pop_front();
		for (auto dir:dirs) {
			if (0<=curr[0]+dir[0] && curr[0]+dir[0]<dim && 0<=curr[1]+dir[1] && curr[1]+dir[1]<dim) { // bounds check
				if (map[curr[0]+dir[0]][curr[1]+dir[1]]) {
					if (visited.contains({curr[0]+dir[0], curr[1]+dir[1]})) {
						if (visited[{curr[0]+dir[0], curr[1]+dir[1]}] >curr[2]+1) {
							visited[{curr[0]+dir[0], curr[1]+dir[1]}] = curr[2]+1;
							q.push_back({curr[0]+dir[0], curr[1]+dir[1], curr[2]+1});
						}
					}
					else {
						q.push_back({curr[0]+dir[0], curr[1]+dir[1], curr[2]+1});
						visited[{curr[0]+dir[0], curr[1]+dir[1]}] = curr[2]+1;
					}
				}
			}
		}
	}
	return visited[{dim-1,dim-1}];
}

//HACK: optimize this using binary search
pair<int,int> get_result_p2 (vector<vector<int>> corruptions, const int dim, int start_time) {
	vector<vector<bool>> map(dim,vector<bool>(dim, true));
	for(int i = 0; i<start_time; ++i) {
		map[corruptions[i][0]][corruptions[i][1]] = false;
	}
	for(int i = start_time; i<corruptions.size(); ++i){
		map[corruptions[i][0]][corruptions[i][1]] = false;
		// use bfs for to get solution;
		deque<vector<int>> q;
		unordered_map<vector<int>,int, pair_hash> visited;
		vector<vector<int>> dirs = {{0,1},{0,-1},{-1,0},{1,0}};
		q.push_back({0,0,0});// y,x,distance
		while (!q.empty()) {
			vector<int> curr = q.front();
			q.pop_front();
			for (auto dir:dirs) {
				if (0<=curr[0]+dir[0] && curr[0]+dir[0]<dim && 0<=curr[1]+dir[1] && curr[1]+dir[1]<dim) { // bounds check
					if (map[curr[0]+dir[0]][curr[1]+dir[1]]) {
						if (visited.contains({curr[0]+dir[0], curr[1]+dir[1]})) {
							if (visited[{curr[0]+dir[0], curr[1]+dir[1]}] >curr[2]+1) {
								visited[{curr[0]+dir[0], curr[1]+dir[1]}] = curr[2]+1;
								q.push_back({curr[0]+dir[0], curr[1]+dir[1], curr[2]+1});
							}
						}
						else {
							q.push_back({curr[0]+dir[0], curr[1]+dir[1], curr[2]+1});
							visited[{curr[0]+dir[0], curr[1]+dir[1]}] = curr[2]+1;
						}
					}
				}
			}
		}
		if (!visited.contains({dim-1, dim-1})) return {corruptions[i][1],corruptions[i][0]};
	}
	return {-1,-1};
}

int main() {
	string filename;
	cout << "Enter the filename: \n";
	cin >> filename;
	filename += ".txt";
	string dim_str;
	cout << "Enter the dim: \n";
	cin >> dim_str;
	int dim = stoi(dim_str);
	string its_str;
	cout << "Enter the its: \n";
	cin >> its_str;
	int its = stoi(its_str);
	vector<vector<int>> input = parse_file(filename);
	int result1 = get_result(input, dim+1,its);
	cout << "Result 1: " << result1 << '\n';
	pair<int,int> result2 = get_result_p2(input, dim+1, its);
	cout << "Result 2: " << result2.first <<','<< result2.second << '\n';

	return 0;
}

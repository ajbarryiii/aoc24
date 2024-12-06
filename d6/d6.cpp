#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <unordered_set>
#include <unordered_map>

using namespace std;
// Custom hash function for std::pair<int, int>
struct pair_hash {
	size_t operator()(const pair<int, int>& p) const {
		return hash<int>()(p.first) ^ hash<int>()(p.second);
	}
};

pair<pair<int,int>,vector<vector<bool>>> parse_file (string filename) {
	vector<vector<bool>> map;
	pair<int,int> start;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;
	int i = 0;
	while (getline(file, line)) {
		vector<bool> line_vec;
		for (int j = 0; j<line.size(); ++j) {
			if (line[j] == '.') {
				line_vec.emplace_back(false);
			}
			else if (line[j] == '#') {
				line_vec.emplace_back(true);
			}
			else if (line[j] == '^') {
				start.first = i;
				start.second = j;
				line_vec.emplace_back(false);
			}
			
		}
		// see if grid matches expected format
		if (i>0 && line_vec.size() != map[0].size()) {
			cout << "line size is not equal to others, line: " << i << '\n';
		}
		map.emplace_back(line_vec);
		++i;
	}

	file.close();
	// return start pos , boolean map
	return pair<pair<int,int>, vector<vector<bool>>>(start, map);
}

int get_result(pair<pair<int,int>, vector<vector<bool>>> input) {
	vector<vector<bool>> map = input.second;
	int i = input.first.first, j = input.first.second;
	unordered_set<pair<int,int>, pair_hash> visited;
	visited.insert(input.first);
	int dir = 1;
	int dy = -1, dx = 0;

	while (0<=i+dy && i+dy<map.size() && 0<=j+dx && j+dy<= map[0].size()) {
		if (map[i+dy][j+dx]) {
			//cout << "obstacle found at: " << i+dy << " , " << j+dx<< '\n';
			++dir;
			swap(dy,dx);
			if (dir % 2 == 0) {
				dy *= -1;
				dx *= -1;
			}
			//cout << "New dy, dx = " << dy << " , " << dx << '\n';
		}
		else {
			i+=dy;
			j+=dx;
			pair<int,int> curr(i,j);
			visited.insert(curr);
		}
	}
	return visited.size();
}

unordered_map<pair<int,int>,unordered_set<int>, pair_hash> get_visited(pair<pair<int,int>, vector<vector<bool>>> input) {
	vector<vector<bool>> map = input.second;
	int i = input.first.first, j = input.first.second;
	unordered_map<pair<int,int>,unordered_set<int>, pair_hash> visited;
	visited[input.first].insert(1);
	int dir = 1;
	int dy = -1, dx = 0;

	while (0<=i+dy && i+dy<map.size() && 0<=j+dx && j+dy<= map[0].size()) {
		if (map[i+dy][j+dx]) {
			//cout << "obstacle found at: " << i+dy << " , " << j+dx<< '\n';
			++dir;
			swap(dy,dx);
			if (dir % 2 == 0) {
				dy *= -1;
				dx *= -1;
			}
			//cout << "New dy, dx = " << dy << " , " << dx << '\n';
		}
		else {
			i+=dy;
			j+=dx;
			pair<int,int> curr(i,j);
			visited[curr].insert(dir%4);
		}
	}
	return visited;
}

bool is_cycle(vector<vector<bool>> map, pair<int,int> start_coord) {
	int i = start_coord.first, j = start_coord.second;
	unordered_map<pair<int,int>,unordered_set<int>, pair_hash> visited;
	visited[start_coord].insert(1);
	int dir = 1;
	int dy = -1, dx = 0;

	while (0<=i+dy && i+dy<map.size() && 0<=j+dx && j+dy<= map[0].size()) {
		if (map[i+dy][j+dx]) {
			//cout << "obstacle found at: " << i+dy << " , " << j+dx<< '\n';
			++dir;
			swap(dy,dx);
			if (dir % 2 == 0) {
				dy *= -1;
				dx *= -1;
			}
			//cout << "New dy, dx = " << dy << " , " << dx << '\n';
		}
		else {
			i+=dy;
			j+=dx;
			pair<int,int> curr(i,j);
			if (visited.find(curr)!= visited.end()) {
				if (visited[curr].find(dir%4) != visited[curr].end()){
					return true;
				}
			}
			visited[curr].insert(dir%4);
		}
	}
	return false;
}

int get_result_p2(pair<pair<int,int>, vector<vector<bool>>> input) {
	vector<vector<bool>> map = input.second;
	int i = input.first.first, j = input.first.second;
	unordered_map<pair<int, int>, unordered_set<int>,pair_hash> visited = get_visited(input);
	int result = 0;

	for (auto item : visited) {
		vector<vector<bool>> curr_map = map;
		curr_map[item.first.first][item.first.second] = true;
		for (auto dir : item.second) {
			if (is_cycle(curr_map, input.first)){
				++result;
				break;
			}
		}
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename +=".txt";
	pair<pair<int,int>, vector<vector<bool>>> lines = parse_file(filename);
	int result = get_result(lines);
	int result2 = get_result_p2(lines);
	cout << "Result 1: " << result << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}


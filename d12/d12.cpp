#include <iostream>
#include <vector> 
#include <string>
#include <fstream> 
#include <cassert> 
#include <deque>
#include <unordered_set>

using namespace std;

struct pair_hash {
   template <class T1, class T2>
   size_t operator()(const pair<T1, T2>& p) const {
       return hash<T1>()(p.first) ^ (hash<T2>()(p.second) << 1);
   }
};

vector<vector<char>> parse_file(string filename) {
	ifstream file(filename);
	vector<vector<char>> map;

	assert(file.is_open());

	string line;
	int i = 0;
	int line_len = 0;
	while(getline(file, line)) {
		vector<char> line_vec;
		if (i == 0) line_len = line.size();
		assert(line.size() == line_len);
		for (int j = 0; j < line_len; ++j) line_vec.push_back(line[j]);
		map.push_back(line_vec);
		++i;
	}
	return map;
}

pair<vector<vector<char>>, unordered_set<char>> parse_file_2(string filename) {
	ifstream file(filename);
	vector<vector<char>> map;
	unordered_set<char> chars;

	assert(file.is_open());

	string line;
	int i = 0;
	int line_len = 0;
	while(getline(file, line)) {
		vector<char> line_vec;
		if (i == 0) line_len = line.size();
		assert(line.size() == line_len);
		for (int j = 0; j < line_len; ++j){
			line_vec.push_back(line[j]);
			chars.insert(line[j]);
		}
		map.push_back(line_vec);
		
		++i;
	}
	return {map,chars};
}

int bfs(const vector<vector<char>> map){
	const int len = map.size(), wid = map[0].size();
	int y = 0, x = 0, result = 0;
	unordered_set<pair<int,int>,pair_hash> visited;
	vector<pair<int,int>> to_visit;
	vector<pair<int,int>> dirs = {{1,0}, {-1,0}, {0,1}, {0,-1}};
	while (visited.size()<(len*wid)) {
		deque<pair<int,int>> q;
		q.push_back({y,x});
		const char curr_char = map[y][x];
		int area = 0, perimiter = 0;
		while(!q.empty()){
			y = q.front().first, x = q.front().second;
			if (!visited.contains(q.front())){
				visited.insert(q.front());
				q.pop_front();
				int curr_per = 4;
				for (int i = 0; i < dirs.size(); ++i) {
					int curr_y = y+dirs[i].first, curr_x = x+dirs[i].second;
					if ( 0<= curr_y && curr_y<len && 0<=curr_x && curr_x<wid){
						if (map[curr_y][curr_x] == curr_char){
							if (!visited.contains(pair<int,int>(curr_y,curr_x))){
								q.push_back({curr_y,curr_x});
							}
							--curr_per;
						}
						else if(!visited.contains(pair<int,int>(curr_y,curr_x))){
							to_visit.push_back(pair<int,int>(curr_y,curr_x));
						}
					}
				}
				perimiter += curr_per;
				++area;
			}
			else {
				q.pop_front();
			}
		}
		//cout << "Char : " << curr_char<< " area: " << area << " perimiter: " << perimiter << '\n';
		result += (area*perimiter);
		y = to_visit.back().first, x = to_visit.back().second;
		to_visit.pop_back();
	}
	return result;
}

int sides(const vector<vector<char>> map, const unordered_set<char> chars) {
	const int len = map.size(), wid = map[0].size();
	int price = 0;
	for (auto curr_char:chars){
		int curr_char_edges = 0;
		int area = 0;
		unordered_set<int> prev_edges;
		// get horizontal edges
		for (int i = 0; i<len;++i) {
			int j = 0;
			unordered_set<int> curr_edges;
			while (j < wid) {
				if (map[i][j] == curr_char){
					curr_edges.insert(j);
					if (!prev_edges.contains(j)){
						curr_char_edges +=1;
					}
				 	while (j<wid && map[i][j] == curr_char) {
						++j;
						++area;
					}
					curr_edges.insert(j);
					if (!prev_edges.contains(j)){
						curr_char_edges +=1;
					}
				}
				while (j< wid && map[i][j] != curr_char) ++j;
			}
			prev_edges = curr_edges;
		}
		// get vertical edges
		prev_edges.clear();
		for (int j = 0; j<wid; ++j){
			int i = 0;
			unordered_set<int> curr_edges;
			while (i < len) {
				if (map[i][j] == curr_char){
					curr_edges.insert(i);
					if (!prev_edges.contains(i)){
						curr_char_edges +=1;
					}
					while (i<len && map[i][j] == curr_char) {
						++i;
					}
					curr_edges.insert(i);
					if (!prev_edges.contains(i)){
						curr_char_edges +=1;
					}
				}
				while (i< len && map[i][j] != curr_char) ++i;
			}
			prev_edges = curr_edges;

		}
		price += (curr_char_edges*area);
		cout << "Curr char: " << curr_char << ", Curr char edges: " << curr_char_edges <<", area: "<< area <<", Total price: " << price << '\n';
	}
	return price;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename+=".txt";
	if (filename == "test.txt") {
		vector<pair<string,int>> tests = {{"d12t1.txt", 140}, {"d12t2.txt", 772},{"d12t3.txt", 1930}}; 
		for (int i = 0; i<tests.size(); ++i) {
			vector<vector<char>> map = parse_file(tests[i].first);
			int result = bfs(map);
			cout << "Result t" << i+1<< " : " << result << " , Expected: " << tests[i].second << '\n';
		}
	}
	else {
		vector<vector<char>> map = parse_file(filename);
		unordered_set<char> chars = parse_file_2(filename).second;
		int result1 = bfs(map);
		int result2 = sides(map, chars);
		cout<<"Result 1 : " << result1 << '\n';
		cout<<"Result 2 : " << result2 << '\n';
	}
	return 0;
}

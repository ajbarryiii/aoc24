#include <iostream> 
#include <vector>
#include <string> 
#include <cassert>
#include <fstream>

using namespace std;

pair<int,int> char_to_dir(char arrow) {
	if (arrow == '^'){
		return {-1,0};
	}
	else if (arrow == 'v'){
		return {1,0};
	}
	else if (arrow == '>'){
		return {0,1};
	}
	else if (arrow == '<'){
		return {0,-1};
	}
	else {
		assert(false);
	}
}
pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> parse_file (string filename) {
	string line;
	ifstream file(filename);
	vector<vector<char>> map;
	vector<pair<int,int>> moves;
	pair<int,int> start_pos;
	int line_no = 0; 
	bool directions = false;

	while(getline(file, line)) {
		if (!line.empty()) {
			if (directions == false) {
				vector<char> line_vec;
				for (int i = 0; i<line.size(); ++i){
					line_vec.push_back(line[i]);
					if (line[i]=='@') start_pos = {line_no,i};
				}
				map.push_back(line_vec);
				++line_no;
			}
			else {
				for(int i = 0; i<line.size(); ++i) moves.push_back(char_to_dir(line[i]));
			}
		}
		else {
			directions = true;
		}
	}
	return {{map, moves}, start_pos};
}

vector<vector<char>> simulate (pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> input) {
	pair<int,int> pos = input.second;
	vector<vector<char>> map = input.first.first;
	vector<pair<int,int>> directions = input.first.second;
	for (int i = 0; i<directions.size();++i) {
		int dy = directions[i].first, dx = directions[i].second;
		pair<int,int> box_train = pos;
		int train_len = 0; 
		while(0<=box_train.first+dy && box_train.first+dy <map.size() &&
				0<=box_train.second+dx && box_train.second+dx <map[0].size() && 
				map[box_train.first+dy][box_train.second+dx]=='O') {
			box_train.first += dy;
			box_train.second+= dx;
			++train_len;
			//cout << "train len: " << train_len << '\n';
		}
		if (0<=box_train.first+dy && box_train.first+dy <map.size() &&
				0<=box_train.second+dx && box_train.second+dx <map[0].size() && 
				!(map[box_train.first+dy][box_train.second+dx]== '#')){
			map[pos.first][pos.second] = '.';
			map[pos.first+dy][pos.second+dx] = '@';
			pos.first += dy;
			pos.second +=dx;
			for(int j = 1; j<= train_len; ++j) {
				map[pos.first+(j*dy)][pos.second+(j*dx)] = 'O';
			}
		}

		//for (auto line: map) { 
		//	for (auto character: line) cout <<character;
		//	cout <<'\n';
		//}
		//cout << "\n\n\n\n";
	}
	return map;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename += ".txt";
	pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> input = parse_file(filename);
	cout << "Successfully parsed file \n";
	vector<vector<char>> final_map = simulate(input);
	int result1 = 0; 
	for (int i = 0; i<final_map.size();++i) {
		for (int j = 0; j < final_map[0].size(); ++j) {
			if (final_map[i][j] == 'O') {
				result1+= (100*i + j);
			}
		}
	}
	cout << "Result 1: " << result1 << '\n';
	return 0;
}

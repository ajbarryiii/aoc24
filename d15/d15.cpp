#include <iostream> 
#include <vector>
#include <string> 
#include <cassert>
#include <deque>
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

pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> parse_file2 (string filename) {
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
					if (line[i] == 'O'){
						line_vec.push_back('[');
						line_vec.push_back(']');
					}
					else if (line[i]=='@') {
						start_pos = {line_no,i*2};
						line_vec.push_back('@');
						line_vec.push_back('.');
					}
					else {
						line_vec.push_back(line[i]);
						line_vec.push_back(line[i]);
					}
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

vector<vector<char>> simulate2 (pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> input) {
	pair<int,int> pos = input.second;
	vector<vector<char>> map = input.first.first;
	vector<pair<int,int>> directions = input.first.second;
	//for (auto line: map) { 
	//	for (auto character: line) cout <<character;
	//	cout <<'\n';
	//}
	//cout << "\n\n\n\n";
	for (int i = 0; i<directions.size();++i) {
		int dy = directions[i].first, dx = directions[i].second;
		if (dy != 0){
			bool move = true;
			vector<vector<pair<char,pair<int,int>>>> points_to_update;
			points_to_update.push_back(vector<pair<char,pair<int,int>>>({{'@',{pos}}}));
			while (move){
				vector<pair<char,pair<int,int>>> back = points_to_update.back();
				vector<pair<char,pair<int,int>>> next;
				for (int j = 0; j<back.size();++j) { 
					pair<int,int> curr = back[j].second;
					if (map[curr.first+dy][curr.second+dx] == '#') {
						move = false;
						break;
					}
					else if (map[curr.first+dy][curr.second+dx] == '[') {
						next.push_back({map[curr.first+dy][curr.second+dx], {curr.first+dy, curr.second+dx}});
						next.push_back({map[curr.first+dy][curr.second+dx+1], {curr.first+dy, curr.second+dx+1}});
					}
					else if (map[curr.first+dy][curr.second+dx] == ']') {
						next.push_back({map[curr.first+dy][curr.second+dx], {curr.first+dy, curr.second+dx}});
						next.push_back({map[curr.first+dy][curr.second+dx-1], {curr.first+dy, curr.second+dx-1}});
					}
				}
				if (next.empty()) {
					break;
				}
				points_to_update.push_back(next);
			}
			if (move) {
				while (!points_to_update.empty()) {
					vector<pair<char,pair<int,int>>> curr = points_to_update.back();
					points_to_update.pop_back();
					for (int j = 0; j<curr.size(); ++j) {
						char c = curr[j].first;
						int y = curr[j].second.first, x = curr[j].second.second;
						map[y][x] = '.';
						map[y+dy][x+dx] = c;
					}
				}
				pos.first += dy;
				pos.second += dx;
			}
		}
		else {
			vector<pair<char,pair<int,int>>> points_to_change;
			points_to_change.push_back({'@', pos});
			pair<int,int> curr_coords = pos;
			bool move = true;
			while(true){
				if (map[curr_coords.first+dy][curr_coords.second+dx] == '#') {
					move = false;
					break;
				}
				else if (map[curr_coords.first+dy][curr_coords.second+dx] == '.') {
					break;
				}
				else {
					curr_coords.second += dx;
					curr_coords.first+=dy;
					int y = curr_coords.first, x = curr_coords.second;
					char c = map[curr_coords.first][curr_coords.second];
					points_to_change.push_back({c, {y,x}});
				}
			}
			if(move) {
				while(!points_to_change.empty()) {
					char c = points_to_change.back().first;
					int new_y = points_to_change.back().second.first, new_x = points_to_change.back().second.second;
					map[new_y][new_x] = '.';
					map[new_y+dy][new_x+dx] = c;
					points_to_change.pop_back();
				}
				pos.first += dy;
				pos.second += dx;
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
	vector<vector<char>> final_map = simulate(input);
	cout << "complete p1 \n";
	pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> input2 = parse_file2(filename);
	cout << "complete parsing flie in p2 \n";
	vector<vector<char>> final_map2 = simulate2(input2);
	int result1 = 0; 
	for (int i = 0; i<final_map.size();++i) {
		for (int j = 0; j < final_map[0].size(); ++j) {
			if (final_map[i][j] == 'O') {
				result1+= (100*i + j);
			}
		}
	}
	int result2 = 0; 
	for (int i = 0; i<final_map2.size();++i) {
		for (int j = 0; j < final_map2[0].size(); ++j) {
			if (final_map2[i][j] == '[') {
				result2+= (100*i + j);
			}
		}
	}
	cout << "Result 1: " << result1 << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}

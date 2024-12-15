#include <iostream> 
#include <unordered_map>
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
						start_pos = {line_no,i};
						line.push_back('@');
						line.push_back('.');
					}
					else {
						line.push_back(line[i]);
						line.push_back(line[i]);
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

struct CharTree {
	pair<char,pair<int,int>> val;
	CharTree *left;
	CharTree *right;
	CharTree(pair<char,pair<int,int>> x): val(x), left(nullptr), right(nullptr) {}
};

vector<vector<char>> simulate2 (pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> input) {
	pair<int,int> pos = input.second;
	vector<vector<char>> map = input.first.first;
	vector<pair<int,int>> directions = input.first.second;
	for (int i = 0; i<directions.size();++i) {
		int dy = directions[i].first, dx = directions[i].second;
		if (dy != 0){
			bool move = true;
			CharTree* tree = new CharTree({map[pos.first][pos.second],pos});
			deque<CharTree*> q;
			q.push_back(tree);
			while (!q.empty()){
				CharTree* curr = q.front();
				q.pop_front();
				pair<int,int> curr_coords = curr->val.second;
				if (0<=curr_coords.first+dy && curr_coords.first+dy <map.size() &&
						0<=curr_coords.second+dx && curr_coords.second+dx <map[0].size() 
						){
					if (map[curr_coords.first+dy][curr_coords.second+dx]== '#') {
						move = false;
						break;
					}
					else if ( map[curr_coords.first][curr_coords.second]=='[' && map[curr_coords.first+dy][curr_coords.second+dx] == ']') {
						curr->right  = new CharTree({']', {curr_coords.first+dy, curr_coords.second+dx}});
						curr->left  = new CharTree({'[', {curr_coords.first+dy, curr_coords.second+dx-1}});
						q.push_back(curr->right);
						q.push_back(curr->left);
					}
					else if ( map[curr_coords.first][curr_coords.second]==']' && map[curr_coords.first+dy][curr_coords.second+dx] == '[') {
						curr->right  = new CharTree({'[', {curr_coords.first+dy, curr_coords.second+dx}});
						curr->left  = new CharTree({']', {curr_coords.first+dy, curr_coords.second+dx+1}});
						q.push_back(curr->right);
						q.push_back(curr->left);
					}
					else if ( map[curr_coords.first][curr_coords.second]=='[' && map[curr_coords.first+dy][curr_coords.second+dx] == '[') {
						curr->right  = new CharTree({'[', {curr_coords.first+dy, curr_coords.second+dx}});
						q.push_back(curr->right);
					}
					else if ( map[curr_coords.first][curr_coords.second]==']' && map[curr_coords.first+dy][curr_coords.second+dx] == ']') {
						curr->right  = new CharTree({']', {curr_coords.first+dy, curr_coords.second+dx}});
						q.push_back(curr->right);
					}
				}
			}
			if (move) {
				deque<CharTree*> q;
				vector<pair<char,pair<int,int>>> points_to_change;
				q.push_back(tree);
				while (!q.empty()) {
					CharTree* curr = q.front();
					q.pop_front();
					points_to_change.push_back(curr->val);
					if (curr->left != nullptr) q.push_back(curr->left);
					if (curr->right != nullptr) q.push_back(curr->right);
				}
				while(!points_to_change.empty()) {
					char c = points_to_change.back().first;
					int y = points_to_change.back().second.first, x = points_to_change.back().second.first;
					map[y][x] = '.';
					map[y+dy][x+dx] = c;
					points_to_change.pop_back();
				}

			}
		}
		else {
			vector<pair<char,pair<int,int>>> points_to_change;
			points_to_change.push_back({'@',pos});
			pair<int,int> curr_coords = pos;
			bool move = true;
			while(0<= curr_coords.second+dx && curr_coords.second <map[0].size()){
				if (map[curr_coords.first][curr_coords.second+dx] == '#') {
					move = false;
					break;
				}
				else if (map[curr_coords.first][curr_coords.second+dx] == '.') {
					break;
				}
				else {
					points_to_change.push_back({map[curr_coords.first][curr_coords.second], curr_coords});
					curr_coords.second += dx;
				}
			}
			if(move) {
				while(!points_to_change.empty()) {
					char c = points_to_change.back().first;
					int y = points_to_change.back().second.first, x = points_to_change.back().second.first;
					map[y][x] = '.';
					map[y+dy][x+dx] = c;
					points_to_change.pop_back();
				}
			}
		}

		for (auto line: map) { 
			for (auto character: line) cout <<character;
			cout <<'\n';
		}
		cout << "\n\n\n\n";
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
	pair<pair<vector<vector<char>>,vector<pair<int,int>>>,pair<int,int>> input2 = parse_file2(filename);
	vector<vector<char>> final_map2 = simulate2(input2);
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

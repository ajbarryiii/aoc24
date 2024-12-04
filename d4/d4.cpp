#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 

using namespace std;

vector<string> parse_file (string filename) {
	vector<string> result;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;
	while (getline(file, line)) {
		result.push_back(line);
	}
	file.close();
	return result;
}

int get_result ( vector<string> text) {
	vector<pair<int,int>> directions = {{0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,1},{1,-1},{-1,-1}};
	string verify = "XMAS";
	int result = 0;

	for(int i = 0; i < text.size(); ++i) {
		for (int j = 0; j < text[0].size(); ++j) {
			if (text[i][j] == 'X') {
				int curr_x = i, curr_y = j;
				for (auto dir : directions) {
					if ( 0 <= curr_x + (3*dir.first) && curr_x + (3*dir.first) <text.size() &&  0 <= curr_y + (3*dir.second) && curr_y + (3*dir.second) < text[0].size()){
						bool xmas = true;
						for (int k = 1; k < verify.size(); ++k) {
							if ( text[curr_x + (k*dir.first)][curr_y + (k*dir.second)] != verify[k]){
								xmas = false;
								break;
							}
						}
						if (xmas) {
							++result;
						}
					}
				}
			}
		}
	}
	return result;
}

int get_result_2 ( vector<string> text) {
	vector<pair<int,int>> directions = {{1,1},{-1,1},{1,-1},{-1,-1}};
	int result = 0;

	for(int i = 0; i < text.size(); ++i) {
		for (int j = 0; j < text[0].size(); ++j) {
			if (text[i][j] == 'A') {
				int curr_x = i, curr_y = j;
				int correct_dirs = 0;
				for (auto dir : directions) {
					if ( 0 <= curr_x - dir.first && curr_x - dir.first <text.size() && 0 <= curr_x + dir.first && curr_x + dir.first <text.size() &&  0 <= curr_y + dir.second && curr_y + dir.second < text[0].size() && 0 <= curr_y + dir.second && curr_y + dir.second < text[0].size()){
						if (text[curr_x+dir.first][curr_y + dir.second] == 'S' && text[curr_x-dir.first][curr_y - dir.second] == 'M' ) {
							++ correct_dirs;
							if (correct_dirs == 2) {
								++result;
								break;
							}
						}
					}
				}
			}
		}
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	vector<string> lines = parse_file(filename);
	int result = get_result(lines);
	int result2 = get_result_2(lines);
	cout << "Result 1: " << result << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}




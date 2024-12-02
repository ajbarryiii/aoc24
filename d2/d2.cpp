#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <algorithm>
#include <sstream>

using namespace std;

vector<vector<int>> parse_file (string filename) {
	vector<vector<int>> result;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;

	while (getline(file, line)) {
		vector<int> line_vec;
		istringstream stream(line);
		string word;
		while (stream >> word) {
			line_vec.push_back(stoi(word));
		}
		if (line_vec.size() < 2) {
			cout << "line has len < 2";
		}
		result.push_back(line_vec);
	}
	file.close();
	return result;
}

int isSafe (vector<int> line) {
	if (line.size() < 2){
		cout << "line has let < 2";
		return 1;
	}
	
	// case of increasing
	if (line[0] < line[1]) {
		for(int i = 0; i < line.size()-1; ++i) {
			if (!(line[i] < line[i+1] and line[i] >= line[i+1] - 3)) {
				return 0;
			}
		}
		return 1;
	}
	else if (line[0] > line[1]) {
		for(int i = 0; i < line.size()-1; ++i) {
			if (!(line[i] < line[i+1] and line[i]-3 <= line[i+1] - 3)) {
				return 0;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}

int compute_ans ( vector<vector<int>> input) {
	int result = 0;
	for(auto i : input) {
		result += isSafe(i);
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	vector<vector<int>> lines = parse_file(filename);
	int result = compute_ans(lines);
	cout << "Result: " << result << '\n';

	return 0;
}



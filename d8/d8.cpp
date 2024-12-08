#include <iostream> 
#include <fstream> 
#include <sstream>
#include <stdexcept>
#include <string> 
#include <vector> 
#include <unordered_map>
#include <cassert>
#include <unordered_set>

using namespace std;

struct pair_hash {
	size_t operator()(const pair<int, int>& p) const {
		return hash<int>()(p.first) ^ hash<int>()(p.second);
	}
};

pair<unordered_map<char, vector<pair<int,int>>>,pair<int,int>> parse_file (string filename) {
	unordered_map<char, vector<pair<int,int>>> map;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;
	int i = 0;
	int width;
	while (getline(file, line)) {
		if (i == 0) {
			width = line.size();
		}
		for(int j = 0; j< line.size(); ++j) {
			if (line[j] != '.') {
				map[line[j]].emplace_back(pair<int,int>(i,j));
			}
		}
		++i;
	}
	file.close();
	pair<int,int> dims(i, width);
	return pair<unordered_map<char, vector<pair<int,int>>>,pair<int,int>>(map, dims);
}
// what things can we do to speed up computation.
// can we easily identify the points which will be ob?

pair<pair<int,int>,pair<int,int>> get_antinodes(pair<int,int> node1, pair<int,int> node2) {
	int dy = node1.first - node2.first, dx = node1.second - node2.second;
	pair<int,int> first_pt( node1.first+dy, node1.second +dx);
	pair<int,int> second_pt( node2.first-dy, node2.second -dx);
	return pair<pair<int,int>,pair<int,int>>(first_pt, second_pt);
}

int get_result ( pair<unordered_map<char, vector<pair<int,int>>>,pair<int,int>> input) {
	const int len = input.second.first, width = input.second.second;
	const unordered_map<char, vector<pair<int,int>>> map = input.first;
	unordered_set<pair<int,int>, pair_hash> antinodes;
	for (auto antenna : map) {
		const vector<pair<int,int>> antennas = antenna.second;
		for(int i = 0; i < antennas.size(); ++i) {
			for (int j = i+1; j < antennas.size(); ++j){
				pair<pair<int,int>,pair<int,int>> candidates = get_antinodes(antennas[i], antennas[j]);
				int c1y = candidates.first.first, c2y = candidates.second.first, c1x = candidates.first.second, c2x = candidates.second.second;
				if (0 <=c1y && c1y<len && 0<=c1x && c1x < width) antinodes.insert(candidates.first);
				if (0 <=c2y && c2y<len && 0<=c2x && c2x < width) antinodes.insert(candidates.second);
			}
		}
	}
	return antinodes.size();
}



int main() {
	//pair<pair<int,int>,pair<int,int>> antinodes = get_antinodes(pair<int, int>(1,0), pair<int, int>(0,1));
	//cout << "test antinodes: (" << antinodes.first.first << ',' << antinodes.first.second << "), (" << antinodes.second.first << ',' << antinodes.second.second << ") \n";
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename +=".txt";
	pair<unordered_map<char, vector<pair<int,int>>>,pair<int,int>> lines = parse_file(filename);
	int result = get_result(lines);
	//long result2 = get_result_p2(lines);
	cout << "Result 1: " << result << '\n';
	//cout << "Result 2: " << result2 << '\n';
	return 0;
}


#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <fstream> 

using namespace std;

// for p1, have an unordered map<string,unordered_set<*string>>

unordered_map<string,unordered_set<string>> parse_file (string filename) {
	unordered_map<string,unordered_set<string>> map;
	string line;
	ifstream file(filename);
	assert(file.is_open());
	while (getline(file,line)) {
		string first = line.substr(0,line.find_first_of('-'));
		string second = line.substr(line.find_first_of('-')+1);
		map[first].insert(second);
		map[second].insert(first);
	}
	return map;
}

int get_sets_of_3 (unordered_map<string,unordered_set<string>> map ) {
	int count=0;
	for (auto key:map) {
		//note we need to remove keys from our original key;
		unordered_set<string> visited;
		if (key.first[0]=='t') {
			for (auto relation1:key.second) {
				visited.insert(relation1);
				for (auto relation2:key.second) {
					if (relation1!=relation2 && !visited.contains(relation2)) {
						//only need to do one check because relations are reciprocal
						if (map[relation1].contains(relation2)) {
							//BUG: this seems to double count each relation;
							//HACK: checking if relation1[0]==t seems to remove the double count issues for the ones we care about;
							map[relation1].erase(key.first);
							map[relation2].erase(key.first);
							count++;
							//cout << "Group " << count << ": " << key.first << '-' << relation1 << '-' << relation2 << '\n';
						}
					}
				}
			}
		}
	}
	return count;
}

int main () {
	unordered_map<string,unordered_set<string>> map_test = parse_file("d23t.txt");
	int count_g3_t = get_sets_of_3(map_test);
	assert (count_g3_t==7);
	unordered_map<string,unordered_set<string>> map = parse_file("d23.txt");
	int result1 = get_sets_of_3(map);
	cout << "Result 1: " << result1 << '\n';
	return 0;
}

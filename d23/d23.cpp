#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <fstream> 
#include <algorithm>
#include <variant>

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

vector<string> find_max_clique (unordered_map<string,unordered_set<string>> map) {
	vector<unordered_set<string>> curr_cliques;
	//tactic: build up cliques recursively;
	//first init the curr_cliques of size 2;
	unordered_map<string,unordered_set<string>> map_copy = map;
	for (auto node:map) {
		for (auto rel:node.second) {
			unordered_set<string> clique;
			map_copy[rel].erase(node.first);
			clique.insert(rel);
			clique.insert(node.first);
			curr_cliques.push_back(clique);
		}
	}
	//How do we not double count for the next step?
	while (true) {
		vector<unordered_set<string>> next_cliques;
		unordered_set<string> visited;
		cout << curr_cliques.size() << '\n';
		for (int i=0;i<curr_cliques.size();++i) {
			for (auto key:map) {
				if (!curr_cliques[i].contains(key.first)) {
					bool valid = all_of ( curr_cliques[i].begin(), curr_cliques[i].end(), 
							[&] (string s) { return map[s].contains(key.first);});
					if (valid) {
						unordered_set<string> clique_next = curr_cliques[i];
						clique_next.insert(key.first);
						vector<string> prevent_double;
						for(auto item:clique_next) {
							prevent_double.push_back(item);
						}
						sort(prevent_double.begin(),prevent_double.end());
						string prevent_double_check = "";
						for (int str=0;str<prevent_double.size();++str) prevent_double_check+=prevent_double[str];
						if (!visited.contains(prevent_double_check)) {
							next_cliques.push_back(clique_next);
							visited.insert(prevent_double_check);
						}
					}
				}
			}
		}
		if (!next_cliques.empty()) {
			curr_cliques = next_cliques;
		}
		else break;
	}
	unordered_set<string> max_clique = curr_cliques[0];
	vector<string> result;
	for (auto node:max_clique) {
		result.push_back(node);
	}
	return result;
}

int main () {
	unordered_map<string,unordered_set<string>> map_test = parse_file("d23t.txt");
	int count_g3_t = get_sets_of_3(map_test);
	assert (count_g3_t==7);
	unordered_map<string,unordered_set<string>> map = parse_file("d23.txt");
	int result1 = get_sets_of_3(map);
	cout << "Result 1: " << result1 << '\n';
	vector<string> testp2 = find_max_clique(map);
	cout << "testp2: ";
	for (int i=0;i<testp2.size();++i) {
		cout << testp2[i] << ',';
	}
	cout << '\n';
	return 0;
}

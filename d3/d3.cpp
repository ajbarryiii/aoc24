#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <sstream>
#include <regex>

using namespace std;

string get_file (string filename) {
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	ostringstream buffer;

	buffer << file.rdbuf();

	return buffer.str();
}

int get_muls (string file) {
	regex pattern(R"(mul\((\d+),(\d+)\))");
	int result = 0;

	for (sregex_iterator it(file.begin(), file.end(), pattern), end_it; it != end_it; ++it) {
		string int1 = (*it)[1].str();
		string int2 = (*it)[2].str();
		result += (stoi(int1) * stoi(int2));
	}
	return result;
}

pair< string , int> get_segment( string file , int acc) {
	int i = 0;
	while (i < file.size()) {
		if (file[i] == 'd') {
			if (file.substr(i, 4) == "do()") {
				i += 4;
				break;
			}
			else {
				++i;
			}
		}
		else {
			++i;
		}
	}
	int begin = i;
	while ( i < file.size()) {
		if (file[i] == 'd') {
			if (file.substr(i, 7) == "don't()") {
				i += 7;
				break;
			}
			else {
				++i;
			}
		}
		else {
			++i;
		}
	}
	int segment_result = get_muls(file.substr(begin, i-begin));
	if (i>= file.size()) {
		return pair<string,int>("", acc + segment_result);
		cout << "segment result: " << segment_result<< '\n';
	}
	cout << "segment result: " << segment_result<< '\n';
	pair< string , int> result(file.substr(i), acc + segment_result);
	return result;
}	

int get_mul_p2 ( string file) {
	int i = 0;

	while ( i < file.size()) {
		if (file[i] == 'd') {
			if (file.substr(i, 7) == "don't()") {
				cout << "dont at: " << i<< "\n";
				i += 7;
				break;
			}
			else ++i;
		}
		else {
			++i;
		}
	}
	int acc = get_muls(file.substr(0, i));
	cout << acc << '\n';
	cout << file.substr(0, i) << '\n';

	string next = file.substr(i);

	while (next != "") {
		pair<string, int> temp = get_segment(next, acc);
		next = temp.first;
		acc = temp.second;
	}
	return acc;
}


int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	string lines = get_file(filename);
	int result = get_muls(lines);
	int result2 = get_mul_p2(lines);
	cout << "Result 1: " << result << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}



#include <iostream>
#include <vector> 
#include <regex>
#include <string>
#include <fstream> 
#include <cassert> 

using namespace std;

// can we use linear algebra to solve the system?

vector<pair<pair<int,int>,pair<pair<int,int>,pair<int,int>>>> parse_file ( string filename) {
	vector<pair<pair<int,int>,pair<pair<int,int>,pair<int,int>>>> result;
	ifstream file(filename);
	string line;
	int line_no = 1;
	pair<int,int> A, B, prize;
	while(getline(file, line)){
		if (line_no%4!=0) { 
			regex pattern("X[+=](\\d+),\\s*Y[+=](\\d+)");
			smatch match;
			regex_search(line,match, pattern);
			if (line_no%4==3) {
				prize = pair<int,int>(stoi(match[1].str()),stoi(match[2].str()));
				//cout << "prize: " << prize.first << ',' << prize.second << ", A: " << A.first << ',' << A.second << ", B: " << B.first << ',' << B.second << '\n';
				result.push_back({prize,{A,B}});
			}
			else if (line_no%4==1){
				A = pair<int,int>(stoi(match[1].str()),stoi(match[2].str()));
			}
			else if (line_no%4==2){
				B = pair<int,int>(stoi(match[1].str()),stoi(match[2].str()));
			}
			
		}
		else {
		}
		++line_no;
	}
	return result;
}

int get_result(vector<pair<pair<int,int>,pair<pair<int,int>,pair<int,int>>>> input) {
	int result = 0;
	for ( auto game: input) {
		pair<int,int> prize = game.first, A = game.second.first, B=game.second.second;

		int det = (A.first*B.second)-(B.first*A.second);
		if (det==0) {
			// case where A,B are linearly dependent
			if (prize.first%A.first==0 && prize.second%A.second==0) {
				result += min(prize.first/A.first, 3*(prize.first/B.first));
			}
		}
		else {
			int a_units = (B.second*prize.first + (-1*B.first * prize.second));
			int b_units = ((-1*A.second*prize.first)+(A.first*prize.second));
			if (a_units%det!=0 || b_units%det != 0){
				result +=0;
			}
			else {
				//cout << "line result: " << a_units/det + 3*(b_units/det) << '\n';
				//cout << "a: " << a_units/det << " b: " << b_units/det << '\n';
				result += 3*(a_units/det) + (b_units/det);
			}
		}
	}
	return result;
}

long get_result_p2(vector<pair<pair<int,int>,pair<pair<int,int>,pair<int,int>>>> input) {
	long result = 0;
	for ( auto game: input) {
		pair<long,long> prize = game.first, A = game.second.first, B=game.second.second;
		prize.first += 10000000000000;
		prize.second += 10000000000000;
		long det = (A.first*B.second)-(B.first*A.second);
		if (det==0) {
			// case where A,B are linearly dependent
			if (prize.first%A.first==0 && prize.second%A.second==0) {
				result += min(prize.first/A.first, 3*(prize.first/B.first));
			}
		}
		else {
			long a_units = (B.second*prize.first + (-1*B.first * prize.second));
			long b_units = ((-1*A.second*prize.first)+(A.first*prize.second));
			if (a_units%det!=0 || b_units%det != 0){
				result +=0;
			}
			else {
				//cout << "line result: " << a_units/det + 3*(b_units/det) << '\n';
				//cout << "a: " << a_units/det << " b: " << b_units/det << '\n';
				result += 3*(a_units/det) + (b_units/det);
			}
		}
	}
	return result;
}
int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename+=".txt";
	vector<pair<pair<int,int>,pair<pair<int,int>,pair<int,int>>>> input = parse_file(filename);
	int result1 = get_result(input);
	long result2 = get_result_p2(input);
	cout << "Result 1: " << result1 << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}

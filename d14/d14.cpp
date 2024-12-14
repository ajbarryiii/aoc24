#include <cfloat>
#include <climits>
#include <iostream>
#include <numeric>
#include <vector> 
#include <string>
#include <regex>
#include <fstream> 
#include <cmath>
#include <stdexcept>
#include <utility>
#include <cassert> 

using namespace std;

vector<pair<pair<int,int>,pair<int,int>>> parse_file( string filename) {
	vector<pair<pair<int,int>,pair<int,int>>> points;
	ifstream file(filename);
	string line;
	pair<int,int> p, v;
	while(getline(file,line)){
		regex pattern(R"(p=(\d+),(\d+) v=(-?\d+),(-?\d+))");
		smatch match;
		regex_search(line,match, pattern);
		p = {stoi(match[1].str()),stoi(match[2].str())};
		v = {stoi(match[3].str()),stoi(match[4].str())};
		points.push_back({p,v});
		//cout << "p: (" << p.first << ',' << p.second << ") , v: (" << v.first << ',' << v.second << ")\n";
	}
	return points;
}

int mod(int a, int m) {
	int result = a%m;
	return result<0 ? result+m:result;
}

int safety_factor(const int wid, const int len, const int seconds, vector<pair<pair<int,int>,pair<int,int>>> input){
	int mid_x = wid/2+1, mid_y = len/2+1;
	//cout << "mid_x: " << mid_x << ", mid_y: " << mid_y<< '\n';
	int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
	for (auto robot: input) {
		// add 1 to make it 1 indexed?
		pair<int,int> pos = {mod((robot.first.first + seconds*robot.second.first),wid)+1, mod((robot.first.second + seconds*robot.second.second),len)+1};
		//cout << "final position: " << pos.first << ',' << pos.second << '\n';
		if(pos.first < mid_x && pos.second < mid_y) {
			++q1;
		}
		else if(pos.first > mid_x && pos.second < mid_y) {
			++q2;
		}
		else if(pos.first < mid_x && pos.second > mid_y) {
			++q3;
		}
		else if(pos.first > mid_x && pos.second > mid_y) {
			++q4;
		}
	}
	//cout << "q1: " << q1 << ", q2: " << q2 << ", q3: " << q3 << ", q4: " << q4 << '\n';
	return q1*q2*q3*q4;
}

double geometric_mean_of_variances(const vector<pair<int, int>>& data) {
    if (data.empty()) {
        return 0.0;
    }

    auto variance = [](const vector<int>& values) {
        if (values.empty()) {
            return 0.0;
        }

        double sum = accumulate(values.begin(), values.end(), 0.0);
        double mean = sum / values.size();

        double sq_sum = accumulate(values.begin(), values.end(), 0.0,
                                   [mean](double acc, int x) {
                                       return acc + pow(x - mean, 2);
                                   });

        return sq_sum / values.size(); // Population variance
    };

    vector<int> first_elements;
    vector<int> second_elements;

    for (const auto& p : data) {
        first_elements.push_back(p.first);
        second_elements.push_back(p.second);
    }

    double var_first = variance(first_elements);
    double var_second = variance(second_elements);

    if (var_first == 0.0 || var_second == 0.0) {
        return 0.0;
    }

    return sqrt(var_first * var_second);
}

double p2 (const int wid, const int len, const int seconds, vector<pair<pair<int,int>,pair<int,int>>> input){
	int mid_x = wid/2+1, mid_y = len/2+1;
	vector<pair<int,int>> final;
	//cout << "mid_x: " << mid_x << ", mid_y: " << mid_y<< '\n';
	int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
	for (auto robot: input) {
		// add 1 to make it 1 indexed?
		pair<int,int> pos = {mod((robot.first.first + seconds*robot.second.first),wid)+1, mod((robot.first.second + seconds*robot.second.second),len)+1};
		final.push_back(pos);
	}
	//cout << "q1: " << q1 << ", q2: " << q2 << ", q3: " << q3 << ", q4: " << q4 << '\n';
	return geometric_mean_of_variances(final);
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename+=".txt";
	vector<pair<pair<int,int>,pair<int,int>>> input = parse_file(filename);
	int min_danger_it;
	double min_danger = DBL_MAX;
	for (int i = 0; i<100000; ++i){ 
		double danger = p2(101, 103, i, input);
		if (danger < min_danger) {
			min_danger = danger;
			min_danger_it = i;
		}
	}
	int result1 = safety_factor(101, 103, 100, input);
	cout << "Result 1: " << result1 << '\n';
	cout << "test p2: " << min_danger_it <<" min_danger: " <<min_danger<< '\n';
	//cout << "Result 2: " << result2 << '\n';
	return 0;
}

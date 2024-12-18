#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map> 
#include <functional>
#include <cctype>

using namespace std;

// parse file out pair<registers(vector len 3), vector<int> program(all ints will be mod 8)>
pair<vector<long>,vector<int>> parse_file ( string filename ) {
	ifstream file(filename);
	string line;
	vector<long> registers;
	vector<int> program;
	while(getline(file,line)) {
		if (line.substr(0,9) == "Register ") {
			registers.push_back(stol(line.substr(11)));
		}
		else if (!line.empty()) {
			line = line.substr(9);
			for (int i = 0; i<line.size(); ++i) {
				if (line[i] != ',') {
					int val = line[i]-'0';
					program.push_back(val);
				}
			}
		}
	}
	return {registers, program};
}

struct Program {
	vector<long> registers;
	vector<int> program;
	int idx;
	unordered_map<int,long*> combo_operands;
	vector<int> out_program;
	// methods
	void adv (){
		registers[0] = registers[0]/(1 << *combo_operands[program[idx+1]]);
		idx+=2;
	}
	void bdv (){
		registers[1] = registers[0]/(1 << *combo_operands[program[idx+1]]);
		idx+=2;
	}
	void cdv (){
		registers[2] = registers[0]/(1 << *combo_operands[program[idx+1]]);
		idx+=2;
	}
	void bxl () {
		registers[1] = registers[1]^program[idx+1];
		idx+=2;
	}
	void bst () {
		registers[1] = *combo_operands[program[idx+1]]%8;
		idx+= 2;
	}
	void jnz () {
		if (registers[0] != 0) {
			idx = program[idx+1];
		}
		else {
			idx +=2;
		}
	}
	void bxc () {
		registers[1] = registers[1]^registers[2];
		idx += 2;
	}
	void out () {
		//cout << *combo_operands[program[idx+1]]%8 <<',' ;
		out_program.push_back(*combo_operands[program[idx+1]]%8);
		idx+=2;
	}
	// def of program mapping
	unordered_map<int, function<void(Program*)>> instructions = {
		{0, [](Program* p) { p->adv(); }},
		{6, [](Program* p) { p->bdv(); }},
		{7, [](Program* p) { p->cdv(); }},
		{1, [](Program* p) { p->bxl(); }},
		{2, [](Program* p) { p->bst(); }},
		{3, [](Program* p) { p->jnz(); }},
		{4, [](Program* p) { p->bxc(); }},
		{5, [](Program* p) { p->out(); }}
	};
	
	//init
	Program(vector<long> registers_init, vector<int> program_init ){
		registers = registers_init;
		program = program_init;
		idx = 0;
		unordered_map<int,long*> combo_operands_init;
		combo_operands_init[0] = new long(0);
		combo_operands_init[1] = new long(1);
		combo_operands_init[2] = new long(2);
		combo_operands_init[3] = new long(3);
		combo_operands_init[4] = &registers[0];
		combo_operands_init[5] = &registers[1];
		combo_operands_init[6] = &registers[3];
		combo_operands = combo_operands_init;
	}

	//execute command
	void execute () {
		assert(0<=program[idx] && program[idx] < 8);
		assert(registers[0]>=0 && registers[1]>=0 && registers[2]>=0);
		//cout << "Executing instruction: " << program[idx] << ", Index: " <<idx << '\n';
		instructions[program[idx]](this);
	}
	long p2bf() {
		long i = 0;
		while (true) {
			registers[0] = i;
			registers[1] = 0;
			registers[2] = 0;
			idx = 0;
			out_program.clear();
			while(idx < program.size()){
				instructions[program[idx]](this);
				if (!out_program.empty()){
					if (out_program.back()!=program[out_program.size()-1]){
						break;
					}
					else if (out_program.size() > program.size()){
						break;
					}
				}
			}
			if (out_program == program) return i;
			if (i%100000 == 0) cout << "\n Iteration: " << i;
			++i;
		}
		// this case shouldnt happen
		return -1;
	}
	
	long step () {
		long i = 1;
		int index_to_check = 1;
		while (index_to_check <= program.size()) {
			registers[0] = i;
			registers[1] = 0;
			registers[2] = 0;
			idx = 0;
			out_program.clear();
			while(idx < program.size()){
				instructions[program[idx]](this);
			}
			if (program == out_program) return i;
			if ( out_program == vector<int>(program.begin()+program.size()-index_to_check, program.end())) {
				cout << "found index " << index_to_check-1 << '\n';
				i = i << 3;
				assert(i>0);
				assert ( out_program == vector<int>(program.begin()+program.size()-index_to_check, program.end()));
				index_to_check++;
			}
			else {
				++i;
			}
		}
		return -1;
	}
};

void get_result (pair<vector<long>,vector<int>> input) {
	Program program(input.first, input.second);
	while(program.idx < program.program.size()){
		program.execute();
	}
	for (auto i: program.out_program) {
		cout << i << ',';
	}
	cout << '\n';
}
// idea for p2. work through the possible values that coud output each char in reverse order?
long long get_result2_bf (pair<vector<long>,vector<int>> input) {
	Program program(input.first, input.second);
	long result = program.step();
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: \n";
	cin >> filename;
	filename+=".txt";
	pair<vector<long>,vector<int>> input = parse_file(filename);
	cout << "file parsed successfully\n";
	cout << "registers:";
	for (auto i : input.first) cout << i << ',';
	cout << "\nprogram:";
	for (auto i : input.second) cout << i << ',';
	cout <<"\nResult: ";
	get_result(input);
	long p2test = get_result2_bf(input);
	cout << "\nResult p2 brute force: " << p2test<< '\n';
	return 0;
}

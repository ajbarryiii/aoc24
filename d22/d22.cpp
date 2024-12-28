#include <string>
#include <vector> 
#include <iostream> 
#include <fstream> 
#include <cassert>

using namespace std;

long prune (long secret_no) {
	const long  mod = (1 << 24)-1;
	assert ((secret_no & mod) >= 0);
	return secret_no & mod;
}

long mix (long secret_no, long value) {
	assert ((secret_no ^ value) >= 0);
	return secret_no ^ value;
}

long next_secret (long secret_no) {
	long val_to_mix1 = secret_no << 6;
	assert (val_to_mix1 >=0);
	secret_no = mix(secret_no,val_to_mix1);
	secret_no = prune(secret_no);

	long val_to_mix2 = secret_no >> 5;
	assert (val_to_mix2 >=0);
	secret_no = mix(secret_no,val_to_mix2);
	secret_no = prune(secret_no);

	long val_to_mix3 = secret_no << 11;
	assert (val_to_mix3 >=0);
	secret_no = mix(secret_no,val_to_mix3);
	secret_no = prune(secret_no);

	return secret_no;
}
long parse_file(string filename) {
	long result=0;
	string line;
	ifstream file(filename);
	while (getline(file,line)) {
		long secret_no = stol(line);
		for (int i=0;i<2000; ++i) {
			secret_no = next_secret(secret_no);
		}
		result+=secret_no;
	}
	return result;
}

int main () {
	long test = parse_file("d22t.txt");
	assert(test==37327623);
	long result1 = parse_file("d22.txt");
	cout << "Result 1: " << result1 <<'\n';
	return 0;
}


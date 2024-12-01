#include <iostream> 
#include <fstream> 
#include <string> 

int main() {
	 std::ifstream file("dn.txt");

	 if (!file.is_open()) {
		 std::cerr << "Err: could not open file \n";
		 return -1;
	 }

	 std::string line; 
	 while (std::getline(file, line)) {
		 std::cout<<line<<'\n';
	 }
	 file.close();
	 return 0;
}

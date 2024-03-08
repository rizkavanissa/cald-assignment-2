#include <iostream>
#include <fstream>
#include <string>

int main() {
  std::ifstream input_file("circuits/474a_circuit1.txt");
  std::ofstream output_file("output.v");
  if (input_file.is_open() && output_file.is_open()) {
    std::string line;
    while (std::getline(input_file, line)) {
      output_file << line << '\n';
    }
    input_file.close();
    output_file.close();
  } else {
    std::cout << "Unable to open file";
  }
  return 0;
}

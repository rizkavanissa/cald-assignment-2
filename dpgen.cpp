#include <filesystem> //  Provides functions to perform operations on file systems (e.g., querying file attributes, iterating through directory contents, and manipulating paths)
#include <iostream> // Provides the basic input/output stream functionality in C++ (e.g., std::cin and std::cout)
#include <fstream> // Provides functionality for working with files in C++ (e.g., std::ifstream, std::ofstream, and std::fstream)
#include <vector> // Provides a dynamic array-like container that stores elements in contiguous memory, allowing for fast access to elements using iterators or indices. Also, it automatically handles memory allocation and resizing, making it a flexible and efficient choice for storing and manipulating collections of objects.

#include "parser.h"

/*
    A directive that allows you to use names from the std namespace without prefixing them with 'std::'
    The std namespace contains many standard library components for tasks like I/O operations, string manipulation, and working with containers.
*/
using namespace std;

// Check the whether the file can be used
bool check_conditions(const string& filename)
{
    // Check if the filename is provided
    // if (filename.empty()) {
    //     cerr << "Error: No filename provided." << endl;
    //     return false;
    // }
    
    // Open the specified file
    ifstream file(filename);

    // Check if the Netlist file exists
    if ( !filesystem::exists(filename) )
    {
        cerr << "Netlist file does not exist: " << filename << endl;
        return false;

    }
    else if ( !file.is_open() ) // Check whether the file can be opened
    {
        cerr << "Error: Unable to open the text file of " << filename << ".txt" << endl;
        return false;
    }
    else if ( !file.good() ) // Checks if the state of the file stream is good for reading if no errors have occurred during previous operations on the file stream (such as reading or writing)
    {
        cerr << "Error: Invalid state of file stream for reading from the text file of " << filename << ".txt" << endl;
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    /*
        Output warning for invalid usage
    */
    if ( argc != 3 ) // User is expected to provide three arguments in the terminal following the usage statement below
	{
		cout << "Usage: dpgen netlistFile verilogFile" << endl;
        cout << "\t-    dpgen   : Directory to the dpgen of the CMake build file. (commonly located in ./src/dpgen)" << endl;
		cout << "\t- netlistFile: Directory to the Behavioral Netlist to be converted. (e.g., [netlist-file-name].txt)" << endl;
		cout << "\t- verilogFile: Directory to store the output of the Verilog code conversion file. (e.g., [verilog-file-name].v) " << endl;
		return 0;
	}

    // Check additional conditions before opening the file
    if (!check_conditions(argv[1])) {
        return 1; // Exit the program if conditions are not met
    }

    NetParser netParser; // Create an instance of the NetParser class

    if(netParser.convertToVerilog(argv[1], argv[2])) // Perform the conversion from behavioral netlist text format to Verilog code
    {
        cout << argv[2] << " Verilog file successfully created" << endl;
    } else { cout << "Verilog file failed to be created due to incomplete Behavioral Netlist" << endl;} // IF the conversion is unsuccessful, this error message is displayed instead

    return 0;
}
#include "parser.h"

#include <iostream> // Provides the basic input/output stream functionality in C++ (e.g., cin and cout)
#include <fstream> // Provides functionality for working with files in C++ (e.g., ifstream, ofstream, and fstream)
#include <vector> // Provides a dynamic array-like container that stores elements in contiguous memory, allowing for fast access to elements using iterators or indices. Also, it automatically handles memory allocation and resizing, making it a flexible and efficient choice for storing and manipulating collections of objects.
#include <regex> // Provides operations for regular expressions
#include <map> 
/*
    A directive that allows you to use names from the std namespace without prefixing them with ''
    The std namespace contains many standard library components for tasks like I/O operations, string manipulation, and working with containers.
*/
using namespace std;


bool checkBitWidth(const string& input) // Check if the bitwidth is valid
{
    regex pattern("Int([0-9]+)"); // Define the pattern to match
    smatch matches; // Store any matches from the regex

    if (regex_search(input, matches, pattern)) { // Check whether the input matches the pattern

        int numBits = stoi(matches[1].str()); // Extract the number of bits as an integer

        if (numBits == 1) {
            // Handle the special case when numBits is 1
            return true;
        }
        else {
            /*
                o In binary representation, a power of 2 has only one bit set to 1, and all other bits are 0.
                o Subtracting 1 from a power of 2 results in setting all bits to the right of the rightmost 1 bit to 1.
                o Performing a bitwise AND operation between a power of 2 and one less than it will result in clearing 
                all bits except for the rightmost 1 bit.

                For example, 8 (1000 in binary), subtracting 1 from it gives 0111 in binary.
                Performing a bitwise AND operation between 1000 and 0111 will result in 0000.
                This property holds true for any bit values of 2^n, where n is a non-negative integer.
            */
            return (numBits > 0 && (numBits & (numBits - 1)) == 0); // Check if numBits is a power of 2
        }
    }

    cout << "No match found. Please use correct the format in the behavioral netlist." << endl;
    return false;
}


// Perform conversion from behavior netlist text file to Verilog file
bool NetParser::convertToVerilog(char* inFile, char* outFile)
{
    /*

        ███████╗██╗██╗  ████████╗███████╗██████╗      █████╗ ███╗   ██╗██████╗     ███████╗████████╗ ██████╗ ██████╗ ███████╗    ██╗███╗   ██╗██████╗ ██╗   ██╗████████╗
        ██╔════╝██║██║  ╚══██╔══╝██╔════╝██╔══██╗    ██╔══██╗████╗  ██║██╔══██╗    ██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝    ██║████╗  ██║██╔══██╗██║   ██║╚══██╔══╝
        █████╗  ██║██║     ██║   █████╗  ██████╔╝    ███████║██╔██╗ ██║██║  ██║    ███████╗   ██║   ██║   ██║██████╔╝█████╗      ██║██╔██╗ ██║██████╔╝██║   ██║   ██║   
        ██╔══╝  ██║██║     ██║   ██╔══╝  ██╔══██╗    ██╔══██║██║╚██╗██║██║  ██║    ╚════██║   ██║   ██║   ██║██╔══██╗██╔══╝      ██║██║╚██╗██║██╔═══╝ ██║   ██║   ██║   
        ██║     ██║███████╗██║   ███████╗██║  ██║    ██║  ██║██║ ╚████║██████╔╝    ███████║   ██║   ╚██████╔╝██║  ██║███████╗    ██║██║ ╚████║██║     ╚██████╔╝   ██║   
        ╚═╝     ╚═╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝     ╚══════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝    ╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝    ╚═╝   

    */

	ifstream netlistFile(inFile);

	vector<string> text_lines; // Declare a vector of words for each line
	string line; // // Declare a string to one line

    // Loop through each line in netlistFile
    while ( getline(netlistFile, line, '\n') ) // Delimiter '\n' is explicitly passed here just in case
    {
        // Skip processing if the line is empty
        if (!line.empty())
        {
            // Check if "//" is present in the string (error handling)
            if (size_t pos = line.find("//"); pos != string::npos)
            {
                string afterComment = line.substr(pos + 2);
                
                // Trim whitespace characters from both ends of the extracted substring
                size_t start = afterComment.find_first_not_of(" \t\f\v\n\r");
                size_t end = afterComment.find_last_not_of(" \t\f\v\n\r");

                if (start != std::string::npos && end != std::string::npos)
                {
                    afterComment = afterComment.substr(start, end - start + 1);
                }

                cout << "ERROR FOUND: " << line.substr(pos + 2) << endl; // extract the substring after "//" to output the error message
                return 1; // Exit program
            }

            // Define the set of whitespace characters to search
            string whitespaceChars = " \t\f\v\n\r";

            // Check if the last character of the string is a whitespace character
            while (!line.empty()); // Loop indefinitely
            {
                /*
                    line.back() returns a reference to the last character of the string line.
                    whitespaceChars.find(line.back()) searches for the last character of line in the set of whitespace characters.
                    'string::npos' is a return value if the search is unsuccessful

                    NOTE:
                        For an empty line that contains no visible characters, 
                        the loop will find the last character to be the newline character ('\n') 
                        which is considered a whitespace character. Therefore, 
                        the loop will remove this newline character, effectively treating 
                        the empty line as a line with trailing whitespace characters.
                */
                if (!line.empty() && whitespaceChars.find(line.back()) != string::npos) {
                    // The last character is a whitespace character
                    line.pop_back(); // Remove the last character
                } else {
                    // The last character is not a whitespace character
                    break; // Exit the loop
                }
            }

            text_lines.push_back(line); // After filtering, "line" is added to the vector of strings
        }
    }

// map
	std::regex intRegex("\\bInt(\\d+)\\b"); // structure of the operation ... Int(number) ...
	std::map<string, int> variable;
	std::stringstream ss(line);

	while (! line.empty()) {
		
		for (std::regex_iterator it(line.begin(), line.end(), intRegex); it != std::sregex_iterator(); ++it)
		{
			// regex varRegex("\\bInt(\\d+)([A-Za-z])");
			// obtain the variable name
			
			// checks the validity of the input based on the regex
			bool isValidInput(const std::string& input)
			{
				std::regex varRegex("\\bInt(\\d+)([A-Za-z])");
				return std::regex_match(input, varRegex);
			}

			if (isValidInput(line))
			{
				
			}
				
			int intValue = std::stoi(it->str());
			variable[] = intValue;
		}
	}

    /*

    ██████╗ ██████╗ ███╗   ██╗██╗   ██╗███████╗██████╗ ████████╗    ████████╗ ██████╗     ██╗   ██╗███████╗██████╗ ██╗██╗      ██████╗  ██████╗ 
    ██╔════╝██╔═══██╗████╗  ██║██║   ██║██╔════╝██╔══██╗╚══██╔══╝    ╚══██╔══╝██╔═══██╗    ██║   ██║██╔════╝██╔══██╗██║██║     ██╔═══██╗██╔════╝ 
    ██║     ██║   ██║██╔██╗ ██║██║   ██║█████╗  ██████╔╝   ██║          ██║   ██║   ██║    ██║   ██║█████╗  ██████╔╝██║██║     ██║   ██║██║  ███╗
    ██║     ██║   ██║██║╚██╗██║╚██╗ ██╔╝██╔══╝  ██╔══██╗   ██║          ██║   ██║   ██║    ╚██╗ ██╔╝██╔══╝  ██╔══██╗██║██║     ██║   ██║██║   ██║
    ╚██████╗╚██████╔╝██║ ╚████║ ╚████╔╝ ███████╗██║  ██║   ██║          ██║   ╚██████╔╝     ╚████╔╝ ███████╗██║  ██║██║███████╗╚██████╔╝╚██████╔╝
    ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝   ╚═╝          ╚═╝    ╚═════╝       ╚═══╝  ╚══════╝╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝  ╚═════╝ 

    */

    NetParser netParser; // Create an instance of NetParser object

    /*
        'auto' tells the compiler to infer the type of it based on the initialization expression text_lines.begin().
    */
    for (const auto& line : text_lines)
    {
        istringstream lineStream(line); // Initialize a stream from a string and then parse it (purposely for >>)
        string netType; // Declare string variable to store token

        lineStream >> netType; // Store the first token in netType
        
        if( !checkBitWidth(netType) ) // Check whether the bit width is valid
        {
            return false;
        }
        else if( netType.compare(INPUT) == 0 && checkBitWidth(netType) ) // Check whether the extracted token is equal to INPUT
        {
            netParser.setInput(parseInput(line)); // Pass the string in the current line to the function
        }
        else if( netType.compare(OUTPUT) == 0 && checkBitWidth(netType)) // Check whether the extracted token is equal to INPUT
        {
            netParser.setOutput(parseOutput(line)); // Pass the string in the current line to the function
        }
        else if( netType.compare(WIRE) == 0 && checkBitWidth(netType) ) // Check whether the extracted token is equal to INPUT
        {
            netParser.setWire(parseWire(line)); // Pass the string in the current line to the function
        }
        else if( netType.compare(REGISTER) == 0 && checkBitWidth(netType) ) // Check whether the extracted token is equal to INPUT
        {
            netParser.setRegister(parseRegister(line)); // Pass the string in the current line to the function
        }
        else // Otherwise the line is an operation expression
        {
            netParser.setOperation(parseOperation(line)); // Pass the string in the current line to the function
        }
    }

    writeToOutput(outFile, netParser); // Do the conversion and write the result to the output file

    return true;
}

/*

    ██╗    ██╗██████╗ ██╗████████╗███████╗    ████████╗ ██████╗      ██████╗ ██╗   ██╗████████╗██████╗ ██╗   ██╗████████╗
    ██║    ██║██╔══██╗██║╚══██╔══╝██╔════╝    ╚══██╔══╝██╔═══██╗    ██╔═══██╗██║   ██║╚══██╔══╝██╔══██╗██║   ██║╚══██╔══╝
    ██║ █╗ ██║██████╔╝██║   ██║   █████╗         ██║   ██║   ██║    ██║   ██║██║   ██║   ██║   ██████╔╝██║   ██║   ██║   
    ██║███╗██║██╔══██╗██║   ██║   ██╔══╝         ██║   ██║   ██║    ██║   ██║██║   ██║   ██║   ██╔═══╝ ██║   ██║   ██║   
    ╚███╔███╔╝██║  ██║██║   ██║   ███████╗       ██║   ╚██████╔╝    ╚██████╔╝╚██████╔╝   ██║   ██║     ╚██████╔╝   ██║   
    ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝   ╚═╝   ╚══════╝       ╚═╝    ╚═════╝      ╚═════╝  ╚═════╝    ╚═╝   ╚═╝      ╚═════╝    ╚═╝   

*/

/*
    Creates the Verilog file given the results from the convertExpression
    convertDeclaration functions.
*/
void writeToOutput(string verilogFile, NetParser &netParser)
{
	ofstream file(verilogFile); // Open the output file

    // Create a reference to a vector of object corresponding to its net type using the referenced "netParser" instance
    const vector<SetNet>& inputs = netParser.getInputs();
    const vector<SetNet>& outputs = netParser.getOutputs();
    const vector<SetNet>& wires = netParser.getWires();
    const vector<SetNet>& registers = netParser.getRegisters();
    const vector<SetOp>& operations = netParser.getOperations();

    // Write the time unit and module header to the output file 
	file << "`timescale 1ns / 1ps" << endl;
	file << "module " << verilogFile << "(" << endl;
    file << "input Clk, Rst," << endl;
    for (const SetNet& input : inputs) { // Iterate through each object in the referenced vector
        input.printInput(file); // Print out each object to the output file
    }
    for (const SetNet& output : outputs) { // Iterate through each object in the referenced vector
        output.printInput(file); // Print out each object to the output file
        /*
            The code below is necessary because in the Verilog code the last declaration of net does not have a comma
            For example,
            module Circuit4 (
                input [63:0] a, b, c,
                input Clk, Rst,
                output [31:0] z, x    <- no comma here
            );
        */
        if ( outputs.size() != 1 || &output != &outputs.back() ) { // Check whether current output is the last element of outputs
            file << "," << endl;
        }
        
    }
    file << ");" << endl;

    for (const SetNet& wire : wires) {
        wire.printWire(file, netParser);
    }
    for (const SetNet& input : inputs) {
        input.printInput(file);
    }
    file << endl;

	file << "endmodule";
	return;
}


/*
    Print each net types to the output file
*/
void SetNet::printInput(ofstream& file) const
{
    file << "\t" << this->getNetType() << " [" << this->getBitWidth() << ":0] " << this->getVarNames() << "," << endl;
    return;
}

void SetNet::printOutput(ofstream& file) const
{
    file << "\t" << this->getNetType() << " [" << this->getBitWidth() << ":0] " << this->getVarNames();
    return;
}

void SetNet::printWire(ofstream& file, NetParser &netParser) const
{
    istringstream ss(this->getVarNames());
    string var;
    vector<string> vars; // Vector to store dynamically created string variables
    vector<string> oneBitVars; // Store variables that only require a single bit

	while(ss >> var) // Loop while opStream is not empty
	{
        vars.push_back(var); // Store the token in the vector
    }

    const vector<SetOp>& operations = netParser.getOperations(); // Get a reference to the vector of SetOp objects

    /*
        auto it = vars.begin();: This initializes an iterator it to the beginning of the vars vector. 
                                 vars.begin() returns an iterator pointing to the first element of the vector.

        it != vars.end(): This is the loop condition. The loop will continue executing as long as 
                          the iterator it has not reached the end of the vars vector. vars.end() returns an iterator 
                          pointing one past the last element of the vector, so the loop stops before reaching the end.

        ++it: This is the iteration expression. After each iteration of the loop, the iterator it is 
              incremented to point to the next element in the vector.
    */
    for ( auto it = vars.begin(); it != vars.end(); ) {

        /*
            *it: This is dereferencing the iterator it, which means it accesses the element that it is currently 
                 pointing to. Since it is an iterator for a vector of strings (vector<string>iterator), *it gives 
                 us a reference to the string element that it is pointing to.

            const string& currentVar: This declares a constant reference 'currentVar' to the string element that it 
                                      is currently pointing to. It's important to note that 'currentVar' is a reference 
                                      to the actual string element in the vector, not a copy of it. Using a reference 
                                      allows us to access and modify the element directly without making a copy, which 
                                      can be more efficient especially for large objects like strings.
        */
        const string& currentVar = *it; // Assigns a constant reference to the current string element pointed to by the iterator 

        for (const SetOp& operation : operations) // Iterate through the referenced 'operations' vector
        {
            // Access the vector of operands for each SetOp object
            const vector<string>& operands = operation.getOperands();

            // Iterate through the vector of operands
            for (const string& operand : operands)
            {
                // Inside the most inner if statement:
                if (operands[1] == currentVar && operands[2] == "?")
                {
                    oneBitVars.push_back(currentVar); // Store 'currentVar' to the 'oneBitVars' vector
                    it = vars.erase(it);  // Remove 'currentVar' from 'vars' and update the iterator
                }
                else
                {
                    ++it;  // Move to the next element
                }
            }
        }
    }

    // Now write out the single-bit width variables and the multi-bit width variables!!!

    if( this->getBitWidth() == 1 ) // Check whether the bit width is 1, if so then the bit width does need to be specified
    {
        file << "\t" << this->getNetType() << this->getVarNames() << "," << endl;
        return;
    }
    else
    {
        file << "\t" << this->getNetType() << " [" << this->getBitWidth() << ":0] " << this->getVarNames() << "," << endl;
        return;
    }
}

void SetNet::printRegister(ofstream& file) const
{
    file << "\t" << this->getNetType() << " [" << this->getBitWidth() << ":0] " << this->getVarNames() << "," << endl;
    return;
}

void SetOp::printOperation(ofstream& file) const
{}


/*

    ███████╗████████╗ ██████╗ ██████╗ ███████╗    ███████╗███████╗██████╗  █████╗ ██████╗  █████╗ ████████╗███████╗    ███╗   ██╗███████╗████████╗
    ██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝    ██╔════╝██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝██╔════╝    ████╗  ██║██╔════╝╚══██╔══╝
    ███████╗   ██║   ██║   ██║██████╔╝█████╗      ███████╗█████╗  ██████╔╝███████║██████╔╝███████║   ██║   █████╗      ██╔██╗ ██║█████╗     ██║   
    ╚════██║   ██║   ██║   ██║██╔══██╗██╔══╝      ╚════██║██╔══╝  ██╔═══╝ ██╔══██║██╔══██╗██╔══██║   ██║   ██╔══╝      ██║╚██╗██║██╔══╝     ██║   
    ███████║   ██║   ╚██████╔╝██║  ██║███████╗    ███████║███████╗██║     ██║  ██║██║  ██║██║  ██║   ██║   ███████╗    ██║ ╚████║███████╗   ██║   
    ╚══════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝    ╚══════╝╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚══════╝    ╚═╝  ╚═══╝╚══════╝   ╚═╝   

*/
/*
    Store each net into their respective object type
*/
SetNet parseInput(string inputString) // Tokenize the input string, retaining only the utilized tokens
{
    vector<string> netInput; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})

    // (e.g., extracting the "Int32" or "UInt32" token)
    size_t typeEnd = inputString.find(' '); // Find the end of the type
    netInput.push_back(inputString.substr(typeEnd + 1, inputString.find(' ', typeEnd + 1) - typeEnd - 1)); // Extract the type

    // (e.g., extracting the "a, b, c" token)
    size_t variablesStart = inputString.find(' ', typeEnd + 1); // Find the start of the variables
    netInput.push_back(inputString.substr(variablesStart + 1)); // Extract the variables

	return SetNet("input", netInput[0], netInput[1]); // Return this temporary initialized object
}

SetNet parseOutput(string outputString) // Tokenize the output string, retaining only the utilized tokens
{
    vector<string> netOutput; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})

    size_t typeEnd = outputString.find(' '); // Find the end of the type
    netOutput.push_back(outputString.substr(typeEnd + 1, outputString.find(' ', typeEnd + 1) - typeEnd - 1)); // Extract the type

    size_t variablesStart = outputString.find(' ', typeEnd + 1); // Find the start of the variables
    netOutput.push_back(outputString.substr(variablesStart + 1)); // Extract the variables

	return SetNet("output", netOutput[0], netOutput[1]); // Return this temporary initialized object
}

SetNet parseWire(string wireString) // Tokenize the wire string, retaining only the utilized tokens
{
    vector<string> netWire; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})

    size_t typeEnd = wireString.find(' '); // Find the end of the type
    netWire.push_back(wireString.substr(typeEnd + 1, wireString.find(' ', typeEnd + 1) - typeEnd - 1)); // Extract the type

    size_t variablesStart = wireString.find(' ', typeEnd + 1); // Find the start of the variables
    netWire.push_back(wireString.substr(variablesStart + 1)); // Extract the variables

	return SetNet("wire", netWire[0], netWire[1]); // Return this temporary initialized object
}

SetNet parseRegister(string regString) // Tokenize the register string, retaining only the utilized tokens
{
    vector<string> netReg; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})

    size_t typeEnd = regString.find(' '); // Find the end of the type
    netReg.push_back(regString.substr(typeEnd + 1, regString.find(' ', typeEnd + 1) - typeEnd - 1)); // Extract the type

    size_t variablesStart = regString.find(' ', typeEnd + 1); // Find the start of the variables
    netReg.push_back(regString.substr(variablesStart + 1)); // Extract the variables

	return SetNet("wire", netReg[0], netReg[1]); // Return this temporary initialized object
}

SetOp parseOperation(string opString) // Tokenize the operation string, retaining only the utilized tokens
{
	istringstream opStream(opString); // Initialize a stream from a string and then parse it (purposely for >>)
	string tempOp; // Declare string variable to store token
	vector<string> tempOps; // Vector to store dynamically created string variables

	while(opStream >> tempOp) // Loop while opStream is not empty
	{
		if(tempOp != "=") // Skip over any token that is an equal sign
		{
        	tempOps.push_back(tempOp); // Store the token in the vector
		}
    }
	// Index starts [0]
	if(tempOps[2] == ADD) // Check if the element pointed by this index is an addition operator
	{
		return SetOp("ADD",tempOps);
	}
	else if(tempOps[2] == SUB) // Check if the element pointed by this index is an subtractor operator
	{
		return SetOp("SUB",tempOps);
	}
	else if(tempOps[2] == MUL) // Check if the element pointed by this index is an multiplier operator
	{
		return SetOp("MUL",tempOps);
	}
	else if(tempOps[2] == GT) // Check if the element pointed by this index is an greater than operator
	{
		return SetOp("GT",tempOps);
	}
	else if(tempOps[2] == LT) // Check if the element pointed by this index is an less than operator
	{
		return SetOp("LT",tempOps);
	}
	else if(tempOps[2] == EQ) // Check if the element pointed by this index is an equal to operator
	{
		return SetOp("EQ",tempOps);
	}
	else if(tempOps[2] == SHR) // Check if the element pointed by this index is an shift-right operator
	{
		return SetOp("SHR",tempOps);
	}
	else if(tempOps[2] == SHL) // Check if the element pointed by this index is an shift-left operator
	{
		return SetOp("SHL",tempOps);
	}

	return SetOp(); // Otherwise return empty object
}

/*

    ███████╗████████╗ ██████╗ ██████╗ ███████╗    ███╗   ██╗███████╗████████╗     ██████╗ ██████╗ ██╗     ██╗     ███████╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗
    ██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝    ████╗  ██║██╔════╝╚══██╔══╝    ██╔════╝██╔═══██╗██║     ██║     ██╔════╝██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║
    ███████╗   ██║   ██║   ██║██████╔╝█████╗      ██╔██╗ ██║█████╗     ██║       ██║     ██║   ██║██║     ██║     █████╗  ██║        ██║   ██║██║   ██║██╔██╗ ██║
    ╚════██║   ██║   ██║   ██║██╔══██╗██╔══╝      ██║╚██╗██║██╔══╝     ██║       ██║     ██║   ██║██║     ██║     ██╔══╝  ██║        ██║   ██║██║   ██║██║╚██╗██║
    ███████║   ██║   ╚██████╔╝██║  ██║███████╗    ██║ ╚████║███████╗   ██║       ╚██████╗╚██████╔╝███████╗███████╗███████╗╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║
    ╚══════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝    ╚═╝  ╚═══╝╚══════╝   ╚═╝        ╚═════╝ ╚═════╝ ╚══════╝╚══════╝╚══════╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

*/
/*
    Store each net into their respective vector of those nets
*/
void NetParser::setInput(SetNet input) // Store a single input of SetNet object into the "inputs" vector
{
    this->inputs.push_back(input);
	return;
}

void NetParser::setOutput(SetNet output) // Store a single output of SetNet object into the "outputs" vector
{
	this->outputs.push_back(output);
	return;
}

void NetParser::setWire(SetNet wire) // Store a single wire of SetNet object into the "wires" vector
{
	this->wires.push_back(wire);
	return;
}

void NetParser::setRegister(SetNet reg) // Store a single register of SetNet object into the "registers" vector
{
	this->registers.push_back(reg);
	return;
}

void NetParser::setOperation(SetOp op) // Store a single operation of SetOp object into the "operations" vector
{
	this->operations.push_back(op);
	return;
}

/*

    ██████╗ ███████╗████████╗████████╗███████╗██████╗ ███████╗
    ██╔════╝ ██╔════╝╚══██╔══╝╚══██╔══╝██╔════╝██╔══██╗██╔════╝
    ██║  ███╗█████╗     ██║      ██║   █████╗  ██████╔╝███████╗
    ██║   ██║██╔══╝     ██║      ██║   ██╔══╝  ██╔══██╗╚════██║
    ╚██████╔╝███████╗   ██║      ██║   ███████╗██║  ██║███████║
    ╚═════╝ ╚══════╝   ╚═╝      ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝

*/

/*
    The getters below are specifically for net types of input, output, wire, and register
*/
string SetNet::getNetType() const // Getter for net type
{
    return this->netType;
}

char SetNet::getSignType() const // Getter for sign type
{
    return this->signType;
}

int SetNet::getBitWidth() const // Getter for bit-width
{
    return this->bitWidth;
}

string SetNet::getVarNames() const // Getter for variable names
{
    return this->varNames;
}


/*
    The getters below are specifically for operations
*/
string SetOp::getOpName() const // Getter for the operator's type name
{
	return this->netOperator;
}

vector<string> SetOp::getOperands() const // Getter for the operator's involved operands
{
	return this->operands;
}


/*
    The getters below are specifically for printing to output
*/
const vector<SetNet>& NetParser::getInputs() const // Getter for the set of stored inputs
{
    return this->inputs;
}

const vector<SetNet>& NetParser::getOutputs() const // Getter for the set of stored outputs
{
    return this->outputs;
}

const vector<SetNet>& NetParser::getWires() const // Getter for the set of stored wires
{
    return this->wires;
}

const vector<SetNet>& NetParser::getRegisters() const // Getter for the set of stored registers
{
    return this->registers;
}

const vector<SetOp>& NetParser::getOperations() const // Getter for the set of stored operations
{
    return this->operations;
}

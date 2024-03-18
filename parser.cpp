#include "parser.h"

#include <iostream> // Provides the basic input/output stream functionality in C++ (e.g., cin and cout)
#include <fstream> // Provides functionality for working with files in C++ (e.g., ifstream, ofstream, and fstream)
#include <vector> // Provides a dynamic array-like container that stores elements in contiguous memory, allowing for fast access to elements using iterators or indices. Also, it automatically handles memory allocation and resizing, making it a flexible and efficient choice for storing and manipulating collections of objects.
#include <regex> // Provides operations for regular expressions
#include <sstream>

/*
    A directive that allows you to use names from the std namespace without prefixing them with ''
    The std namespace contains many standard library components for tasks like I/O operations, string manipulation, and working with containers.
*/
using namespace std;


/*

    ███████╗███████╗████████╗████████╗███████╗██████╗ ███████╗
    ██╔════╝██╔════╝╚══██╔══╝╚══██╔══╝██╔════╝██╔══██╗██╔════╝
    ███████╗█████╗     ██║      ██║   █████╗  ██████╔╝███████╗
    ╚════██║██╔══╝     ██║      ██║   ██╔══╝  ██╔══██╗╚════██║
    ███████║███████╗   ██║      ██║   ███████╗██║  ██║███████║
    ╚══════╝╚══════╝   ╚═╝      ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝

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
    Store variables with their corresponding bit value
*/
void NetParser::setVarBit(string netType, char signType, int bit, string var)
{
    variableInfo info;
    info.netType = netType;
    info.signType = signType;
    info.bitWidth = bit;
    this->variableBits[var] = info;
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

/*
    The getter below is to retrieve the unordered map of NetParser
*/
const unordered_map<string, variableInfo>& NetParser::getVariableBits() const {
    return variableBits;
}

/*
    The getter below is to retrieve the largest bit width based on either the input or output
*/
int getMaxBitWidth(int option, vector<string> operands, unordered_map<string, variableInfo> varBits)
{
    /*
        The width of a datapath component (except comparators) should be determined 
        by the size of the output, register, or wire to which the output of the component connects.
    */
    if(option == 1)
    {
        for ( const auto& var : varBits ) // Iterate through the content of the unordered_map
        {
            if (var.first == operands[0]) // Check if the key is equal to the output alias (e.g., the "z" from "z = x + y")
            {
                return var.second.bitWidth; // Return the bit width of the matching operand
            }
        }
    }
    /*
        The width of comparators should be determined by the size of the largest input.
    */
    else if(option == 2)
    {
        int maxBitWidth = 0;

        for (size_t i = 2; i < operands.size(); ++i) // Iterate over each element using an indexed for loop
        {
            const string& operand = operands[i]; // Get the current operand using index i

            for (const auto& var : varBits) // Iterate through the content of the unordered_map
            {
                if (var.first == operand && var.second.bitWidth > maxBitWidth) // Check if the key is equal to the current operand
                {                
                    maxBitWidth = var.second.bitWidth;
                }
            }
        }

        return maxBitWidth; // Return the largest bit width among the matching operands
    }

    return 0;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------- //

/*

    ██████╗██╗  ██╗███████╗ ██████╗██╗  ██╗    ███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
    ██╔════╝██║  ██║██╔════╝██╔════╝██║ ██╔╝    ██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
    ██║     ███████║█████╗  ██║     █████╔╝     █████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
    ██║     ██╔══██║██╔══╝  ██║     ██╔═██╗     ██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
    ╚██████╗██║  ██║███████╗╚██████╗██║  ██╗    ██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
    ╚═════╝╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝    ╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

*/
bool checkBitWidth(const string& input) // Check if the bitwidth is valid
{
    regex pattern("(Int|UInt)([0-9]+)"); // Define the pattern to match
    smatch matches; // Store any matches from the regex

    if (regex_search(input, matches, pattern)) { // Check whether the input matches the pattern

        int numBits = stoi(matches[2].str()); // Extract the number of bits as an integer

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

bool checkOutput(string line, NetParser& netParser) // Check whether the current line of operation requires additional wire and register to be created
{
    istringstream ss(line); // Create a string stream to read
    string outputVar;
    string token; // Store the token
    char signType; // Store the sign type

    ss >> outputVar;

    int tokenCount = 0; // Count the tokens inside the string
    while (getline(ss, token, ' '))
    {
        tokenCount++;
    }

    // Check the number of tokens
    if( tokenCount != 3 )
    {
        for ( const auto& var : netParser.getVariableBits() ) // Iterate through the content of the unordered_map
        {
            // Check if the key is equal to the first token "output"
            if (var.first == outputVar && var.second.netType == "output")
            {
                /*
                    the bit width is subtracted by 1 because that is how it will be used in the Verilog code (e.g., Int64 becomes [63:0] in Verilog)

                    the variable is concatenated with a string called "wire" to differentiate between the wire and register aliases
                */
                netParser.setVarBit("wire", signType, var.second.bitWidth, var.first+"wire");
                netParser.setWire(SetNet("wire", var.second.bitWidth, var.first+"wire"));

                return true;
            }
        }
    }

    return false;
}

bool isSigned(vector<string> operands, unordered_map<string, variableInfo> varBits) // Check whether any of the inputs are signed
{
    for (size_t i = 1; i < operands.size(); ++i)
    {
        for ( const auto& var : varBits ) // Iterate through the content of the unordered_map
        {
            if (var.first == operands[i] && var.second.signType == 's') // Check if the key is equal to the output alias (e.g., the "z" from "z = x + y")
            {
                return true; // Return the bit width of the matching operand
            }
        }
    }

    return false;
}

/*

    ███╗   ███╗██╗███████╗ ██████╗    ███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
    ████╗ ████║██║██╔════╝██╔════╝    ██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
    ██╔████╔██║██║███████╗██║         █████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
    ██║╚██╔╝██║██║╚════██║██║         ██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
    ██║ ╚═╝ ██║██║███████║╚██████╗    ██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
    ╚═╝     ╚═╝╚═╝╚══════╝ ╚═════╝    ╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

*/
void createRegister(string line, NetParser& np)
{
    istringstream opStream(line); // Initialize a stream from a string and then parse it (purposely for >>)
	string tempOp; // Declare string variable to store token
	vector<string> tempOps; // Vector to store dynamically created string variables
    int tokenCount = 0; // Count number of tokens

	while(opStream >> tempOp) // Loop while opStream is not empty
	{
		if(tempOp != "=" && tempOp != ":") // Skip over any token that is an equal sign
		{
        	tempOps.push_back(tempOp); // Store the token in the vector
		}
        tokenCount++; // Increment the counter
    }
    vector<string> tempVec {tempOps[0], tempOps[0]+"wire"}; // Temporary vector
    np.setOperation(SetOp("REG",tempVec)); // Create the register operation

    return;
}

vector<string> checkSignedPadding(int maxBitWidth, string opName, vector<string> operands, unordered_map<string, variableInfo> varBits)
{
    vector<string> tempOps; // To store temporary operands to be returned

    // cout << "My operands: ";
    // for (const string& str : operands) {
    //     cout << str << ",";
    // }
    // cout << "\n" << endl;

    tempOps.push_back(operands[0]); // Store the output variable

    for (size_t i = 1; i < operands.size(); ++i)
    {
        if(i == 1 && opName == "MUX")
        {
            tempOps.push_back(operands[i]); // Store the variable
            continue; // Skip this operand if the operation is a multiplexer
        }
        for (const auto& var : varBits)
        {
            if(operands[i] == var.first) // Find the bit width of the output variable
            {
                if(var.second.bitWidth < maxBitWidth)
                {
                    tempOps.push_back("{{" + to_string(maxBitWidth-var.second.bitWidth) + "{" + operands[i] + "[" + to_string(maxBitWidth-var.second.bitWidth-1) + "]}}," + operands[i] + "}"); // Store the padded operand
                    break; // Exit the loop
                }
                else
                {
                    tempOps.push_back(operands[i]); // Else store the operand as it is
                    break; // Exit the loop
                }
            }
        }
    }

    return tempOps;
}

vector<string> checkUnsignedPadding(int maxBitWidth, string opName, vector<string> operands, unordered_map<string, variableInfo> varBits)
{
    vector<string> tempOps; // To store temporary operands to be returned

    // cout << "My operands: ";
    // for (const string& str : operands) {
    //     cout << str << ",";
    // }
    // cout << "\n" << endl;

    tempOps.push_back(operands[0]); // Store the output variable

    for (size_t i = 1; i < operands.size(); ++i)
    {
        if(i == 1 && opName == "MUX")
        {
            tempOps.push_back(operands[i]); // Store the variable
            continue; // Skip this operand if the operation is a multiplexer
        }
        for (const auto& var : varBits)
        {
            if(operands[i] == var.first) // Find the bit width of the output variable
            {
                if(var.second.bitWidth < maxBitWidth)
                {
                    tempOps.push_back("{" + to_string(maxBitWidth-var.second.bitWidth-1) + "'b0, " + operands[i] + "}"); // Store the padded operand
                    break; // Exit the loop
                }
                else
                {
                    tempOps.push_back(operands[i]); // Else store the operand as it is
                    break; // Exit the loop
                }
            }
        }
    }

    return tempOps;
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

    unordered_map<string, variableInfo> varBits = netParser.getVariableBits(); // Get the collection of variables

    // Write the time unit and module header to the output file 
	file << "`timescale 1ns / 1ps" << "\n" << endl;
	file << "module " << verilogFile.substr(0, verilogFile.find('.')) << " (" << endl;
    file << "\t" << "input Clk, Rst," << endl;

    if(!inputs.empty()) // Check whether the input set is empty
    {
        for (const SetNet& input : inputs) // Iterate through each object in the referenced vector
        {
            input.printInput(file); // Print out each object to the output file
        }
    }
    
    if(!outputs.empty()) // Check whether the output set is empty
    {
        for (const SetNet& output : outputs) // Iterate through each object in the referenced vector
        {
            output.printOutput(file); // Print out each object to the output file
            /*
                The code below is necessary because in the Verilog code the last declaration of net does not have a comma
                For example,
                module Circuit4 (
                    input [63:0] a, b, c,
                    input Clk, Rst,
                    output [31:0] z, x    <- no comma here
                );
            */
            if ( outputs.size() != 1 || &output != &outputs.back() )
            { // Check whether current output is the last element of outputs
                file << "," << endl;
            }
            
        }
    }
    file << "\n" << ");" << endl;

    if(!wires.empty()) // Check whether the wire set is empty
    {
        vector<string> oneBitVars;
        for (const SetNet& wire : wires) // Loop through each wire object
        {
            wire.printWire(file, operations);
        }
        file << endl;
    }

    if(!registers.empty()) // Check whether the register set is empty
    {
        for (const SetNet& reg : registers) // Loop through each register object
        {
            reg.printRegister(file); // Write each register to the output file
        }
        file << endl;
    }
    
    if(!operations.empty()) // Check whether the operation set is empty
    {
        unordered_map<string, int> operationCounts = {
            {"ADD", 0},
            {"SUB", 0},
            {"MUL", 0},
            {"COMP", 0},
            {"MUX", 0},
            {"SHR", 0},
            {"SHL", 0},
            {"REG", 0}
        };

        for (size_t index = 0; index < operations.size(); ++index) // Loop through each operation object
        {
            const SetOp& operation = operations[index]; // Store a single operation object

            if(operation.getOpName() == "ADD")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "SUB")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "MUL")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "GT")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "LT")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "EQ")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "MUX")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "SHR")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "SHL")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
            else if(operation.getOpName() == "REG")
            {
                operationCounts[operation.getOpName()] += 1;
                // The 'index' is used as a unique ID for the created module
                operation.printOperation(file, operationCounts[operation.getOpName()], varBits); // Write each operation to the output file
            }
        }
    }

	file << "\n" << "endmodule";

	return;
}


/*
    Print each net types to the output file
*/
void SetNet::printInput(ofstream& file) const
{
    file << "\t" << this->getNetType() << " [" << this->getBitWidth()-1 << ":0] " << this->getVarNames() << "," << endl;
    return;
}

void SetNet::printOutput(ofstream& file) const
{
    file << "\t" << this->getNetType() << " [" << this->getBitWidth()-1 << ":0] " << this->getVarNames();
    return;
}

void SetNet::printWire(ofstream& file, vector<SetOp> ops) const
{
    istringstream ss(this->getVarNames());
    vector<string> vars; // Vector to store dynamically created string variables
    string var; // Store a single variable
    vector<string> oneBitVars; // Store variables that only require a single bit
    string str1; // Store the one bit variables
    string str2; // Store the multi-bit variables
    bool isOneBit = false;

    while ( getline(ss, var, ',') ) // Loop the variables
    {
        // Trim leading and trailing whitespaces from var
        var.erase(0, var.find_first_not_of(" \t\r\n"));
        var.erase(var.find_last_not_of(" \t\r\n") + 1);

        vars.push_back(var); // Store the token in the vector
    }

    for ( auto it = vars.begin(); it != vars.end(); ) // Separate any wire that is only required one bit of width from the multi-bit width
    {
        const string& currentVar = *it; // Assigns a constant reference to the current string element pointed to by the iterator 

        for (const SetOp& operation : ops) // Iterate through the referenced 'operations' vector
        {
            const vector<string>& operands = operation.getOperands(); // Access the vector of operands for each SetOp object

            if (operands[1] == currentVar && operation.getOpName() == "MUX")
            {
                oneBitVars.push_back(currentVar); // Store 'currentVar' to the 'oneBitVars' vector
                it = vars.erase(it);  // Remove 'currentVar' from 'vars' and update the iterator
                isOneBit = true; // Set to true if the operand satisfies the condition
                break;
            }
        }
        if(isOneBit) // If a match is found for the operand, exit the loop
        {
            break;
        }
        ++it;
    }


    if (!oneBitVars.empty()) // Check whether there are operands with one bit width
    {
        for (size_t i = 0; i < oneBitVars.size(); ++i) // Store the one bit variables into a single string
        {
            str1 += oneBitVars[i];
            if (i != oneBitVars.size() - 1) { // Add comma if not the last element
                str1 += ",";
            }
        }

        file << "\t" << this->getNetType() << " " << str1 << ";" << endl; // Write the one bit variable into the output file

        for (size_t i = 0; i < vars.size(); ++i) // Store the multi-bit variables into a single string
        {
            str2 += vars[i];
            if (i != vars.size() - 1) { // Add comma if not the last element
                str2 += ",";
            }
        }

        file << "\t" << this->getNetType() << " [" << this->getBitWidth()-1 << ":0] " << str2 << ";" << endl; // Write the multi-bit variable into the output file
    } else
    {
        file << "\t" << this->getNetType() << " [" << this->getBitWidth()-1 << ":0] " << this->getVarNames() << ";" << endl; // Write the multi-bit variable into the output file
    }

    return;
}

void SetNet::printRegister(ofstream& file) const
{
    file << "\t" << this->getNetType() << " [" << this->getBitWidth()-1 << ":0] " << this->getVarNames() << ";" << endl;
    return;
}

void SetOp::printOperation(ofstream& file, int indexOp, unordered_map<string, variableInfo> varBits) const
{
    int maxBitWidth; // To store the maximum bit width
    bool signType = false; // To store boolean value for the sign type
    vector<string> operands; // To store operands

    signType = isSigned(this->getOperands(), varBits); // Check whether the operands cause the module to be signed or unsigned

    if( this->getOpName() == "ADD")
    {
        maxBitWidth = getMaxBitWidth(1, this->getOperands(), varBits); // Get the maximum bit width for the module based on the output

        /*
            Following the format:  ADD #(.DATAWIDTH(8)) ADD1(a, b, d); // d = a + b
        */
        if(signType) // Check if the operation is a signed type
        {
            operands = checkSignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "S" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[1] << ", " << operands[2] << ", " << operands[0] <<");" << endl;
        }
        else
        {
            operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[1] << ", " << operands[2] << ", " << operands[0] << ");" << endl;
        }
    }
    else if( this->getOpName() == "SUB")
    {
        maxBitWidth = getMaxBitWidth(1, this->getOperands(), varBits); // Get the maximum bit width for the module based on the output

        /*
            Following the format: SUB #(.DATAWIDTH(16)) SUB1 (f, d, xwire); // xwire = f - d
        */
        if(signType) // Check if the operation is a signed type
        {
            operands = checkSignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "S" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[1] << ", " << operands[2] << ", " << operands[0] << ");" << endl;
        }
        else
        {
            operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "("<< this->operands[1] << ", " << this->operands[2] << ", " << this->operands[0] << ");" << endl;
        }
    }
    else if( this->getOpName() == "MUL")
    {
        maxBitWidth = getMaxBitWidth(1, this->getOperands(), varBits); // Get the maximum bit width for the module based on the output

        /*
            Following the format: MUL #(.DATAWIDTH(16)) MUL1 (a, c, f); // f = a * c
        */
        if(signType) // Check if the operation is a signed type
        {
            operands = checkSignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "S" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[1] << ", " << operands[2] << ", " << operands[0] << ");" << endl;
        }
        else
        {
            operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[1] << ", " << operands[2] << ", " << operands[0] << ") " << ");" << endl;
        }
    }
    else if( this->getOpName() == "GT")
    {
        maxBitWidth = getMaxBitWidth(2, this->getOperands(), varBits); // Get the maximum bit width for the module based on the largest input size

        /*
            Following the format: COMP #(.DATAWIDTH(32)) COMP_2(d, e, bGTc, 1'b0, 1'b0); // dLTe = b > c
        */
        if(signType) // Check if the operation is a signed type
        {
            operands = checkSignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "S" << "COMP" << " #(.DATAWIDTH(" << maxBitWidth << ")) " << "COMP" << indexOp << "(" << operands[1] << ", " << operands[2] << ", " << operands[0] << ", 1\'b0, 1\'b0" << ");" << endl;
        }
        else
        {
            operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "COMP" << " #(.DATAWIDTH(" << maxBitWidth << ")) " << "COMP" << indexOp << "(" << operands[1] << ", " << operands[2] << ", " << operands[0] << ", 1\'b0, 1\'b0" << ");" << endl;
        }
    }
    else if( this->getOpName() == "LT")
    {
        maxBitWidth = getMaxBitWidth(2, this->getOperands(), varBits); // Get the maximum bit width for the module based on the largest input size

        /*
            Following the format: COMP #(.DATAWIDTH(32)) COMP_2(d, e, 1'b0, dLTe, 1'b0); // dLTe = d < e
        */
        if(signType) // Check if the operation is a signed type
        {
            operands = checkSignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "S" << "COMP" << " #(.DATAWIDTH(" << maxBitWidth << ")) " << "COMP" << indexOp << "(" << operands[1] << ", " << operands[2] << ",  1\'b0, " << operands[0] << ", 1\'b0" << ");" << endl;
        }
        else
        {
            operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "COMP" << " #(.DATAWIDTH(" << maxBitWidth << ")) " << "COMP" << indexOp << "(" << operands[1] << ", " << operands[2] << ",  1\'b0, " << operands[0] << ", 1\'b0" << ");" << endl;
        }
    }
    else if( this->getOpName() == "EQ")
    {
        maxBitWidth = getMaxBitWidth(2, this->getOperands(), varBits); // Get the maximum bit width for the module based on the largest input size

        /*
            Following the format: COMP #(.DATAWIDTH(32)) COMP_2(d, e, 1'b0, 1'b0, fEQg); // dLTe = f == g
        */
        if(signType) // Check if the operation is a signed type
        {
            operands = checkSignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "S" << "COMP" << " #(.DATAWIDTH(" << maxBitWidth << ")) " << "COMP" << indexOp << "(" << operands[1] << ", " << operands[2] << ",  1\'b0, 1\'b0, " << operands[0] << ");" << endl;
        }
        else
        {
            operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "COMP" << " #(.DATAWIDTH(" << maxBitWidth << ")) " << "COMP" << indexOp << "(" << operands[1] << ", " << operands[2] << ",  1\'b0, 1\'b0, " << operands[0] << ");" << endl;
        }
    }
    else if( this->getOpName() == "MUX")
    {
        maxBitWidth = getMaxBitWidth(1, this->getOperands(), varBits); // Get the maximum bit width for the module based on the output

        /*
            Following the format: MUX2x1 #(.DATAWIDTH(32)) MUX_1(d, e, dLTe, g); // g = dLTe ? d : e
        */
        if(signType) // Check if the operation is a signed type
        {
            operands = checkSignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << "S" << this->getOpName() << "2x1 #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[2] << ", " << operands[3] << ", " << operands[1] << ", " << operands[0] << ");" << endl;
        }
        else
        {
            operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
            file << "\t" << this->getOpName() << "2x1 #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[2] << ", " << operands[3] << ", " << operands[1] << ", " << operands[0] << ");" << endl;
        }
    }
    else if( this->getOpName() == "SHR")
    {
        maxBitWidth = getMaxBitWidth(1, this->getOperands(), varBits); // Get the maximum bit width for the module based on the output

        operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
        /*
            Following the format: SHR #(.DATAWIDTH(32)) SHR2(l2div2, l2div4, sa); // l2div4 = l2div2 >> sa
        */
        file << "\t" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[1] << ", " << operands[0] << ", " << operands[2] << ");" << endl;
    }
    else if( this->getOpName() == "SHL")
    {
        maxBitWidth = getMaxBitWidth(1, this->getOperands(), varBits); // Get the maximum bit width for the module based on the output

        operands = checkUnsignedPadding(maxBitWidth, this->getOpName(), this->getOperands(), varBits); // Check whether the operands need padding
        /*
            Following the format: SHL #(.DATAWIDTH(32)) SHL_1(g, xwire, {31'b0, dLTe}); // xwire = g << dLTe
        */
        file << "\t" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << operands[1] << ", " << operands[0] << ", " << operands[2] << ");" << endl;
    }
    else if( this->getOpName() == "REG" )
    {
        maxBitWidth = getMaxBitWidth(1, this->getOperands(), varBits); // Get the maximum bit width for the module based on the output

        /*
            Following the format: REG #(.DATAWIDTH(32)) REG_2(zwire, Clk, Rst, z); // z = zwire
        */
        if(signType) // Check if the operation is a signed type
        {
            file << "\t" << "S" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << this->getOperands()[1] << ", Clk, Rst, " << this->getOperands()[0] << ");" << endl;
        }
        else
        {
            file << "\t" << this->getOpName() << " #(.DATAWIDTH(" << maxBitWidth << ")) " << this->getOpName() << indexOp << "(" << this->getOperands()[1] << ", Clk, Rst, " << this->getOperands()[0] << ");" << endl;
        }
    }
    
    return;
}


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
SetNet parseInput(string inputString, NetParser& np) // Tokenize the input string, retaining only the utilized tokens
{
    vector<string> netInput; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})
    char signType; // Temporarily store the sign type

    size_t typeEnd = inputString.find(' '); // Finds the index of the first encountered whitespace character
    netInput.push_back(inputString.substr(typeEnd + 1, inputString.find(' ', typeEnd + 1) - typeEnd - 1)); // Find and extract the bitwidth (e.g., "Int8" from "input Int8 a, b, c") and push into the vector

    size_t variablesStart = inputString.find(' ', typeEnd + 1); // Finds the next encountered whitespace character after typeEnd+1 index
    netInput.push_back(inputString.substr(variablesStart + 1)); // Extract the variables (e.g., "a, b , c" from "input Int8 a, b, c") and push into the vector

    // Determine sign type
    if (netInput[0].find('U') != string::npos) { // Check if the character 'U' is found in the string
        signType = 'u'; // Unsigned datatype
    } else {
        signType = 's'; // Signed datatype
    }

    /*
        The code below (excluding the return) is to extract each variable along their net type, sign type, and bitwidth 
        which is then stored in a vector in the NetParser object
    */
    // Extract the number associated with the type
    string type = netInput[0];
    int bitValue;
    if (type.substr(0, 3) == "Int")
    {
        bitValue = stoi(type.substr(3)); // Extract the bit-value from "Int[bit-value]"
    } else if (type.substr(0, 4) == "UInt")
    {
        bitValue = stoi(type.substr(4)); // Extract the bit-value from "UInt[bit-value]"
    }

     // Extract variable names and store in variableBits
    stringstream ss(netInput[1]); // Selects the element of the bitwidth (e.g., "Int8")
    string variableName;
    while (getline(ss, variableName, ','))
    {
        // Trim leading and trailing whitespaces from variableName
        variableName.erase(0, variableName.find_first_not_of(" \t\r\n"));
        variableName.erase(variableName.find_last_not_of(" \t\r\n") + 1);

        /*
            The unordered map of setVarBit should, for example, contain the following mapping:
                setVarBit["a"] = 8;
                setVarBit["b"] = 8;
                setVarBit["c"] = 8;
        */
        np.setVarBit("input", signType, bitValue, variableName); // Store variableName in variableBits with associated bitwidth
    }

    /*
        The returned object will contain for example, "input", 8, 's', "a, b, c"
    */
	return SetNet("input", bitValue, netInput[1]); // Return this temporary initialized object
}

SetNet parseOutput(string outputString, NetParser& np) // Tokenize the output string, retaining only the utilized tokens
{
    vector<string> netOutput; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})
    char signType; // Temporarily store the sign type

    size_t typeEnd = outputString.find(' '); // Finds the index of the first encountered whitespace character
    netOutput.push_back(outputString.substr(typeEnd + 1, outputString.find(' ', typeEnd + 1) - typeEnd - 1)); // Find and extract the bitwidth (e.g., "Int8" from "output Int8 a, b, c") and push into the vector

    size_t variablesStart = outputString.find(' ', typeEnd + 1); // Finds the next encountered whitespace character after typeEnd+1 index
    netOutput.push_back(outputString.substr(variablesStart + 1)); // Extract the variables (e.g., "a, b , c" from "output Int8 a, b, c") and push into the vector

    // Determine sign type
    if (netOutput[0].find('U') != string::npos) { // Check if the character 'U' is found in the string
        signType = 'u'; // Unsigned datatype
    } else {
        signType = 's'; // Signed datatype
    }

    /*
        The code below (excluding the return) is to extract each variable along their net type, sign type, and bitwidth 
        which is then stored in a vector in the NetParser object
    */
    // Extract the number associated with the type
    string type = netOutput[0]; // Selects the element of the bitwidth (e.g., "Int8")
    int bitValue;
    if (type.substr(0, 3) == "Int")
    {
        bitValue = stoi(type.substr(3)); // Extract the bit-value from "Int[bit-value]"
    } else if (type.substr(0, 4) == "UInt")
    {
        bitValue = stoi(type.substr(4)); // Extract the bit-value from "UInt[bit-value]"
    }

     // Extract variable names and store in variableBits
    stringstream ss(netOutput[1]); // Bitstream the variables in netOutput[1] (e.g., "a, b, c")
    string variableName;
    while (getline(ss, variableName, ','))
    {
        // Trim leading and trailing whitespaces from variableName
        variableName.erase(0, variableName.find_first_not_of(" \t\r\n"));
        variableName.erase(variableName.find_last_not_of(" \t\r\n") + 1);

        /*
            The unordered map of setVarBit should, for example, contain the following mapping:
                setVarBit["a"] = 8;
                setVarBit["b"] = 8;
                setVarBit["c"] = 8;
        */
        np.setVarBit("output", signType, bitValue, variableName); // Store variableName in variableBits with associated bitwidth
    }

    /*
        The returned object will contain for example, "output", 8, 's', "a, b, c"
    */
	return SetNet("output", bitValue, netOutput[1]); // Return this temporary initialized object
}

SetNet parseWire(string wireString, NetParser& np) // Tokenize the wire string, retaining only the utilized tokens
{
    vector<string> netWire; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})
    char signType; // Temporarily store the sign type

    size_t typeEnd = wireString.find(' '); // Find the end of the type
    netWire.push_back(wireString.substr(typeEnd + 1, wireString.find(' ', typeEnd + 1) - typeEnd - 1)); // Extract the type

    size_t variablesStart = wireString.find(' ', typeEnd + 1); // Find the start of the variables
    netWire.push_back(wireString.substr(variablesStart + 1)); // Extract the variables

    // Determine sign type
    if (netWire[0].find('U') != string::npos) { // Check if the character 'U' is found in the string
        signType = 'u'; // Unsigned datatype
    } else {
        signType = 's'; // Signed datatype
    }

    /*
        The code below (excluding the return) is to extract each variable along their net type, sign type, and bitwidth 
        which is then stored in a vector in the NetParser object
    */
    // Extract the number associated with the type
    string type = netWire[0];
    int bitValue;
    if (type.substr(0, 3) == "Int")
    {
        bitValue = stoi(type.substr(3)); // Extract the bit-value from "Int[bit-value]"
    } else if (type.substr(0, 4) == "UInt")
    {
        bitValue = stoi(type.substr(4)); // Extract the bit-value from "UInt[bit-value]"
    }

     // Extract variable names and store in variableBits
    stringstream ss(netWire[1]); // Selects the element of the bitwidth (e.g., "Int8")
    string variableName;
    while (getline(ss, variableName, ','))
    {
        // Trim leading and trailing whitespaces from variableName
        variableName.erase(0, variableName.find_first_not_of(" \t\r\n"));
        variableName.erase(variableName.find_last_not_of(" \t\r\n") + 1);

        /*
            The unordered map of setVarBit should, for example, contain the following mapping:
                setVarBit["a"] = {"wire", 'u', 8};
                setVarBit["b"] = {"wire", 'u', 8};
                setVarBit["c"] = {"wire", 'u', 8};
        */
        np.setVarBit("wire", signType, bitValue, variableName); // Store variableName in variableBits with associated bitwidth
    }

    /*
        The returned object will contain for example, "wire", 8, 's', "a, b, c"
    */
	return SetNet("wire", bitValue, netWire[1]); // Return this temporary initialized object
}

SetNet parseRegister(string regString, NetParser& np) // Tokenize the register string, retaining only the utilized tokens
{
    vector<string> netReg; // Store the net-type and variable names (e.g., {"Int32", "a, b, c"})
    char signType; // Temporarily store the sign type

    size_t typeEnd = regString.find(' '); // Find the end of the type
    netReg.push_back(regString.substr(typeEnd + 1, regString.find(' ', typeEnd + 1) - typeEnd - 1)); // Extract the type

    size_t variablesStart = regString.find(' ', typeEnd + 1); // Find the start of the variables
    netReg.push_back(regString.substr(variablesStart + 1)); // Extract the variables

    // Determine sign type
    if (netReg[0].find('U') != string::npos) { // Check if the character 'U' is found in the string
        signType = 'u'; // Unsigned datatype
    } else {
        signType = 's'; // Signed datatype
    }

    /*
        The code below (excluding the return) is to extract each variable along their net type, sign type, and bitwidth 
        which is then stored in a vector in the NetParser object
    */
    // Extract the number associated with the type
    string type = netReg[0];
    int bitValue;
    if (type.substr(0, 3) == "Int")
    {
        bitValue = stoi(type.substr(3)); // Extract the bit-value from "Int[bit-value]"
    } else if (type.substr(0, 4) == "UInt")
    {
        bitValue = stoi(type.substr(4)); // Extract the bit-value from "UInt[bit-value]"
    }

     // Extract variable names and store in variableBits
    stringstream ss(netReg[1]); // Selects the element of the bitwidth (e.g., "Int8")
    string variableName;
    while (getline(ss, variableName, ','))
    {
        // Trim leading and trailing whitespaces from variableName
        variableName.erase(0, variableName.find_first_not_of(" \t\r\n"));
        variableName.erase(variableName.find_last_not_of(" \t\r\n") + 1);

        /*
            The unordered map of setVarBit should, for example, contain the following mapping:
                setVarBit["a"] = {"reg", 'u', 8};
                setVarBit["b"] = {"reg", 'u', 8};
                setVarBit["c"] = {"reg", 'u', 8};
        */
        np.setVarBit("reg", signType, bitValue, variableName); // Store variableName in variableBits with associated bitwidth
    }

    /*
        The returned object will contain for example, "wire", 8, 's', "a, b, c"
    */
	return SetNet("wire", bitValue, netReg[1]); // Return this temporary initialized object
}

SetOp parseOperation(string opString, bool createReg) // Tokenize the operation string, retaining only the utilized tokens
{
	istringstream opStream(opString); // Initialize a stream from a string and then parse it (purposely for >>)
	string tempOp; // Declare string variable to store token
	vector<string> tempOps; // Vector to store dynamically created string variables
    int tokenCount = 0; // Count number of tokens

	while(opStream >> tempOp) // Loop while opStream is not empty
	{
		if(tempOp != "=" && tempOp != ":") // Skip over any token that is an equal sign
		{
        	tempOps.push_back(tempOp); // Store the token in the vector
		}
        tokenCount++; // Increment the counter
    }

    if(createReg) // Check whether a new register needs to be created
    {
        tempOps[0] += "wire";
    }

	// Index starts [0]
    if(tokenCount == 3)
    {
        return SetOp("REG",tempOps);
    }
	else if(tempOps[2] == ADD) // Check if the element pointed by this index is an addition operator
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
	else if(tempOps[2] == MUX) // Check if the element pointed by this index is an equal to operator
	{
		return SetOp("MUX",tempOps);
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


//------------------------------------------------------------------------------------------------------------------------------------------------------------- //

/*

    _______ _______ _______  ______ _______      _     _ _______  ______ _______
    |______    |    |_____| |_____/    |         |_____| |______ |_____/ |______
    ______|    |    |     | |    \_    |         |     | |______ |    \_ |______

*/

// Perform conversion from behavior netlist text file to Verilog file
bool NetParser::convertToVerilog(string inputFile, string outputFile)
{
    /*

        ███████╗██╗██╗  ████████╗███████╗██████╗      █████╗ ███╗   ██╗██████╗     ███████╗████████╗ ██████╗ ██████╗ ███████╗    ██╗███╗   ██╗██████╗ ██╗   ██╗████████╗
        ██╔════╝██║██║  ╚══██╔══╝██╔════╝██╔══██╗    ██╔══██╗████╗  ██║██╔══██╗    ██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝    ██║████╗  ██║██╔══██╗██║   ██║╚══██╔══╝
        █████╗  ██║██║     ██║   █████╗  ██████╔╝    ███████║██╔██╗ ██║██║  ██║    ███████╗   ██║   ██║   ██║██████╔╝█████╗      ██║██╔██╗ ██║██████╔╝██║   ██║   ██║   
        ██╔══╝  ██║██║     ██║   ██╔══╝  ██╔══██╗    ██╔══██║██║╚██╗██║██║  ██║    ╚════██║   ██║   ██║   ██║██╔══██╗██╔══╝      ██║██║╚██╗██║██╔═══╝ ██║   ██║   ██║   
        ██║     ██║███████╗██║   ███████╗██║  ██║    ██║  ██║██║ ╚████║██████╔╝    ███████║   ██║   ╚██████╔╝██║  ██║███████╗    ██║██║ ╚████║██║     ╚██████╔╝   ██║   
        ╚═╝     ╚═╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝     ╚══════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝    ╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝    ╚═╝   

    */

	ifstream netlistFile(inputFile);

	vector<string> text_lines; // Declare a vector of words for each line
	string line; // // Declare a string to one line

    // Loop through each line in netlistFile
    while ( getline(netlistFile, line) ) // Delimiter '\n' is explicitly passed here just in case
    {
        // Skip processing if the line is empty
        if (!line.empty())
        {
            size_t pos = line.find("//");
            // Check if "//" is present in the string
            if (pos != string::npos)
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
                return false; // Exit program
            }

            // Define the set of whitespace characters to search
            string whitespaceChars = " \t\f\v\n\r";

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
            while (!line.empty() && whitespaceChars.find(line.back()) != string::npos) {
                // The last character is a whitespace character
                line.pop_back(); // Remove the last character
            }
            /*
                Red Hat Linux issues a warning if this is not put in, for some reason.
                Though this checking whether the line is empty is already done above on the outer if statement
            */
            if(!line.empty())
            {
                text_lines.push_back(line); // After filtering, "line" is added to the vector of strings
            }
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
        string bitWidth;

        lineStream >> netType >> bitWidth; // Store the first token in netType
        
        if( netType.compare(INPUT) == 0 && checkBitWidth(bitWidth) ) // Check whether the extracted token is equal to INPUT
        {
            netParser.setInput(parseInput(line, netParser)); // Pass the string in the current line to the function
        }
        else if( netType.compare(OUTPUT) == 0 && checkBitWidth(bitWidth)) // Check whether the extracted token is equal to INPUT
        {
            netParser.setOutput(parseOutput(line, netParser)); // Pass the string in the current line to the function
        }
        else if( netType.compare(WIRE) == 0 && checkBitWidth(bitWidth) ) // Check whether the extracted token is equal to INPUT
        {
            netParser.setWire(parseWire(line, netParser)); // Pass the string in the current line to the function
        }
        else if( netType.compare(REGISTER) == 0 && checkBitWidth(bitWidth) ) // Check whether the extracted token is equal to INPUT
        {
            netParser.setRegister(parseRegister(line, netParser)); // Pass the string in the current line to the function
        }
        else // Check if current line is an operation expression
        {
            bool createReg = checkOutput(line, netParser);
            netParser.setOperation(parseOperation(line, createReg)); // Pass the string in the current line to the function
            if(createReg) // Checks if register needs to be created
            {
                createRegister(line, netParser); 
            }
        }
    }

    // // Output each string in the vector
    // for (const string& lain : text_lines) {
    //     cout << lain << endl;
    //     verilogFile << lain << endl;
    // }

    // for (const auto& wire : netParser.getOperations() ) {
    //     cout << "OpName: " << wire.getOpName() << endl;
    //     // Iterate through the vector using a range-based for loop
    //     cout << "Operands: ";
    //     for (const auto& op : wire.getOperands()) {
    //         cout << op << ",";
    //     }
    //     cout << "\n" << endl;
    // }

    // for (const auto& wire : netParser.getRegisters() ) {
    //     cout << "NetType: " << wire.getNetType() << ", Bitwidth: " << wire.getBitWidth() << ", VarNames: " << wire.getVarNames() << endl;
    // }

    // unordered_map<string, variableInfo> temp = netParser.getVariableBits();

    // for (const auto& pair : temp) {
    //     cout << "Key: " << pair.first << ", NetType: " << pair.second.netType << ", SignType: " << pair.second.signType << ", BitWidth: " << pair.second.bitWidth << endl;
    // }

    writeToOutput(outputFile, netParser); // Do the conversion and write the result to the output file

    return true;
}


#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <map>

/*
    A directive that allows you to use names from the std namespace without prefixing them with ''
    The std namespace contains many standard library components for tasks like I/O operations, string manipulation, and working with containers.
*/
using namespace std;

// Define constants for net types and operators
#define INPUT "input"
#define OUTPUT "output"
#define WIRE "wire"
#define REGISTER "register"
#define EMPTY "\0"
#define ADD "+" // Addition operator constant
#define SUB "-" // Subtraction operator constant
#define MUL "*" // Multiplication operator constant
#define GT ">" // Greater than operator constant
#define LT "<" // Less than operator constant
#define EQ	"==" // Equal operator constant
#define SEL "?" // Multiplexer operator constant
#define SHR ">>" // Shift right operator constant
#define SHL "<<" // Shift left operator constant


// class CheckSyntax
// {
//     private:


//     public:

// };

// Class to store each net type (input, output, wire, register)
class SetNet
{
    private:
        
        string netType; // the type: "i" for input, "o" for output, "w" for wire, "u" for unassigned
        char signType; // "u" for unsigned, "s" for signed
        int bitWidth;  //the number of bits for the variable
        string varNames; //the name of the variable

    public:

        // Default Constructor
        SetNet()
        {
            this->netType = "";
            this->signType = '\0';
            this->bitWidth = 0;
            this->varNames = "";
        }

        // Parameterized Constructor
        SetNet( string netType, string dataType, string var )
        {
            this->netType = netType; // Assign the corresponding net type

            // Determine sign type
            if (dataType.find('U') != string::npos) { // Check if the character 'U' is found in the string
                this->signType = 'u'; // Unsigned datatype
            } else {
                this->signType = 's'; // Signed datatype
            }

            // Extract integer number from "dataType" string
            size_t pos = dataType.find_first_of("0123456789"); // Find first encounter of any of these numbers
            string numberStr = dataType.substr(pos); // Extracts a substring from the string starting from the position "pos" to the end of the string
            /*
                stoi() (A.K.A string to integer) is a standard C++ 
                function used to convert a string representation 
                of an integer to an actual integer value.
            */
            std::map<std::string, int> varMap;

            bitWidth = std::stoi(numberStr) - 1; // Convert string to integer (kind of like type casting) and subtract 1

            this->varNames = var; // Store the variable names as it is (e.g., "a, b, c")

        }

    string getVarNames() const;
    string getNetType() const;
    int getBitWidth() const;
    char getSignType() const;

    void printInput(ofstream& file) const;
    void printOutput(ofstream& file) const;
    void printWire(ofstream& file, NetParser& netParser) const;
    void printRegister(ofstream& file) const;
};

// Class to store each operation
class SetOp
{
    private:
        string netOperator; // Store the net operator
        vector<string> operands; // Store the operands

        // bool isInputExist();
        // bool isOutputExist();

    public:

        // Default Constructor
        SetOp()
        {
            this->netOperator = ""; // Declare empty string
            this->operands = vector<string>(0); // Declare empty vector of strings
        }

        // Parameterized Constructor
        SetOp(string netOperator, vector<string> operands)
        {
            this->netOperator = netOperator; // Assign the type of operator
            this->operands = operands; // Operands used in the operation
            this->operands.erase(this->operands.begin() + 2); // Remove the element at the specified index (i.e., the operators like '+' or '-' or '*' or etc)
        }

        string getOpName() const;
        vector<string> getOperands() const;

        void printOperation(ofstream& file) const;
};

// Class that stores a set of those net types or operations
class NetParser //this class implements not only wires, but also inputs, outputs, and registers
{
    private:

        vector<SetNet> inputs;
		vector<SetNet> outputs;
		vector<SetNet> wires;
		vector<SetNet> registers;
        vector<SetOp> operations;
        
        //map<string, vector<string>> netOperator; // Create a map of strings to a vector strings

    public:

    	void setInput(SetNet input);
		void setOutput(SetNet output);
		void setWire(SetNet wire);
		void setRegister(SetNet reg);
        void setOperation(SetOp op);

        const vector<SetNet>& getInputs() const;
        const vector<SetNet>& getOutputs() const;
        const vector<SetNet>& getWires() const;
        const vector<SetNet>& getRegisters() const;
        const vector<SetOp>& getOperations() const;

        bool convertToVerilog(char* inFile, char* outFile);




};

#endif
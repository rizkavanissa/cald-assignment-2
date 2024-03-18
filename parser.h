#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

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
#define MUX "?" // Multiplexer operator constant
#define SHR ">>" // Shift right operator constant
#define SHL "<<" // Shift left operator constant

// Define a struct to hold variable information
struct variableInfo  // Group several related variables into one place
{
    string netType; // Store the net type (e.g., "input", "output", "wire", "register")
    char signType; // "u" for unsigned, "s" for signed
    int bitWidth; // Bitwidth of the variable
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

            if(netOperator != "REG") // Skip this code if the netOperator is REG because it does not have an operator
            {
                this->operands.erase(this->operands.begin() + 2); // Remove the element at the specified index (i.e., the operators like '+' or '-' or '*' or etc)
            }
        }

        string getOpName() const;
        vector<string> getOperands() const;

        void printOperation(ofstream& file, int indexOp, unordered_map<string, variableInfo> varBits) const;
};

// Class to store each net type (input, output, wire, register)
class SetNet
{
    private:
        
        string netType; // the type: "i" for input, "o" for output, "w" for wire, "u" for unassigned
        int bitWidth; // Bitwidth of the variable
        string varNames; //the name of the variable

    public:

        // Default Constructor
        SetNet()
        {
            this->netType = "";
            this->bitWidth = 0;
            this->varNames = "";
        }

        // Parameterized Constructor
        SetNet( string netType, int bit, string var )
        {
            this->netType = netType; // Assign the corresponding net type
            this->bitWidth = bit; // Store the bit width of te variables
            this->varNames = var; // Store the variable names as it is (e.g., "a, b, c")
        }

    	string getVarNames() const;
		string getNetType() const;
		int getBitWidth() const;

        void printInput(ofstream& file) const;
        void printOutput(ofstream& file) const;
        void printWire(ofstream& file, vector<SetOp> ops) const;
        void printRegister(ofstream& file) const;
};

// Class that stores a set of net types or operations
class NetParser //this class implements not only wires, but also inputs, outputs, and registers
{
    private:

        vector<SetNet> inputs;
		vector<SetNet> outputs;
		vector<SetNet> wires;
		vector<SetNet> registers;
        vector<SetOp> operations;
        
        unordered_map<string, variableInfo> variableBits; // Create an unordered map to store each variable with a vector strings

    public:

    	void setInput(SetNet input);
		void setOutput(SetNet output);
		void setWire(SetNet wire);
		void setRegister(SetNet reg);
        void setOperation(SetOp op);

        void setVarBit(string netType, char signType, int bit, string var);
        const unordered_map<string, variableInfo>& getVariableBits() const;

        const vector<SetNet>& getInputs() const;
        const vector<SetNet>& getOutputs() const;
        const vector<SetNet>& getWires() const;
        const vector<SetNet>& getRegisters() const;
        const vector<SetOp>& getOperations() const;

        bool convertToVerilog(string inputFile, string outputFile);
};

#endif
#include "tables.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

string get_opcode(string str, map<string,string> OpTab){
    return OpTab[str];
    }

map<string,string> construct_optab(){

    map<string, string> OpTab;

    OpTab["ADD"] = "18";
    OpTab["ADDF"] = "58";
    OpTab["ADDR"] = "90";
    OpTab["AND"] = "40";
    OpTab["CLEAR"] = "B4";
    OpTab["COMP"] = "28";
    OpTab["COMPF"] = "88";
    OpTab["COMPR"] = "A0";
    OpTab["DIV"] = "24";
    OpTab["DIVF"] = "64";
    OpTab["DIVR"] = "9C";
    OpTab["FIX"] = "C4";
    OpTab["FLOAT"] = "C0";
    OpTab["HIO"] = "F4";
    OpTab["J"] = "3C";
    OpTab["JEQ"] = "30";
    OpTab["JGT"] = "34";
    OpTab["JLT"] = "38";
    OpTab["JSUB"] = "48";
    OpTab["LDA"] = "00";
    OpTab["LDB"] = "68";
    OpTab["LDCH"] = "50";
    OpTab["LDF"] = "70";
    OpTab["LDL"] = "08";
    OpTab["LDS"] = "6C";
    OpTab["LDT"] = "74";
    OpTab["LDX"] = "04";
    OpTab["LPS"] = "D0";
    OpTab["MUL"] = "20";
    OpTab["MULF"] = "60";
    OpTab["MULR"] = "98";
    OpTab["NORM"] = "C8";
    OpTab["OR"] = "44";
    OpTab["RD"] = "D8";
    OpTab["RMO"] = "AC";
    OpTab["RSUB"] = "4C";
    OpTab["SHIFTL"] = "A4";
    OpTab["SHIFTR"] = "A8";
    OpTab["SIO"] = "F0";
    OpTab["SSK"] = "EC";
    OpTab["STA"] = "0C";
    OpTab["STB"] = "78";
    OpTab["STCH"] = "54";
    OpTab["STF"] = "80";
    OpTab["STI"] = "D4";
    OpTab["STL"] = "14";
    OpTab["STS"] = "7C";
    OpTab["STSW"] = "E8";
    OpTab["STT"] = "84";
    OpTab["STX"] = "10";
    OpTab["SUB"] = "1C";
    OpTab["SUBF"] = "5C";
    OpTab["SUBR"] = "94";
    OpTab["SVC"] = "B0";
    OpTab["TD"] = "E0";
    OpTab["TIO"] = "F8";
    OpTab["TIX"] = "2C";
    OpTab["TIXR"] = "B8";
    OpTab["WD"] = "DC";

    return OpTab;

}

void saveSymbolTableToFile(const map<string, int>& symbolTable, const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    // Write each symbol and its corresponding address to the file
    for (const auto& entry : symbolTable) {
        outputFile << entry.first << " " << entry.second << endl;
    }

    outputFile.close();
}

map<string, int> readSymbolTableFromFile(const string& filename) {
    map<string, int> symbolTable;
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for reading." << endl;
        return symbolTable; // Return empty symbol table if file cannot be opened
    }

    string symbol;
    int address;
    while (inputFile >> symbol >> address) {
        symbolTable[symbol] = address;
    }

    inputFile.close();
    return symbolTable;
}
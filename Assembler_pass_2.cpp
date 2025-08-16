#include <iostream>
#include <fstream>
#include <sstream>
#include<iomanip>
#include <string>
#include <map>
//#include"tables.cpp"
#include "functions2.h"
#include <vector>
using namespace std;


int main() {
    ifstream intermediateFile("intermediate_file.txt");
    ofstream outputFile("output.txt");
    ofstream fout("record.txt");
    string file = "data.txt";
    string text_start_addr;
    string prog_start_addr;
    string line;
    ifstream fin(file);
    getline(fin, line);
    line.resize(6, ' ');
    fout << "H^" << line << "^";
    getline(fin, line);
    stringstream ss;
    ss << uppercase << setw(6) << setfill('0')  << line;
    fout << ss.str()<<"^";
    text_start_addr = ss.str();
    prog_start_addr = ss.str();
    ss.str("");
    getline(fin, line);
    ss << uppercase << setw(6) << setfill('0')  << line;
    fout << ss.str() << endl;
    if (!intermediateFile.is_open() || !outputFile.is_open()) {
        cerr << "Error: Unable to open files." << endl;
        return 1;
    }
    map<string,string> OpTab = construct_optab();
    map<string,int> SymbolTable = readSymbolTableFromFile("symbon_table.txt");
    vector<string> lines;
    string text;
    string text_record;
    string byte;
    int text_length = 0;
//    stringstream ss;
//    string line;
    bool prev = false;
    string locctr = "";
    int indexed = 0;
    while (getline(intermediateFile, line)) {
        indexed = 0;
        lines = split(line,' ');
        locctr = lines[0];
        string loc = lines[1];
        string opcode = lines[2];
        string label;
        if(lines.size() == 3){
            label = "";
        }
        else{
            label = lines[3];
        }
        auto it = OpTab.find(opcode);
        if (it != OpTab.end()) {
                string completeOpcode = "";
                // Opcode found, append value of label from Symbol table
                if(prev){
                    text_start_addr = locctr;
                }
                prev = false;
                if (!label.empty() && label.length() >= 2 && label.substr(label.length() - 2) == ",X") {
                    label.erase(label.length() - 2); // Remove ",X"
                    indexed = 1;
                }
                auto symbolIt = SymbolTable.find(label);
                if (symbolIt != SymbolTable.end()) {
                    // Symbol found in Symbol table, convert its value to hexadecimal string
                    string hexValue;
                    if(indexed == 1){
                        hexValue = intToHexString(symbolIt->second + 32768,4);
                    }
                    else{
                        hexValue = intToHexString(symbolIt->second,4);
                    }
                    // Append the hexadecimal value to the opcode
                    completeOpcode = it->second + hexValue;

                    // Write the complete opcode to the output file
                    outputFile << completeOpcode << endl;
                }
                else if(label == "") {
                    string hex = intToHexString(0,4);
                    completeOpcode = it->second + hex;
                    // Write the complete opcode to the output file
                    outputFile << completeOpcode << endl;
                }
                else {
                    outputFile << "Error: Symbol not found in Symbol table." << endl;
                }
                handleTextRecord(text, text_length, text_start_addr, locctr, completeOpcode, fout);
            } 
            else{
                if (opcode == "BYTE") {
                // Write the last string (label) to the output file
                if(prev){
                    text_start_addr = locctr;
                }
                prev =false;
                byte = processLabel(label, outputFile);
                handleTextRecord(text, text_length, text_start_addr, locctr, byte, fout);
            }
            else if (opcode == "WORD"){
                // Convert the last string (label) to integer, then to hexadecimal chars and write it
                if(prev){
                    text_start_addr = locctr;
                }
                prev = false;
                int intValue = atoi(label.c_str());
                string hexValue = intToHexString(intValue,6);
                outputFile << hexValue << endl;
                handleTextRecord(text, text_length, text_start_addr, locctr, hexValue, fout);
            }
            else if(opcode == "RESB" || opcode == "RESW"){
                if(prev == true){
                    continue;
                }
                else{
                    prev = true;
                    handleTextRecord(text, text_length, text_start_addr, locctr,"", fout, true);
                }
            }
            else if(opcode == "END"){
                outputFile << "END OF FILE" << endl;
            }
            else if(opcode == "START"){
                outputFile << "START OF FILE" << endl;
            }
            else{
                outputFile << "Error: Opcode not found in OpTab." << endl;
            }
        }
    }
    handleTextRecord(text, text_length, text_start_addr, locctr,"", fout, true);

// WRITING END RECORD
    fout << "E^" << prog_start_addr << endl;
    intermediateFile.close();
    outputFile.close();
    fout.clear();
    cout << "Object code to output.txt" << endl;
    cout << "Records written to record.txt" << endl;

    return 0;
}
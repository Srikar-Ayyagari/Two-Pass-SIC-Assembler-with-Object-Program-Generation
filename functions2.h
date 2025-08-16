#include <iostream>
#include <fstream>
#include <sstream>
#include<iomanip>
#include <string>
#include <map>
#include"tables.cpp"
#include <vector>
using namespace std;

// Function to search symbol table for operand
int searchSymbolTable(const map<string, int>& symbolTable, const string& operand) {
    auto it = symbolTable.find(operand);
    if (it != symbolTable.end()) {
        return it->second;
    } else {
        return 0; // Not found, return 0 (assuming 0 represents undefined symbol)
    }
}

string intToHexString(int value, int width) {
    stringstream ss;
    ss << uppercase << setw(width) << setfill('0') << hex << value;
    return ss.str();
}


vector<string> split(string str, char del)
{
      vector<string> v;
      string temp = "";
   
      for(int i=0; i<str.size(); i++)
        {
            if(str[i] != del)
            {
                temp += str[i];
            }
            else
            {
                v.push_back(temp);
                temp = "";
            }
        }
       
      v.push_back(temp);
      return v;
}
string processLabel(const std::string& label, std::ostream& outputFile) {
    if (label.empty()) {
        outputFile << "Empty label" << std::endl;
        return "";
    }
    string extracted = "";
    char firstChar = label[0];
    
    if (firstChar == 'X') {
        // Extracting the substring within single quotes
        size_t startPos = label.find('\'');
        size_t endPos = label.find('\'', startPos + 1);
        
        if (startPos != std::string::npos && endPos != std::string::npos && endPos > startPos) {
            extracted = label.substr(startPos + 1, endPos - startPos - 1);
            outputFile << extracted << std::endl;
        } else {
            outputFile << "Invalid label format" << std::endl;
        }
    } else if (firstChar == 'C') {
        // Convert each character to its ASCII value
        for (size_t i = 2; i < label.size() - 1; ++i) {
            extracted.append(intToHexString((label[i]),2));
        }
        outputFile << extracted << endl;
    } else {
        outputFile << "Unsupported label format" << std::endl;
    }
    return extracted;
}

void readHeader(std::ofstream &fout, string file){
    string line;
    ifstream fin(file);
    getline(fin, line);
    line.resize(6, ' ');
    fout << "H^" << line << "^";
    getline(fin, line);
    stringstream ss;
    ss << uppercase << setw(6) << setfill('0')  << line;
    fout << ss.str()<<"^";
    ss.str("");
    getline(fin, line);
    ss << uppercase << setw(6) << setfill('0')  << line;
    fout << ss.str();
}

void handleTextRecord(string& text, int& text_length, string& text_start_addr, string locctr, const string& completeOpcode, ofstream& fout, bool writeOut = false) {
    stringstream ss;
    if (text_length + completeOpcode.size() <= 60) {
        text_length += completeOpcode.size();
        text.append("^" + completeOpcode);
    } else {
        string text_record = "T^";
        ss << uppercase << setw(6) << setfill('0') << text_start_addr;
        text_record.append(ss.str() + "^");
        ss.str("");
        ss << uppercase << setw(2) << setfill('0') << hex << text_length / 2;
        text_record.append(ss.str());
        text_record.append(text);
        fout << text_record << endl;
        text_record = "";
        text = "";
        text_length = 0;
        text_start_addr = locctr;
        text_length += completeOpcode.size();
        text.append("^" + completeOpcode);
    }
    if(writeOut){
        string text_record = "";
        text_record.append("T^");
        ss.str("");
        ss << uppercase << setw(6) << setfill('0')  << text_start_addr;
        text_record.append(ss.str() + "^");
        ss.str("");
        ss << uppercase << setw(2) << setfill('0') << hex << text_length/2;
        text_record.append(ss.str());
        ss.str("");
        text_record.append(text);
        fout << text_record << endl;
        text_record = "";
        text = "";
        text_length = 0;
        text_start_addr = locctr;

    }
}
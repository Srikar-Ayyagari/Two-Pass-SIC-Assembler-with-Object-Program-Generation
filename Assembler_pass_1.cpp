#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "tables.cpp"
#include "functions.h"

using namespace std;



int main()
{
    int locctr;
    int start_add;
    int prog_len;
    int len;
    string name;
    string line;
    vector<string> inst_fields;
    map <string, int> SymTab;
    map<string, string> OpTab = construct_optab();
    ifstream fin("input.txt");
    ofstream fout("intermediate_file.txt");

    getline(fin, line);
    inst_fields = split(line,' ');
    string label = inst_fields[0];
    name = label;
    string opcode = inst_fields[1];
    string operand = inst_fields[2];
    
    if(opcode == "START"){
        start_add =  hextodec(operand);
        locctr = start_add;
        fout << dectohex(locctr) << " " << line << endl;
        getline(fin, line);
        inst_fields = split(line,' ');
        if(inst_fields[0]!="")
        label = inst_fields[0];
        if(inst_fields[1]!="")
        opcode = inst_fields[1];
        if(inst_fields[2]!="")
        operand = inst_fields[2];
    }
    else{
        locctr = 0;
    }


    while(opcode != "END")
    {
        if (label != "")
        {
            if(SymTab[label]){
                cout << locctr <<  "Error : Duplicate Symbol " << label << endl;
            }
            else{
                SymTab[label] = locctr;
            }
        }
        fout << dectohex(locctr) << " " << line << endl;
        if(get_opcode(opcode, OpTab)!="")
            locctr = locctr + 3;
        else if (opcode == "WORD")
            locctr = locctr + 3;
        else if (opcode == "RESW")
            locctr = locctr + (3*stoi(operand));
        else if (opcode == "RESB")
            locctr = locctr + stoi(operand);
        else if (opcode == "BYTE")
        {
            len = operand.length() - 3; //removing the characters {c ' '}
            if(operand[0] == 'X'){
                len /=2;
            }
            locctr = locctr + len;
        }
        else{
            cout << "Error : Invalid operation code" << endl;
        }
        label = "";
        getline(fin, line);
        inst_fields = split(line,' ');
        int length = inst_fields.size();
        if(length-- &&  inst_fields[0]!="")
        label = inst_fields[0];
        if(length-- &&  inst_fields[1]!="")
        opcode = inst_fields[1];
        if(length-- &&  inst_fields[2]!="")
        operand = inst_fields[2];
    }
    fout << " " << line << endl;

    prog_len = locctr - start_add;

    cout << "Len: " << dectohex(prog_len) << "  StartAdd : " << dectohex(start_add) << " Name : " << name << endl;

    saveSymbolTableToFile(SymTab, "symbon_table.txt");
    cout << "Symbol Table is saved " << endl;
    fin.close();
    fout.close();
    ofstream fout2("data.txt");
    fout2 << name << endl;
    fout2 << dectohex(start_add) << endl;
    fout2 << dectohex(prog_len) << endl;
    return 0;
}
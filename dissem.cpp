#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <stdio.h>
#include <math.h>

using namespace std;

//Helper functons
extern string breakdownLine(string, map<string, string>, map<string, string*>);
extern string header(string);
extern string text(string, map<string, string>, map<string, string*>);
extern string modification(string, string);
extern string end(string, string);
extern map<string, string> addSymbols(map<string, string>, string);
extern map<string, string*> addLiterals(map<string, string*>, string);
extern string addHex(string, string);
extern int hexToDec(string);
extern string decToHex(int);
extern string hexToBin(char);
extern string binToHex(string);
extern string fourHex(string);
extern int negHex(string);
extern string subHex(string, int);

// op code for the corresponding mnemonic
map<string, string> OPS = {
    {"18", "ADD"}, {"58", "ADDF"}, {"40", "AND"}, {"28", "COMP"},
{"88", "COMPF"}, {"24", "DIV"}, {"64", "DIVF"}, {"C4", "FIX"},
{"C0", "FLOAT"}, {"F4", "HIO"}, {"3C", "J"}, {"30", "JEQ"}, {"34", "JGT"}, {"38", "JLT"},
{"48", "JSUB"}, {"00", "LDA"}, {"68", "LDB"}, {"50", "LDCH"}, {"70", "LDF"}, {"08", "LDL"},
{"6C", "LDS"}, {"74", "LDT"}, {"04", "LDX"}, {"D0", "LPS"}, {"20", "MUL"}, {"60", "MULF"},
{"C8", "NORM"}, {"44", "OR"}, {"D8", "RD"}, {"4C", "RSUB"},
{"F0", "SIO"}, {"EC", "SSK"}, {"0C", "STA"}, {"78", "STB"},
{"54", "STCH"}, {"80", "STF"}, {"D4", "STI"}, {"14", "STL"}, {"7C", "STS"}, {"E8", "STSW"},
{"84", "STT"}, {"10", "STX"}, {"1C", "SUB"}, {"5C", "SUBF"},
{"E0", "TD"}, {"F8", "TIO"}, {"2C", "TIX"}, {"DC", "WD"}
};

map<string, string> OPS_2 = {
{"90", "ADDR"}, {"B4", "CLEAR"}, {"A0", "COMPR"}, {"9C", "DIVR"}, {"98", "MULR"},
{"AC", "RMO"}, {"A4", "SHIFTL"}, {"A8", "SHIFTR"}, {"94", "SUBR"}, {"B0", "SVC"},
{"B8", "TIXR"}
};

map<char, string> registers;
// A, X, L, B, S, T, F are all registers to hold
/*
const static string OPS[] = {
"18", "58", "40", "28",
"88", "24", "64", "C4",
"C0", "F4", "3C", "30", "34", "38",
"48", "00", "68", "50", "70", "08",
"6C", "74", "04", "D0", "20", "60",
"C8", "44", "D8", "4C",
"F0", "EC", "0C", "78",
"54", "80", "D4", "14", "7C", "E8",
"84", "10", "1C", "5C",
"E0", "F8", "2C", "DC"
};

const static string OPS2[] = {
"90", "B4", "A0", "9C", "98",
"AC", "A4", "A8", "94", "B0",
"B8"
};

// use to find op code in ops array
const static string MNEMONICS[] = {
"ADD", "ADDF", "AND", "COMP",
"COMPF", "DIV", "DIVF", "FIX",
"FLOAT", "HIO", "J", "JEQ", "JGT", "JLT",
"JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL",
"LDS", "LDT", "LDX", "LPS", "MUL", "MULF",
"NORM", "OR", "RD", "RSUB",
"SIO", "SSK", "STA", "STB",
"STCH", "STF", "STI", "STL","STS", "STSW",
"STT", "STX", "SUB", "SUBF",
"TD", "TIO", "TIX", "WD"
};

const static string MNEMONICS2[] = {
    "ADDR", "CLEAR", "COMPR", "DIVR", "MULR",
    "RMO", "SHIFTL", "SHIFTR", "SUBR", "SVC",
    "TIXR"
};*/

/*
Function name:  main()
Parameters:     argc - number of command line arguments
                argv - an array where each element is an independent argument

Description:    Depending on the content of the entered argu

Output:         A generated text file that will have disa
*/
int main( int argc, char *argv[] ){

    if (argc != 3){
        cout << "One or more of the following files were missing in the command line: test.obj, test.sym" << endl;
        cout << "Please execute in the following way: >>./dissem test.obj test.sym" << endl;
        return 0;
    }
    
    ifstream obj;
    ofstream outfile ("out.lst");
    string line;
    obj.open (argv[1]);
    
    map<string, string> symbols;
    symbols = addSymbols(symbols, argv[2]);
    
    map<string, string*> lits;
    lits = addLiterals(lits, argv[2]);
    
    /* debugging purposes
    if (symbols.find("000A") != symbols.end()){
        cout << "found" << endl;
    }
    else{
        cout << "not found" << endl;
    }*/

    while(getline(obj, line))
        outfile << breakdownLine(line, symbols, lits);

    outfile.close();
    obj.close();
    return 0;
}

/*
Function name:  breakdownLine()
Parameters:     line - a string that holds the entire record
                sym - the .sym file that holds the different locations w/ names

Description:    Depending on the content of the entered argu

Output:         A generated text file that will have disa
*/
string breakdownLine(string line, map<string, string> symbols, map<string, string*> lits){
    // header record type
    string s;

    if (line[0] == 'H'){
        s.append(header(line));
    }
    // text record type
    else if (line[0] == 'T'){
        s.append(text(line, symbols, lits));
    }
    // modification record type
    else if (line[0] == 'M'){
        //s = modification(line, sym);
    }
    // end record type
    else if (line[0] == 'E'){
        //s = end(line, sym);
    }
    else {cout << "Error reading line: " << line << endl; s = "";}

    return s;
}


string header(string line){
    string name;
    string c;
    int x;
    for (int i = 1; isalpha(line[i]); i++){
        c = line[i];
        name.append(c);
        x = i;
    }
    string header_address;
    x += 3;
    for (int i = x; i < x+4; i++){
        c = line[i];
        header_address.append(c);
    }
    
    string output = header_address + "\t" + name + "\tSTART\t" + to_string(hexToDec(header_address)) + "\n";
    registers['P'] = header_address;

    return output;
}

string text(string line, map<string, string> symbols, map<string, string*> lits){
    string output;

    string bin;
    string addr;
    string object;
    string name;
    string operation;
    string val;
    string c;
    string disp;
    bool x;
    int addressing_type;
    int ptr = 3;
    int format;
    int length;
    char n;
    char i_bit;
    string *arr;

    // get starting address
    for (; ptr < 7; ptr++){
        c = line[ptr];
        addr.append(c);
    }
    ptr += 2;

    cout << "record starting address: " + addr + " PC: " + registers['P'] << endl;

    // get a queue of symbols that appear between PC and beginning of text record
    /*
    queue<string> q;
    while (addr != registers['P']){
        for ( auto const& it : lits){
            if (hexToDec(registers['P']) <= hexToDec(it.first) < hexToDec(addr)){
                q.push(it.first);
            }
        }
    }
    */
    
    //continue until the entire text file is read
    while(ptr < line.length()){
        // reset the vars so they are empty strings and can be easily appended
        name = "";
        operation = "";
        val = "";
        object = "";
        bin = "";
        disp = "";
        

        // check if the address is in any of the map
        if (lits.find(addr) != lits.end()){
            arr = lits[addr];

            name = arr[0];
            val = arr[1];
            
            // go through operation to find out the length of address change
            for (int i = 0; i < val.length(); i++){
                if (val[i] == '\''){
                    length = i;
                    break;
                }
            }
            for (int i = length+1; i< val.length()-1; i++){
                c = val[i];
                object.append(c);
            }
            length = (val.length() - length - 2)/2;
            format = length;
            operation = "BYTE";
            // now have object code and length to change addr by
            output = addr + "\t" + name + "\t" + operation + "\t" + val + "\t" + object + "\n";
        }
        //not in lits map
        else{
            if (symbols.find(addr) != symbols.end()){
                name = symbols[addr];
            }
            else    name = "\t";

            // name & addr is now assigned by this point
            c = line[ptr];
            bin.append(c);
            c = hexToBin(line[ptr+1]); // 1111
            n = c[2];
            i_bit = c[3];
            c[2] = '0';
            c[3] = '0';                 //1100
            bin.append(binToHex(c));

            // check if format 2,
            if (OPS_2.find(bin) != OPS_2.end()){
                operation = OPS_2[bin];
                format = 2;
                for(int i = ptr; i < ptr+4; i++){
                    c = line[i];
                    object.append(c);
                }
            }
            else{
                // check the e bit, if 1 format 4, otherwise format 3
                operation = OPS[bin];
                bin = hexToBin(line[ptr+2]);
                if (bin[0] == '1')  x = true;
                else                x = false;
                /*
                Different addressing types:
                1 = simple              m
                2 = indexed simple      m,X
                3 = indirect            @m
                4 = immediate           #m
                */
                if (n == '1' && i_bit == '1'){
                    // simple addressing
                    addressing_type = 1;
                    if (x)  addressing_type++;
                }
                else if(n == '1' && i_bit == '0')   addressing_type = 3;
                else if (n == '0' && i_bit == '1')  addressing_type = 4;
                if (bin[3] == '1'){
                    format = 4;
                    // even though its 5 bytes, we only use the right 4 tbh
                    for (int i = ptr+4; i < ptr+8; i++){
                        c = line[i];
                        disp.append(c);
                    }
                    // check if indexed
                    if (addressing_type == 2){
                        val = OPS[fourHex(subHex(disp,hexToDec(registers['X'])))];
                        val.append(",X");
                        //TA - (X)
                    }
                    // check if its a constant
                    else if (symbols.find(disp) == symbols.end()){
                        // gets rid of the extra numbers/0's
                        val = "#"+decToHex(hexToDec(disp));
                    }
                    else if (addressing_type == 4){
                        val = "#"+symbols[disp];
                    }
                    // do regular direct addressing
                    else{
                        val = symbols[disp];
                        if (addressing_type == 3){
                            val = "@" + val;
                        }
                        else if(addressing_type == 4){
                            val = "#" + val;
                        }
                    }
                    for (int i = ptr; i < ptr+8; i++){
                        c = line[i];
                        object.append(c);
                    }
                }
                else{
                    format = 3;
                    for (int i = ptr+3; i < 6; i++){
                        c = line[i];
                        disp.append(c);
                    }

                    if (addressing_type == 3) val.append("@");
                    else if (addressing_type == 4) val.append("#");

                    // check whether pc or base
                    bin = hexToBin(line[2]);
                    for (int i = ptr+3; i < ptr+6; i++){
                        c = line[i];
                        disp.append(c);
                    }
                    disp = fourHex(disp);
                    // check if base
                    if(bin[1] == '1'){
                    }
                    // will be pc if not base
                    else{
                        val = symbols[fourHex(addHex(disp, addHex(addr,"3")))];
                    }
                    if(addressing_type == 3){
                        val = "@" + val;
                    }
                    else if (addressing_type == 4){
                        val = "#" + val;
                    }
                    for(int i = ptr; i < ptr+6; i++){
                        c = line[i];
                        object.append(c);
                    }
                }
            }
            output.append(addr+"\t"+name+"\t"+operation+"\t"+val+"\t"+object+"\n");
            //special cases
            if(operation == "LDB"){
                object = "";
                for (int i = 1; i < val.length(); i++){
                    c = val[i];
                    object.append(c);
                }
                output.append("\t\t\t\tBASE\t"+object+"\n");
                registers['B'] = addr;
            }
            
        }
        
        cout << addr << " " << operation << endl;
        addr = fourHex(addHex(addr,decToHex(format)));
        registers['P'] = addr;
        ptr += (format*2);
    }

    return output;
}


//*************************** Hash Table operations                 ***********************//
/*
Function name:  addSymbol()
Parameters:     symbols - the hash table that will get appended/manipulated
                sym     - the file that we will access and sort through to get the symbols

Description:    Run through the .sym file and add to the hash table to be accessed later

Output:         None, the unordered map will be internally affected
*/
map<string, string> addSymbols(map<string, string> symbols, string sym){
    ifstream file;
    string line;
    file.open(sym);

    int count = 0;
    string c;
    string name;
    string hex_address;
    int i = 0;
    while(getline(file, line)){
        name = "";
        hex_address = "";
        c = line[0];
        if (count == 2){
            // get name
            for (i = 0; line[i] != ' '; i++){
                c = line[i];
                name.append(c);
            }
            // get address
            while (line[i] == ' '){
                i++;
            }
            i += 2;     // move over to the important bytes
            while (line[i] != ' '){
                c = line[i];
                hex_address.append(c);
                i++;
            }
            if (symbols.find(hex_address) != symbols.end()) break;
            // add to the hashmap
            symbols[hex_address] = name;
            // cout << hex_address << ": " << symbols[hex_address] << endl;;
        }
        else count++;

    }
    file.close();
    return symbols;
}

map<string, string*> addLiterals(map<string, string*> lit, string sym){
    ifstream file;
    file.open(sym);

    int count = 0;
    int i;
    string c;
    string line;
    string name;
    string hex_address;
    string lit_s;
    string *arr = new string[2];

    while(getline(file, line)){
        if (line[0] == '-'){
            count++;
        }
        else if (count == 2){
            name = "";
            hex_address = "";
            lit_s = "";
            i = 0;

            // get name
            for (i = 0; line[i] != ' '; i++){
                c = line[i];
                name.append(c);
            }

            // skip over spaces
            while (line[i] == ' '){
                i++;
            }

            // get literal
            for (; line[i] != ' '; i++){
                c = line[i];
                lit_s.append(c);
            }

            // skip over spaces
            while (line[i] == ' '){
                i++;
            }
            // skip length
            while (line[i] != ' '){
                i++;
            }
            while (line[i] == ' '){
                i++;
            }
            //skip to important bit
            i += 2;
            // get address
            while (i != line.length()){
                c = line[i];
                hex_address.append(c);
                i++;
            }
            //cout << hex_address << ": " << name << " " << lit_s << endl;
            string x(*arr);
            arr[0] = name;
            arr[1] = lit_s;

            lit[hex_address] = arr;
        }
        
    }

    file.close();
    return lit;
}



//*************************** Hex and Decimal Conversion/Operations***********************//
/*
Function name:  addHex
Parameters:     operand1 - a string of chars that represent a hex value
                operand2 - same as operand1

Description:    Compare two different values in Hex and add them together.
                Will output as a specific length.

Output:         An array of chars that are a hex representation
*/
string addHex(string operand1, string operand2){
    int op1 = hexToDec(operand1);
    int op2 = hexToDec(operand2);

    //cout << op1 << " " << op2 << endl;

    op1 = op1 + op2;

    //cout << op1 << endl;

    return decToHex(op1);
}

// convert hex into an int
int hexToDec(string hex){
    string s = string(hex);
    int x;
    int sum = 0;
    int p = 0;
    for (int i = s.size() - 1; i >= 0; i--){
        if (s[i] == '0')        x = 0;
        else if (s[i] == '1')   x = 1;
        else if (s[i] == '2')   x = 2;
        else if (s[i] == '3')   x = 3;
        else if (s[i] == '4')   x = 4;
        else if (s[i] == '5')   x = 5;
        else if (s[i] == '6')   x = 6;
        else if (s[i] == '7')   x = 7;
        else if (s[i] == '8')   x = 8;
        else if (s[i] == '9')   x = 9;
        else if (s[i] == 'A')   x = 10;
        else if (s[i] == 'B')   x = 11;
        else if (s[i] == 'C')   x = 12;
        else if (s[i] == 'D')   x = 13;
        else if (s[i] == 'E')   x = 14;
        else if (s[i] == 'F')   x = 15;
        // somehow something bad/invalid was passed in
        else                    x = -1;

        if (x > 0) sum += (x * pow(16, p));
        p++;
    }

    return sum;
}

// convert an int into a string that represents a hex value
string decToHex(int n){
    int p = 0;
    string s;
    
    while (pow(16, p) <= n){
        p++;
    }
    p--;
    int e;
    
    while (p >= 0){
        e = 0;
        while(pow(16, p) <= n){
            n -= pow(16, p);
            e++;
        }
        if (e == 0)         s.append("0");
        else if (e == 1)    s.append("1");
        else if (e == 2)    s.append("2");
        else if (e == 3)    s.append("3");
        else if (e == 4)    s.append("4");
        else if (e == 5)    s.append("5");
        else if (e == 6)    s.append("6");
        else if (e == 7)    s.append("7");
        else if (e == 8)    s.append("8");
        else if (e == 9)    s.append("9");
        else if (e == 10)   s.append("A");
        else if (e == 11)   s.append("B");
        else if (e == 12)   s.append("C");
        else if (e == 13)   s.append("D");
        else if (e == 14)   s.append("E");
        else if (e == 15)   s.append("F");
        else                s.append("X");
        p--;
    }
    return s;
}

string hexToBin(char c){
    string output;
    if (c == '0')        output = "0000";
    else if (c == '1')   output = "0001";
    else if (c == '2')   output = "0010";
    else if (c == '3')   output = "0011";
    else if (c == '4')   output = "0100";
    else if (c == '5')   output = "0101";
    else if (c == '6')   output = "0110";
    else if (c == '7')   output = "0111";
    else if (c == '8')   output = "1000";
    else if (c == '9')   output = "1001";
    else if (c == 'A')   output = "1010";
    else if (c == 'B')   output = "1011";
    else if (c == 'C')   output = "1100";
    else if (c == 'D')   output = "1101";
    else if (c == 'E')   output = "1110";
    else if (c == 'F')   output = "1111";
    return output;
}

string binToHex(string bin){
    if (bin == "0000") return "0";
    else if (bin == "0001") return "1";
    else if (bin == "0010") return "2";
    else if (bin == "0011") return "3";
    else if (bin == "0100") return "4";
    else if (bin == "0101") return "5";
    else if (bin == "0110") return "6";
    else if (bin == "0111") return "7";
    else if (bin == "1000") return "8";
    else if (bin == "1001") return "9";
    else if (bin == "1010") return "A";
    else if (bin == "1011") return "B";
    else if (bin == "1100") return "C";
    else if (bin == "1101") return "D";
    else if (bin == "1110") return "E";
    else if (bin == "1111") return "F";
    else return "X";
}

string fourHex(string hex){
    while(hex.length() < 4){
        hex = "0" + hex;
    }

    return hex;
}

int negHex(string hex){
    string bin;
    string hex2;
    bool neg = false;
    for(int i = 0; i < hex.length(); i++){
        bin = hexToBin(hex[i]);
        if (i == 0 && bin[0] == '1') neg = true;
        if (neg){
            for (int j = 0; j < 4; j++){
                if (bin[i] == '0')  bin[i] = '1';
                else                bin[i] = '0';
            }
        }
        // bin now = hex of 
        hex2.append(binToHex(bin));
    }

    if (neg)    return (0-(hexToDec(hex2))+1);
    else        return hexToDec(hex2);
}

string subHex(string hex, int val){
    string output;
    val = hexToDec(hex) - val;
    output = decToHex(val);

    return output;
}

//EOF
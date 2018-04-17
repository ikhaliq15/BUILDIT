#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <locale>
#include <cmath>
#include <ctime>

using namespace std;

//KEYWORDS

string PRINT = "SAY";
string PRINTLN = "SAYLN";
string INPUT = "ASK";
string ASSIGNMENT = "LET";
string REASSIGNMENT = "CHANGE";
string ADDITION = "ADD";
string SUBTRACTION = "SUB";
string MULTIPLICATION = "MULT";
string DIVISION = "DIV";
string SQRT = "SQRT";
string MOD = "MOD";
string CASTTOSTRING = "TOSTRING";
string CHARAT = "CHARAT";
string LENGTH = "LEN";
string CASTTOINT = "TONUM";
string CASTTOASCII = "TOASCII";
string RANDOMINT = "RANDINT";
string REVERSESTRING = "REVERSE";
string IF = "IF";
string THEN = "THEN";
string END = "ENDIF";
string EQUAL = "EQU";
string NOTEQUAL = "NEQU";
string GREATERTHAN = "GRE";
string LESSTHAN = "LESS";
string GREATERTHANEQuAL = "GEQU";
string LESSTHANEQUAL = "LEQU";
string STRINGEQUAL = "SEQU";
string NOTSTRINGEQUAL = "NSEQU";
string WHILE = "WHILE";
string WHILETHEN = "DO";
string WHILEEND = "ENDWHILE";
string BREAKLOOP = "LEAVE";
string STARTCOMMENT = "SC";
string ENDCOMMENT = "EC";
string ADDMARKER = "SETMARKER";
string GOTOMARKER = "GOTOMARKER";
string READFILE = "READFILE";
//Group Commands
string CREATEGROUP = "NEWGROUP";
string APPENDTOGROUP = "ADDTOGROUP";
string ACCESSGROUP = "ACCESSGROUP";
string SETGROUPAT = "SETGROUP";

std::vector<std::string> toks;

std::vector<std::string> varNames;
std::vector<std::string> varValues;

std::vector<string> markerNames;
std::vector<int> markerLocations;

std::vector<std::string> groupNames;
std::vector<std::vector<int> > groupValues;

bool inString = false;


bool isDigits(const std::string &str)
{
	return str.find_first_not_of("0123456789.") == std::string::npos;
}

long stringToInt(const string &str) {
	stringstream ss;
	ss << str;
	long value;
	ss >> value;
	//cout << "INPUT: " << str << ", OUTPUT: " << value << endl;
	return value;
}

template <typename T>
  string NumberToString ( T Number )
  {
	 ostringstream ss;
	 ss << Number;
	 return ss.str();
  }

long varToInt(string var){
	long indexOfVar = 0;
	for(long j = 0; j < varNames.size(); j++){
		if(varNames[j] == var){
			indexOfVar = j;
			break;
		}
	}

	long firstValue = 0;

	if(varValues[indexOfVar].substr(0, 6) == "STRING"){
		string x = varValues[indexOfVar].substr(8, varValues[indexOfVar].length());
		firstValue = stringToInt((x.substr(0, x.length()-1)));
	}else if(varValues[indexOfVar].substr(0, 7) == "INTEGER"){
		string x = varValues[indexOfVar].substr(8, varValues[indexOfVar].length());
		firstValue = stringToInt(x);
	}

	return firstValue;
}

string varToString(string var){
	long indexOfVar = 0;
	for(long j = 0; j < varNames.size(); j++){
		if(varNames[j] == var){
			indexOfVar = j;
			break;
		}
	}

	string firstValue = "";

	if(varValues[indexOfVar].substr(0, 6) == "STRING"){
		string x = varValues[indexOfVar].substr(8, varValues[indexOfVar].length());
		firstValue = x;;
	}else if(varValues[indexOfVar].substr(0, 7) == "INTEGER"){
		string x = varValues[indexOfVar].substr(8, varValues[indexOfVar].length());
		firstValue = (x);
	}

	return firstValue;
}

string reverseStr(string str)
{
	//cout << "Reversing " << str << endl;
    int n = str.length();
 
    // Swap character starting from two
    // corners
    for (int i=0; i<n/2; i++)
       swap(str[i], str[n-i-1]);
   return str;
}

bool checkStringConditional(string op, string first, string second){
	if(op == "stringequal"){
		if(first == second){
			return true;
		}else{
			return false;
		}
	}else if(op == "notstringequal"){
		if(first != second){
			return true;
		}else{
			return false;
		}
	}
}

bool checkConditional(string op, long first, long second){
	if(op == "intequal"){
		if(first == second){
			return true;
		}else{
			return false;
		}
	}else if (op == "intnotequal"){
		if(first != second){
			return true;
		}else{
			return false;
		}
	}else if(op == "intgreater"){
		if(first > second){
			//cout << "FIRST: " << first << ", SECOND: " << second <<"! GREATER TRUE!" << endl;
			return true;
		}else{
			return false;
		}
	}else if(op == "intless"){
		if(first < second){
			return true;
		}else{
			return false;
		}
	}else if(op == "intgreaterequal"){
		if(first >= second){
			return true;
		}else{
			return false;
		}
	}else if(op == "intlessequal"){
		if(first <= second){
			return true;
		}else{
			return false;
		}
	} 
}

long changeVariable(string name, string newValue){
	long indexOfVar = 0;
	for(long j = 0; j < varNames.size(); j++){
		if(varNames[j] == name){
			indexOfVar = j;
			break;
		}
	}
	varValues[indexOfVar] = newValue;
}

long randintinrange(long min, long max) {
    return (rand()%(max-min+1)) + min;
}

long doMath(string op, long first, long second){
	long answer = 0;

	if(op == "add"){
		answer = (first + second);
	}else if(op == "sub"){
		answer = (first - second);
	}else if(op == "mult"){
		answer = (first * second);
	}else if(op == "div"){
		answer = (first / second); 
	}else if(op == "randomint"){
		answer = randintinrange(first, second);
	}else if(op == "modulo") {
		answer = first % second;
	}

	return answer;
}

//This translates the file
void readFile(const char* filenombre)
{

	std::vector<std::string> words;

	ifstream file;
	file.open (filenombre);
	if (!file.is_open()) return;

	string word;
	string curString;
	while (file >> word)
	{
	   words.push_back(word);
	}

	long i = 0;
	while (i < words.size()){
		word = words[i];
		//cout << word << endl;
		if(not inString){
			if(word == PRINT){
				toks.push_back("print");
			}else if(word == PRINTLN){
				toks.push_back("println");
			}else if(word.at(0) == '\"'){
				if(word.at(word.length()-1) == '\"' and word != "\""){
					inString = false;
					curString = curString + word;
					toks.push_back("STRING " + curString);
					curString = "";
				} else {
					inString = true;
					curString = word + ' ';
				}
			}else if(word == ASSIGNMENT){
				toks.push_back("assign");
			}else if(word == REASSIGNMENT){
				toks.push_back("reassign");
			}else if (word == CHARAT) {
				toks.push_back("charatindex");
			}else if(word == INPUT){
				toks.push_back("input");
			}else if(isDigits(word)){
				toks.push_back("INTEGER " + word);
			}else if(word == ADDITION){
				toks.push_back("add");
			}else if(word == SUBTRACTION){
				toks.push_back("sub");
			}else if(word == MULTIPLICATION){
				toks.push_back("mult");
			}else if(word == DIVISION){
				toks.push_back("div");
			}else if (word == SQRT){
				toks.push_back("sqrt");
			}else if (word == MOD) {
				toks.push_back("modulo");
			}else if (word == RANDOMINT) {
				toks.push_back("randomint");
			}else if (word == REVERSESTRING) {
				toks.push_back("reversestring");
			}else if (word == CASTTOASCII) {
				toks.push_back("converttoascii");
			}else if (word == LENGTH) {
				toks.push_back("lengthof");
			}else if(word == IF){
				toks.push_back("if");
			}else if(word == THEN){
				toks.push_back("ifthen");
			}else if(word == END){
				toks.push_back("ifend");
			}else if(word == EQUAL){
				toks.push_back("intequal");
			}else if (word == NOTEQUAL){
				toks.push_back("intnotequal");
			}else if(word == GREATERTHAN){
				toks.push_back("intgreater");
			}else if(word == LESSTHAN){
				toks.push_back("intless");
			}else if(word == GREATERTHANEQuAL){
				toks.push_back("intgreaterequal");
			}else if(word == LESSTHANEQUAL){
				toks.push_back("intlessequal");
			}else if(word == STRINGEQUAL){
				toks.push_back("stringequal");
			}else if(word == NOTSTRINGEQUAL){
				toks.push_back("notstringequal");
			}else if(word == WHILE){
				toks.push_back("startwhile");
			}else if(word == WHILETHEN){
				toks.push_back("dowhile");
			}else if(word == WHILEEND){
				toks.push_back("endwhile");
			}else if (word == BREAKLOOP){
				toks.push_back("breakloop");
			}else if(word == STARTCOMMENT){
				while(words[i] != ENDCOMMENT){
					i ++;
				}
			}else if (word == ADDMARKER){
				toks.push_back("createmarker");
				toks.push_back(words[i + 1]);
				i++;
			}else if (word == GOTOMARKER){
				toks.push_back("gotomarker");
				toks.push_back(words[i + 1]);
				i++;
			}else if (word == CASTTOINT) {
				toks.push_back("casttoint");
			}else if (word == CASTTOSTRING) {
				toks.push_back("casttostring");
			}else if (word == READFILE) {
				toks.push_back("readfile");
			}else if (word == CREATEGROUP) {
				toks.push_back("createemptygroup");
			}else if (word == APPENDTOGROUP) {
				toks.push_back("appendtogroup");
			}else if (word == ACCESSGROUP) {
				toks.push_back("accessgroup");
			}else if (word == SETGROUPAT) {
				toks.push_back("setgroupat");
			}else{
				toks.push_back("VAR " + word);
			}
		}else if(inString){
			if(word.at(word.length()-1) == '\"'){
				inString = false;
				curString = curString + word;
				toks.push_back("STRING " + curString);
				curString = "";
			}else{
				curString = curString + word + ' ';
			}
		}
		i++;
	}
	

}

inline const char * const BoolToString(bool b) {
	return b ? "true" : "false";
}

bool StringToBool(string s) {
	if (s == "true") {
		return true;
	}
	return false;
}

bool checkIfDataIs(string x, string type) {
	if (x.length() >= type.length()) {
		return x.substr(0, type.length()) == type;
	}
	return false;
}


std::vector<std::string> getConditionalValue(std::vector<std::string> tokens, long position){
	std::vector<std::string> output;

	long moveOver = 0;
	if(tokens[position+1] == "stringequal" || tokens[position+1] == "notstringequal"){
		if(checkIfDataIs(tokens[position+2], "VAR") and checkIfDataIs(tokens[position+3], "VAR")){
			string firstValue = varToString(tokens[position+2]);
			string secondValue = varToString(tokens[position+3]);
			output.push_back(BoolToString(checkStringConditional(tokens[position+1], firstValue, secondValue)));
		}else if(checkIfDataIs(tokens[position+2], "VAR") and checkIfDataIs(tokens[position+3], "STRING")){
			string firstValue = varToString(tokens[position+2]);					
			string secondValue = (tokens[position+3].substr(8, tokens[position+3].length()-1));
			output.push_back(BoolToString(checkStringConditional(tokens[position+1], firstValue, secondValue)));
		}else if(tokens[position+3].substr(0, 3) == "VAR" and checkIfDataIs(tokens[position+2], "STRING")){
			string secondValue = varToString(tokens[position+3]);					
			string firstValue = (tokens[position+2].substr(8, tokens[position+2].length()-1));
			output.push_back(BoolToString(checkStringConditional(tokens[position+1], firstValue, secondValue)));
		}else if(checkIfDataIs(tokens[position+3], "STRING") and checkIfDataIs(tokens[position+2], "STRING")){
			string firstValue = varToString(tokens[position+2].substr(8, tokens[position+2].length()-1));
			string secondValue = varToString(tokens[position+3].substr(8, tokens[position+3].length()-1));
			output.push_back(BoolToString(checkStringConditional(tokens[position+1], firstValue, secondValue)));
		}
	}else{
		if(checkIfDataIs(tokens[position+2], "VAR") and checkIfDataIs(tokens[position+3], "VAR")){
			long firstValue = varToInt(tokens[position+2]);
			long secondValue = varToInt(tokens[position+3]);
			output.push_back(BoolToString(checkConditional(tokens[position+1], firstValue, secondValue)));
		}else if(checkIfDataIs(tokens[position+2], "VAR") and checkIfDataIs(tokens[position+3], "INTEGER")){
			long firstValue = varToInt(tokens[position+2]);					
			long secondValue = stringToInt(tokens[position+3].substr(8, tokens[position+3].length()));
			output.push_back(BoolToString(checkConditional(tokens[position+1], firstValue, secondValue)));
		}else if(checkIfDataIs(tokens[position+3], "VAR") and checkIfDataIs(tokens[position+2], "INTEGER")){
			long secondValue = varToInt(tokens[position+3]);
			long firstValue = stringToInt(tokens[position+2].substr(8, tokens[position+2].length()));
			output.push_back(BoolToString(checkConditional(tokens[position+1], firstValue, secondValue)));
		}else if(checkIfDataIs(tokens[position+3], "INTEGER") and checkIfDataIs(tokens[position+3], "INTEGER")){
			long firstValue = stringToInt(tokens[position+2].substr(8, tokens[position+2].length()));
			long secondValue = stringToInt(tokens[position+3].substr(8, tokens[position+3].length()));
			output.push_back(BoolToString(checkConditional(tokens[position+1], firstValue, secondValue)));
		}
	}
	moveOver += 4;
	output.push_back(NumberToString(moveOver));
	return output;
} 

bool isMathOperator(string x) {
	return x == "add" || x == "sub" || x == "mult" || x == "div" || x == "randomint" || x == "modulo";
}

void testLexer(){
	//Testing the lexer
	cout << "Num Toks: " << toks.size() << endl;
	
	for(long i = 0; i <= toks.size()-1;i++){
		cout << toks[i] << endl;
	}

	cout << endl << endl;
}

void testVars(){
	cout << "Num Vars: " << varNames.size() << endl;
	for (long i = 0; i <= varNames.size()-1;i++){
		cout << "Name: " << varNames[i] << " " << "Value: " << varValues[i] << endl;
	}
}


//This takes the translated file and performs actions.
void parse()
{
	long i = 0;
	bool doAction = true;
	bool doWhileAction = true;
	long startOfWhile = -21;

	while (i < toks.size()) {
		if (toks[i] == "createmarker"){
			markerNames.push_back(toks[i+1]);
			i++;
			markerLocations.push_back(i);
		}
		i++;
	}

	i=0;

	while( i < toks.size()){
		if(doAction && doWhileAction){
			if(toks[i] == "print"){
				if(toks[i + 1].substr(0, 7) == "STRING "){
					string returnValue = toks[i + 1].substr(8);
					cout << returnValue.substr(0, returnValue.length()-1);
				}else if(toks[i + 1].substr(0, 3) == "VAR"){
					long indexOfVar = 0;
					for(long j = 0; j < varNames.size(); j++){
						if(varNames[j] == toks[i+1]){
							indexOfVar = j;
							break;
						}
					}
					if(varValues[indexOfVar].substr(0, 7) == "STRING "){
						string returnValue = varValues[indexOfVar].substr(8);
						cout << returnValue.substr(0, returnValue.length()-1);
					}else if(varValues[indexOfVar].substr(0, 7) == "INTEGER"){
						string returnValue = varValues[indexOfVar].substr(8);
						cout << returnValue.substr(0, returnValue.length());
					}  
				}else if(toks[i + 1].substr(0, 7) == "INTEGER"){
					cout << toks[i+1].substr(8, toks[i+1].length());
				}
				i++;
			}else if(toks[i] == "println"){
				if(toks[i + 1].substr(0, 7) == "STRING "){
					string returnValue = toks[i + 1].substr(8);
					cout << returnValue.substr(0, returnValue.length()-1) << endl;
				}else if(toks[i + 1].substr(0, 3) == "VAR"){
					long indexOfVar = 0;
					for(long j = 0; j < varNames.size(); j++){
						if(varNames[j] == toks[i+1]){
							indexOfVar = j;
							break;
						}
					}
					if(varValues[indexOfVar].substr(0, 7) == "STRING "){
						string returnValue = varValues[indexOfVar].substr(8);
						cout << returnValue.substr(0, returnValue.length()-1) << endl;
					}else if(varValues[indexOfVar].substr(0, 7) == "INTEGER"){
						string returnValue = varValues[indexOfVar].substr(8);
						cout << returnValue.substr(0, returnValue.length()) << endl;
					}  
				}else if(toks[i + 1].substr(0, 7) == "INTEGER"){
					cout << toks[i+1].substr(8, toks[i+1].length()) << endl;
				}
				i++;
			}else if(toks[i] == "input"){
				string x;
				cin >> x;
				bool varExists = false;
				for (long j = 0; j < varNames.size(); j++) {
					if (varNames[j] == toks[i+1]) {
						varValues[j] = "STRING \"" + x + "\"";
						varExists = true;
					}
				}
				if (!varExists) {
					varNames.push_back(toks[i+1]);
					varValues.push_back("STRING \"" + x + "\"");
				}
				i++;
			}else if(toks[i] == "assign"){
				varNames.push_back(toks[i+1]);
				if(isMathOperator(toks[i+2])) {
					if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 3) == "VAR"){
						long firstValue = varToInt(toks[i+3]);
						long secondValue = varToInt(toks[i+4]);
						long answer = doMath(toks[i+2], firstValue, secondValue);
						varValues.push_back("INTEGER " +  NumberToString(answer));
						i += 2;
					}else if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 7) == "INTEGER"){
						long firstValue = varToInt(toks[i+3]);
						long secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						long answer = doMath(toks[i+2], firstValue, secondValue);
						varValues.push_back("INTEGER " +  NumberToString(answer));
						i += 2;
					}else if(toks[i+4].substr(0, 3) == "VAR" and toks[i+3].substr(0, 7) == "INTEGER"){
						long secondValue = varToInt(toks[i+4]);
						long firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						long answer = doMath(toks[i+2], firstValue, secondValue);
						varValues.push_back("INTEGER " +  NumberToString(answer));
						i += 2;
					}else if(toks[i+4].substr(0, 7) == "INTEGER" and toks[i+3].substr(0, 7) == "INTEGER"){
						long firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						long secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						long answer = doMath(toks[i+2], firstValue, secondValue);
						varValues.push_back("INTEGER " +  NumberToString(answer));
						i += 2;
					}
				}else if(toks[i+2] == "sqrt"){
					if(toks[i+3].substr(0,3) == "VAR"){
						varValues.push_back("INTEGER " + NumberToString(sqrt(varToInt(toks[i+3]))));
					}else if (toks[i+3].substr(0, 8) == "INTEGER"){
						varValues.push_back("INTEGER " + NumberToString(sqrt(stringToInt(toks[i+3].substr(8, toks[i+3].length())))));
					}
					i += 1;
				}else if (toks[i+2] == "casttostring") {
					if(toks[i+3].substr(0,3) == "VAR"){
						//cout << "STRING \"" + varToString(toks[i+3]) << endl;
						varValues.push_back("STRING \"" + NumberToString(varToInt(toks[i+3])) + "\"");
					}else if (toks[i+3].substr(0, 7) == "INTEGER"){
						varValues.push_back("STRING \"" + toks[i+3].substr(8) + "\"");
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						varValues.push_back(toks[i+3]);
					}
					i += 1;
				}else if (toks[i+2] == "casttoint") {	
					if(toks[i+3].substr(0,3) == "VAR"){
						varValues.push_back("INTEGER " + NumberToString(varToInt(toks[i+3])));
					}else if (toks[i+3].substr(0, 7) == "INTEGER"){
						varValues.push_back(toks[i+3]);
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						varValues.push_back("INTEGER " + toks[i+3].substr(8, toks[i+3].length()));
					}
					i += 1;
				}else if (toks[i+2] == "reversestring") {	
					if(toks[i+3].substr(0,3) == "VAR"){
						varValues.push_back("STRING " + reverseStr( varToString(toks[i + 3]) ) + "\"");
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						//cout << "SENDING " << toks[i+3].substr(8, toks[i+3].length() - 2 ) << endl;
						varValues.push_back("STRING " + reverseStr( toks[i+3].substr(8, toks[i+3].length() - 2 ) ) + "\"");
					}
					i += 1;
				}else if (toks[i+2] == "charatindex") {
					string newValue = "";	
					if(toks[i+3].substr(0,3) == "VAR"){
						if (toks[i+4].substr(0, 3) == "VAR") {
							newValue = ("STRING \"" + ( varToString(toks[i+3]).substr(varToInt(toks[i+4]) - 1,1)) + "\"");
							//cout << "INDEX: " << varToInt(toks[i+4]) << " WORD: " << varToString(toks[i+3]) << " NEW VALUE: " << newValue << endl;
						} else {
							newValue = ("STRING " + ( varToString(toks[i + 3]).substr(stringToInt(toks[i+4]), stringToInt(toks[i+4]) + 1) ) + "\"" );
							//cout << "INDEX: " << (toks[i+4]) << endl;
						}
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						if (toks[i+4].substr(0,3) == "VAR") {
							newValue = ("STRING \"" + ( toks[i+3].substr(8 + varToInt(toks[i+4]) - 1 , 1)) + "\"");
							//cout << "INDEX: " << varToInt(toks[i+4]) << " WORD: " << toks[i+3] << " NEW VALUE: " << newValue << endl;
						} else {
							newValue = ("STRING " + ( toks[i+3].substr(8 + stringToInt(toks[i+4]) + 1, stringToInt(toks[i+4]) + 2)) + "\"");
							//cout << "INDEX: " << (toks[i+4]) << endl;
						}
					}
					varValues.push_back(newValue);
					i += 2;
				}else if (toks[i+2] == "lengthof") {
					string newValue = "";	
					if(toks[i+3].substr(0,3) == "VAR"){
						if (toks[i+4].substr(0, 3) == "VAR") {
							newValue = ("INTEGER " + NumberToString(varToString(toks[i+3]).length() - 1));
							//cout << "INDEX: " << varToInt(toks[i+4]) << " WORD: " << varToString(toks[i+3]) << " NEW VALUE: " << newValue << endl;
						} else {
							newValue = ("INTEGER " + NumberToString(varToString(toks[i + 3]).length() - 1));
							//cout << "INDEX: " << (toks[i+4]) << endl;
						}
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						if (toks[i+4].substr(0,3) == "VAR") {
							newValue = ("INTEGER " + NumberToString(toks[i+3].length()-1) );
							//cout << "INDEX: " << varToInt(toks[i+4]) << " WORD: " << toks[i+3] << " NEW VALUE: " << newValue << endl;
						} else {
							newValue = ("INTEGER " + NumberToString(toks[i+3].length()-9));
							//cout << "INDEX: " << (toks[i+4]) << endl;
						}
					}
					varValues.push_back(newValue);
					i += 1;
				}else if (toks[i+2] == "accessgroup") {
					string newValue = "";
					if (toks[i+4].substr(0, 3) == "VAR") {
						int access_index = varToInt(toks[i+4]) - 1;
						string groupsName = toks[i+3];
						int num = -1;
						for (int k = 0; k < groupNames.size(); k++) {
							if (groupNames[k] == groupsName) {
								num = groupValues[k][access_index];
							}
						}
						newValue = "INTEGER " + NumberToString(num);
					} else {
						//ASSUME AN INTEGER.
						int access_index = stringToInt(toks[i+4].substr(8)) - 1;
						string groupsName = toks[i+3];
						int num = -1;
						for (int k = 0; k < groupNames.size(); k++) {
							if (groupNames[k] == groupsName) {
								num = groupValues[k][access_index];
							}
						}
						newValue = "INTEGER " + NumberToString(num);
					}
					varValues.push_back(newValue);
					i += 2;
				}else{
					varValues.push_back(toks[i+2]);
				}
				//testVars();
				i += 2;
			}else if(toks[i] == "ifend"){
				doAction = true;
			}else if(toks[i] == "if"){
				std::vector<string> v = getConditionalValue(toks, i);
				doAction = StringToBool(v[0]);
				i += stringToInt(v[1]);
			}else if(toks[i] == "reassign"){
				if(isMathOperator(toks[i+2])){
					string newValue = toks[i+2];
					if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 3) == "VAR"){
						long firstValue = varToInt(toks[i+3]);
						long secondValue = varToInt(toks[i+4]);
						long answer = doMath(toks[i+2], firstValue, secondValue);
						newValue = ("INTEGER " +  NumberToString(answer));
						//i += 2;
					}else if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 7) == "INTEGER"){
						long firstValue = varToInt(toks[i+3]);
						long secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						long answer = doMath(toks[i+2], firstValue, secondValue);
						newValue = ("INTEGER " +  NumberToString(answer));
						//i += 2;
					}else if(toks[i+4].substr(0, 3) == "VAR" and toks[i+3].substr(0, 7) == "INTEGER"){
						long secondValue = varToInt(toks[i+4]);
						long firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						long answer = doMath(toks[i+2], firstValue, secondValue);
						newValue = ("INTEGER " +  NumberToString(answer));
						//i += 2;
					}else if(toks[i+4].substr(0, 7) == "INTEGER" and toks[i+3].substr(0, 7) == "INTEGER"){
						long firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						long secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						long answer = doMath(toks[i+2], firstValue, secondValue);
						newValue = ("INTEGER " +  NumberToString(answer));
						//i += 2;
					}
					//cout << "NEW VALUE OF " << toks[i+1] << " IS " << newValue << "!" << endl;
					changeVariable(toks[i+1], newValue);\
					i += 2;
					//testVars();
				}else if(toks[i+2] == "sqrt"){
					if(toks[i+3].substr(0,3) == "VAR"){
						changeVariable(toks[i+1], "INTEGER " + NumberToString(sqrt(varToInt(toks[i+3]))));
					}else if (toks[i+3].substr(0, 7) == "INTEGER"){
						for (long j = 0; j < varNames.size(); j++) {
							if (varNames[j] == toks[i + 1]) {
								varValues[j] = "INTEGER " + NumberToString(sqrt(stringToInt(toks[i+3].substr(8, toks[i+3].length()))));
							}
						}
					}
					i += 1;
				}else if (toks[i+2] == "casttoint") {
					string newValue = "";	
					if(toks[i+3].substr(0,3) == "VAR"){
						newValue = ("INTEGER " + NumberToString(varToInt(toks[i+3])));
					}else if (toks[i+3].substr(0, 7) == "INTEGER"){
						newValue = (toks[i+3]);
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						newValue = ("INTEGER " + toks[i+3].substr(8, toks[i+3].length()));
					}
					for (long j = 0; j < varNames.size(); j++) {
						if (varNames[j] == toks[i + 1]) {
							varValues[j] = newValue;
						}
					}
					i += 1;
				}else if (toks[i+2] == "casttostring") {
					string newValue = "";
					if(toks[i+3].substr(0,3) == "VAR"){
						newValue = ("STRING \"" + NumberToString(varToInt(toks[i+3])) + "\"");
					}else if (toks[i+3].substr(0, 7) == "INTEGER"){
						newValue = ("STRING \"" + toks[i+3].substr(8) + "\"");
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						newValue = (toks[i+3]);
					}
					for (long j = 0; j < varNames.size(); j++) {
						if (varNames[j] == toks[i + 1]) {
							varValues[j] = newValue;
						}
					}
					i += 1;
				}else if (toks[i+2] == "reversestring") {
					string newValue = "";	
					if(toks[i+3].substr(0,3) == "VAR"){
						newValue = ("STRING " + reverseStr( varToString(toks[i + 3]) ) + "\"" );
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						newValue = ("STRING " + reverseStr( toks[i+3].substr(8, toks[i+3].length() ) ) + "\"");
					}
					for (long j = 0; j < varNames.size(); j++) {
						if (varNames[j] == toks[i + 1]) {
							varValues[j] = newValue;
						}
					}
					i += 1;
				}else if (toks[i+2] == "converttoascii") {
					string newValue = "";
					if (toks[i+3].substr(0, 3) == "VAR") {
						int asciinumber = varToInt(toks[i+3]);
						char W = static_cast<char>(asciinumber);
						std::string asciistring(1, W);
						newValue = "STRING \"" + asciistring + "\"";
					} else {
						//ASSUME INTEGER
						int asciinumber = stringToInt(toks[i+3].substr(8));
						char W = static_cast<char>(asciinumber);
						std::string asciistring(1, W);
						newValue = "STRING \"" + asciistring + "\"";
					}
					for (long j = 0; j < varNames.size(); j++) {
						if (varNames[j] == toks[i + 1]) {
							varValues[j] = newValue;
						}
					}
					i += 1;
				}else if (toks[i+2] == "charatindex") {
					string newValue = "";	
					if(toks[i+3].substr(0,3) == "VAR"){
						if (toks[i+4].substr(0, 3) == "VAR") {
							newValue = ("STRING \"" + ( varToString(toks[i+3]).substr(varToInt(toks[i+4]) - 1,1)) + "\"");
							//cout << "INDEX: " << varToInt(toks[i+4]) << " WORD: " << varToString(toks[i+3]) << " NEW VALUE: " << newValue << endl;
						} else {
							newValue = ("STRING " + ( varToString(toks[i + 3]).substr(stringToInt(toks[i+4]), stringToInt(toks[i+4]) + 1) ) + "\"" );
							//cout << "INDEX: " << (toks[i+4]) << endl;
						}
					}else if (toks[i+3].substr(0, 6) == "STRING") {
						if (toks[i+4].substr(0,3) == "VAR") {
							newValue = ("STRING \"" + ( toks[i+3].substr(8 + varToInt(toks[i+4]) - 1 , 1)) + "\"");
							//cout << "INDEX: " << varToInt(toks[i+4]) << " WORD: " << toks[i+3] << " NEW VALUE: " << newValue << endl;
						} else {
							newValue = ("STRING " + ( toks[i+3].substr(8 + stringToInt(toks[i+4]) + 1, stringToInt(toks[i+4]) + 2)) + "\"");
							//cout << "INDEX: " << (toks[i+4]) << endl;
						}
					}
					for (long j = 0; j < varNames.size(); j++) {
						if (varNames[j] == toks[i + 1]) {
							varValues[j] = newValue;
						}
					}
					i += 2;
				}else if (toks[i+2] == "accessgroup") {
					string newValue = "";
					if (toks[i+4].substr(0, 3) == "VAR") {
						int access_index = varToInt(toks[i+4]) - 1;
						string groupsName = toks[i+3];
						int num = -1;
						for (int k = 0; k < groupNames.size(); k++) {
							if (groupNames[k] == groupsName) {
								num = groupValues[k][access_index];
							}
						}
						newValue = "INTEGER " + NumberToString(num);
					} else {
						//ASSUME AN INTEGER.
						int access_index = stringToInt(toks[i+4].substr(8)) - 1;
						string groupsName = toks[i+3];
						int num = -1;
						for (int k = 0; k < groupNames.size(); k++) {
							if (groupNames[k] == groupsName) {
								num = groupValues[k][access_index];
							}
						}
						newValue = "INTEGER " + NumberToString(num);
					}
					for (long j = 0; j < varNames.size(); j++) {
						if (varNames[j] == toks[i + 1]) {
							varValues[j] = newValue;
						}
					}
					i += 2;
				}else{
					changeVariable(toks[i+1], toks[i+2]);
				}
				i += 2;
			}else if(toks[i] == "startwhile"){
				std::vector<string> v = getConditionalValue(toks, i);
				doWhileAction = StringToBool(v[0]);
				startOfWhile = i-1;
				i += stringToInt(v[1]);
			}else if(toks[i] == "dowhile"){
				//startOfWhile = i;
			}else if(toks[i] == "endwhile"){
				i = startOfWhile;
			}else if (toks[i] == "breakloop"){
				doWhileAction = false;
			}else if (toks[i] == "createmarker"){
				i++;
			}else if (toks[i] == "readfile") {
				string x;
				std::ifstream t(toks[i+1].substr(8, toks[i+1].length() -9) );
				std::stringstream buffer;
				buffer << t.rdbuf();
				x = buffer.str();
				bool varExists = false;
				for (long j = 0; j < varNames.size(); j++) {
					if (varNames[j] == toks[i+2]) {
						varValues[j] = "STRING \"" + x + "\"";
						varExists = true;
					}
				}
				if (!varExists) {
					varNames.push_back(toks[i+2]);
					varValues.push_back("STRING \"" + x + "\"");
				}
				i += 2;
			}else if (toks[i] == "gotomarker") {
				long location = 0;
				for (long j = 0; j < markerNames.size(); j++) {
					if (markerNames[j] == toks[i + 1]) {
						location = j;
					}
				}
				i = markerLocations[location];
			}else if (toks[i] == "createemptygroup") {
				groupNames.push_back(toks[i+1]);
				std::vector<int> temp;
				groupValues.push_back(temp);
				i++;
			}else if (toks[i] == "appendtogroup") {
				for (int k = 0; k < groupNames.size(); k++) {
					if (groupNames[k] == toks[i+1]) {
						groupValues[k].push_back(stringToInt(toks[i+2].substr(8)));
					}
				}
				i += 2;
			}else if (toks[i] == "setgroupat"){
				for (int k = 0; k < groupNames.size(); k++) {
					int location;
					if (toks[i+2].substr(0,3) == "VAR") {
						location = varToInt(toks[i+2]);
					} else {
						//ASSUME INTEGER
						location = stringToInt(toks[i+2].substr(8));
					}
					if (groupNames[k] == toks[i+1]) {
						if (toks[i+3].substr(0, 3) == "VAR") {
							groupValues[k].at(location - 1) = varToInt(toks[i+3]);
						} else {
							//ASSUME INTEGER
							groupValues[k].at(location - 1) = stringToInt(toks[i+3].substr(8));
						}
					}
				}
				i += 3;
			}else{
				cerr << endl << "Unkown Token: " + toks[i] << endl;
			}
		}else{
			if(toks[i] == "ifend" && !doAction){
				doAction = true;
			}else if(toks[i] == "endwhile" && !doWhileAction){
				doWhileAction = true;
				startOfWhile = -23;
			}
		}
		i++;
	}
} 

int main ( int argc, char *argv[] )
{
	srand(time(NULL));
  if ( argc != 2 && argc != 3)
	cout<<"usage: "<< argv[0] <<" <filename> [-d]" << endl;
  else {
	ifstream the_file ( argv[1] );
	if ( !the_file.is_open() )
	  cout<<"Could not open file" << endl;
	} 

  readFile(argv[1]);

  for (long i = 0; i < argc; i++){
  	std::string t = argv[i];
  	if (t == "-d") {
  		testLexer();
  	}
   }
  parse();
}
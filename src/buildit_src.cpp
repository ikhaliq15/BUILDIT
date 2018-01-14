#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <locale>
#include <cmath>

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

std::vector<std::string> toks;

std::vector<std::string> varNames;
std::vector<std::string> varValues;

std::vector<string> markerNames;
std::vector<int> markerLocations;

bool inString = false;


bool isDigits(const std::string &str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}

int stringToInt(const string &str) {
	stringstream ss;
	ss << str;
	int value;
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

int varToInt(string var){
	int indexOfVar = 0;
	for(int j = 0; j < varNames.size(); j++){
		if(varNames[j] == var){
			indexOfVar = j;
			break;
		}
	}

	int firstValue = 0;

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
	int indexOfVar = 0;
	for(int j = 0; j < varNames.size(); j++){
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

bool checkConditional(string op, int first, int second){
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

int changeVariable(string name, string newValue){
	int indexOfVar = 0;
	for(int j = 0; j < varNames.size(); j++){
		if(varNames[j] == name){
			indexOfVar = j;
			break;
		}
	}
	varValues[indexOfVar] = newValue;
}

int doMath(string op, int first, int second){
	int answer = 0;

	if(op == "add"){
		answer = (first + second);
	}else if(op == "sub"){
		answer = (first - second);
	}else if(op == "mult"){
		answer = (first * second);
	}else if(op == "div"){
		answer = (first / second); 
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

	int i = 0;
	while (i < words.size()){
		word = words[i];
		if(not inString){
			if(word == PRINT){
				toks.push_back("print");
			}else if(word == PRINTLN){
				toks.push_back("println");
			}else if(word.at(0) == '\"'){
				if(word.at(word.length()-1) == '\"'){
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

void testLexer(){
	//Testing the lexer
	cout << "Num Toks: " << toks.size() << endl;
	
	for(int i = 0; i <= toks.size()-1;i++){
		cout << toks[i] << endl;
	}

	cout << endl << endl;
}

void testVars(){
	cout << "Num Vars: " << varNames.size() << endl;
	for (int i = 0; i <= varNames.size()-1;i++){
		cout << "Name: " << varNames[i] << " " << "Value: " << varValues[i] << endl;
	}
}


//This takes the translated file and performs actions.
void parse()
{
	int i = 0;
	bool doAction = true;
	bool doWhileAction = true;
	int startOfWhile = -21;

	while( i < toks.size()){
		if(doAction && doWhileAction){
			if(toks[i] == "print"){
				if(toks[i + 1].substr(0, 7) == "STRING "){
					string returnValue = toks[i + 1].substr(8);
					cout << returnValue.substr(0, returnValue.length()-1);
				}else if(toks[i + 1].substr(0, 3) == "VAR"){
					int indexOfVar = 0;
					for(int j = 0; j < varNames.size(); j++){
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
					int indexOfVar = 0;
					for(int j = 0; j < varNames.size(); j++){
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
				for (int j = 0; j < varNames.size(); j++) {
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
				if(toks[i+2] == "add" || toks[i+2] == "sub" || toks[i+2] == "mult" || toks[i+2] == "div"){
					if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 3) == "VAR"){
						int firstValue = varToInt(toks[i+3]);
						int secondValue = varToInt(toks[i+4]);
						int answer = doMath(toks[i+2], firstValue, secondValue);
						varValues.push_back("INTEGER " +  NumberToString(answer));
						i += 2;
					}else if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 7) == "INTEGER"){
						int firstValue = varToInt(toks[i+3]);
						int secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						int answer = doMath(toks[i+2], firstValue, secondValue);
						varValues.push_back("INTEGER " +  NumberToString(answer));
						i += 2;
					}else if(toks[i+4].substr(0, 3) == "VAR" and toks[i+3].substr(0, 7) == "INTEGER"){
						int secondValue = varToInt(toks[i+4]);
						int firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						int answer = doMath(toks[i+2], firstValue, secondValue);
						varValues.push_back("INTEGER " +  NumberToString(answer));
						i += 2;
					}else if(toks[i+4].substr(0, 7) == "INTEGER" and toks[i+3].substr(0, 7) == "INTEGER"){
						int firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						int secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						int answer = doMath(toks[i+2], firstValue, secondValue);
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
				}else{
					varValues.push_back(toks[i+2]);
				}
				//testVars();
				i += 2;
			}else if(toks[i] == "ifend"){
				doAction = true;
			}else if(toks[i] == "if"){
				if(toks[i+1] == "stringequal" || toks[i+1] == "notstringequal"){
					if(toks[i+2].substr(0, 3) == "VAR" and toks[i+3].substr(0, 3) == "VAR"){
						string firstValue = varToString(toks[i+2]);
						string secondValue = varToString(toks[i+3]);
						doAction = checkStringConditional(toks[i+1], firstValue, secondValue);
					}else if(toks[i+2].substr(0, 3) == "VAR" and toks[i+3].substr(0, 6) == "STRING"){
						string firstValue = varToString(toks[i+2]);					
						string secondValue = (toks[i+3].substr(8, toks[i+3].length()-1));
						doAction = checkStringConditional(toks[i+1], firstValue, secondValue);
					}else if(toks[i+3].substr(0, 3) == "VAR" and toks[i+2].substr(0, 6) == "STRING"){
						string secondValue = varToString(toks[i+3]);					
						string firstValue = (toks[i+2].substr(8, toks[i+2].length()-1));
						doAction = checkStringConditional(toks[i+1], firstValue, secondValue);
					}else if(toks[i+3].substr(0, 6) == "STRING" and toks[i+2].substr(0, 6) == "STRING"){
						string firstValue = varToString(toks[i+2].substr(8, toks[i+2].length()-1));
						string secondValue = varToString(toks[i+3].substr(8, toks[i+3].length()-1));
						cout << toks[i+1] << ", " << firstValue << ", " << secondValue << endl;
						doAction = checkStringConditional(toks[i+1], firstValue, secondValue);
					}
				}else{
					if(toks[i+2].substr(0, 3) == "VAR" and toks[i+3].substr(0, 3) == "VAR"){
						int firstValue = varToInt(toks[i+2]);
						int secondValue = varToInt(toks[i+3]);
						doAction = checkConditional(toks[i+1], firstValue, secondValue);
					}else if(toks[i+2].substr(0, 3) == "VAR" and toks[i+3].substr(0, 7) == "INTEGER"){
						int firstValue = varToInt(toks[i+2]);					
						int secondValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						doAction = checkConditional(toks[i+1], firstValue, secondValue);
					}else if(toks[i+3].substr(0, 3) == "VAR" and toks[i+2].substr(0, 7) == "INTEGER"){
						int secondValue = varToInt(toks[i+3]);
						int firstValue = stringToInt(toks[i+2].substr(8, toks[i+2].length()));
						doAction = checkConditional(toks[i+1], firstValue, secondValue);
					}else if(toks[i+3].substr(0, 7) == "INTEGER" and toks[i+2].substr(0, 7) == "INTEGER"){
						int firstValue = stringToInt(toks[i+2].substr(8, toks[i+2].length()));
						int secondValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						doAction = checkConditional(toks[i+1], firstValue, secondValue);
					}
				}
				i += 4;
			}else if(toks[i] == "reassign"){
				if(toks[i+2] == "add" || toks[i+2] == "sub" || toks[i+2] == "mult" || toks[i+2] == "div"){
					string newValue = toks[i+2];
					if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 3) == "VAR"){
						int firstValue = varToInt(toks[i+3]);
						int secondValue = varToInt(toks[i+4]);
						int answer = doMath(toks[i+2], firstValue, secondValue);
						newValue = ("INTEGER " +  NumberToString(answer));
						//i += 2;
					}else if(toks[i+3].substr(0, 3) == "VAR" and toks[i+4].substr(0, 7) == "INTEGER"){
						int firstValue = varToInt(toks[i+3]);
						int secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						int answer = doMath(toks[i+2], firstValue, secondValue);
						newValue = ("INTEGER " +  NumberToString(answer));
						//i += 2;
					}else if(toks[i+4].substr(0, 3) == "VAR" and toks[i+3].substr(0, 7) == "INTEGER"){
						int secondValue = varToInt(toks[i+4]);
						int firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						int answer = doMath(toks[i+2], firstValue, secondValue);
						newValue = ("INTEGER " +  NumberToString(answer));
						//i += 2;
					}else if(toks[i+4].substr(0, 7) == "INTEGER" and toks[i+3].substr(0, 7) == "INTEGER"){
						int firstValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
						int secondValue = stringToInt(toks[i+4].substr(8, toks[i+4].length()));
						int answer = doMath(toks[i+2], firstValue, secondValue);
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
						for (int j = 0; j < varNames.size(); j++) {
							if (varNames[j] == toks[i + 1]) {
								varValues[j] = "INTEGER " + NumberToString(sqrt(stringToInt(toks[i+3].substr(8, toks[i+3].length()))));
							}
						}
						//changeVariable(toks[i+1], "INTEGER " + NumberToString(sqrt(stringToInt(toks[i+3].substr(8, toks[i+3].length())))));
					}
					i += 1;
				}else{
					changeVariable(toks[i+1], toks[i+2]);
				}
				i += 2;
			}else if(toks[i] == "startwhile"){
				if(toks[i+2].substr(0, 3) == "VAR" and toks[i+3].substr(0, 3) == "VAR"){
					int firstValue = varToInt(toks[i+2]);
					int secondValue = varToInt(toks[i+3]);
					doWhileAction = checkConditional(toks[i+1], firstValue, secondValue);
				}else if(toks[i+2].substr(0, 3) == "VAR" and toks[i+3].substr(0, 7) == "INTEGER"){
					int firstValue = varToInt(toks[i+2]);
					//cout << "SECOND VALUE AS NUMBER: " << toks[i+3].substr(8, toks[i+3].length()) << endl;
					int secondValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
					doWhileAction = checkConditional(toks[i+1], firstValue, secondValue);
				}else if(toks[i+3].substr(0, 3) == "VAR" and toks[i+2].substr(0, 7) == "INTEGER"){
					int secondValue = varToInt(toks[i+3]);
					int firstValue = stringToInt(toks[i+2].substr(8, toks[i+2].length()));
					doWhileAction = checkConditional(toks[i+1], firstValue, secondValue);
				}else if(toks[i+3].substr(0, 7) == "INTEGER" and toks[i+2].substr(0, 7) == "INTEGER"){
					int firstValue = stringToInt(toks[i+2].substr(8, toks[i+2].length()));
					int secondValue = stringToInt(toks[i+3].substr(8, toks[i+3].length()));
					doWhileAction = checkConditional(toks[i+1], firstValue, secondValue);
				}
				//cout << doWhileAction << endl;
				startOfWhile = i-1;
				i += 4;
			}else if(toks[i] == "dowhile"){
				//startOfWhile = i;
			}else if(toks[i] == "endwhile"){
				i = startOfWhile;
			}else if (toks[i] == "breakloop"){
				doWhileAction = false;
			}else if (toks[i] == "createmarker"){
				markerNames.push_back(toks[i+1]);
				i++;
				markerLocations.push_back(i);
			}else if (toks[i] == "gotomarker") {
				int location = 0;
				for (int j = 0; j < markerNames.size(); j++) {
					if (markerNames[j] == toks[i + 1]) {
						location = j;
					}
				}
				i = markerLocations[location];
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
  if ( argc != 2 && argc != 3)
	cout<<"usage: "<< argv[0] <<" <filename> [-d]" << endl;
  else {
	ifstream the_file ( argv[1] );
	if ( !the_file.is_open() )
	  cout<<"Could not open file" << endl;
	} 

  readFile(argv[1]);

  for (int i = 0; i < argc; i++){
  	std::string t = argv[i];
  	if (t == "-d") {
  		testLexer();
  	}
   }
  parse();
}
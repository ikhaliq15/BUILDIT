#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <locale>


using namespace std;

//KEYWORDS

string PRINT = "SAY";
string PRINTLN = "SAYLN";
string INPUT = "ASK";
string ASSIGNMENT = "LET";
string ADDITION = "ADD";
string SUBTRACTION = "SUB";
string MULTIPLICATION = "MULT";
string DIVISION = "DIV";
string IF = "IF";
string THEN = "THEN";
string END = "ENDIF";
string EQUAL = "EQU";
string GREATERTHAN = "GRE";
string LESSTHAN = "LESS";
string GREATERTHANEQuAL = "GEQU";
string LESSTHANEQUAL = "LEQU";

std::vector<std::string> toks;

std::vector<std::string> varNames;
std::vector<std::string> varValues;

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

bool checkConditional(string op, int first, int second){
	if(op == "intequal"){
		if(first == second){
			return true;
		}else{
			return false;
		}
	}else if(op == "intgreater"){
		if(first > second){
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
				inString = true;
				curString = word + ' ';
			}else if(word == ASSIGNMENT){
				toks.push_back("assign");
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
			}else if(word == IF){
				toks.push_back("if");
			}else if(word == THEN){
				toks.push_back("ifthen");
			}else if(word == END){
				toks.push_back("ifend");
			}else if(word == EQUAL){
				toks.push_back("intequal");
			}else if(word == GREATERTHAN){
				toks.push_back("intgreater");
			}else if(word == LESSTHAN){
				toks.push_back("intless");
			}else if(word == GREATERTHANEQuAL){
				toks.push_back("intgreaterequal");
			}else if(word == LESSTHANEQUAL){
				toks.push_back("intlessequal");
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
	cout << "Num Toks: " << toks.size() << "\n";
	
	for(int i = 0; i <= toks.size()-1;i++){
		cout << toks[i] << "\n";
	}

	cout << "\n\n";
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

	while( i < toks.size()){
		if(doAction){
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
					cout << returnValue.substr(0, returnValue.length()-1) << "\n";
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
						cout << returnValue.substr(0, returnValue.length()-1) << "\n";
					}else if(varValues[indexOfVar].substr(0, 7) == "INTEGER"){
						string returnValue = varValues[indexOfVar].substr(8);
						cout << returnValue.substr(0, returnValue.length()) << "\n";
					}  
				}else if(toks[i + 1].substr(0, 7) == "INTEGER"){
					cout << toks[i+1].substr(8, toks[i+1].length()) << "\n";
				}
				i++;
			}else if(toks[i] == "input"){
				string x;
				cin >> x;
				varNames.push_back(toks[i+1]);
				varValues.push_back("STRING \"" + x + "\"");
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
				}else{
					varValues.push_back(toks[i+2]);
				}
				i += 2;
			}else if(toks[i] == "ifend"){
				doAction = true;
			}else if(toks[i] == "if"){
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
				i += 4;
			}else{
				cerr << "Unkown Token: " + toks[i] << "\n";
			}
		}else{
			if(toks[i] == "ifend"){
				doAction = true;
			}
		}
		i++;
	}
} 

int main ( int argc, char *argv[] )
{
  if ( argc != 2 )
	cout<<"usage: "<< argv[0] <<" <filename>\n";
  else {
	ifstream the_file ( argv[1] );
	if ( !the_file.is_open() )
	  cout<<"Could not open file\n";
	} 

  readFile(argv[1]);
  //testLexer();
  parse();
}
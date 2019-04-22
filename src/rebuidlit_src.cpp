#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

string keywords[] = {
	"SAY", "SAYLN", "ASK", "LET", "CHANGE", "ADD", "SUB", "MULT", "DIV", "SQRT", "MOD", "TOSTRING",
	"CHARAT", "LEN", "LENGTH", "TONUM", "TOASCII", "RANDINT", "REVERSE", "IF", "THEN", "ENDIF", "EQU",
	"NEQU", "GRE", "LESS", "GEQU", "LEQU", "SEQU", "NSEQU", "WHILE", "DO", "ENDWHILE", "LEAVE",
	"READFILE", "NEWGROUP", "ADDTOGROUP", "ACCESSGROUP", "SETGROUP",
};

string keywordTranslations[] = {
	"print", "println", "input", "assign", "reassign", "add", "sub", "mult", "div", "sqrt", "modulus",
	"casttostring", "charatindex", "lengthof", "lengthof", "casttoint", "converttoascii", "randomint", "reversestring", 
	"if", "ifthen", "endif", "intequal", "intnotequal", "intgreater", "intless", "intgreaterequal",
	"intlessequal", "stringequal", "stringnotequal", "startwhile", "dowhile", "endwhile", "breakloop",
	"readfile", "createemptygroup", "appendtogroup", "accessgroup", "setgroupat"
};

struct token {
	string type;
	string value;
};
void copyToken(const token t1, token &t2) {
	t2.type = t1.type;
	t2.value = t1.value;
}
vector<token> tokens;

struct variable {
	string name;
	string type;
	string value;
};
vector<variable> variables;

struct marker {
	string name;
	int position;
};
vector<marker> markers;

struct value{
	string type;
	string value;
	int positionsSkipped;
};

struct loop{
	int startPosition;
	int doPosition;
	bool isRunning;
};
vector<loop> loops;

struct group {
	string name;
	vector<value> values;
};
vector<group> groups;

// int findString(string a[], int l, string s);
// Returns the index of string s in array a, or -1 if not found.
int findString(string a[], int l, string s) {
	for (int i = 0; i < l; i++) {
		if (a[i] == s) {
			return i;
		}
	}
	return -1;
}

// bool isDigits(string s);
// Returns true if s is all numerical digits, otherwise false.
bool isDigits(string s) {
	return s.find_first_not_of("0123456789.") == string::npos;
}

// string reverseString(string s)
string reverseString(string s){
    int n = s.length();
    for (int i=0; i<n/2; i++)
       swap(s[i], s[n-i-1]);
   return s;
}

// long stringToInt(string s);
long stringToInteger(string s){
	stringstream ss;
	ss << s;
	long value;
	ss >> value;
	return value;
}

// string boolToString(bool b);
string boolToString(bool b) {
  return b ? "true" : "false";
}

// void assignVariable(string n, value v);
void assignVariable(string n, value v) {
	// Check for an already existing variable with name n.
	for (int i = 0; i < variables.size(); i++) {
		variable currentVariable = variables[i];
		if (currentVariable.name == n) {
			variables[i].value = v.value;
			variables[i].type = v.type;
			return;
		}
	}

	// If no existing variable was found, create one.
	variable newVariable = {n, v.type, v.value};
	variables.push_back(newVariable);
}

// value getVariable(string n);
value getVariable(string n) {
	for (int i = 0; i < variables.size(); i++) {
		variable currentVariable = variables[i];
		if (currentVariable.name == n) {
			value v = {currentVariable.type, currentVariable.value};
			return v;
		}
	}
	value emptyValue = {"", ""};
	return emptyValue;
}

// void addToGroup(string n, value v);
void addToGroup(string n, value v, int in = -2) {
	for (int i = 0; i < groups.size(); i++) {
		group currentGroup = groups[i];
		if (currentGroup.name == n) {
			if (in == -2) {
				groups[i].values.push_back(v);
			} else {
				groups[i].values[in - 1] = v;
			}
			return;
		}
	}
}

// group getGroup(string n);
group getGroup(string n) {
	for (int i = 0; i < groups.size(); i++) {
		group currentGroup = groups[i];
		if (currentGroup.name == n) {
			return currentGroup;
		}
	}
	vector<value> emptyValues;
	group emptyGroup = {"", emptyValues};
	return emptyGroup;
}

// int findNextTokenAfter(token t, int i);
int findNextTokenAfter(token t, int i) {
	for (int j = i; j < tokens.size(); j++) {
		token currentToken = tokens[j];
		if (currentToken.value == t.value && currentToken.type == t.type) {
			return j;
		}
	}
	return -1;
}

// marker findMarker(string n);
marker findMarker(string n) {
	for (int i = 0; i < markers.size(); i++) {
		marker currentMarker = markers[i];
		if (currentMarker.name == n) {
			return currentMarker;
		}
	}
	marker emptyMarker = {"", -1};
	return emptyMarker;
}

// value getExpression(int i);
// Returns the value of the expression at index i in vector tokens.
value getExpression(int i) {
	value v = {"", "", 1};

	if (tokens[i].type == "string") {
		v.type = "string";
		v.value = tokens[i].value;
	} else if (tokens[i].type == "integer") {
		v.type = "integer";
		v.value = tokens[i].value;
	} else if (tokens[i].type == "boolean") {
		v.type = "boolean";
		v.value = tokens[i].value;
	} else if (tokens[i].type == "variable") {
		value r = getVariable(tokens[i].value);
		r.positionsSkipped = 1;
		return r;
	} else if (tokens[i].value == "add") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "integer";
		v.value = to_string(stringToInteger(v1.value) + stringToInteger(v2.value));
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "sub") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "integer";
		v.value = to_string(stringToInteger(v1.value) - stringToInteger(v2.value));
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "mult") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "integer";
		v.value = to_string(stringToInteger(v1.value) * stringToInteger(v2.value));
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "div") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "integer";
		v.value = to_string(stringToInteger(v1.value) / stringToInteger(v2.value));
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "modulus") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "integer";
		v.value = to_string(stringToInteger(v1.value) % stringToInteger(v2.value));
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "sqrt") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		v.type = "integer";
		v.value = to_string(pow(stringToInteger(v1.value), 0.5));
		v.positionsSkipped += v1.positionsSkipped;
	} else if (tokens[i].value == "lengthof") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		v.type = "integer";
		v.value = to_string(v1.value.size());
		v.positionsSkipped += v1.positionsSkipped;
	} else if (tokens[i].value == "charatindex") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "string";
		string s(1, v1.value.at(stringToInteger(v2.value) - 1));
		v.value = s;
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "converttoascii") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		v.type = "string";
		string s(1, static_cast<char>(stringToInteger(v1.value)));
		v.value = s;
		v.positionsSkipped += v1.positionsSkipped;
	} else if (tokens[i].value == "reversestring") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		v.type = "string";
		v.value = reverseString(v1.value);
		v.positionsSkipped += v1.positionsSkipped;
	} else if (tokens[i].value == "casttostring") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		v.type = "string";
		v.value = v1.value;
		v.positionsSkipped += v1.positionsSkipped;
	} else if (tokens[i].value == "casttoint") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		v.type = "integer";
		v.value = v1.value;
		v.positionsSkipped += v1.positionsSkipped;
	} else if (tokens[i].value == "intequal" || tokens[i].value == "stringequal") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "boolean";
		v.value = boolToString(v1.value == v2.value && v1.type == v2.type);
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "intnotequal" || tokens[i].value == "stringnotequal") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "boolean";
		v.value = boolToString(v1.value != v2.value || v1.type != v2.type);
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "intgreater") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "boolean";
		v.value = boolToString(stringToInteger(v1.value) > stringToInteger(v2.value) && v1.type == v2.type);
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "intless") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "boolean";
		v.value = boolToString(stringToInteger(v1.value) < stringToInteger(v2.value) && v1.type == v2.type);
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "intgreaterequal") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "boolean";
		v.value = boolToString(stringToInteger(v1.value) >= stringToInteger(v2.value) && v1.type == v2.type);
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "intlessequal") {
		value v1 = getExpression(i + 1);
		i += v1.positionsSkipped;
		value v2 = getExpression(i + 1);
		i += v2.positionsSkipped;
		v.type = "boolean";
		v.value = boolToString(stringToInteger(v1.value) <= stringToInteger(v2.value) && v1.type == v2.type);
		v.positionsSkipped += v1.positionsSkipped + v2.positionsSkipped;
	} else if (tokens[i].value == "accessgroup") {
		group selectedGroup = getGroup(tokens[i + 1].value);
		value v = getExpression(i + 2);
		value selectedItem = selectedGroup.values[stringToInteger(v.value) - 1];
		value r = {selectedItem.type, selectedItem.value};
		r.positionsSkipped = 3;
		return r;
	}

	return v;
}

// void tranlsateProgram(char* filename);
// Iterates through the words in filename input stream and 
// populate the token vector with the proper translation.
void translateProgram(char* filename) {
	
	// Open the program file.
	ifstream programFile;
	programFile.open(filename);

	// Make sure the file can be opened.
	if (!programFile.is_open()) {
		return;
	}

	// Put all the words in programFile stream into the words vector.
	vector<string> words;
	string currentWord = "";
	while (!programFile.eof()) {
		programFile >> currentWord;
		words.push_back(currentWord);
	}

	// Loop through all the words and translate.
	long i = 0;
	currentWord = "";
	string currentString = "";
	bool inString = false;
	while (i < words.size()) {
		currentWord = words[i];

		// Check if this is a string.
		if (inString) {
			if(currentWord.at(currentWord.length()-1) == '\"'){
				inString = false;
				currentString = currentString + currentWord;
				token t = {"string", currentString.substr(1, currentString.length()-2)};
				tokens.push_back(t);
				currentString = "";
			}else{
				currentString = currentString + currentWord + ' ';
			}
			i++;
			continue;
		}
		if(currentWord.at(0) == '\"'){
			if(currentWord.at(currentWord.length()-1) == '\"' && currentWord != "\""){
				inString = false;
				currentString = currentString + currentWord;
				token t = {"string", currentString.substr(1, currentString.length()-2)};
				tokens.push_back(t);
				currentString = "";
			} else {
				inString = true;
				currentString = currentWord + ' ';
			}
			i++;
			continue;
		}

		// Check if the current word is a number.
		if (isDigits(currentWord)) {
			token t = {"integer", currentWord};
			tokens.push_back(t);
			i++;
			continue;
		}

		// Check if the current word is a boolean.
		if (currentWord == "true" || currentWord == "false") {
			token t = {"boolean", currentWord};
			tokens.push_back(t);
			i++;
			continue;
		}

		// Check if setting a marker location.
		if (currentWord == "SETMARKER") {
			token t1 = {"keyword", "setmarker"};
			token t2 = {"marker", words[i+1]};
			tokens.push_back(t1);
			tokens.push_back(t2);
			i += 2;
			continue;
		}

		// Check if going to a marker location.
		if (currentWord == "GOTOMARKER") {
			token t1 = {"keyword", "gotomarker"};
			token t2 = {"marker", words[i+1]};
			tokens.push_back(t1);
			tokens.push_back(t2);
			i += 2;
			continue;
		}

		// Check if currentWord is a keyword.
		int currentKeywordIndex = findString(keywords, sizeof(keywords)/sizeof(keywords[0]), currentWord);
		if (currentKeywordIndex != -1) {
			token t = {"keyword", keywordTranslations[currentKeywordIndex]};
			tokens.push_back(t);
			i++;
			continue;
		}

		token t = {"variable", currentWord};
		tokens.push_back(t);

		i++;
	}
}

// int executeTokens();
// Iterates through the tokens and executes commands.
void executeTokens(int startIndex = 0) {

	long i = 0;

	// Load the positions of all markers in the program.
	while (i < tokens.size() && startIndex == 0) {
		if (tokens[i].value == "setmarker"){
			marker m = {tokens[i+1].value, i};
			markers.push_back(m);
			i++;
		}
		i++;
	}

	token currentToken = {"", ""};
	token nextToken = {"", ""};
	i = startIndex;
	while (i < tokens.size()) {

		int tokensToSkip = 1;

		copyToken(tokens[i], currentToken);

		// If the current 'while' loop is not running, skip this code.
		// TODO: Make sure that the endwhile is the corresponding one.
		if (loops.size() > 0) {
			if (!loops[loops.size()-1].isRunning && currentToken.value != "endwhile") {
				i++;
				continue;
			}
		}

		if (currentToken.value == "print") {
			value nextExpression = getExpression(i + 1);
			tokensToSkip += nextExpression.positionsSkipped;
			cout << nextExpression.value;
		} else if (currentToken.value == "println") {
			value nextExpression = getExpression(i + 1);
			tokensToSkip += nextExpression.positionsSkipped;
			cout << nextExpression.value << endl;
		} else if (currentToken.value == "input") {
			string userInput = "";
			string nameOfVariable = tokens[i + 1].value;
			tokensToSkip += 1;
			getline(cin, userInput);
			value v = {"string", userInput};
			assignVariable(nameOfVariable, v);
		} else if (currentToken.value == "readfile") {
			string fileContents = "";

			// Get the name of the file to be read.
			value fileName = getExpression(i + tokensToSkip);
			tokensToSkip += fileName.positionsSkipped;

			// Get the name of the variable to put the file contents in.
			string nameOfVariable = tokens[i + tokensToSkip].value;
			tokensToSkip += 1;

			// Read the file and put them into fileContents
			ifstream t(fileName.value);
			stringstream buffer;
			buffer << t.rdbuf();
			fileContents = buffer.str();

			// Assign the variable with fileContents.
			value v = {"string", fileContents};
			assignVariable(nameOfVariable, v);
		} else if (currentToken.value == "assign" || currentToken.value == "reassign") {
			value nextExpression = getExpression(i + 2);
			string nameOfVariable = tokens[i + 1].value;
			tokensToSkip += 1;
			tokensToSkip += nextExpression.positionsSkipped;
			assignVariable(nameOfVariable, nextExpression);
		} else if(currentToken.value == "createemptygroup") {
			string nameOfGroup = tokens[i + 1].value;
			tokensToSkip += 1;
			value v = {"group", nameOfGroup};
			assignVariable(nameOfGroup, v);
			vector<value> emptyValues;
			group emptyGroup = {nameOfGroup, emptyValues};
			groups.push_back(emptyGroup);
		} else if (currentToken.value == "appendtogroup") {
			string nameOfGroup = tokens[i + 1].value;
			tokensToSkip += 1;
			value nextExpression = getExpression(i + 2);
			tokensToSkip += nextExpression.positionsSkipped;
			addToGroup(nameOfGroup, nextExpression, -2);
		} else if (currentToken.value == "setgroupat") {
			string nameOfGroup = tokens[i + 1].value;
			tokensToSkip += 1;
			value exp1 = getExpression(i + 2);
			tokensToSkip += exp1.positionsSkipped;
			value exp2 = getExpression(i + tokensToSkip);
			tokensToSkip += exp2.positionsSkipped;
			addToGroup(nameOfGroup, exp2, stringToInteger(exp1.value));
		} else if (currentToken.value == "if") {
			value nextExpression = getExpression(i + 1);
			tokensToSkip += nextExpression.positionsSkipped;
			if (nextExpression.value == "false") {
				token endIf = {"keyword", "endif"};
				int nextEndIf = findNextTokenAfter(endIf, i);
				if (nextEndIf != -1) {
					i = nextEndIf + 1;
					continue;
				}
			}
		} else if (currentToken.value == "startwhile") {
			loop l = {i, i + getExpression(i + 1).positionsSkipped + 1, getExpression(i + 1).value == "true"};
			loops.push_back(l);
		} else if (currentToken.value == "endwhile") {
			if (getExpression(loops[loops.size() - 1].startPosition + 1).value == "true") {
				i = loops[loops.size() - 1].doPosition;
			} else {
				loops.pop_back();
			}
		} else if (currentToken.value == "gotomarker") {
			string nameOfMarker = tokens[i + 1].value;
			tokensToSkip += 1;
			marker m = findMarker(nameOfMarker);
			i = m.position;
			continue;
		}

		i += tokensToSkip;
	}
}

int main(int argc, char *argv[]) {

	// Booleans for argument settesr
	bool debugMode = false;
	bool dontRun = false;

	// Set the seed for random.
	srand(time(0));

	// Check if the command is being used properly.
	if (argc != 2 && argc != 3) {
		cout << "usage: " << argv[0] << " <filename> [-d]" << endl;
  	} else {
  		// Check if the file exists and was opened correctly.
		ifstream the_file ( argv[1] );
		if (!the_file.is_open()) {
		  cout << "Could not open file" << endl;
		}
	} 

	// Run through arguments and act accordingly.
	for (int i = 0; i < argc; i++){
  		string currentArgument = argv[i];
  		if (currentArgument == "-d") {
  			debugMode = true;
  		}
  		if (currentArgument == "-n") {
  			dontRun = true;
  		}
   	}

	// Run the program through the lexer.
	translateProgram(argv[1]);

	// If debug mode is on, print results of lexer.
	if (debugMode) {
		cout << "Number of Tokens: " << tokens.size() << endl;
		for(long i = 0; i <= tokens.size()-1; i++){
			cout << "#: " << i << " " << tokens[i].type << ": " << tokens[i].value << endl;
		}
		cout << endl << endl;
	}

	// Put the tokens through the parser.
	if (!dontRun) {
		executeTokens();
	}

	return 0;
}
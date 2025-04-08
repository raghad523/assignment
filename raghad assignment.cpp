#include <iostream>      // Includes the input/output stream library (used for cout, etc.)
#include <cctype>        // Includes character handling functions (isalpha, isdigit, isspace, etc.)
#include <string>        // Includes the string class for handling strings

using namespace std;     // Allows using standard names (like cout, string) without prefixing with std::

#define LETTER 0         // Constant representing a letter character
#define DIGIT 1          // Constant representing a digit character
#define UNKNOWN 99       // Constant for unknown (symbols/operators)
#define END_OF_FILE -1   // Constant for end of file/input

// Token type definitions (representing different types of tokens)
#define INT_LIT 10       // Integer literal (e.g., 5, 123)
#define IDENT 11         // Identifier (e.g., variable names)
#define ASSIGN_OP 20     // Assignment operator '='
#define ADD_OP 21        // Addition operator '+'
#define SUB_OP 22        // Subtraction operator '-'
#define MULT_OP 23       // Multiplication operator '*'
#define DIV_OP 24        // Division operator '/'
#define LEFT_PAREN 25    // Left parenthesis '('
#define RIGHT_PAREN 26   // Right parenthesis ')'

// Global variables used in the lexical analyzer
int charClass;           // Class of the current character (LETTER, DIGIT, etc.)
string lexeme;           // Current lexeme (token string)
char nextChar;           // Next character from the input
int lexLen;              // Length of the current lexeme
int token;               // Current token type
int nextToken;           // Next token type
string inputLine;        // Input line to be analyzed
int lineIndex = 0;       // Current index in the input line

// Function declarations
void addChar();          // Adds the current character to the lexeme
void getChar();          // Gets the next character from input
void getNonBlank();      // Skips whitespace characters
int lookup(char ch);     // Determines token type for single-character operators/symbols
int lex();               // Lexical analyzer function

/* Adds the current character to the lexeme */
void addChar() {
    if (lexLen <= 98) {             // Check if lexeme length is within the limit
        lexeme += nextChar;         // Append current character to the lexeme
        lexLen++;                   // Increase the lexeme length counter
    }
    else {
        cout << "Error - lexeme is too long" << endl;  // Print error if too long
    }
}

/* Gets the next character from the input line */
void getChar() {
    if (lineIndex < inputLine.length()) {   // Check if more characters are available
        nextChar = inputLine[lineIndex++];  // Get next character and advance index
        if (isalpha(nextChar))              // Check if it's a letter
            charClass = LETTER;             // Assign class LETTER
        else if (isdigit(nextChar))         // Check if it's a digit
            charClass = DIGIT;              // Assign class DIGIT
        else
            charClass = UNKNOWN;            // Otherwise, assign class UNKNOWN
    }
    else {
        charClass = END_OF_FILE;            // No more characters; mark end of input
        nextChar = '\0';                    // Set nextChar to null character
    }
}

/* Skips blank spaces */
void getNonBlank() {
    while (isspace(nextChar))   // While current character is a space or tab
        getChar();              // Get the next character
}

/* Looks up and returns the token code for a given symbol character */
int lookup(char ch) {
    switch (ch) {
    case '(': addChar(); nextToken = LEFT_PAREN; break;   // Left parenthesis
    case ')': addChar(); nextToken = RIGHT_PAREN; break;  // Right parenthesis
    case '+': addChar(); nextToken = ADD_OP; break;       // Plus operator
    case '-': addChar(); nextToken = SUB_OP; break;       // Minus operator
    case '*': addChar(); nextToken = MULT_OP; break;      // Multiplication
    case '/': addChar(); nextToken = DIV_OP; break;       // Division
    case '=': addChar(); nextToken = ASSIGN_OP; break;    // Assignment operator
    default:  addChar(); nextToken = UNKNOWN; break;      // Unknown character
    }
    return nextToken;  // Return the token type
}

/* The lexical analyzer function that identifies the next token */
int lex() {
    lexLen = 0;          // Reset lexeme length
    lexeme = "";         // Clear the lexeme string
    getNonBlank();       // Skip any whitespace

    switch (charClass) {
    case LETTER:             // If character is a letter
        addChar();           // Add to lexeme
        getChar();           // Move to next character
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();       // Keep adding letters/digits
            getChar();
        }
        nextToken = IDENT;   // This is an identifier
        break;

    case DIGIT:              // If character is a digit
        addChar();           // Add to lexeme
        getChar();           // Move to next character
        while (charClass == DIGIT) {
            addChar();       // Keep adding digits
            getChar();
        }
        nextToken = INT_LIT; // This is an integer literal
        break;

    case UNKNOWN:            // If character is unknown (probably an operator)
        lookup(nextChar);    // Try to match it to a known token
        getChar();           // Move to the next character
        break;

    case END_OF_FILE:        // If we reached the end of input
        nextToken = END_OF_FILE; // Set token as END_OF_FILE
        lexeme = "EOF";          // Set lexeme text to "EOF"
        break;
    }

    // Output the token and lexeme found
    cout << "Next token is: " << nextToken << ", Next lexeme is: " << lexeme << endl;
    return nextToken;  // Return the token type
}

/* Main function: entry point of the program */
int main() {
    inputLine = "G(8%2)-3";  // The input line to analyze
    getChar();              // Start by reading the first character
    while (nextToken != END_OF_FILE) {  // Continue until end of input
        lex();                          // Analyze the next token
    }
    return 0;   // Program ends successfully
}
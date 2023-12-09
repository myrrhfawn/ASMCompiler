#pragma once
#include <fstream>
#include <map>

#define MAX_TOKENS 1000
#define MAX_IDENTIFIER 10
#define SyntaxAnalyxer
#define BaseNode


// перерахування, яке описує всі можливі типи лексем
enum TypeOfTokens
{
    StartProgram,   // Program
    VarStart,          // Start    
    Variable,		// Var
    Type,   		// Int_4
    EndProgram,		// Finsih
    Input,			// input
    Output,			// Print
    If,	    		// If
    Then,			// then
    Else,           // Else
    While,          // While
    Do,             // Do
    End,             // End
    Repeat,          // Repeat
    Until,           // Until
    For,             // For
    Downto,          // Downto
    To,              // To
    Goto,            //Goto
    Identifier,	    // Identifier

    Number,		    // Number
    Tab,
    Assign,		    // :=
    Add,			// +
    Sub,			// -
    Mul,			// *
    Div,			// /
    Mod,            // %

    Equality,		// =
    NotEquality,	// <>
    Greate,		    // >
    Less,			// <
    Not,			// !
    And,			// &
    Or,			    // |
    
    Expression,
    Condition,

    LBraket,		// (
    RBraket,		// )
    Semicolon,		// ;
    Comma,			// ,
    Comment,        // ??
    Unknown
};




// структура для зберігання інформації про лексему
struct Token
{
    std::string name;
    int value;          // значення лексеми (для цілих констант)
    int line;           // номер рядка
    TypeOfTokens type;  // тип лексеми
    bool isNode = false;
};

typedef  char Id[5];

// перерахування, яке описує стани лексичного аналізатора
enum States
{
    Start,      // початок виділення чергової лексеми    
    Finish,     // кінець виділення чергової лексеми
    Letter,     // опрацювання слів (ключові слова і ідентифікатори)
    Digit,      // опрацювання цифри
    Keyword, // видалення пробілів, символів табуляції і переходу на новий рядок
    Another     // опрацювання інших символів
};

// функція отримує лексеми з вхідного файлу F і записує їх у таблицю лексем TokenTable 
// результат функції - кількість лексем
unsigned int GetTokens(std::ifstream &inputFile, Token TokenTable[]);

void PrintTokens(Token TokenTable[], unsigned int TokensNum);

void PrintTokensToFile(char* FileName, Token TokenTable[], unsigned int TokensNum);

#include <memory>
#include <string>
#include <vector>

class RootNode {
public:
    virtual void addChild(RootNode* child);
    virtual std::vector<RootNode*> getChildren() const;
    virtual void print(const int tab = 0) const;
    virtual TypeOfTokens getType() const;

private:
    std::vector<RootNode*> children;
    TypeOfTokens type;
};


class ProgramNode : public RootNode {
public:
    ProgramNode(TypeOfTokens type = StartProgram) : type(type) {}
    void print(const int tab = 0) const override;
private:
   TypeOfTokens type;
};

class CommentNode : public RootNode {
public:
    CommentNode(std::vector<Token> comment, TypeOfTokens type = Comment);
    void print(const int tab = 0) const override;
private:
    std::vector<Token> comment;
    TypeOfTokens type;
};


class StartVarNode : public RootNode {
public:
    StartVarNode(TypeOfTokens type = VarStart) : type(type) {}
    void print(const int tab = 0) const override;

private:
    TypeOfTokens type;
};

class VarNode : public RootNode {
public:
    VarNode(const std::string identifier, int value, TypeOfTokens type = Identifier);
    void print(const int tab = 0) const override;
    void getVar();
private:
    TypeOfTokens type;
    std::string identifier;
    int value;
};

class InputNode : public RootNode {
public:
    InputNode(RootNode* var, TypeOfTokens type = Input);
    void print(const int tab = 0) const override;
private:
    TypeOfTokens type;
    RootNode* var;
};

class OutputNode : public RootNode {
public:
    OutputNode(RootNode* var, TypeOfTokens type = Output);
    void print(const int tab = 0) const override;
private:
    TypeOfTokens type;
    RootNode* var;
};


class AssignNode : public RootNode {
public:
    AssignNode(TypeOfTokens type = Assign);
    void print(const int tab = 0) const override;

private:
    TypeOfTokens type;
};

class ExpressionNode : public RootNode {
public:
    ExpressionNode(TypeOfTokens type = Expression);
    ExpressionNode(const ExpressionNode& other);
    ExpressionNode& operator=(const ExpressionNode& other);
    void print(const int tab = 0) const override;
    void setSign(std::string sign);
    void addLeftExp(RootNode* expression);
    void addRightExp(RootNode* expression);
    TypeOfTokens getType() const override;
    RootNode* GetRight() const;
    RootNode* GetLeft() const;

private:
    RootNode* leftExp;
    RootNode* rightExp;
    std::string sign;
    TypeOfTokens type;
};

class TokenProcessor {
public:
    TokenProcessor(std::vector<Token>& tokenString) : tokens(tokenString), currentIndex(0) {}

    // Видалити поточний елемент і повернути його
    Token getNextToken();
    void addToken(const Token& newToken);
    int getSize();
private:
    std::vector<Token>& tokens;
    size_t currentIndex;
};

class LineProcessor {
public:
    LineProcessor(std::vector<std::vector<Token>>& TokenTable) : lines(TokenTable), currentIndex(0) {}

    // Видалити поточний елемент і повернути його
    std::vector<Token> getNextLine();
    void addLine(std::vector<Token> line);
    int getSize();
private:
    std::vector<std::vector<Token>>& lines;
    size_t currentIndex;
};

class NumberNode : public RootNode {
public:
    NumberNode(int value = 0, TypeOfTokens type = Number);
    void setValue(int value);
    void print(const int tab = 0) const override;
    TypeOfTokens getType() const override;
private:
    int value;
    TypeOfTokens type;
};

class IfNode : public RootNode {
public:
    IfNode(RootNode* condition, RootNode* elseNode =  nullptr,TypeOfTokens type = If);
    void print(const int tab = 0) const override;

private:
    TypeOfTokens type;
    RootNode* condition;
    RootNode* elseNode;
};

class ElseNode : public RootNode {
public:
    ElseNode(TypeOfTokens type = Else) : type(type) {}
    void print(const int tab = 0) const override;

private:
    TypeOfTokens type;
};

class ForNode : public RootNode {
public:
    ForNode(RootNode* var, RootNode* assigment, int step, RootNode* goal, TypeOfTokens type = For);
    void print(const int tab = 0) const override;

private:
    TypeOfTokens type;
    RootNode* var;
    RootNode* assign;
    RootNode* goal;
    int step;
};

class NotNode : public RootNode {
public:
    NotNode(TypeOfTokens type = Not);
    void print(const int tab = 0) const override;

private:
    TypeOfTokens type;
};

class ConditionNode : public RootNode {
public:
    ConditionNode(TypeOfTokens type = Condition);
    ConditionNode(const ConditionNode& other);
    ConditionNode& operator=(const ConditionNode& other);
    void print(const int tab = 0) const override;
    void setSign(std::string sign);
    void addLeftExp(RootNode* expression);
    void addRightExp(RootNode* expression);
    TypeOfTokens getType() const override;
    RootNode* GetRight() const;
    RootNode* GetLeft() const;

private:
    RootNode* leftExp;
    RootNode* rightExp;
    std::string sign;
    TypeOfTokens type;
};


class SyntaxAnalyzer {
public:
    // Конструктор
    SyntaxAnalyzer(const Token* TokenTable, unsigned int NumberOfLines);

    // Методи
    void analyzeSyntax();
    void createNodes(LineProcessor* lineProc, RootNode* rootNode, int pos = 0);
    RootNode* generateExpressionNode(TokenProcessor* tokenProc, int priority);
    RootNode* generateConditionNode(TokenProcessor* tokenProc, int priority);
    void writeToFile(const std::vector<std::string>& lines, const std::string& section = "");
    void createFile();


private:
    // Вектор вказівників на об'єкти класу Token
    const Token* TokenTable;
    TokenProcessor* tokenProcessor;
    unsigned int NumberOfTokens;
    std::string filename;
};


#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Data.h"
#include <fstream>
#include <vector>


    // Конструктор класу SyntaxAnalyzer, може приймати рядок чи масив токенів
SyntaxAnalyzer::SyntaxAnalyzer(const Token* TokenTable, unsigned int NumberOfTokens) {
    this->TokenTable = TokenTable;
    this->NumberOfTokens = NumberOfTokens;
};

    // Метод для аналізу синтаксису
void SyntaxAnalyzer::analyzeSyntax() {
    int current_line = 1;
    int token_pos = 0;
	int line_count = this->TokenTable[this->NumberOfTokens-1].line;
	//Token** TokenStringTable = new Token*[line_count];
    std::vector<std::vector<Token>> TokenStringTable;

    int current_pos = 0;
    int offset = 0;
    
    while (offset < this->NumberOfTokens) {
        int start_line = this->TokenTable[offset].line;
        std::vector<Token> TokenString;
        while (this->TokenTable[offset].line <= start_line && offset <= this->NumberOfTokens) {
            TokenString.push_back(TokenTable[offset]);
            offset++;
        }
        TokenStringTable.push_back(TokenString);
    }

    RootNode* rootNode = new RootNode();
    this->createNodes(TokenStringTable, rootNode);
    rootNode->print();
}



Token TokenProcessor::getNextToken()
{
    if (currentIndex < tokens.size()) {
        Token removedToken = tokens[0];
        tokens.erase(tokens.begin());
        return removedToken;
    }
    else {
        throw std::out_of_range("No more tokens in the vector.");
    }
}

void TokenProcessor::addToken(const Token& newToken) {
    // Додати елемент на початок вектора
    tokens.insert(tokens.begin(), newToken);
}

int TokenProcessor::getSize()
{
    return this->tokens.size();
}

InputNode::InputNode(RootNode* var, TypeOfTokens type)
{
    this->var = var;
}

void InputNode::print(const std::string& tab) const
{
    std::cout << "Need to input: ";
    this->var->print();
    std::cout << std::endl;
}

OutputNode::OutputNode(RootNode* var, TypeOfTokens type)
{
    this->var = var;
}

void OutputNode::print(const std::string& tab) const
{
    std::cout << "Print:  ";
    this->var->print();
    std::cout << std::endl;

}




void SyntaxAnalyzer::createNodes(std::vector<std::vector<Token>> TokenStringTable, RootNode* rootNode) {
    for (const auto& TokenString : TokenStringTable) {
        if (TokenString.size() != 0) {
            if (TokenString[0].type == StartProgram) {
                ProgramNode* program = new ProgramNode();

                std::vector<std::vector<Token>> newTable(TokenStringTable.end() - TokenStringTable.size() + 1, TokenStringTable.end());
                // Видалення останніх п'яти елементів із вихідного вектора
                TokenStringTable.erase(TokenStringTable.end() - TokenStringTable.size() + 1, TokenStringTable.end());
                // Виклик функції та передача нового вектора як параметра
                ;
                createNodes(newTable, program);
                rootNode->addChild(program);
            }
            if (TokenString[0].type == VarStart) {
                StartVarNode* startVar = new StartVarNode();
                for (const auto& token : TokenString) {
                    if (token.type == Identifier) {
                        VarNode* var = new VarNode(token.name, token.value);
                        startVar->addChild(var);
                    }
                }
                rootNode->addChild(startVar);
            }
            if (TokenString[0].type == Identifier) {
                if (TokenString[1].type == Assign) {

                    AssignNode* assign = new AssignNode();
                    VarNode* identifier = new VarNode(TokenString[0].name, TokenString[0].value);
                    std::vector<Token> newString(TokenString.end() - TokenString.size() + 2, TokenString.end());
                    RootNode* expression = new RootNode();
                    TokenProcessor* tokenProc = new TokenProcessor(newString);
                    expression = generateExpressionNode(tokenProc, 0);
                    assign->addChild(identifier);
                    assign->addChild(expression);
                    rootNode->addChild(assign);
                }
            }
            if (TokenString[0].type == Comment) {
                CommentNode* comment = new CommentNode(TokenString);
                rootNode->addChild(comment);
            }
            if (TokenString[0].type == Input) {
                if (TokenString.size() > 2) {
                    std::cout << "too many arguments for Scan." << std::endl;
                }
                else {
                    VarNode* var = new VarNode(TokenString[1].name, TokenString[1].value);
                    InputNode* input = new InputNode(var);
                    rootNode->addChild(input);
                }
            }
            if (TokenString[0].type == Output) {
                if (TokenString.size() > 2) {
                    std::cout << "too many arguments for Print." << std::endl;
                }
                else {
                    VarNode* var = new VarNode(TokenString[1].name, TokenString[1].value);
                    OutputNode* output = new OutputNode(var);
                    rootNode->addChild(output);
                }
            }
            if (TokenString[0].type == EndProgram) {
                return;
            }
        }
    }
}

int getPriority(Token token) {
    try {
        switch (token.type) {
        case Add:
            return 1;
            break;
        case Sub:
            return 1;
            break;
        case Mul:
            return 2;
            break;
        case Div:
            return 2;
            break;
        case Mod:
            return 3;
            break;
        case LBraket:
            return 4;
            break;
        case RBraket:
            return 4;
            break;
        case Equality:
            return 1;
            break;
        case NotEquality:
            return 1;
            break;
        case Greate:
            return 1;
            break;
        case Less:
            return 1;
            break;
        case Not:
            return 1;
            break;
        case And:
            return 1;
            break;
        case Or:
            return 1;
            break;
        case Unknown:
            throw std::out_of_range("Unknow sign: ");
            break;
        }
    } catch(const std::out_of_range& e) {
        std::cout << std::endl;
        std::cout << "[ERROR] " << e.what() << "\"" << token.name << "\"" << " in line " << token.line << "." << std::endl;
        std::cout << std::endl;
        return -1;
    }
}

RootNode* parseSimpleExpression(Token token) {
   
    if (token.type == Number) {
        NumberNode* number = new NumberNode(token.value);
        return number;
    }
    else {
        VarNode* var = new VarNode(token.name, token.value);
        return var;
    }
    
}

RootNode* SyntaxAnalyzer::generateExpressionNode(TokenProcessor* tokenProc, int priority) {
   // Функція викликається рекурсивно для обробки токенів
   // Починаємо з першого токена
   //r <- 7 ++ 9 ** 8 ++ 10;
    ExpressionNode* expression = new ExpressionNode();
    Token firstToken = tokenProc->getNextToken();
    if (firstToken.type == LBraket) {
        std::vector<Token> newString;

        while (!(firstToken.type == RBraket)) {
            if (tokenProc->getSize() != 0) {
                firstToken = tokenProc->getNextToken();
            }
            else {
                break;
            }
            if (firstToken.type != RBraket) {
                newString.push_back(firstToken);
            }
        }
        auto after_bracket_priority = 0;
        bool isFirst = true;
        for (;;) {
            if (tokenProc->getSize() != 0) {
                firstToken = tokenProc->getNextToken();
            }
            else {
                break;
            }
            if (firstToken.type != Number && firstToken.type != Identifier) {
                after_bracket_priority = getPriority(firstToken);
            }
            if (priority != 0 && after_bracket_priority > priority) {
                if (isFirst) {
                    Token rbracket;
                    rbracket.name = ")";
                    rbracket.type = RBraket;
                    newString.push_back(rbracket);
                    Token lbracket;
                    lbracket.name = "(";
                    lbracket.type = LBraket;
                    newString.insert(newString.begin(), lbracket);
                }
                newString.push_back(firstToken);
                isFirst = false;
            }
            else {
                tokenProc->addToken(firstToken);
                break;
            }
        }
        TokenProcessor* proc = new TokenProcessor(newString);
        expression->addLeftExp(generateExpressionNode(proc, 0));
    }else {
        expression->addLeftExp(parseSimpleExpression(firstToken));

    }

    for (;;) {
        Token token;
        if (tokenProc->getSize() == 0) {
            return expression->GetLeft();
        }
        else {
            token = tokenProc->getNextToken();
        }

        auto current_priority = getPriority(token);

        if (current_priority <= priority) {
            tokenProc->addToken(token);
            return expression->GetLeft();
        }

        RootNode* right = generateExpressionNode(tokenProc, current_priority);
        TypeOfTokens type = right->getType();

        expression->addRightExp(right);
   
        if (tokenProc->getSize() == 0) {
            expression->setSign(token.name);
            return expression;
        }
        else {
            ExpressionNode* expression2 = new ExpressionNode();
            expression2->addLeftExp(expression->GetLeft());
            expression2->addRightExp(expression->GetRight());
            expression2->setSign(token.name);
            expression->addLeftExp(expression2);
        }  
    }
    return expression;
}



void SyntaxAnalyzer::writeToFile(const std::vector<std::string>& lines, const std::string& section) {
    std::ifstream inputFile(this->filename);
    std::ofstream outputFile;

    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file: " << this->filename << std::endl;
        return;
    }

    std::vector<std::string> fileLines;
    std::string line;

    // Зчитуємо рядки з файлу у вектор
    while (std::getline(inputFile, line)) {
        fileLines.push_back(line);
    }

    // Шукаємо рядок із вказаним розділом
    auto it = std::find(fileLines.begin(), fileLines.end(), section);
    if (it != fileLines.end()) {
        // Вставляємо нові рядки після рядка із вказаним розділом
        fileLines.insert(it + 1, lines.begin(), lines.end());
    }
    else {
        // Додаємо нові рядки в кінець файлу
        fileLines.insert(fileLines.end(), lines.begin(), lines.end());
    }

    // Відкриваємо файл для запису
    outputFile.open(this->filename);

    // Записуємо оновлені рядки у файл
    std::copy(fileLines.begin(), fileLines.end(), std::ostream_iterator<std::string>(outputFile, "\n"));

    // Виводимо повідомлення
    std::cout << "Assembly code has been written to file: " << this->filename << std::endl;
}


void SyntaxAnalyzer::createFile() {
    std::ofstream file(this->filename);

    if (file.is_open()) {
        std::cout << "File created: " << this->filename << std::endl;
        file.close();
    }
    else {
        std::cerr << "Unable to create file: " << this->filename << std::endl;
    }
}
/*case StartProgram:
    this->filename = TokenString[1].name + ext;
    createFile();
    assemblyCode = {
    "test program",
    "",
    ".data",
    "",
    ".code"
    };

    writeToFile(assemblyCode);
    break;
case VarStart:
    for (const auto& token : TokenString) {
        if (token.type == Identifier) {
            assemblyCode.push_back(token.name + " db 0;");
        }
    }
    writeToFile(assemblyCode, ".data");
    break;*/

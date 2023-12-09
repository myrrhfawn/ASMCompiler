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
    LineProcessor* LineProc = new LineProcessor(TokenStringTable);
    this->createNodes(LineProc, rootNode);
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


std::vector<Token> LineProcessor::getNextLine()
{
    if (currentIndex < lines.size()) {
        std::vector<Token> removedLine = lines[0];
        lines.erase(lines.begin());
        return removedLine;
    }
    else {
        throw std::out_of_range("No more lines in the vector.");
    }
}

void LineProcessor::addLine(std::vector<Token> line)
{
    this->lines.insert(this->lines.begin(), line);
}

int LineProcessor::getSize()
{
    return this->lines.size();
}





void SyntaxAnalyzer::createNodes(LineProcessor* lineProc, RootNode* rootNode, int pos) {

    std::cout << std::endl << "==============================CREATING NODES===============================" << std::endl;
    std::vector<Token> TokenString;

    while (lineProc->getSize() != 0) {
        TokenString = lineProc->getNextLine();
        if (TokenString.size() != 0) {
            if (TokenString[pos].type == StartProgram) {
                ProgramNode* program = new ProgramNode();
                std::vector<std::vector<Token>> newTokenTable;
                while (lineProc->getSize() != 0) {
                    newTokenTable.push_back(lineProc->getNextLine());
                }
                LineProcessor* newLineProc = new LineProcessor(newTokenTable);
                createNodes(newLineProc, program);
                rootNode->addChild(program);
            }
            if (TokenString[pos].type == VarStart) {
                StartVarNode* startVar = new StartVarNode();
                for (const auto& token : TokenString) {
                    if (token.type == Identifier) {
                        VarNode* var = new VarNode(token.name, token.value);
                        startVar->addChild(var);
                    }
                }
                rootNode->addChild(startVar);
            }
            if (TokenString[pos].type == Identifier) {
                if (TokenString[pos + 1].type == Assign) {

                    AssignNode* assign = new AssignNode();
                    VarNode* identifier = new VarNode(TokenString[pos].name, TokenString[pos].value);
                    std::vector<Token> newString(TokenString.end() - TokenString.size() + pos + 2, TokenString.end());
                    RootNode* expression = new RootNode();
                    TokenProcessor* tokenProc = new TokenProcessor(newString);
                    expression = generateExpressionNode(tokenProc, 0);
                    assign->addChild(identifier);
                    assign->addChild(expression);
                    rootNode->addChild(assign);
                }
            }
            if (TokenString[pos].type == Comment) {
                CommentNode* comment = new CommentNode(TokenString);
                rootNode->addChild(comment);
            }
            if (TokenString[pos].type == Input) {
                if (TokenString.size() - pos > 2) {
                    std::cout << "too many arguments for Scan." << std::endl;
                }
                else {
                    VarNode* var = new VarNode(TokenString[pos + 1].name, TokenString[pos + 1].value);
                    InputNode* input = new InputNode(var);
                    rootNode->addChild(input);
                }
            }
            if (TokenString[pos].type == Output) {
                if (TokenString.size() - pos > 2) {
                    std::cout << "too many arguments for Print." << std::endl;
                }
                else {
                    VarNode* var = new VarNode(TokenString[pos + 1].name, TokenString[pos + 1].value);
                    OutputNode* output = new OutputNode(var);
                    rootNode->addChild(output);
                }
            }
            if (TokenString[pos].type == If) {
                std::vector<Token> newString(TokenString.end() - TokenString.size() + pos + 1, TokenString.end());
                RootNode* condition = new RootNode();
                TokenProcessor* tokenProc = new TokenProcessor(newString);
                condition = generateConditionNode(tokenProc, 0);
                

                std::vector<std::vector<Token>> newTokenStringTable;
                std::vector<Token> string = lineProc->getNextLine();
                bool isElse = false;
                int ifCounter = 0;

                while (string[pos].type == Tab || string[pos].type == Else) {
                    if (string[pos].type == Else) {
                        isElse = true;
                        string = lineProc->getNextLine();
                        break;
                       
                    }
                    newTokenStringTable.push_back(string);
                    if (lineProc->getSize() != 0) {
                        string = lineProc->getNextLine();
                    }
                    else {
                        break;
                    }
                }
                if (isElse) {
                    std::vector<std::vector<Token>> elseTokenString;
                    while (string[pos].type == Tab) {
                        elseTokenString.push_back(string);
                        if (lineProc->getSize() != 0) {
                            string = lineProc->getNextLine();
                        }
                        else {
                            break;
                        }
                    }
                    lineProc->addLine(string);
                    LineProcessor* elseLineProc = new LineProcessor(elseTokenString);
                    ElseNode* elseNode = new ElseNode();
                    createNodes(elseLineProc, elseNode, pos + 1);

                    LineProcessor* ifLineProc = new LineProcessor(newTokenStringTable);
                    IfNode* ifnode = new IfNode(condition, elseNode);
                    createNodes(ifLineProc, ifnode, pos + 1);
                    rootNode->addChild(ifnode);
                }
                else {
                    lineProc->addLine(string);
                    LineProcessor* ifLineProc = new LineProcessor(newTokenStringTable);
                    IfNode* ifnode = new IfNode(condition);
                    createNodes(ifLineProc, ifnode, pos + 1);
                    rootNode->addChild(ifnode);
                }
            }
            if (TokenString[pos].type == For) {
                VarNode* i = new VarNode(TokenString[pos + 1].name, TokenString[pos + 1].value);
                AssignNode* assign = new AssignNode();
                std::vector<Token> newString;
                bool isExpression = false;
                int step = 0;
                for (const auto token : TokenString) {
                    if (token.type == Assign) {
                        isExpression = true;
                    }
                    if (token.type == To || token.type == Downto) {
                        if (token.type == To) {
                            step = 1;
                        }
                        else {
                            step = -1;
                        }
                        break;
                    }
                    if (isExpression && token.type != Assign) {
                        newString.push_back(token);
                    }
                }
                RootNode* expression = new RootNode();
                TokenProcessor* tokenProc = new TokenProcessor(newString);
                expression = generateExpressionNode(tokenProc, 0);
                assign->addChild(i);
                assign->addChild(expression);
                
                std::vector<Token> goalString;
                isExpression = false;
                for (const auto token : TokenString) {
                    if (token.type == To || token.type == Downto) {
                        isExpression = true;
                    }
                    if (token.type == Do) {
                        break;
                    }
                    if (isExpression && token.type != To && token.type != Downto) {
                        goalString.push_back(token);
                    }
                }
                RootNode* goal = new RootNode();
                TokenProcessor* goalProc = new TokenProcessor(goalString);
                goal = generateExpressionNode(goalProc, 0);

                ForNode* forNode = new ForNode(i, assign, step, goal);


                std::vector<std::vector<Token>> newTokenStringTable;
                std::vector<Token> string = lineProc->getNextLine();
                int ifCounter = 0;
                while (string[pos].type == Tab) {
                    newTokenStringTable.push_back(string);
                    if (lineProc->getSize() != 0) {
                        string = lineProc->getNextLine();
                    }
                    else {
                        break;
                    }
                }
                lineProc->addLine(string);
                LineProcessor* forLineProc = new LineProcessor(newTokenStringTable);
                createNodes(forLineProc, forNode, pos + 1);
                rootNode->addChild(forNode);
            }
            if (TokenString[pos].type == EndProgram) {
                return;
            }
            if (TokenString[pos].type == Unknown) {
                std::cout << "[ERROR] Unknown lexem in line: " << TokenString[pos].line << std::endl;
                return;
            }
        }
        std::cout << "No matches" << std::endl;
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
            return 5;
            break;
        case RBraket:
            return 5;
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
            return 4;
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

        int bracket_count = 1;
        while (bracket_count != 0) {
            if (tokenProc->getSize() != 0) {
                firstToken = tokenProc->getNextToken();
            }
            else {
                break;
            }
            if (firstToken.type == LBraket) {
                bracket_count += 1;
            }
            if (firstToken.type == RBraket) {
                bracket_count -= 1;
            }
            if (bracket_count != 0) {
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

RootNode* SyntaxAnalyzer::generateConditionNode(TokenProcessor* tokenProc, int priority) {
    // Функція викликається рекурсивно для обробки токенів
    // Починаємо з першого токена
    //r <- 7 ++ 9 ** 8 ++ 10;
    ConditionNode* condition = new ConditionNode();
    Token firstToken = tokenProc->getNextToken();
    if (firstToken.type == Not) {
        condition->setSign("Not");
        NotNode* notnode = new NotNode();
        condition->addLeftExp(notnode);
        condition->addRightExp(generateConditionNode(tokenProc, getPriority(firstToken)));
        if (tokenProc->getSize() != 0) {
            ConditionNode* condition2 = new ConditionNode();
            condition2->addLeftExp(condition->GetLeft());
            condition2->addRightExp(condition->GetRight());
            condition->addLeftExp(condition2);
        }
        else {
            return condition;
        }
    }
    else {
        if (firstToken.type == LBraket) {
            std::vector<Token> newString;
            int bracket_count = 1;
            while (bracket_count != 0) {
                if (tokenProc->getSize() != 0) {
                    firstToken = tokenProc->getNextToken();
                }
                else {
                    break;
                }
                if (firstToken.type == LBraket) {
                    bracket_count += 1;
                }
                if (firstToken.type == RBraket) {
                    bracket_count -= 1;
                }
                if (bracket_count != 0) {
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
            condition->addLeftExp(generateConditionNode(proc, 0));
        } else {
            condition->addLeftExp(parseSimpleExpression(firstToken));
        }
    }
    for (;;) {
        Token token;
        if (tokenProc->getSize() == 0) {
            return condition->GetLeft();
        }
        else {
            token = tokenProc->getNextToken();
        }

        auto current_priority = getPriority(token);

        if (current_priority <= priority) {
            tokenProc->addToken(token);
            return condition->GetLeft();
        }

        RootNode* right = generateConditionNode(tokenProc, current_priority);

        condition->addRightExp(right);

        if (tokenProc->getSize() == 0) {
            condition->setSign(token.name);
            return condition;
        }
        else {
            ConditionNode* condition2 = new ConditionNode();
            condition2->addLeftExp(condition->GetLeft());
            condition2->addRightExp(condition->GetRight());
            condition2->setSign(token.name);
            condition->addLeftExp(condition2);
        }
    }
    return condition;
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

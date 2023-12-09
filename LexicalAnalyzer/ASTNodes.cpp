#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Data.h"
#include <fstream>
#include <vector>



void RootNode::addChild(RootNode* child)
{
    children.push_back(child);
}

std::vector<RootNode*> RootNode::getChildren() const {
    return children;
}

void RootNode::print(const int tab) const
{
    for (const RootNode* child : children) {
        child->print(tab);
    }
}

TypeOfTokens RootNode::getType() const
{
    return this->type;
}



void ProgramNode::print(const int tab) const
{
    std::cout << "Program Start" << std::endl;
    RootNode::print();
    std::cout << std::endl;
}



VarNode::VarNode(const std::string identifier, int value, TypeOfTokens type) : type(type) {
    this->value = value;
    this->identifier = identifier;
    this->type = type;
};

void StartVarNode::print(const int tab) const
{
    std::cout << "Data: " << std::endl;
    RootNode::print(tab);
    std::cout << std::endl;
}


void VarNode::print(const int tab) const
{   
    std::cout << "Var " << this->identifier << "(" << value << ")";
}

void VarNode::getVar()
{
    std::cout << this->value;
}

AssignNode::AssignNode(TypeOfTokens type) {
    this->type = type;
}

void AssignNode::print(const int tab) const {
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    std::vector<RootNode*> children = this->getChildren();
    std::cout << "Assignment to variable: ";
    children[0]->print();
    std::cout << " <- ";
    children[1]->print();
    std::cout << std::endl;
}

ExpressionNode::ExpressionNode(TypeOfTokens type) {
    this->type = type;
    this->sign = sign;
}

RootNode* ExpressionNode::GetRight() const
{
    return this->rightExp;
}

RootNode* ExpressionNode::GetLeft() const
{
    return this->leftExp;
}

void ExpressionNode::print(const int tab) const {
   
    RootNode* leftExpression = GetLeft();
    RootNode* rightExpression = GetRight();
    std::cout << "( ";
    leftExpression->print();
    std::cout << " " << this->sign << " ";
    rightExpression->print();
    std::cout << " )";

}

void ExpressionNode::setSign(std::string sign)
{
    this->sign = sign;
}


void ExpressionNode::addLeftExp(RootNode* expression)
{
    this->leftExp = expression;
}

void ExpressionNode::addRightExp(RootNode* expression)
{
    this->rightExp = expression;
}

// Реалізація конструктора копіювання
// Функція копіювання
ExpressionNode::ExpressionNode(const ExpressionNode& other) {
    type = other.type;
    sign = other.sign;

    // Рекурсивне копіювання лівого та правого піддерев
    if (other.leftExp) {
        leftExp = new ExpressionNode(*dynamic_cast<ExpressionNode*>(other.leftExp));
    }
    else {
        leftExp = nullptr;
    }

    if (other.rightExp) {
        rightExp = new ExpressionNode(*dynamic_cast<ExpressionNode*>(other.rightExp));
    }
    else {
        rightExp = nullptr;
    }
}

// Оператор присвоєння
ExpressionNode& ExpressionNode::operator=(const ExpressionNode& other) {
    if (this != &other) {  // Перевірка на самоприсвоєння
        type = other.type;
        sign = other.sign;

        // Видалення існуючих піддерев
        delete leftExp;
        delete rightExp;

        // Рекурсивне копіювання лівого та правого піддерев
        if (other.leftExp) {
            leftExp = new ExpressionNode(*dynamic_cast<ExpressionNode*>(other.leftExp));
        }
        else {
            leftExp = nullptr;
        }

        if (other.rightExp) {
            rightExp = new ExpressionNode(*dynamic_cast<ExpressionNode*>(other.rightExp));
        }
        else {
            rightExp = nullptr;
        }
    }

    return *this;
}


TypeOfTokens ExpressionNode::getType() const
{
    return this->type;
}


ConditionNode::ConditionNode(TypeOfTokens type)
{
    this->type = type;
    this->sign = sign;
}

ConditionNode::ConditionNode(const ConditionNode& other)
{
    type = other.type;
    sign = other.sign;

    // Рекурсивне копіювання лівого та правого піддерев
    if (other.leftExp) {
        leftExp = new ExpressionNode(*dynamic_cast<ExpressionNode*>(other.leftExp));
    }
    else {
        leftExp = nullptr;
    }

    if (other.rightExp) {
        rightExp = new ExpressionNode(*dynamic_cast<ExpressionNode*>(other.rightExp));
    }
    else {
        rightExp = nullptr;
    }
}

ConditionNode& ConditionNode::operator=(const ConditionNode& other)
{
    if (this != &other) {  // Перевірка на самоприсвоєння
        type = other.type;
        sign = other.sign;

        // Видалення існуючих піддерев
        delete leftExp;
        delete rightExp;

        // Рекурсивне копіювання лівого та правого піддерев
        if (other.leftExp) {
            leftExp = new ConditionNode(*dynamic_cast<ConditionNode*>(other.leftExp));
        }
        else {
            leftExp = nullptr;
        }

        if (other.rightExp) {
            rightExp = new ConditionNode(*dynamic_cast<ConditionNode*>(other.rightExp));
        }
        else {
            rightExp = nullptr;
        }
    }

    return *this;
}

void ConditionNode::print(const int tab) const {
    RootNode* leftExpression = GetLeft();
    RootNode* rightExpression = GetRight();
    std::cout << "( ";
    leftExpression->print();
    std::cout << " " << this->sign << " ";
    rightExpression->print();
    std::cout << " )";
}

void ConditionNode::setSign(std::string sign)
{
    this->sign = sign;
}

void ConditionNode::addLeftExp(RootNode* expression)
{
    this->leftExp = expression;

}

void ConditionNode::addRightExp(RootNode* expression)
{
    this->rightExp = expression;
}

TypeOfTokens ConditionNode::getType() const
{
    return this->type;
}

RootNode* ConditionNode::GetRight() const
{
    return this->rightExp;
}

RootNode* ConditionNode::GetLeft() const
{
    return this->leftExp;
}

NumberNode::NumberNode(int value, TypeOfTokens type)
{
    this->type = type;
    this->value = value;
}

void NumberNode::print(const int tab) const {
    std::cout << value;
}

TypeOfTokens NumberNode::getType() const
{
    return this->type;
}

void NumberNode::setValue(int value)  {
    this->value = value;
}


IfNode::IfNode(RootNode* condition, RootNode* elseNode, TypeOfTokens type)
{
    this->type = type;
    this->condition = condition;
    this->elseNode = elseNode;
}

void IfNode::print(const int tab) const
{
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    std::vector<RootNode*> children;
    children = RootNode::getChildren();
    std::cout << "IF ";
    this->condition->print();
    std::cout << ":" << std::endl;
    bool firstChild = true;

    for (const auto& child : children) {
        child->print(tab + 1);
    }
    if (this->elseNode != nullptr) {
        this->elseNode->print(tab);
    }
}

void ElseNode::print(const int tab) const
{
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    std::vector<RootNode*> children;
    children = RootNode::getChildren();
    std::cout << "Else: " << std::endl;
    for (const auto& child : children) {
        child->print(tab + 1);
    }
}


ForNode::ForNode(RootNode* var, RootNode* assigment, int step, RootNode* goal, TypeOfTokens type)
{
    this->var = var;
    this->assign = assigment;
    this->step = step;
    this->goal = goal;
}

void ForNode::print(const int tab) const
{
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    this->assign->print();
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    std::cout << "For( ";
    this->var->print();
    if (this->step == 1) {
        std::cout << " To ";
    }
    else {
        std::cout << " To ";
    }
    this->goal->print();
    std::cout << "): " << std::endl;
    std::vector<RootNode*> children;
    children = RootNode::getChildren();
    for (const auto& child : children) {
        child->print(tab + 1);
    }
}


NotNode::NotNode(TypeOfTokens type)
{
    this->type = type;
}

void NotNode::print(const int tab) const
{
    std::cout << "";
}

CommentNode::CommentNode(std::vector<Token> comment, TypeOfTokens type)
{
    this->comment = comment;
}

void CommentNode::print(const int tab) const
{
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    std::cout << "Comment: ";
    for (const auto token : comment) {
        std::cout << token.name << " ";
    }
    std::cout << "." << std::endl;
}

InputNode::InputNode(RootNode* var, TypeOfTokens type)
{

    this->var = var;
}

void InputNode::print(const int tab) const
{
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    std::cout << "Need to input: ";
    this->var->print();
    std::cout << std::endl;
}

OutputNode::OutputNode(RootNode* var, TypeOfTokens type)
{
    this->var = var;
}

void OutputNode::print(const int tab) const
{
    for (int i = 0; i < tab; i++) {
        std::cout << '\t';
    }
    std::cout << "Print:  ";
    this->var->print();
    std::cout << std::endl;

}

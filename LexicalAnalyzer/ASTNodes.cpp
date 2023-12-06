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

void RootNode::print(const std::string& tab) const
{
    for (const RootNode* child : children) {
        std::cout << tab;
        child->print(tab);
    }
}

TypeOfTokens RootNode::getType() const
{
    return this->type;
}



void ProgramNode::print(const std::string& tab) const
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

void StartVarNode::print(const std::string& tab) const
{
    std::cout << tab << "Data: " << std::endl;
    RootNode::print(tab);
    std::cout << std::endl;
}


void VarNode::print(const std::string& tab) const
{
    std::cout << tab << "Var " << this->identifier << "(" << value << ")";
}

void VarNode::getVar()
{
    std::cout << this->value;
}

AssignNode::AssignNode(TypeOfTokens type) {
    this->type = type;
}

void AssignNode::print(const std::string& tab) const {
    std::vector<RootNode*> children = this->getChildren();
    std::cout << tab << "Assignment to variable ";
    children[0]->print();
    std::cout << ": " << std::endl;
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

void ExpressionNode::print(const std::string& tab) const {
   
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




NumberNode::NumberNode(int value, TypeOfTokens type)
{
    this->type = type;
    this->value = value;
}

void NumberNode::print(const std::string& tab) const {
    std::cout << value;
}

TypeOfTokens NumberNode::getType() const
{
    return this->type;
}

void NumberNode::setValue(int value)  {
    this->value = value;
}


IfNode::IfNode(RootNode* condition, TypeOfTokens type)
{
    RootNode::addChild(condition);
}

void IfNode::print(const std::string& tab) const {
    std::vector<RootNode*> children;
    children = RootNode::getChildren();
    std::cout << "IF( ";
    children[0]->print();
    std::cout << " ):" << std::endl;
    bool firstChild = true;

    for (const auto& child : children) {
        if (!firstChild) {
            child->print("\t");
        }
        else {
            firstChild = false;
        }

    }
}


ConditionNode::ConditionNode(const std::string& sign, TypeOfTokens type) {
    this->type = type;
    this->sign = sign;
}

void ConditionNode::print(const std::string& tab) const {
    std::vector<RootNode*> children;
    children = RootNode::getChildren();
    std::cout << "Condition(";
    bool firstChild = true;

    for (const auto& child : children) {
        if (!firstChild) {
            std::cout << " " << sign << " ";
        }
        else {
            firstChild = false;
        }

        child->print();
    }
    std::cout << ")";
}


CommentNode::CommentNode(std::vector<Token> comment, TypeOfTokens type)
{
    this->comment = comment;
}

void CommentNode::print(const std::string& tab) const
{
    std::cout << "Comment: ";
    for (const auto token : comment) {
        std::cout << token.name << " ";
    }
    std::cout << "." << std::endl;
}

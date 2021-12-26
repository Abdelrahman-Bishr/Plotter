#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H
#include <math.h>
#include <cassert>
#include<bits/stdc++.h>


using namespace std;
class ExpressionEvaluator
{
public:
    ExpressionEvaluator();
    void set_expression(string exp);
    double evaluate(double x);
    bool is_const();

private:
    string expression;
    int precedence(char op);
    double applyOp(double a, double b, char op);
    double extract_number(string tokens,int &i);
    void apply_higher_precedence_ops(stack<double> & values,stack<char> &ops,char op);
    string expression_sanity_check(string tokens);
    string sanity_check(string tokens);
    string replace_(string s,string old,string n);
};

#endif // EXPRESSIONEVALUATOR_H

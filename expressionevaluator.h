#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H
#include <QDebug>
#include <math.h>
#include <cassert>
#include<bits/stdc++.h>
#include <QObject>

using namespace std;
class ExpressionEvaluator : public QObject
{
    Q_OBJECT
public:
    ExpressionEvaluator();
    void set_expression(QString exp);
    double evaluate(double x);
    bool is_const();
signals:
    void error_message(QString message);
private:
    QString expression;
    int precedence(char op);
    double applyOp(double a, double b, char op);
    double extract_number(string tokens,int &i);
    void apply_higher_precedence_ops(stack<double> & values,stack<char> &ops,char op);
    QString expression_sanity_check(QString tokens);
    string sanity_check(string tokens);
};

#endif // EXPRESSIONEVALUATOR_H

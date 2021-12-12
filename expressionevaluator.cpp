#include "expressionevaluator.h"

ExpressionEvaluator::ExpressionEvaluator()
{
    expression="-x";
}

void ExpressionEvaluator::set_expression(QString exp)
{
    expression=exp.replace(" ","") ;
    expression = expression_sanity_check(expression);
    if (expression[0]=='_'){
        if (expression.length()==2){
            QString message = "UNRECOGNIZED CHARACTER "+expression[1];
            emit error_message(message);
        }
        else if (expression.length()==3){
            QString message = "UNBALANCED BRACKETS";
            emit error_message(message);
        }
    }
}

int ExpressionEvaluator::precedence(char op){
    if(op == '+'||op == '-')
    return 1;
    if(op == '*'||op == '/')
    return 2;
    if(op=='^')
    return 3;

    return 0;
}

double ExpressionEvaluator::applyOp(double a, double b, char op){
    switch(op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': {
            if (b==0){
                emit error_message("Zero Division");
                return -1;
            }
            return a / b;
            }
        case '^': return pow(a,b);
    }
}

double ExpressionEvaluator::extract_number(string tokens,int &i){
    double val = 0;

    // There may be more than one
    // digits in number.
    bool fraction=false;
    int fraction_val=0;
    while(i < tokens.length() &&
                (isdigit(tokens[i]) || tokens[i]=='.'))
    {
        if (tokens[i]=='.'){
            i++;
            fraction=true;
            continue;
        }
        if (!fraction){
            val = (val*10) + (tokens[i]-'0');
        }
        else{
            ++fraction_val;
            val += (double) (tokens[i]-'0')/pow(10,fraction_val);
        }

        i++;
    }
    return val;

}

void ExpressionEvaluator::apply_higher_precedence_ops(stack<double> & values,stack<char> &ops,char op){
    while(!ops.empty() && precedence(ops.top())
                        >= precedence(op)){
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }
}

QString ExpressionEvaluator::expression_sanity_check(QString tokens)
{

    if (precedence(tokens[0].toLatin1())!=0 && tokens[0]!='-'){
        QString message = "BINARY OPERATOR ";
        message.append(tokens[0]);
        message+= " LACKING ENOUGH OPERANDS";
        emit error_message(message);
        return message;
    }


    // "_"+strange character : strange char
    // "__"+"("   :  INBALANCED BRACKETS
    int brackets=0;
    for(int i = 0; i < tokens.length(); i++){
        // MAKE SURE NO STRANGE CHARACTERS
        if (!isdigit(tokens[i].toLatin1()) && tokens[i] !='+' && tokens[i] !='-' && tokens[i] !='*' &&
        tokens[i] !='/' && tokens[i] !='(' && tokens[i] !=')' && tokens[i] !='.' &&
         tokens[i] !='^' && tokens[i] !='x' && tokens[i] !='X'){
            QString tempo = "_";
            tempo+=tokens[i];
            return tempo;
        }
        if(tokens[i]=='(' || tokens[i]=='x' || tokens[i]=='X'){
            if(i>=1){
                if (isdigit(tokens[i-1].toLatin1())){
                    tokens=tokens.insert(i,"*");
                    i++;
                }
            }
        }
        // MAKE SURE OF BALANCED BRACKETS
        if(tokens[i]=='(')
            brackets++;
        else if(tokens[i]==')')
            brackets--;

    }
    if (brackets!=0)
        return "__(";
    return tokens;
}

string ExpressionEvaluator::sanity_check(string tokens){
    // "___"+"op"+"op"   :

    for(int i = 0; i < tokens.length(); i++){
        // MAKE SURE OF MULTIPLICATION FOR VAR COEFFICIENTS AND BRACKETS
        if (precedence(tokens[i])!=0 && i+1 < tokens.length() && precedence(tokens[i+1])!=0 ){
                string tempo = "__";
            if(tokens[i+1]!='-'){
                tempo+= tokens[i];
                tempo+= tokens[i+1];
                return tempo;
            }
            else if(! (i+2<tokens.length()) || precedence(tokens[i+2])!=0 ){
                tempo+= tokens[i];
                tempo+= tokens[i+1];
                return tempo;
            }
        }


    }
    return tokens;
}


double ExpressionEvaluator::evaluate(double x){
    int i;
    QString exp_copy = expression;
    if (expression[0]=='_'){
        emit error_message(QString("can't evaluate with current function"));
        return -1;
    }
    string tokens = exp_copy.replace("-x","-1*x",Qt::CaseInsensitive)
            .replace("-(","-1*(",Qt::CaseInsensitive)
                .replace("x",QString::number(x),Qt::CaseInsensitive).toStdString();

    tokens = sanity_check(tokens);
    if (tokens[0]=='_'){
        QChar op1(tokens[2]),op2(tokens[3]);
        QString message = "BINARY OPERATORS ";
        message.append(op1);
        message+= " AND ";
        message.append(op2);
        message+= " LACKING ENOUGH OPERANDS";
        emit error_message(message);
        return -1;
    }
    // stack to store integer values.
    stack <double> values;

    // stack to store operators.
    stack <char> ops;
    int negative_sign=1;
    for(i = 0; i < tokens.length(); i++){

        // Current token is a whitespace,
        // skip it.
        if(tokens[i] == ' ')
            continue;

        // Current token is an opening
        // brace, push it to 'ops'
        else if(tokens[i] == '('){
            ops.push(tokens[i]);
        }

        // Current token is a number, push
        // it to stack for numbers.
        else if(isdigit(tokens[i])){
            double val = extract_number(tokens,i);
            values.push(val * negative_sign);
            negative_sign=1;
            // right now the i points to
            // the character next to the digit,
            // since the for loop also increases
            // the i, we would skip one
            // token position; we need to
            // decrease the value of i by 1 to
            // correct the offset.
            i--;
        }

        // Closing brace encountered, solve
        // entire brace.
        else if(tokens[i] == ')')
        {
            while(!ops.empty() && ops.top() != '(')
            {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }

            // pop opening brace.
            if(!ops.empty())
            ops.pop();
        }

        // Current token is an operator.
        else
        {

            // check sign operator
            if (tokens[i]=='-'){
                if(i==0){
                    assert(tokens.length()>1);
                    // sign
                    if (tokens[i+1]=='-'){
                        i++;
                        continue;
                    }
                    else {
                        negative_sign*=-1;
                        continue;
                    }
                }else{
                    assert(tokens.length()>i+1);
                    // sign , not operator , followed by a number , no need for more evaluations
                    double val = 0;
                    if ((isdigit(tokens[i+1]) && !isdigit(tokens[i-1]))){
                        negative_sign*=-1;
                        continue;
                    }
                    // sign , not operator , followed by an expression that needs evaluation
                    else if(tokens[i+1]=='(' && ((tokens[i-1]=='(' || (precedence(tokens[i-1])!=0) ))){
                        int l=1,open_brackets=1;
                        while(open_brackets>0 && (i+l)<tokens.length()){
                            l++;
                            if(tokens[i+ l]=='(')
                                open_brackets++;
                            else if (tokens[i+l]==')')
                                open_brackets--;
                        }
                        if (i+l >= tokens.length()){
                            emit error_message(QString("ERROR , UNBALANCED PARENTHES"));
                            return -1;
                        }
                        string cpy = tokens.substr(i+1,l);
                        QString exp = this->expression;
                        this->expression = QString::fromStdString(cpy);
                        val = -1 * evaluate(x);
                        this->expression = exp;
                        values.push(val);
                        i+=l;
                        continue;
                    }
                    // sign
                    else if(tokens[i-1]=='(' && tokens[i+1]=='-'){
                        i++;
                        continue;
                    }
                    // fault handling
                    else if(precedence(tokens[i+1])!=0 && tokens[i+1]!='-'){
                        QString message = "ERROR , SUBTRACTION CAN'T BE FOLLOWED BY ANOTHER OPERATOR";
                        emit error_message(message);
                        return -1;
                    }
                }
            }
            // While top of 'ops' has same or greater
            // precedence to current token, which
            // is an operator. Apply operator on top
            // of 'ops' to top two elements in values stack.
            apply_higher_precedence_ops(values,ops,tokens[i]);

            // Push current token to 'ops'.
            ops.push(tokens[i]);
        }
    }

    // Entire expression has been parsed at this
    // point, apply remaining ops to remaining
    // values.
    while(!ops.empty()){
        if (values.size()<2){
            emit error_message(QString("OPERATORS LACKING OPERANDS"));
            return -1;
        }
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }

    // Top of 'values' contains result, return it.
    return values.top();
}

bool ExpressionEvaluator::is_const(){
    for (int i=0;i<expression.length();i++){
        if (expression[i] =='x' || expression[i] =='X')
            return false;
    }
    return true;
}


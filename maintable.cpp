#include "maintable.h"
#include "ui_maintable.h"
#include <QStack>
#include <QString>
MainTable::MainTable(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTable)
{
    ui->setupUi(this);
    resultTable = ui->tableWidget;
    enterInfix = ui->lineEdit;
    calculateButton = ui->Button;
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setVerticalHeaderLabels(QStringList() << "Инфиксное выражение" << "Постфиксное выражение" << "Результат");
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<" ");
    connect(calculateButton, &QPushButton::clicked, this, &MainTable::calculatePostfixResult);
}

MainTable::~MainTable()
{
    delete ui;
}
bool MainTable::isOperator(QChar c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
int MainTable::getPrecedence(QChar c)
{
    if (c == '^')
        return 3;
    else if (c == '*' || c == '/')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return 0;
}
int MainTable::calculate(int x, int y, QChar o)
{
    switch (o.unicode())
    {
    case '+':
        return x + y;
    case '-':
        return x - y;
    case '*':
        return x * y;
    case '/':
        return x / y;
    default:
        return 0;
    }
}
bool MainTable::isDigit(QChar c)
{
    return c.isDigit();
}

void MainTable::postfixNotation(const QString& infix, QString& postfix)
{
    QStack<QChar> stack;
    postfix.clear();
    QString operand;

    for (QChar c : infix)
    {
        if (isDigit(c))
        {
            operand += c;
        } else
        {
            if (!operand.isEmpty())
            {
                postfix += operand;
                postfix += ' ';
                operand.clear();
            }
            if (c == '(')
            {
                stack.push(c);
            } else if (c == ')')
            {
                while (!stack.isEmpty() && stack.top() != '(')
                {
                    postfix += stack.pop();
                    postfix += ' ';
                }
                if (!stack.isEmpty() && stack.top() == '(')
                {
                    stack.pop();
                }
            } else if (isOperator(c))
            {
                while (!stack.isEmpty() && stack.top() != '(' && getPrecedence(c) <= getPrecedence(stack.top()))
                {
                    postfix += stack.pop();
                    postfix += ' ';
                }
                stack.push(c);
            }
        }
    }

    if (!operand.isEmpty())
    {
        postfix += operand;
        postfix += ' ';
    }

    while (!stack.isEmpty())
    {
        postfix += stack.pop();
        postfix += ' ';
    }
}

int MainTable::evaluatePostfix(const QString& postfix)
{
    QStack<int> stack;

    QString token;
    int pos = 0;
    while (pos < postfix.size())
    {
        if (postfix[pos] == ' ')
        {
            pos++;
            continue;
        }
        else if (isDigit(postfix[pos]))
        {
            token.clear();
            while (pos < postfix.size() && isDigit(postfix[pos]))
            {
                token += postfix[pos];
                pos++;
            }
        }
        else
        {
            token = postfix[pos];
            pos++;
        }
        if (!token.isEmpty())
        {
            if (isDigit(token[0]))
            {
                stack.push(token.toInt());
            }
            else if (isOperator(token[0]))
            {
                int operand2 = stack.pop();
                int operand1 = stack.pop();

                int result = calculate(operand1, operand2, token[0]);
                stack.push(result);
            }
        }
    }

    return stack.pop();
}
void MainTable::calculatePostfixResult()
{
    QString infix = enterInfix->text();
    QString postfix;
    int result;
    postfixNotation(infix, postfix);
    result = evaluatePostfix(postfix);
    QString resultStr = QString::number(result);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(infix));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem(postfix));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem(resultStr));

}

#ifndef MAINTABLE_H
#define MAINTABLE_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QTableWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainTable;
}
QT_END_NAMESPACE

class MainTable : public QMainWindow
{
    Q_OBJECT

public:
    MainTable(QWidget *parent = nullptr);
    ~MainTable();
private slots:
    bool isOperator(QChar c);
    int getPrecedence(QChar c);
    int calculate(int x, int y, QChar o);
    bool isDigit(QChar c);
    void postfixNotation(const QString& infix, QString& postfix);
    int evaluatePostfix(const QString& postfix);
    void calculatePostfixResult();
private:
    Ui::MainTable *ui;
    QPushButton *calculateButton;
    QLineEdit *enterInfix;
    QTableWidget *resultTable;

};
#endif // MAINTABLE_H

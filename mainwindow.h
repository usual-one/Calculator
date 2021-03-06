#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

#define MAX_DIGITS 15

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private
    slots:

    void appendDigit();

    void appendDot();

    void changeSign();

    void appendOp();

    void calculateUnaryOp();

    void deleteExpression();

    void deleteOperand();

    void deleteDigit();

    void calculate();

    void changeMemory();

    void appendText(QLabel *label, QString text);

private:
    double first_num;
    double second_num;
    double memory_num;
    QPushButton* op;
    bool operator_appended;
    bool error;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

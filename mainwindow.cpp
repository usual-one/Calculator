#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QString>
#include <cmath>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->btn_0, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_1, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_2, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_3, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_4, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_5, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_6, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_7, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_8, SIGNAL(clicked()), this, SLOT(appendDigit()));
    connect(ui->btn_9, SIGNAL(clicked()), this, SLOT(appendDigit()));

    connect(ui->btn_dot, SIGNAL(clicked()), this, SLOT(appendDot()));

    connect(ui->btn_plus, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_minus, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_mult, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_div, SIGNAL(clicked()), this, SLOT(appendOp()));

    connect(ui->btn_sin, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));
    connect(ui->btn_cos, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));
    connect(ui->btn_tg, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));
    connect(ui->btn_arcsin, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));
    connect(ui->btn_arccos, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));
    connect(ui->btn_arctg, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));
    connect(ui->btn_sqrt, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));
    connect(ui->btn_reverse, SIGNAL(clicked()), this, SLOT(calculateUnaryOp()));

    connect(ui->btn_c, SIGNAL(clicked()), this, SLOT(deleteExpression()));
    connect(ui->btn_ce, SIGNAL(clicked()), this, SLOT(deleteOperand()));
    connect(ui->btn_del, SIGNAL(clicked()), this, SLOT(deleteDigit()));

    connect(ui->btn_eq, SIGNAL(clicked()), this, SLOT(calculate()));

    connect(ui->btn_mc, SIGNAL(clicked()), this, SLOT(changeMemory()));
    connect(ui->btn_ms, SIGNAL(clicked()), this, SLOT(changeMemory()));
    connect(ui->btn_mr, SIGNAL(clicked()), this, SLOT(changeMemory()));
    connect(ui->btn_mplus, SIGNAL(clicked()), this, SLOT(changeMemory()));
    connect(ui->btn_mminus, SIGNAL(clicked()), this, SLOT(changeMemory()));

    ui->btn_plus->setCheckable(true);
    ui->btn_minus->setCheckable(true);
    ui->btn_mult->setCheckable(true);
    ui->btn_div->setCheckable(true);

    ui->btn_eq->setCheckable(true);
    ui->btn_eq->setChecked(true);

    operator_appended = false;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendDigit() {
    QPushButton *button = (QPushButton *) sender();
    if (ui->btn_eq->isChecked()) {
        ui->lbl_result->clear();
        ui->lbl_action->clear();
        ui->btn_eq->setChecked(false);
    }
    if (operator_appended) {
        operator_appended = false;
        ui->lbl_result->clear();
    }
    appendText(ui->lbl_result, button->text());
}

void MainWindow::appendDot() {
    if (ui->btn_eq->isChecked()) {
        ui->lbl_result->setText("0");
        ui->btn_eq->setChecked(false);
    }

    QString last_operand = ui->lbl_result->text();
    if (!last_operand.contains(".")) {
        if (last_operand.size() == 0) {
            appendText(ui->lbl_result, "0");
        }
        appendText(ui->lbl_result, ".");
    }
}

void MainWindow::appendOp() {
    QPushButton *button = (QPushButton *) sender();
    if (ui->btn_eq->isChecked()) {
        if (ui->lbl_result->text().contains("Zero Division Error")) {
            button->setChecked(false);
            return;
        }
        ui->lbl_action->clear();
        ui->btn_eq->setChecked(false);
    }

    QString expression = ui->lbl_result->text();

    int negative_expression_flag = 0;
    if (expression[0] == "-") {
        negative_expression_flag = 1;
        expression.remove(0, 1);
    }


    if (expression.contains(button->text())) {
        button->setChecked(true);
        return;
    }
    button->setChecked(false);
    QPushButton *op = getOperator();
    if (op) {
        if (expression[expression.size() - 1] != op->text()) {
            return;
        }
        op->setChecked(false);
        expression.chop(1);
        if (negative_expression_flag) {
            expression = "-" + expression;
        }
        ui->lbl_result->setText(expression);
    }

    first_num = expression.toDouble();
    button->setChecked(true);
    operator_appended = true;
    appendText(ui->lbl_action, ui->lbl_result->text() + button->text());
}

void MainWindow::calculateUnaryOp()
{
    QPushButton *button = (QPushButton *) sender();
    QString btext = button->text();
    first_num = ui->lbl_result->text().toDouble();

    if (btext == "sin") {
        first_num = sin(first_num);
    }
    if (btext == "cos") {
        first_num = cos(first_num);
    }
    if (btext == "tg") {
        first_num = tan(first_num);
    }
    if (btext == "arcsin") {
        first_num = asin(first_num);
    }
    if (btext == "arccos") {
        first_num = acos(first_num);
    }
    if (btext == "arctg") {
        first_num = atan(first_num);
    }
    if (btext == "√") {
        first_num = sqrt(first_num);
    }
    if (btext == "1/x") {
        first_num = 1 / first_num;
        btext.chop(1);
    }

    ui->lbl_action->setText(btext + ui->lbl_result->text());
    ui->lbl_result->setText(QString::number(first_num, 'g', MAX_DIGITS));
    ui->btn_eq->setChecked(true);
}

void MainWindow::deleteExpression() {
    ui->lbl_result->setText("0");
    ui->lbl_action->clear();
    ui->btn_eq->setChecked(true);

    ui->btn_plus->setChecked(false);
    ui->btn_minus->setChecked(false);
    ui->btn_mult->setChecked(false);
    ui->btn_div->setChecked(false);
}

void MainWindow::deleteOperand() {
    ui->lbl_result->setText("0");
}

void MainWindow::deleteDigit() {
    if (ui->btn_eq->isChecked()) {
        return;
    }

    QString expression = ui->lbl_result->text();
    if (expression.size() > 1) {
        expression.chop(1);
        ui->lbl_result->setText(expression);
    } else if (expression != "0") {
        ui->lbl_result->setText("0");
        ui->btn_eq->setChecked(true);
    }
}

void MainWindow::calculate() {
    if (ui->lbl_result->text().contains("Zero Division Error")) {
        deleteExpression();
        return;
    }

    second_num = ui->lbl_result->text().toDouble();
    if (ui->btn_plus->isChecked()) {
        ui->btn_plus->setChecked(false);
        first_num = first_num + second_num;
    } else if (ui->btn_minus->isChecked()) {
        ui->btn_minus->setChecked(false);
        first_num = first_num - second_num;
    } else if (ui->btn_mult->isChecked()) {
        ui->btn_mult->setChecked(false);
        first_num = first_num * second_num;
    } else if (ui->btn_div->isChecked()) {
        ui->btn_div->setChecked(false);
        if (second_num != 0) {
            first_num = first_num / second_num;
        } else {
            appendText(ui->lbl_action, QString::number(second_num, 'g', MAX_DIGITS));
            ui->lbl_result->setText("Zero Division Error");
            return;
        }
    } else {
        first_num = ui->lbl_result->text().toDouble();
    }
    QString result = QString::number(first_num, 'g', MAX_DIGITS);
    appendText(ui->lbl_action, QString::number(second_num, 'g', MAX_DIGITS));
    ui->lbl_result->setText(result);
    ui->btn_eq->setChecked(true);
}

void MainWindow::changeMemory() {
    QPushButton *button = (QPushButton *) sender();
    QString btext = button->text().remove(0, 1);

    if (btext == "S") {
        ui->lbl_memory->setText("M");
        memory_num = ui->lbl_result->text().toDouble();
    }
    if (!ui->lbl_memory->text().size()) {
        return;
    }

    if (btext == "C") {
        ui->lbl_memory->clear();
    }
    if (btext == "R") {
        ui->lbl_result->setText(QString::number(memory_num, 'g', MAX_DIGITS));
    }
    if (btext == "+") {
        memory_num += ui->lbl_result->text().toDouble();
    }
    if (btext == "-") {
        memory_num -= ui->lbl_result->text().toDouble();
    }
}

void MainWindow::appendText(QLabel *label, QString text) {
    QString new_text = label->text() + text;
    label->setText(new_text);
}

QPushButton *MainWindow::getOperator() {
    QPushButton *button = nullptr;
    if (ui->btn_plus->isChecked()) {
        button = ui->btn_plus;
    }
    if (ui->btn_minus->isChecked()) {
        button = ui->btn_minus;
    }
    if (ui->btn_mult->isChecked()) {
        button = ui->btn_mult;
    }
    if (ui->btn_div->isChecked()) {
        button = ui->btn_div;
    }
    return button;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QString>

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

    connect(ui->btn_c, SIGNAL(clicked()), this, SLOT(deleteExpression()));
    connect(ui->btn_ce, SIGNAL(clicked()), this, SLOT(deleteOperand()));
    connect(ui->btn_del, SIGNAL(clicked()), this, SLOT(deleteDigit()));

    connect(ui->btn_eq, SIGNAL(clicked()), this, SLOT(calculate()));

    ui->btn_plus->setCheckable(true);
    ui->btn_minus->setCheckable(true);
    ui->btn_mult->setCheckable(true);
    ui->btn_div->setCheckable(true);
    ui->btn_eq->setCheckable(true);
    ui->btn_eq->setChecked(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendDigit() {
    QPushButton *button = (QPushButton *) sender();
    if (ui->btn_eq->isChecked()) {
        ui->lbl_result->clear();
        ui->btn_eq->setChecked(false);
    }
    appendText(button->text());
}

void MainWindow::appendDot() {
    if (ui->btn_eq->isChecked()) {
        ui->lbl_result->setText("0");
        ui->btn_eq->setChecked(false);
    }

    QPushButton* op = getOperator();
    QString last_operand = ui->lbl_result->text();
    if (op) {
        last_operand = last_operand.split(op->text())[1];
    }
    if (!last_operand.contains(".")) {
        if (last_operand.size() == 0) {
            appendText("0");
        }
        appendText(".");
    }
}

void MainWindow::appendOp() {
    if (ui->btn_eq->isChecked()) {
        if (ui->lbl_result->text().contains("Zero Division Error")) {
            return;
        }
        ui->btn_eq->setChecked(false);
    }
    QPushButton *button = (QPushButton *) sender();
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
    appendText(button->text());
}

void MainWindow::deleteExpression() {
    ui->lbl_result->setText("0");
    ui->btn_eq->setChecked(true);

    ui->btn_plus->setChecked(false);
    ui->btn_minus->setChecked(false);
    ui->btn_mult->setChecked(false);
    ui->btn_div->setChecked(false);
}

void MainWindow::deleteOperand() {
    QPushButton *button = getOperator();
    if (!button) {
        deleteExpression();
        return;
    }
    QString expression = ui->lbl_result->text().split(button->text())[0];
    ui->lbl_result->setText(expression + button->text());
}

void MainWindow::deleteDigit() {
    if (ui->btn_eq->isChecked()) {
        return;
    }

    QString expression = ui->lbl_result->text();
    QPushButton *op = getOperator();
    if (op) {
        if (expression[expression.size() - 1] == op->text()) {
            return;
        }
    }
    if (expression.size() > 1) {
        expression.chop(1);
        ui->lbl_result->setText(expression);
    } else if (expression != "0") {
        ui->lbl_result->setText("0");
        ui->btn_eq->setChecked(true);
    }
}

void MainWindow::calculate() {
    ui->btn_eq->setChecked(true);
    if (ui->btn_plus->isChecked()) {
        ui->btn_plus->setChecked(false);
        second_num = ui->lbl_result->text().split(ui->btn_plus->text())[1].toDouble();
        first_num = first_num + second_num;
    } else if (ui->btn_minus->isChecked()) {
        ui->btn_minus->setChecked(false);
        second_num = ui->lbl_result->text().split(ui->btn_minus->text())[1].toDouble();
        first_num = first_num - second_num;
    } else if (ui->btn_mult->isChecked()) {
        ui->btn_mult->setChecked(false);
        second_num = ui->lbl_result->text().split(ui->btn_mult->text())[1].toDouble();
        first_num = first_num * second_num;
    } else if (ui->btn_div->isChecked()) {
        ui->btn_div->setChecked(false);
        second_num = ui->lbl_result->text().split(ui->btn_div->text())[1].toDouble();
        if (second_num != 0) {
            first_num = first_num / second_num;
        } else {
            ui->lbl_result->setText("Zero Division Error");
            return;
        }
    } else {
        first_num = ui->lbl_result->text().toDouble();
    }
    QString result = QString::number(first_num, 'g', MAX_DIGITS);
    ui->lbl_result->setText(result);
}

void MainWindow::appendText(QString text) {
    QString new_text = ui->lbl_result->text() + text;
    ui->lbl_result->setText(new_text);
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

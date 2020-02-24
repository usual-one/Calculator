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

    error = false;
    operator_appended = false;

    op = nullptr;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendDigit() {
    QPushButton *button = (QPushButton *) sender();
    if (ui->btn_eq->isChecked()) {
        if (error) {
            error = false;
        }
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
    if (ui->btn_eq->isChecked() || operator_appended) {
        if (error) {
            error = false;
        }
        if (ui->btn_eq->isChecked()) {
            ui->btn_eq->setChecked(false);
            ui->lbl_action->clear();
        }
        ui->lbl_result->setText("0");
        operator_appended = false;
    }

    QString operand = ui->lbl_result->text();
    if (!operand.contains(".")) {
        appendText(ui->lbl_result, ".");
    }
}

void MainWindow::appendOp() {
    QPushButton *button = (QPushButton *) sender();

    if (ui->btn_eq->isChecked()) {
        if (error) {
            if (button->text() == "-") {
                deleteExpression();
                ui->btn_eq->setChecked(false);
                ui->lbl_result->setText(button->text());
            }
            button->setChecked(false);
            return;
        }
        ui->lbl_action->clear();
        ui->btn_eq->setChecked(false);
        if (button->text() == "-" && ui->lbl_result->text() == "0") {
            ui->lbl_result->setText(button->text());
            button->setChecked(false);
            return;
        }
    }

    QString expression = ui->lbl_action->text();
    if (!expression.size()) {
        expression = ui->lbl_result->text();
    }

    bool negative_expression_flag = false;
    if (expression[0] == "-") {
        negative_expression_flag = true;
        expression.remove(0, 1);
    }

    if (!expression.size()) {
        if (button->text() == "-") {
            deleteExpression();
        }
        return; // lbl_result: "-"
    }


    if (expression.contains(button->text())) {
        button->setChecked(true);
        return;
    }
    button->setChecked(false);
    QPushButton *op = getOperator();
    if (op) {
        op->setChecked(false);
        expression.chop(1);
    }

    if (negative_expression_flag) {
        expression = "-" + expression;
    }
    first_num = expression.toDouble();
    button->setChecked(true);
    this->op = button;
    operator_appended = true;
    ui->lbl_action->setText(expression + button->text());
}

void MainWindow::calculateUnaryOp() {
    if (error) {
        return;
    }
    QPushButton *button = (QPushButton *) sender();
    QString btext = button->text();

    if (ui->lbl_result->text() == "-") {
        return;
    }

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
        if (-1 <= first_num && first_num <= 1) {
            first_num = asin(first_num);
        } else {
            error = true;
        }
    }
    if (btext == "arccos") {
        if (-1 <= first_num && first_num <= 1) {
            first_num = acos(first_num);
        } else {
            error = true;
        }
    }
    if (btext == "arctg") {
        first_num = atan(first_num);
    }
    if (btext == "√") {
        if (first_num >= 0) {
            first_num = sqrt(first_num);
        } else {
            error = true;
        }
    }
    if (btext == "1/x") {
        if (first_num != 0) {
            first_num = 1 / first_num;
        } else {
            error = true;
        }
        btext.chop(1);
    }

    ui->lbl_action->setText(btext + ui->lbl_result->text());
    if (!error) {
        ui->lbl_result->setText(QString::number(first_num, 'g', MAX_DIGITS));
        this->op = button;
    } else {
        ui->lbl_result->setText("Arithmetic error");
    }

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

    error = false;
    operator_appended = false;
    this->op = nullptr;
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
    if (error) {
        ui->btn_eq->setChecked(true);
        return;
    }

    if (op) {
        if (op == ui->btn_plus || op == ui->btn_minus || op == ui->btn_mult || op == ui->btn_div) {
            if (ui->lbl_action->text()[ui->lbl_action->text().size() - 1] != op->text()) {
                ui->lbl_action->setText(QString::number(first_num, 'g', MAX_DIGITS));
                op->click();
            } else {
                second_num = ui->lbl_result->text().toDouble();
            }
        } else {
            op->click();
            return;
        }
    }

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
            ui->lbl_result->setText("Arithmetic error");
            ui->btn_eq->setChecked(true);
            error = true;
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

    if (ui->lbl_result->text() == "-") {
        return;
    }

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

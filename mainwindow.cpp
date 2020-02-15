#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QString>

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

    connect(ui->btn_plus, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_minus, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_mult, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_div, SIGNAL(clicked()), this, SLOT(appendOp()));

    ui->btn_plus->setCheckable(true);
    ui->btn_minus->setCheckable(true);
    ui->btn_mult->setCheckable(true);
    ui->btn_div->setCheckable(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendDigit() {
    QPushButton *button = (QPushButton *) sender();
    appendText(button->text());
}

void MainWindow::appendOp() {
    QPushButton *button = (QPushButton *) sender();
    first_num = ui->lbl_result->text().toDouble();
    button->setChecked(true);
    appendText(button->text());
}

void MainWindow::appendText(QString text) {
    QString new_text = ui->lbl_result->text() + text;
    ui->lbl_result->setText(new_text);
}


void MainWindow::on_btn_c_clicked() {
    ui->lbl_result->setText("");
}

void MainWindow::on_btn_eq_clicked() {
    if (ui->btn_plus->isChecked()) {
        second_num = ui->lbl_result->text().split(ui->btn_plus->text())[1].toDouble();
        first_num = first_num + second_num;
    }
    if (ui->btn_minus->isChecked()) {
        second_num = ui->lbl_result->text().split(ui->btn_minus->text())[1].toDouble();
        first_num = first_num - second_num;
    }
    if (ui->btn_mult->isChecked()) {
        second_num = ui->lbl_result->text().split(ui->btn_mult->text())[1].toDouble();
        first_num = first_num * second_num;
    }
    if (ui->btn_div->isChecked()) {
        second_num = ui->lbl_result->text().split(ui->btn_div->text())[1].toDouble();
        if (second_num != 0) {
            first_num = first_num / second_num;
        } else {
            ui->lbl_result->setText("Zero Division Error");
            return;
        }
    }
    QString result = QString::number(first_num, 'g', MAX_DIGITS);
    ui->lbl_result->setText(result);
}

void MainWindow::on_btn_dot_clicked() {

    if (!ui->lbl_result->text().contains(".")) {
        appendText(".");
    }
}

//QString getLastOperand(QString expression)
//{
//    QString operators = "+-×÷";
//    for
//}

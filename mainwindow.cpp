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
    connect(ui->btn_9, SIGNAL(clicked()), this, SLOT(appendDigit()));

    connect(ui->btn_plus, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_minus, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_mult, SIGNAL(clicked()), this, SLOT(appendOp()));
    connect(ui->btn_div, SIGNAL(clicked()), this, SLOT(appendOp()));
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
    last_op = button->text();
    appendText(last_op);
}

void MainWindow::appendText(QString text) {
    QString new_text = ui->lbl_result->text() + text;
    ui->lbl_result->setText(new_text);
}


void MainWindow::on_btn_c_clicked() {
    ui->lbl_result->setText("");
}


void MainWindow::on_btn_eq_clicked() {
    second_num = ui->lbl_result->text().split(last_op)[1].toDouble();
    if (last_op == "+") {
        first_num = first_num + second_num;
    }
    if (last_op == "-") {
        first_num = first_num - second_num;
    }
    if (last_op == "×") {
        first_num = first_num * second_num;
    }
    if (last_op == "÷") {
        first_num = first_num / second_num;
    }
    QString result = QString::number(first_num);
    ui->lbl_result->setText(result);
}


void MainWindow::on_btn_dot_clicked() {
    if (!ui->lbl_result->text().contains(".")) {
        appendText(".");
    }
}

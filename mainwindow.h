#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    void appendOp();

    void appendText(QString text);

    QPushButton *getOperator();

    void on_btn_c_clicked();

    void on_btn_eq_clicked();

    void on_btn_dot_clicked();

    void on_btn_del_clicked();

    void on_btn_ce_clicked();

private:
    double first_num;
    double second_num;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

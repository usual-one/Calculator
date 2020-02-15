#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_btn_c_clicked();

    void on_btn_eq_clicked();

    void on_btn_dot_clicked();

private:
    double first_num;
    double second_num;
    QString last_op;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

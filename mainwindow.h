#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private slots:
    void on_pushBtnStart_clicked();
    void timer_slot();

    void on_pushBtnSetTime_clicked();

    void on_pushBtnNext_clicked();

private:
    Ui::mainwindow *ui;

    void readsettings();
    void writesettings();

    QTimer *timer;
    int m, s, ms;
    short countSession;
    short pastSession;
    short studyStatus; // 2 = study, 1 = rest, 0 = stop
    bool flagbtn = true; // btn start/stop
};
#endif // MAINWINDOW_H

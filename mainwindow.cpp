#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
    m = 25;
    s = ms = 0;
    ui->spinBoxStudyTime->setValue(25);
    ui->spinBoxRestTime->setValue(5);
    ui->spinBoxSession->setValue(3);
    readsettings();
}

mainwindow::~mainwindow()
{
    writesettings();
    delete ui;
}

void mainwindow::timer_slot()
{
    if(m != 0 || s != 0 || ms != 0)
    {
        ms--;
        if(ms < 0)
        {
            ms = 99;
            s--;
            if(s < 0)
            {
                s = 59;
                if(m > 0)
                    m--;
            }
        }
    }
    else if(ui->actionAutostart_next_session->isChecked() && pastSession != countSession)
    {
        if(studyStatus == 2)
        {
            studyStatus = 1;
            ++pastSession;
            m = ui->spinBoxRestTime->value();
        }
        else if(studyStatus == 1)
        {
            studyStatus = 2;
            m = ui->spinBoxStudyTime->value();
        }
    }
    else if(pastSession != countSession)
    {
        on_pushBtnStart_clicked();
        ui->pushBtnNext->show();
        if(studyStatus == 2)
            ui->pushBtnNext->setText("Start rest");
        else
            ui->pushBtnNext->setText("Start next session");
    }
    else
    {
        flagbtn = !flagbtn;
        studyStatus = 0;
        ui->pushBtnStart->setText("Start");
        timer->stop();
        ui->pushBtnSetTime->show();
        ui->spinBoxStudyTime->setEnabled(true);
        ui->spinBoxRestTime->setEnabled(true);
        ui->spinBoxSession->setEnabled(true);
    }

    ui->statusBar->showMessage("Session " + QString::number(pastSession) + "/" + QString::number(countSession));

    if(ui->actionShow_seconds->isChecked())
        ui->labelTimer->setText(QString("%1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
    else
        ui->labelTimer->setText(QString("%1").arg(m + 1, 3));
}

void mainwindow::on_pushBtnStart_clicked()
{
    flagbtn = !flagbtn;
    if(flagbtn)
    {
        ui->pushBtnStart->setText("Start");
        timer->stop();
        ui->pushBtnSetTime->show();
        ui->spinBoxStudyTime->setEnabled(true);
        ui->spinBoxRestTime->setEnabled(true);
        ui->spinBoxSession->setEnabled(true);
    }
    else
    {

        ui->pushBtnStart->setText("Stop");
        timer->start(10);
        ui->pushBtnSetTime->hide();
        ui->spinBoxStudyTime->setEnabled(false);
        ui->spinBoxRestTime->setEnabled(false);
        ui->spinBoxSession->setEnabled(false);
    }
    if (studyStatus == 0)
        studyStatus = 2;
}



void mainwindow::on_pushBtnSetTime_clicked()
{
    m = ui->spinBoxStudyTime->value();
    s = ms = 0;
    countSession = ui->spinBoxSession->value();

    studyStatus = 2;

    if(ui->actionShow_seconds->isChecked())
        ui->labelTimer->setText(QString("%1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
    else
        ui->labelTimer->setText(QString("%1").arg(m + 1, 3));
}

void mainwindow::on_pushBtnNext_clicked()
{
    ui->pushBtnNext->hide();
    if(studyStatus == 2)
    {
        studyStatus = 1;
        ++pastSession;
        m = ui->spinBoxRestTime->value();
    }
    else if(studyStatus == 1)
    {
        studyStatus = 2;
        m = ui->spinBoxStudyTime->value();
    }
    on_pushBtnStart_clicked();
}

void mainwindow::readsettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    m = settings.value("studyTime").toInt();
    s = ms = 0;

    if(int i = settings.value("studyTime").toInt(); i == 0)
        ui->spinBoxStudyTime->setValue(25);
    else
        ui->spinBoxStudyTime->setValue(i);
    if(int i = settings.value("restTime").toInt(); i == 0)
        ui->spinBoxRestTime->setValue(5);
    else
        ui->spinBoxRestTime->setValue(i);
    if(int i = settings.value("countSessions").toInt(); i == 0)
        ui->spinBoxSession->setValue(3);
    else
        ui->spinBoxSession->setValue(i);
    countSession = ui->spinBoxSession->value();

    ui->actionShow_seconds->setChecked(settings.value("showSeconds").toBool());
    ui->actionAutostart_next_session->setChecked(settings.value("autostartCheck").toBool());

    pastSession = 0;
    studyStatus = 0;

    ui->pushBtnNext->hide();

    if(ui->actionShow_seconds->isChecked())
        ui->labelTimer->setText(QString("%1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
    else
        ui->labelTimer->setText(QString("%1").arg(m + 1, 3));
}

void mainwindow::writesettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("studyTime", ui->spinBoxStudyTime->value());
    settings.setValue("restTime", ui->spinBoxRestTime->value());
    settings.setValue("countSessions", ui->spinBoxSession->value());
    settings.setValue("showSeconds", ui->actionShow_seconds->isChecked());
    settings.setValue("autostastCheck", ui->actionAutostart_next_session->isChecked());
}


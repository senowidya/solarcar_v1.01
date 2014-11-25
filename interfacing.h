#ifndef INTERFACING_H
#define INTERFACING_H

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>
#include <QTimer>
#include <QTime>


namespace Ui {
class interfacing;
}

class interfacing : public QMainWindow
{
    Q_OBJECT

public:
    explicit interfacing(QWidget *parent = 0);
    ~interfacing();

private:
    Ui::interfacing *ui;
    QSerialPort* obj_port;
    QTimer* obj_timer;
    uint vcount;

private slots:
    void on_actionQuit_triggered();
    void on_actionAboutQt_triggered();
    void readData();
    void on_btn_Open_clicked();
    bool eventFilter(QObject *obj, QEvent *event);
    void on_btn_Clear_clicked();

    void on_pushButton_clicked();
};

#endif // INTERFACING_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QString>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include<QEventLoop>
#include<QNetworkReply>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void keyPressEvent(QKeyEvent *keyValue);
    void show_msg();
    void show_msg(QString str,QColor color);
    void closeEvent(QCloseEvent *event);

    QString sn_bleid_str,argv;
    QStringList sn_bleid;
    QString factory_code;
    QDateTime current_date_time;
    QString current_time;           //  yyyy-MM-dd hh:mm:ss ddd
    QString current_date;           //  yyyy-MM-dd
    int count_pass,count_false;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

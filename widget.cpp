#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDateTime>
#include <QMessageBox>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , sn_bleid("")
    , factory_code("")
    , count_pass(0)
    , count_false(0)
{
    ui->setupUi(this);
    this->setWindowTitle("欧宁添加设备工具");
    ui->lineEdit_count_false->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_count_pass->setFocusPolicy(Qt::NoFocus);
    QDateTime current_date_time = QDateTime::currentDateTime();
    current_time = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    current_date = current_date_time.toString("yyyy-MM-dd");

}

Widget::~Widget()
{
    delete ui;
}

// http://218.17.233.82:8012/addDevice?bleId=OM11831000041;3C971183B128
void Widget::keyPressEvent(QKeyEvent *keyValue)
{
    if(ui->lineEdit_factory_code->text().isEmpty())
    {
        show_msg("请先输入工厂代码！",QColor(255,0,0));
        return;
    }
    if((keyValue->key() == Qt::Key_Return)||(keyValue->key() ==Qt::Key_Enter))
    {
        show_msg();
    }
}

void Widget::on_pushButton_clicked()
{
    if(ui->lineEdit_factory_code->text().isEmpty())
    {
        show_msg("请先输入工厂代码！",QColor(255,0,0));
        return;
    }
    show_msg();
}

void Widget::on_pushButton_2_clicked()
{
    factory_code  = ui->lineEdit_factory_code->text();
    QFile log("./" + factory_code + "_" + current_date +".txt");
    log.open(QIODevice::WriteOnly | QIODevice::Append);
    log.write(current_date.toUtf8());
    log.close();

}

void Widget::show_msg(QString str,QColor color)
{
    QPalette palette;   //设置字体大小和颜色
    QFont ft;
    ft.setPointSize(50);
    ui->mesg->setFont(ft);
    ui->mesg->setStyleSheet("color:white;");
    palette.setColor(QPalette::Background, color);//设置背景颜色为红色
    ui->mesg->setAutoFillBackground(true);
    ui->mesg->setPalette(palette);
    ui->mesg->setText(str);
}

void Widget::show_msg()
{
    sn_bleid = ui->le_num->text().split(";");
    sn_bleid_str = ui->le_num->text();
    if(sn_bleid.count() != 2)
    {
        show_msg("输入有误！",QColor(255,0,0));
        return;
    }
    QNetworkAccessManager mang ;
    QNetworkRequest request("http://218.17.233.82:8012/addDevice?bleId="+sn_bleid.at(1)+"&sn="+sn_bleid.at(0));    // c++11
    QNetworkReply *reply = mang.get(request);

    QEventLoop loop; // 事件循环
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
  //  qDebug() << reply->readAll();
    QString comb_sn_bleid;
    if(reply->readAll().contains("200"))
    {
        show_msg("SN:"+sn_bleid.at(0)+"\nPASS",QColor(0,255,0));
        comb_sn_bleid = "\r\n" + sn_bleid_str + "\tPASS\t"+ current_time ;
        count_pass++;
        ui->lineEdit_count_pass->setText(QString::number(count_pass));
    }else{
        show_msg("SN:"+sn_bleid.at(0)+"\nFALSE",QColor(255,0,0));
        comb_sn_bleid = "\r\n" + sn_bleid_str + "\tFALSE\t"+ current_time;
        count_false++;
        ui->lineEdit_count_false->setText(QString::number(count_false));
    }
    QFile log("./" + factory_code + "_" + current_date +".txt");
    log.open(QIODevice::WriteOnly | QIODevice::Append);
    QByteArray w_str = comb_sn_bleid.toUtf8();
    log.write(w_str);
    log.close();
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(ui->lineEdit_factory_code->text().isEmpty())
    {
        event->accept();
        return;
    }else{
        QFile log("./" + factory_code + "_" + current_date +".txt");
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QString str_count ="\r\n" + current_date + "\tPASS\t" + QString::number(count_pass)
                                                 + "\tFALSE\t" + QString::number(count_false) + "\r\n";

        log.write(str_count.toUtf8());
        log.close();
        event->accept();
    }
}

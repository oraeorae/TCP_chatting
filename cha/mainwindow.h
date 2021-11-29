#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTcpServer *mserver;         //创建QTcpServer对象
    QTcpSocket *msocket;         //创建QTcpSocket对象
    //int num;                     //用户人数

    //用来显示谁在线
    QStringList who;
    QStringListModel *model;

    //学习链接：多客户端https://blog.csdn.net/weixin_44916364/article/details/100586104
    QVector<QTcpSocket*> socketlist;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void new_client();          //接收连接——获取与客户端通信的套接字
    void read_data();           //读取数据

    void send_data(QString which);      //发送信息到客户端

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

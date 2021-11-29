
#include "mainwindow.h"

#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //关联客户端连接信号(当有客户端连接的时候QTcpServer对象会发送newConnection信号--关联槽函数)
    //1.创建QTcpSever对象
    mserver = new QTcpServer(this);
    msocket = NULL;

    //num = 0;

    //3.当服务器被客户端访问时，会发出newConnection()信号，因此为该信号添加槽函数，并用一个QTcpSocket对象接受客户端访问
    connect(mserver,&QTcpServer::newConnection,this,&MainWindow::new_client);

}



MainWindow::~MainWindow()
{
    delete ui;
}



//监听——启动服务器
void MainWindow::on_pushButton_clicked()
{
    //2.侦听一个端口，使得客户端可以使用这个端口访问服务器
    //（listen(ip, port)）
    if( ui->textEdit->toPlainText() == "" ){            //判断是否为空
        ui->textBrowser->append("服务器启动失败，请输入要监听的端口号!");
    }else{
        mserver->listen(QHostAddress::Any,ui->textEdit->toPlainText().toUShort());
        ui->textBrowser->append("服务器启动成功，正在监听端口"+ui->textEdit->toPlainText()+"!");
    }
}



//接受连接
void MainWindow::new_client()
{
    //获取与客户端通信的套接字
    msocket = mserver->nextPendingConnection();

    socketlist.append(msocket);         //将客户端的套接字加入容器中

    //4.使用socket的write函数向客户端发送数据
    msocket->write("服务器连接成功！");

    //获取客户端IP并显示
    QString ip = msocket->peerAddress().toString();
    ip.remove("::ffff:");
    quint16 port = msocket->peerPort();
    QString tmp = QString("客户端[%1:%2] 已上线!").arg(ip).arg(port);
    ui->textBrowser->append(tmp);

    //关联读数据信号(客户端有数据到达服务器QTcpSocket对象会发送readyRead信号--关联槽函数)
    //5.当socket接收缓冲区有新数据到来时，会发出readRead()信号，因此为该信号添加槽函数以读取数据
    connect(msocket,&QTcpSocket::readyRead,this,&MainWindow::read_data);
}



//读取数据
void MainWindow::read_data()
{
    //获取信号发送者
    QTcpSocket *msocket1 = dynamic_cast<QTcpSocket*>(sender());

    //读取数据
    QString msg = msocket1->readAll();
    //获取对方IP和端口
    QString ip = msocket1->peerAddress().toString();
    quint16 port = msocket1->peerPort();
    ip.remove("::ffff:");
    //quint16 port = msocket1->peerPort();
    //QString tmp = QString("客户端[%1:%2]：%3").arg(ip).arg(port).arg(msg);
    if( msg != "@断开连接@" ){              //如果传过来的不是断开连接的信息
        QString tmp = msg;
        ui->textBrowser->append(tmp);

        //传送到每个客户端里面
        for(int i = 0 ; i < socketlist.size() ; i ++ ){
            socketlist[i]->write(tmp.toUtf8());
        }
    }else{              //如果传过来是断开连接的信息
        //num--;          //在线人数减少
        QString tmp = QString("客户端[%1:%2] 已下线!").arg(ip).arg(port);
        ui->textBrowser->append(tmp);
        //QString tmp = "系统信息："+ui->textEdit_2->toPlainText();
        //send_data(tmp);;

    }

}

//断开连接
void MainWindow::on_pushButton_2_clicked()
{
    //断开连接
    ui->textBrowser->append("服务器已经关闭!");
    mserver->disconnect();
    //num = 0;

}

//发送按钮
void MainWindow::on_pushButton_3_clicked()
{

    //向客户端发送信息
    //获取与客户端通信的套接字
    //QTcpSocket *msocket = mserver->nextPendingConnection();
    //使用socket的write函数向客户端发送数据
    QString tmp = "系统信息："+ui->textEdit_2->toPlainText();
    //注意这里要toUtf8，否则会报错
    //msocket->write(tmp.toUtf8());
    send_data(tmp);


}

 //发送信息到客户端
void MainWindow::send_data(QString which){
    //向每个客户端发送
    for(int i = 0 ; i < socketlist.size() ; i++ ){
        socketlist[i]->write(which.toUtf8());
    }

    //服务器本身日志添加
    ui->textBrowser->append(which);

}


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//连接服务器
void MainWindow::on_pushButton_2_clicked()
{
    if( ui->textEdit_4->toPlainText() == "" ){
        QMessageBox::warning(this,tr("提示"),tr("请输入聊天用户名"),tr("确定"));
        return;
    }else if( ui->textEdit->toPlainText() == "" ){
        QMessageBox::warning(this,tr("提示"),tr("请输入IP地址"),tr("确定"));
        return;
    }else if( ui->textEdit_2->toPlainText() == "" ){
        QMessageBox::warning(this,tr("提示"),tr("请输入端口"),tr("确定"));
        return;
    }
    //存储id名字
    idname = ui->textEdit_4->toPlainText();
    msocket = new QTcpSocket(this);
    //关联读数据信号(当QTcpSocket有数据可读会发送readyRead信号--关联槽函数)
    connect(msocket,&QTcpSocket::readyRead,this,&MainWindow::read_data);
    //调用对象方法链接服务器（connectToHost（服务器的IP， 端口））
    //msocket->connectToHost(ui->textEdit->toPlainText(),ui->textEdit_2->toPlainText().toUShort());
    msocket->connectToHost( QHostAddress::LocalHost,ui->textEdit_2->toPlainText().toUShort());


    //禁用按钮以及输入框(412,2),防止重复点击
    ui->pushButton_2->setDisabled(true);
    ui->textEdit->setDisabled(true);
    ui->textEdit_2->setDisabled(true);
    ui->textEdit_4->setDisabled(true);
}

void MainWindow::read_data()
{
    //读取数据
    QString msg = msocket->readAll();
    ui->textBrowser->append(msg);
}


//发送
void MainWindow::on_pushButton_clicked()
{
    if( msocket->state() == QAbstractSocket::ConnectedState ){      //判断连接状态：如果连接
        //向服务器发送数据
        QString data = QString("%1：%2").arg(idname).arg(ui->textEdit_3->toPlainText());
        msocket->write(data.toUtf8());
    }
    //客户端日志
    //QString tmp = QString("客户端：%1").arg(data);
    //ui->textBrowser->append(tmp);
}

//断开连接
void MainWindow::on_pushButton_3_clicked()
{
    if( msocket->state() == QAbstractSocket::ConnectedState ){      //判断连接状态：如果连接
        //断开前发送信息给客户端
        QString data = "@断开连接@";
        msocket->write(data.toUtf8());
        //主动和对方断开连接
        msocket->disconnectFromHost();
        msocket->close();

    }

    //解除禁用按钮以及输入框(412,2)
    ui->pushButton_2->setDisabled(false);
    ui->textEdit->setDisabled(false);
    ui->textEdit_2->setDisabled(false);
    ui->textEdit_4->setDisabled(false);

}


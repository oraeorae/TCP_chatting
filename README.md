# TCP_chatting
【Qt】——TCP聊天室

cha 文件为服务端的代码
chatt_kh 文件为客户端的代码

版本特点:

## 多客户端聊天（2021.10.31）
增加了：
多客户之间的传递
已完成：
1.客户端与服务器端信息的基本传递
1.服务器与客户端交互信息的完善
2.服务器可以向客户端发送信息

学习链接：多客户端https://blog.csdn.net/weixin_44916364/article/details/100586104

关键:
1.套接字容器`QVector<QTcpSocket*> socketlist;`
2. 接受连接的时候，将套接字加入容器中
```
msocket = mserver->nextPendingConnection();
socketlist.append(msocket);         //将客户端的套接字加入容器中
```
3.接收数据的时候，或者发送数据的时候,利用套接字容器将信息发送到每个客户端
```
//传送到每个客户端里面
for(int i = 0 ; i < socketlist.size() ; i ++ ){
	socketlist[i]->write(tmp.toUtf8());
}
```

## 服务器与客户端信息的传递2.0(2021.10.29)
增加了：
1.服务器与客户端交互信息的完善
2.服务器可以向客户端发送信息

## QT客户端与服务端通信(2021.10.29)
```c++
//这个应该放在连接成功那里
 QTcpSocket *msocket = mserver->nextPendingConnection();

//接下来这些就是发送代码
 //使用socket的write函数向客户端发送数据
 QString tmp = "[服务器]："+ui->textEdit_2->toPlainText();
 //注意这里要toUtf8，否则会报错
 msocket->write(tmp.toUtf8());
```

## QT之TCP服务器客户端连接0.1（2021.10.24）
（1）TCP知识点（链接：https://blog.csdn.net/l477918269/article/details/95613127）
在服务端方面：
	TCP要建立监听（listen）和接收链接(accept) 。
在客户端方面：
	要接收链接（connect）
	
具体步骤：
TCP服务端
1.创建套接字（socket）
2.绑定端口(bind)
3.开始监听：
4.接收请求
accept函数
```int accept(int socket, struct sockaddr* address, socklen_t* address_len);```
参数： socket套接字。 address接收到的结构体， address_len结构体的大小。
5.提供服务（自定义）

客户端：
1.创建套接字
2.创建连接
	connect函数
```int connect(int socket, const struct sockaddr* addr, struct sockaddr* addr_len)```
 参数： socket套接字。 address接收到的结构体， address_len结构体的大小。

（2）学习参考链接：https://blog.csdn.net/qq_42449351/article/details/100517623
https://blog.csdn.net/u014695839/article/details/70041771
（3）
问题：
服务器没有监听到
解决方案：
从 https://blog.csdn.net/omg_orange/article/details/73826694 得到灵感，将 `QTcpServer mserver;`改成指针`QTcpServer *mserver;`,并在生成窗口的那里new一个对象。
问题：
客户端没有连接成功
解决方案：
https://blog.csdn.net/WindSunLike/article/details/106248368
当我修改绑定的地址语句为 `QHostAddress::LocalHost` 之后，可以监听成功。

（4）代码

服务器端代码

笔记：
1、服务器除了使用到了QTcpSocket类，还需要用到QTcpSever类。即便如此，也只是比客户端复杂一点点，用到了6个步骤：

（1）创建QTcpSever对象
```c++
server = new QTcpServer();
```
（2）侦听一个端口，使得客户端可以使用这个端口访问服务器
```c++
server->listen(QHostAddress::Any, port);
```
（3）当服务器被客户端访问时，会发出newConnection()信号，因此为该信号添加槽函数，并用一个QTcpSocket对象接受客户端访问
```c++
  	connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
     
    void MainWindow::server_New_Connect()
    {
        //获取客户端连接
        socket = server->nextPendingConnection();
    }
```
（4）使用socket的write函数向客户端发送数据
```c++
socket->write(data);
```
（5）当socket接收缓冲区有新数据到来时，会发出readRead()信号，因此为该信号添加槽函数以读取数据
```c++
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
     
    void MainWindow::socket_Read_Data()
    {
        QByteArray buffer;
        //读取缓冲区数据
        buffer = socket->readAll();
    }
```
（6）取消侦听
```c++
server->close();
```


## QT之TCP服务器客户端连接0.1（2021.10.24）
（1）TCP知识点（链接：https://blog.csdn.net/l477918269/article/details/95613127）
在服务端方面：
	TCP要建立监听（listen）和接收链接(accept) 。
在客户端方面：
	要接收链接（connect）
	
具体步骤：
TCP服务端
    1.创建套接字（socket）
    2.绑定端口(bind)
    3.开始监听：
        例子：饭店拉客
        当我们节假日外出吃饭的时候，经常受到各个店家热情的拉客，当我们被成功拉进馆子之后，在外拉客的店员就不会再管我们了，而是交由店内的店员为我们提供服务。在这个例子中，“拉客”的店员就相当于我们的“监听”端口
        listen函数
        int listen(int socket, int backlo
        参数：sock套接字。
        backlog是一个数字，表示所传链表的大小。具体含义是服务端预留了一小部分资源，这部分资源由一个链表队列位置，这个位置是为了给在排队的进程预留的。作用是为了提高效率，但存在一定的成本。
   4.接收请求
            接收请求就是上面例子中的，在店内为你提供服务的店员。
            accept函数
        	```int accept(int socket, struct sockaddr* address, socklen_t* address_len);```
            参数： socket套接字。 address接收到的结构体， address_len结构体的大小。
    5.提供服务（自定义）

客户端：
1.创建套接字
2.创建连接
	connect函数
```int connect(int socket, const struct sockaddr* addr, struct sockaddr* addr_len)```
 参数： socket套接字。 address接收到的结构体， address_len结构体的大小。

（2）学习参考链接：https://blog.csdn.net/qq_42449351/article/details/100517623
https://blog.csdn.net/u014695839/article/details/70041771
（3）
问题：
服务器没有监听到
解决方案：
从 https://blog.csdn.net/omg_orange/article/details/73826694 得到灵感，将 `QTcpServer mserver;`改成指针`QTcpServer *mserver;`,并在生成窗口的那里new一个对象。
问题：
客户端没有连接成功
解决方案：
https://blog.csdn.net/WindSunLike/article/details/106248368
当我修改绑定的地址语句为 `QHostAddress::LocalHost` 之后，可以监听成功。

（4）代码
服务器端代码
笔记：
1、服务器除了使用到了QTcpSocket类，还需要用到QTcpSever类。即便如此，也只是比客户端复杂一点点，用到了6个步骤：

（1）创建QTcpSever对象
```c++
server = new QTcpServer();
```
（2）侦听一个端口，使得客户端可以使用这个端口访问服务器
```c++
server->listen(QHostAddress::Any, port);
```
（3）当服务器被客户端访问时，会发出newConnection()信号，因此为该信号添加槽函数，并用一个QTcpSocket对象接受客户端访问
```c++
  	connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
     
    void MainWindow::server_New_Connect()
    {
        //获取客户端连接
        socket = server->nextPendingConnection();
    }
```
（4）使用socket的write函数向客户端发送数据
```c++
socket->write(data);
```
（5）当socket接收缓冲区有新数据到来时，会发出readRead()信号，因此为该信号添加槽函数以读取数据
```c++
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
     
    void MainWindow::socket_Read_Data()
    {
        QByteArray buffer;
        //读取缓冲区数据
        buffer = socket->readAll();
    }
```
（6）取消侦听
```c++
server->close();
```







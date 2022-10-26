#include "Server.h"

Server::Server(int port, QWidget* parent):QWidget(parent),NextBlockSize(0)
{
    pServer = new QTcpServer(this);

    //проверка на подключение клиента к серверу
    if (!pServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(0,"Серверная ошибка.","Невозможно запустить сервер:"
                              +pServer->errorString());
        pServer->close();
        return;
    }

    connect(pServer,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));

    txt = new QTextEdit;
    txt->setReadOnly(true);

    QVBoxLayout* Layout = new QVBoxLayout;
    Layout->addWidget(new QLabel("<H1>Сервер</H1>"));
    Layout->addWidget(txt);
    setLayout(Layout);
}

void Server::slotNewConnection()
{
   QTcpSocket* pClientSocket = pServer->nextPendingConnection();

   connect(pClientSocket,SIGNAL(disconnected()),pClientSocket,SLOT(deleteLater()));
   connect(pClientSocket,SIGNAL(readyRead()),this,SLOT(slotReadClient()));
   sendToClient(pClientSocket,"Отклик сервера: Успешно подключено!");
}

void Server::sendToClient(QTcpSocket* pSocket,const QString& str)
{
    QByteArray arr;

    QDataStream out(&arr,QIODevice::WriteOnly);

    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));

    pSocket->write(arr);
}

void Server::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();

    QDataStream in(pClientSocket);

    for(;;)
    {
        if (!NextBlockSize)
        {
            if(pClientSocket->bytesAvailable() < sizeof (quint16))
                break;

            in >> NextBlockSize;
        }
        if (pClientSocket->bytesAvailable() <NextBlockSize)
            break;

        QTime time;
        QString str;
        in >> time >> str;

        QString strMessage;
        strMessage = time.toString() + " " + "Клиент отправил - " + str;
        txt->append(strMessage);

        NextBlockSize = 0;

        sendToClient(pClientSocket,"Отклик сервера: Успешно получено \"" + str + "\"");
    }
}

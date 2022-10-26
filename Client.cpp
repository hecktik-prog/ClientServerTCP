#include "Client.h"
#include "RequiredLibraries.h"

Client::Client(const QString& ip, int port, QWidget* parent): QWidget(parent), NextBlockSize(0)
{
    pSocket = new QTcpSocket(this);
    //установка связи с сервером
    pSocket->connectToHost(ip,port);

    connect(pSocket, SIGNAL(connected()),SLOT(slotConnected()));
    connect(pSocket,SIGNAL(readyRead()),SLOT(slotReadyRead()));
    connect(pSocket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            SLOT(slotError(QAbstractSocket::SocketError)));


    txtInfo = new QTextEdit;
    txtInput = new QLineEdit;
    txtInfo->setReadOnly(true);

    QPushButton* button = new QPushButton("&Отправить");

    connect(button,SIGNAL(clicked()),SLOT(slotSendToServer()));
    connect(txtInput,SIGNAL(returnPressed()),this, SLOT(slotSendToServer()));

    //расположение объектов
    QVBoxLayout* Layout = new QVBoxLayout;
    Layout->addWidget(new QLabel("<H1>Клиент</H1>"));
    Layout->addWidget(txtInfo);
    Layout->addWidget(txtInput);
    Layout->addWidget(button);
    setLayout(Layout);
}

void Client::slotConnected()
{

    txtInfo->append("Получен сигнал подключения");
}

void Client::slotReadyRead()
{
    QDataStream in(pSocket);

    for(;;)
    {
        if (!NextBlockSize)
        {
            if(pSocket->bytesAvailable() < sizeof (quint16))
                break;

            in >> NextBlockSize;
        }

        if (pSocket->bytesAvailable() <NextBlockSize)
            break;

        QTime time;
        QString str;
        in >> time >> str;

        txtInfo->append(time.toString()+ " " + str);
        //обнуление размера блока для следующей итерации
        NextBlockSize = 0;
    }
}

void Client::slotError(QAbstractSocket::SocketError err)
{
    QString strError;

    //формирование сообщения об ошибке
    strError =
            "Ошибка: " + (err == QAbstractSocket::HostNotFoundError ?
                          "Хост не найден.":
                          err == QAbstractSocket::RemoteHostClosedError ?
                          "Удалённый хост закрыт.":
                          err == QAbstractSocket::ConnectionRefusedError ?
                          "Подключение было отклонено.":
                          err == QAbstractSocket::NetworkError?
                          "Произошла ошибка сети.":
                          //приведение к типу QString
                          QString(pSocket->errorString()));

    txtInfo->append(strError);
}

void Client::slotSendToServer()
{
    QByteArray arr;
    QDataStream out(&arr,QIODevice::WriteOnly);

    out << quint16(0) << QTime::currentTime() << txtInput->text();
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));

    pSocket->write(arr);
    txtInput->clear();
}

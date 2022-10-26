#ifndef CLIENT_H
#define CLIENT_H

#pragma once
#include "RequiredLibraries.h"

class Client : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* pSocket;    //атрибут для работы с сокетом
    QTextEdit* txtInfo;     //атрибут для работы с однострочным полем
    QLineEdit* txtInput;    //атрибут для работы с многострочным полем
    quint16 NextBlockSize;  //атрибут для размера блока данных

public:
    Client(const QString& ip, int port, QWidget* parent = 0);//конструктор

private slots:
    void slotReadyRead();                         //метод, вызываемый при поступлении данных от сервера
    void slotError(QAbstractSocket::SocketError); //метод, вызываемый при возникновении ошибок
    void slotSendToServer();                      //метод для отсылки запроса серверу
    void slotConnected();                         //метод для уведомления о установлении соединения
};
#endif // Client_H

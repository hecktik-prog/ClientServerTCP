#ifndef SERVER_H
#define SERVER_H
#pragma once

#include "RequiredLibraries.h"

class Server : public QWidget
{
    Q_OBJECT

private:
    QTcpServer* pServer;    //атрибут для работы с сервером
    QTextEdit* txt;         //атрибут для работы с однострочным полем
    quint16 NextBlockSize;  //атрибут для размера блока данных

private:
    void sendToClient(QTcpSocket* pSocket, const QString& str); //метод для формирования ответа клиенту

public:
    Server(int port, QWidget* parent =0);   //конструктор

public slots:
    virtual void slotNewConnection();       //метод, вызывающийся при соединении с новым клиентом
    void slotReadClient();                  //метод, вызывающийся для обработки информации из запроса клиента
};
#endif // Server_H

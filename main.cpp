#include "mainwindow.h"
#include "Server.h"
#include "Client.h"

int main(int argc, char *argv[])
{
    //Инициализирует оконную систему
    //и создает объект приложения с аргументами командной строки argc в argv
    QApplication app(argc, argv);

    //создаем объект класса сервер
    Server server(1804);
    //запускаем его
    server.show();

    //создаем объект класса клиент
    Client client("localhost",1804);
    //запускаем его
    client.show();

    //с помощью метода exec() запускается основной цикл программы
    return app.exec();
}


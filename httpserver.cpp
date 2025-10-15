#include "httpserver.h"

    HTTPServer::HTTPServer(QObject *parent) :
        QObject(parent)
    {
        server = new QTcpServer(this);

        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

        if(!server->listen(QHostAddress::Any, 9999))
            qDebug() << "Server could not started!";
        else
            qDebug() << "Server started and listening ...";
    }


    void HTTPServer::newConnection()
    {
        socket = server->nextPendingConnection();

        socket->write("client connected\r\n");
        socket->flush();
        socket->waitForBytesWritten(3000);

        char buffer [50];
        sprintf(buffer, "%d\r\n", value);
        socket->write(buffer);
        socket->flush();
        socket->waitForBytesWritten(3000);

        connect(socket, SIGNAL(readyRead()), this, SLOT(readClient()), Qt::DirectConnection);
    }

    void HTTPServer::readClient()
    {
        QByteArray Data = socket->readAll();

        qDebug() << "Data in: " << Data;

        socket->write(Data);

        //if ((std::string stdString(Data.constData(), Data.length())) == "quitServer")
        //    server->close();
    }

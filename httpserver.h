#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

class HTTPServer : public QObject
{
    Q_OBJECT
public:
    explicit HTTPServer(QObject *parent = 0);

    int value;

public slots:
    void newConnection();
    void readClient();

private:
    QTcpServer *server;
    QTcpSocket *socket;

};

#ifndef DUALTCPSERVER_H
#define DUALTCPSERVER_H

#include <QDebug>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextCodec>
#include <QTimer>
class DualTcpServer : public QObject {
    Q_OBJECT
public:
    explicit DualTcpServer(QObject* parent = nullptr);
    bool startServer(QString ip, int port, int subPort);
    QTcpServer* tcpServer;
    QTcpServer* subTcpServer;
    void writeLn(QString s);
signals:
    void onTcpServerExecute(QString msg);
public slots:
    void onTcpServerNewConnection();
    void onSubTcpServerNewConnection();
    void onTcpServerReadyRead();

private:
    QString m_ip;
    int m_port;
    int m_subPort;
    QList<QTcpSocket*> m_clients;
};

#endif // DUALTCPSERVER_H

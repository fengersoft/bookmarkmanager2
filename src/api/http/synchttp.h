#ifndef SYNCHTTP_H
#define SYNCHTTP_H

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QWidget>

class SyncHttp : public QWidget {
    Q_OBJECT
public:
    explicit SyncHttp(QWidget* parent = nullptr);
    bool downloadFile(QString url, QString path);
    void setIpAndPort(const QString& ip, int port, QString protocol = "http");
    int postJsonData(const QString& params, QByteArray& data, QByteArray& ret);
    int getJsonData(const QString& params, QByteArray& ret);
    int getHtmlData(const QString& url, QByteArray& ret);
signals:

private:
    QString m_baseUrl;
};

#endif // SYNCHTTP_H

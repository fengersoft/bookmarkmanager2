#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QObject>

class GlobalData : public QObject {
    Q_OBJECT
public:
    explicit GlobalData(QObject* parent = nullptr);
    static QString qssText;
signals:
};

#endif // GLOBALDATA_H

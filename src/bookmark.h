#ifndef BOOKMARK_H
#define BOOKMARK_H

#include "api/http/webinfo.h"
#include "api/sql/sqlitedao.h"
#include <QAction>
#include <QDesktopServices>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
namespace Ui {
class BookMark;
}

class BookMark : public QWidget {
    Q_OBJECT

public:
    explicit BookMark(QWidget* parent = nullptr);
    ~BookMark();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QString url() const;
    void setUrl(const QString& url);
    QString remark() const;
    void setRemark(const QString& remark);

signals:
    void onClick(QAction* action);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
private slots:
    void onTriggered();

private:
    Ui::BookMark* ui;
    int m_id;
    QString m_name;
    QPixmap m_icon;
    QString m_url;
    QString m_remark;
};

#endif // BOOKMARK_H

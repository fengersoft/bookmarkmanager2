#ifndef MUSTERWIDGET_H
#define MUSTERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QListWidgetItem>

namespace Ui
{
class MusterWidget;
}

class MusterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusterWidget(QWidget* parent = nullptr);
    ~MusterWidget();
    QString text() const;
    void setText(const QString& text);

    int id() const;
    void setId(int id);
    QListWidgetItem* item() const;
    void setItem(QListWidgetItem* item);

signals:
    void onClick(int id);
    void onSelectedItem(QListWidgetItem* item);
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
private:
    Ui::MusterWidget* ui;
    QPixmap m_pix;
    int m_id;
    QString m_text;
    QListWidgetItem* m_item;
};

#endif // MUSTERWIDGET_H

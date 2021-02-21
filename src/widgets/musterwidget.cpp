#include "musterwidget.h"
#include "ui_musterwidget.h"

MusterWidget::MusterWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MusterWidget)
{
    ui->setupUi(this);
    m_pix = QPixmap(":/res/muster.png");
    setMouseTracking(true);
}

MusterWidget::~MusterWidget()
{
    delete ui;
}

void MusterWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter;
    painter.begin(this);
    QRect rc = rect();
    painter.setPen(QColor(197, 197, 197));
    painter.drawRoundedRect(rc, 5, 5);
    QRect iconRc = QRect(32, 0, 64, 64);
    painter.drawPixmap(iconRc, m_pix, m_pix.rect());
    QRect textRc = QRect(0, 64, 128, 32);
    painter.setPen(Qt::black);
    painter.drawText(textRc, Qt::AlignCenter | Qt::ElideRight, m_text);
    painter.end();
}

void MusterWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit onClick(m_id);
    }


}

void MusterWidget::mouseMoveEvent(QMouseEvent* event)
{
    emit onSelectedItem(m_item);
}

QListWidgetItem* MusterWidget::item() const
{
    return m_item;
}

void MusterWidget::setItem(QListWidgetItem* item)
{
    m_item = item;
}

int MusterWidget::id() const
{
    return m_id;
}

void MusterWidget::setId(int id)
{
    m_id = id;
}

QString MusterWidget::text() const
{
    return m_text;
}

void MusterWidget::setText(const QString& text)
{
    m_text = text;
    update();
}

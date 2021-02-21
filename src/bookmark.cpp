#include "bookmark.h"
#include "ui_bookmark.h"

BookMark::BookMark(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::BookMark)
{
    ui->setupUi(this);
    QStringList menuNames;
    menuNames << "显示二维码"
              << "修改"
              << "删除";
    for (int i = 0; i < menuNames.count(); i++)
    {
        QAction* act = new QAction(this);
        act->setText(menuNames[i]);
        connect(act, &QAction::triggered, this, &BookMark::onTriggered);
        addAction(act);
    }
}

BookMark::~BookMark()
{
    delete ui;
}

void BookMark::paintEvent(QPaintEvent* event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setPen(QColor(197, 197, 197));
    painter.drawRoundRect(rect(), 5, 5);
    QRect iconRc(52, 12, 24, 24);
    painter.drawPixmap(iconRc, m_icon, m_icon.rect());
    painter.setPen(Qt::black);
    QRect textRc = QRect(6, 56, 112, 24);
    QFontMetrics fm = painter.fontMetrics();
    QString t_name = m_name;
    t_name = fm.elidedText(t_name, Qt::ElideRight, textRc.width());
    painter.drawText(textRc,
                     Qt::AlignCenter, t_name);
    painter.end();
}

void BookMark::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        QDesktopServices::openUrl(QUrl(m_url));
        QString sql = QString("insert into bookmark_history(bid) values (%1)").arg(m_id);
        sqliteDao()->sqliteWrapper()->execute(sql);
    }
}

void BookMark::onTriggered()
{
    QAction* action = static_cast<QAction*>(sender());
    emit onClick(action);
}

QString BookMark::remark() const
{
    return m_remark;
}

void BookMark::setRemark(const QString& remark)
{
    m_remark = remark;
}

QString BookMark::url() const
{
    return m_url;
}

void BookMark::setUrl(const QString& url)
{
    m_url = url;
    WebInfo* webInfo = new WebInfo();
    bool isBmp;
    QString path = QString(QApplication::applicationDirPath() + "/icons/%1.png").arg(m_id);
    QFileInfo info(path);
    QImage img;
    if (!info.exists())
    {
        img = webInfo->getFavicon(url, isBmp);
        img.save(path, "png");
    }
    else
    {
        img.load(path, "png");
    }
    delete webInfo;
    m_icon = QPixmap::fromImage(img);
    if (m_icon.isNull())
    {
        if (m_url.indexOf("http") == 0)
        {
            m_icon = QPixmap(":/res/ie.png");
        }
        else
        {
            m_icon = QPixmap(":/res/folder.png");
        }
        ;
        img = m_icon.toImage();
        img.save(path, "png");
    }
    m_icon = m_icon.scaled(24, 24);
}

QString BookMark::name() const
{
    return m_name;
}

void BookMark::setName(const QString& name)
{
    m_name = name;
}

int BookMark::id() const
{
    return m_id;
}

void BookMark::setId(int id)
{
    m_id = id;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTheme();
    ui->lvData->setSpacing(2);
    m_categoryid = -1;
    loadAllCateGories(ui->lvData);
    addContextMenus();
    loadMusters();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadAllCateGories(QListWidget* lvData,
                                   bool selectFirst, bool allowCustomAdd, bool enableEvent)
{
    lvData->clear();
    QList<DataCateGoryItem> stl;
    stl << newDataCateGoryItem(-1, "全部")
        << newDataCateGoryItem(-1, "最近")
        << newDataCateGoryItem(-1, "常用")
        << newDataCateGoryItem(-1, "集合");
    QSqlQuery qry;
    sqliteDao()->sqliteWrapper()->select("select * from category order by sxh", qry);
    while (qry.next())
    {
        stl << newDataCateGoryItem(qry.value("id").toInt(),
                                   qry.value("name").toString());
    }

    stl << newDataCateGoryItem(-1, "类别管理")
        << newDataCateGoryItem(-1, "设置");
    for (int i = 0; i < stl.count(); i++)
    {
        DataCateGoryItem data = stl.at(i);
        if (!allowCustomAdd)
        {
            if (data.id == -1)
            {
                continue;
            }
        }
        QListWidgetItem* item = new QListWidgetItem(lvData);
        item->setSizeHint(QSize(lvData->width() - 48, 36));
        DataCateGory* itemWidget = new DataCateGory(lvData);
        itemWidget->setCaption(data.name);
        itemWidget->setId(data.id);
        itemWidget->setItem(item);
        itemWidget->setListWidget(lvData);
        if (enableEvent)
        {
            connect(itemWidget, &DataCateGory::onClick, this, &MainWindow::onItemClick);
        }

        lvData->setItemWidget(item, itemWidget);
        if (selectFirst)
        {
            if (i == 0)
            {
                lvData->setCurrentRow(0);
                emit itemWidget->onClick();
            }
        }
    }
}

void MainWindow::loadBookMarks(QString sql)
{
    QSet<int> ids;
    ui->lvBookMark->setUpdatesEnabled(false);
    ui->lvBookMark->clear();
    ui->lvBookMark->setSpacing(12);
    QSqlQuery qry;
    sqliteDao()->sqliteWrapper()->select(sql, qry);
    ui->lvBookMark->setViewMode(QListView::IconMode);
    while (qry.next())
    {
        if (ids.contains(qry.value("id").toInt()))
        {
            continue;
        }
        ids << qry.value("id").toInt();
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(QSize(128, 80));
        BookMark* bookmark = new BookMark(ui->lvBookMark);
        bookmark->setId(qry.value("id").toInt());
        bookmark->setName(qry.value("name").toString());
        bookmark->setUrl(qry.value("url").toString());
        bookmark->setRemark(qry.value("remark").toString());
        connect(bookmark, &BookMark::onClick, this, &MainWindow::onBookmarkClick);
        ui->lvBookMark->addItem(item);
        ui->lvBookMark->setItemWidget(item, bookmark);
    }
    ui->lvBookMark->setUpdatesEnabled(true);
}

void MainWindow::loadTheme(int index)
{
    QFile file(QApplication::applicationDirPath() + QString("/qss/%1.qss").arg(index));
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
}

void MainWindow::addContextMenus()
{
    QStringList menuNames;
    menuNames << "新增" << "修改" << "删除";
    for (int i = 0; i < menuNames.count(); i++)
    {
        QAction* act = new QAction(this);
        act->setText(menuNames[i]);
        connect(act, &QAction::triggered, this, &MainWindow::onLvGroupTriggered);
        ui->lvGroup->addAction(act);
    }
}

void MainWindow::loadMusters()
{
    ui->lvGroup->setSpacing(12);
    QSqlQuery qry;
    QString sql = "select * from muster order by id desc";
    sqliteDao()->sqliteWrapper()->select(sql, qry);
    ui->lvGroup->clear();
    while (qry.next())
    {
        QListWidgetItem* item = new QListWidgetItem();
        MusterWidget* w = new MusterWidget(this);
        item->setData(Qt::UserRole + 1, qry.value("id").toInt());
        w->setId(qry.value("id").toInt());
        w->setText(qry.value("name").toString());
        connect(w, &MusterWidget::onClick, this, &MainWindow::onMusterWidgetClick);
        connect(w, &MusterWidget::onSelectedItem, this, &MainWindow::onMusterSelectedItem);
        item->setSizeHint(QSize(128, 96));
        w->setItem(item);
        ui->lvGroup->addItem(item);
        ui->lvGroup->setItemWidget(item, w);

    }
}

void MainWindow::onItemClick()
{
    DataCateGory* w = static_cast<DataCateGory*>(sender());
    m_categoryid = w->id();
    QString caption = w->caption();

    if (caption == "类别管理")
    {
        ui->wgtPages->setCurrentIndex(1);
        loadAllCateGories(ui->lvCategory, false, false, false);
    }
    else if (caption == "集合")
    {
        ui->wgtPages->setCurrentIndex(2);


    }
    else
    {
        ui->wgtPages->setCurrentIndex(0);
        if (caption == "全部")
        {
            m_bookmark_sql = "select * from bookmark order by id desc";
            loadBookMarks(m_bookmark_sql);
        }
        else if (caption == "最近")
        {
            m_bookmark_sql = "select a.* from bookmark a left join  bookmark_history b on a.id=b.bid order by b.id desc";
            loadBookMarks(m_bookmark_sql);
        }
        else if (caption == "常用")
        {
            m_bookmark_sql = "select a.* from bookmark a "
                             "left join  (select bid,ifnull(count(*),0) as num "
                             "from bookmark_history group by bid) b on a.id=b.bid "
                             "where b.num is not null order by b.num desc";
            loadBookMarks(m_bookmark_sql);
        }
        else
        {
            m_bookmark_sql = QString("select * from bookmark where pid=%1 order by id desc").arg(w->id());
            loadBookMarks(m_bookmark_sql);
        }
    }
}

void MainWindow::on_btnAdd_clicked()
{
    QString caption;
    bool ret = setValue("新增类别", "类别", caption);
    int sxh = sqliteDao()->sqliteWrapper()->getMaxId("category") + 1;
    if (ret == true)
    {
        sqliteDao()->sqliteWrapper()->execute(
            QString("insert into category(name,sxh) values ('%1',%2)").arg(caption).arg(sxh));
        loadAllCateGories(ui->lvData, false);
        loadAllCateGories(ui->lvCategory, false, false, false);
    }
}

void MainWindow::on_btnEdit_clicked()
{
    QListWidgetItem* item = ui->lvCategory->currentItem();
    if (item == nullptr)
    {
        return;
    }
    DataCateGory* itemWidget = static_cast<DataCateGory*>(
                                   ui->lvCategory->itemWidget(item));
    QString caption = itemWidget->caption();
    bool ret = setValue("修改类别", "类别", caption);
    {
        sqliteDao()->sqliteWrapper()->execute(
            QString("update category set name='%1' where id=%2").arg(caption).arg(itemWidget->id()));
        loadAllCateGories(ui->lvData, false);
        itemWidget->setCaption(caption);
    }
}

void MainWindow::on_btnDelete_clicked()
{
    QListWidgetItem* item = ui->lvCategory->currentItem();
    if (item == nullptr)
    {
        return;
    }
    int ret = QMessageBox::question(this, "提示", "确定删除选中项吗?");
    if (ret == QMessageBox::No)
    {
        return;
    }

    DataCateGory* itemWidget = static_cast<DataCateGory*>(
                                   ui->lvCategory->itemWidget(item));
    sqliteDao()->sqliteWrapper()->execute(
        QString("delete from category  where id=%1").arg(itemWidget->id()));
    ui->lvCategory->takeItem(ui->lvCategory->currentRow());
    loadAllCateGories(ui->lvData, false);
}

void MainWindow::on_btnMoveUp_clicked()
{
    int row = ui->lvCategory->currentRow();
    if (row == 0)
    {
        return;
    }
    QListWidgetItem* curItem = ui->lvCategory->currentItem();
    DataCateGory* oldItemWidget = static_cast<DataCateGory*>(ui->lvCategory->itemWidget(curItem));
    DataCateGory* newItemWidget = new DataCateGory(ui->lvCategory);
    newItemWidget->setId(oldItemWidget->id());
    newItemWidget->setCaption(oldItemWidget->caption());
    ui->lvCategory->takeItem(row);
    row--;
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(QSize(ui->lvCategory->width() - 48, 36));
    ui->lvCategory->insertItem(row, item);
    newItemWidget->setListWidget(ui->lvCategory);
    newItemWidget->setItem(item);
    ui->lvCategory->setItemWidget(item, newItemWidget);
    ui->lvCategory->setCurrentRow(row);
}

void MainWindow::on_btnAddUrl_clicked()
{
    EditUrlDialog* dlg = new EditUrlDialog();
    dlg->setWindowTitle("添加链接");
    dlg->addClipboardData();
    dlg->setPid(m_categoryid);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted)
    {
        QString sql = "insert into bookmark(pid,name,url,remark)"
                      " values (%1,'%2','%3','%4')";
        sqliteDao()->sqliteWrapper()->execute(
            QString(sql).arg(dlg->pid()).arg(dlg->name()).arg(dlg->url()).arg(dlg->remark()));
        loadBookMarks(m_bookmark_sql);
    }
    delete dlg;
}

void MainWindow::on_btnMoveDown_clicked()
{
    int row = ui->lvCategory->currentRow();
    if (row == (ui->lvCategory->count() - 1))
    {
        return;
    }
    QListWidgetItem* curItem = ui->lvCategory->currentItem();
    DataCateGory* oldItemWidget = static_cast<DataCateGory*>(ui->lvCategory->itemWidget(curItem));
    DataCateGory* newItemWidget = new DataCateGory(ui->lvCategory);
    newItemWidget->setId(oldItemWidget->id());
    newItemWidget->setCaption(oldItemWidget->caption());
    ui->lvCategory->takeItem(row);
    row++;
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(QSize(ui->lvCategory->width() - 48, 36));
    ui->lvCategory->insertItem(row, item);
    newItemWidget->setListWidget(ui->lvCategory);
    newItemWidget->setItem(item);
    ui->lvCategory->setItemWidget(item, newItemWidget);
    ui->lvCategory->setCurrentRow(row);
}

void MainWindow::on_btnSaveOrder_clicked()
{
    for (int i = 0; i < ui->lvCategory->count(); i++)
    {
        QListWidgetItem* item = ui->lvCategory->item(i);
        DataCateGory* itemWidget = static_cast<DataCateGory*>(ui->lvCategory->itemWidget(item));
        QString sql = QString("update category set sxh=%1 where id=%2").arg(i + 1).arg(itemWidget->id());
        sqliteDao()->sqliteWrapper()->execute(sql);
    }
    loadAllCateGories(ui->lvData, false, true, true);
}

void MainWindow::onBookmarkClick(QAction* action)
{
    qDebug() << action->text();
    BookMark* bookmark = static_cast<BookMark*>(sender());
    if (action->text() == "修改")
    {
        EditUrlDialog* dlg = new EditUrlDialog();
        dlg->setWindowTitle("修改链接");
        dlg->setInfo(bookmark);
        int ret = dlg->exec();
        if (ret == QDialog::Accepted)
        {
            QString sql = "update bookmark set pid=%1,name='%2',url='%3',remark='%4' where id=%5";

            sqliteDao()->sqliteWrapper()->execute(
                QString(sql).arg(dlg->pid()).arg(dlg->name()).arg(dlg->url()).arg(dlg->remark()).arg(bookmark->id()));
            loadBookMarks(m_bookmark_sql);
        }
        delete dlg;

    }
    else if (action->text() == "删除")
    {
        QString sql = QString("delete from bookmark where id=%1").arg(bookmark->id());
        sqliteDao()->sqliteWrapper()->execute(sql);
        loadBookMarks(m_bookmark_sql);
    }
    else if (action->text() == "显示二维码")
    {
        if (showQrcodeWindow == nullptr)
        {
            delete showQrcodeWindow;
        }
        showQrcodeWindow = new ShowQrcodeWindow(this);
        showQrcodeWindow->showQrcode(bookmark->url());
        showQrcodeWindow->showNormal();
    }
}

void MainWindow::on_edtSearch_returnPressed()
{
    m_bookmark_sql = "select a.* from bookmark a left join  category b on a.pid=b.id "
                     "where (a.name like '%"
                     + ui->edtSearch->text() + "%') or (a.remark like '%"
                     + ui->edtSearch->text() + "%') "
                     "or (b.name like '%"
                     + ui->edtSearch->text() + "%') order by a.id desc";
    loadBookMarks(m_bookmark_sql);
}

void MainWindow::onLvGroupTriggered()
{
    QAction* act = static_cast<QAction*>(sender());
    if (act->text() == "新增")
    {
        EditMusterDialog* dlg = new EditMusterDialog();
        dlg->setWindowTitle("新增");
        int ret = dlg->exec();
        if (ret == QDialog::Accepted)
        {
            dlg->addNewMuster();
            loadMusters();

        }
        delete dlg;
    }
    else if (act->text() == "修改")
    {
        QListWidgetItem* item = ui->lvGroup->currentItem();
        if (item == nullptr)
        {
            return;
        }
        EditMusterDialog* dlg = new EditMusterDialog();
        dlg->setWindowTitle("修改");
        dlg->setId(item->data(Qt::UserRole + 1).toInt());
        int ret = dlg->exec();
        if (ret == QDialog::Accepted)
        {
            dlg->editMuster();
            loadMusters();

        }
        delete dlg;

    }
    else if (act->text() == "删除")
    {
        QListWidgetItem* item = ui->lvGroup->currentItem();
        if (item == nullptr)
        {
            return;
        }

        int ret = QMessageBox::question(this, "提示", "是否删除选中项目?");
        if (ret == QMessageBox::No)
        {
            return;
        }

        QString sql = QString("delete from muster where id=%1").arg(item->data(Qt::UserRole + 1).toInt());
        sqliteDao()->sqliteWrapper()->execute(sql);
        sql = QString("delete from muster_child where pid=%1").arg(item->data(Qt::UserRole + 1).toInt());
        sqliteDao()->sqliteWrapper()->execute(sql);
        loadMusters();

    }



}

void MainWindow::onMusterWidgetClick(int id)
{
    QString sql = QString("select b.* from muster_child a "
                          "left join bookmark b on a.sid=b.id "
                          "where a.pid=%1").arg(id);
    QSqlQuery qry;
    sqliteDao()->sqliteWrapper()->select(sql, qry);
    while (qry.next())
    {
        QString url = qry.value("url").toString();
        QDesktopServices::openUrl(QUrl(url));
        sql = QString("insert into bookmark_history(bid) values (%1)").arg(qry.value("id").toInt());
        sqliteDao()->sqliteWrapper()->execute(sql);
    }
}

void MainWindow::onMusterSelectedItem(QListWidgetItem* item)
{
    ui->lvGroup->setCurrentItem(item);
}

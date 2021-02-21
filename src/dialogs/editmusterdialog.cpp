#include "editmusterdialog.h"
#include "ui_editmusterdialog.h"

EditMusterDialog::EditMusterDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::EditMusterDialog)
{
    ui->setupUi(this);
}

EditMusterDialog::~EditMusterDialog()
{
    delete ui;
}

void EditMusterDialog::addNewMuster()
{
    QString sql = QString("insert into muster(name) values ('%1')").arg(ui->edtTitle->text());
    sqliteDao()->sqliteWrapper()->execute(sql);
    int maxId = sqliteDao()->sqliteWrapper()->getMaxId("muster");
    for (int i = 0; i < ui->lvData->count(); i++)
    {
        QListWidgetItem* item = ui->lvData->item(i);
        QString sql = QString("insert into muster_child(pid,sid) values (%1,%2)").arg(maxId)
                      .arg(item->data(Qt::UserRole + 1).toInt());
        sqliteDao()->sqliteWrapper()->execute(sql);
    }
}

void EditMusterDialog::editMuster()
{
    QString sql = QString("update muster set name='%1' where id=%2").arg(ui->edtTitle->text()).arg(m_id);
    sqliteDao()->sqliteWrapper()->execute(sql);
    sql = QString("delete from muster_child where pid=%1").arg(m_id);
    sqliteDao()->sqliteWrapper()->execute(sql);
    for (int i = 0; i < ui->lvData->count(); i++)
    {
        QListWidgetItem* item = ui->lvData->item(i);
        QString sql = QString("insert into muster_child(pid,sid) values (%1,%2)").arg(m_id)
                      .arg(item->data(Qt::UserRole + 1).toInt());
        sqliteDao()->sqliteWrapper()->execute(sql);
    }
}

void EditMusterDialog::on_edtSearch_returnPressed()
{
    QSqlQuery qry;
    QString sql = "select * from bookmark where name like '%" + ui->edtSearch->text() + "%'";
    sqliteDao()->sqliteWrapper()->select(sql, qry);
    ui->lvSearch->clear();
    while (qry.next())
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(qry.value("name").toString());
        item->setData(Qt::UserRole + 1, qry.value("id").toInt());
        ui->lvSearch->addItem(item);

    }

}

void EditMusterDialog::on_lvSearch_itemDoubleClicked(QListWidgetItem* item)
{
    QListWidgetItem* newItem = new QListWidgetItem();
    newItem->setText(item->text());
    newItem->setData(Qt::UserRole + 1, item->data(Qt::UserRole + 1).toInt());
    ui->lvData->addItem(newItem);


}

void EditMusterDialog::on_lvData_itemDoubleClicked(QListWidgetItem* item)
{
    delete item;
}

void EditMusterDialog::on_btnCancel_clicked()
{
    reject();
}

void EditMusterDialog::on_btnOk_clicked()
{
    accept();
}

int EditMusterDialog::id() const
{
    return m_id;
}

void EditMusterDialog::setId(int id)
{
    m_id = id;
    QString sql = QString("select b.*,c.name as cname "
                          "from muster_child a "
                          "left join bookmark b "
                          "on a.sid=b.id left join muster c "
                          "on a.pid=c.id where c.id=%1").arg(id);
    QSqlQuery qry;
    sqliteDao()->sqliteWrapper()->select(sql, qry);
    bool f = false;
    while (qry.next())
    {
        if (f == false)
        {
            ui->edtTitle->setText(qry.value("cname").toString());
            f = true;
        }
        QListWidgetItem* newItem = new QListWidgetItem();
        newItem->setText(qry.value("name").toString());
        newItem->setData(Qt::UserRole + 1, qry.value("id").toInt());
        ui->lvData->addItem(newItem);
    }
}

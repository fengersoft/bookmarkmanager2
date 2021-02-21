#include "editurldialog.h"
#include "ui_editurldialog.h"

EditUrlDialog::EditUrlDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditUrlDialog)
{
    ui->setupUi(this);
    getCategories();
}

EditUrlDialog::~EditUrlDialog()
{
    delete ui;
}

void EditUrlDialog::addClipboardData()
{
    if (qApp->clipboard()->text() != "")
    {
        QString url = qApp->clipboard()->text();
        if (url.indexOf("http") < 0)
        {
            return;
        }
        ui->edtUrl->setText(url);
        WebInfo* webInfo = new WebInfo();
        QString title = webInfo->getTitle(url);
        ui->edtName->setText(title);
        delete webInfo;
    }
}

int EditUrlDialog::pid()
{
    return ui->cbbCategory->currentData().toInt();
}

QString EditUrlDialog::name()
{
    return ui->edtName->text();
}

QString EditUrlDialog::url()
{
    return ui->edtUrl->text();
}

QString EditUrlDialog::remark()
{
    return ui->edtRemark->text();
}

void EditUrlDialog::setInfo(BookMark* bookmark)
{
    ui->edtName->setText(bookmark->name());
    ui->edtUrl->setText(bookmark->url());
    ui->edtRemark->setText(bookmark->remark());
    int index = getIndex(ui->cbbCategory->currentData().toInt());
    ui->cbbCategory->setCurrentIndex(index);
}

int EditUrlDialog::getIndex(int id)
{
    for (int i = 0; i < ui->cbbCategory->count(); i++)
    {
        if (id == ui->cbbCategory->itemData(i).toInt())
        {
            return i;
        }
    }
    return -1;
}

void EditUrlDialog::setPid(int pid)
{
    ui->cbbCategory->setCurrentIndex(getIndex(pid));
}

void EditUrlDialog::getCategories()
{
    QSqlQuery qry;
    sqliteDao()->sqliteWrapper()->select("select * from category order by sxh", qry);
    while (qry.next())
    {
        ui->cbbCategory->addItem(qry.value("name").toString(),
                                 qry.value("id").toInt());
    }
}

void EditUrlDialog::on_btnCancel_clicked()
{
    reject();
}

void EditUrlDialog::on_btnOK_clicked()
{
    accept();
}

void EditUrlDialog::on_btnOpen_clicked()
{
    QString path = QFileDialog::getExistingDirectory();
    if (path == "")
    {
        return;
    }
    ui->edtUrl->setText(path);
}

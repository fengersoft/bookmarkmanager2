#ifndef EDITURLDIALOG_H
#define EDITURLDIALOG_H

#include "api/http/webinfo.h"
#include "api/sql/sqlitedao.h"
#include "bookmark.h"
#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QFileDialog>

namespace Ui
{
class EditUrlDialog;
}

class EditUrlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUrlDialog(QWidget* parent = nullptr);
    ~EditUrlDialog();
    void addClipboardData();
    int pid();
    QString name();
    QString url();
    QString remark();
    void setInfo(BookMark* bookmark);
    int getIndex(int id);
    void setPid(int pid);

private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

    void on_btnOpen_clicked();

private:
    Ui::EditUrlDialog* ui;
    void getCategories();
};

#endif // EDITURLDIALOG_H

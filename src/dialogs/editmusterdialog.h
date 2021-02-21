#ifndef EDITMUSTERDIALOG_H
#define EDITMUSTERDIALOG_H

#include <QDialog>
#include "../api/sql/sqlitedao.h"
#include <QListWidget>

namespace Ui
{
class EditMusterDialog;
}

class EditMusterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditMusterDialog(QWidget* parent = nullptr);
    ~EditMusterDialog();
    void addNewMuster();
    void editMuster();

    int id() const;
    void setId(int id);

private slots:
    void on_edtSearch_returnPressed();

    void on_lvSearch_itemDoubleClicked(QListWidgetItem* item);

    void on_lvData_itemDoubleClicked(QListWidgetItem* item);

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::EditMusterDialog* ui;
    int m_id;
};

#endif // EDITMUSTERDIALOG_H

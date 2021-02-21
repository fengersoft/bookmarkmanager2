#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "api/controls/category/datacategory.h"
#include "api/controls/setvaluedialog.h"
#include "api/sql/sqlitedao.h"
#include "bookmark.h"
#include "editurldialog.h"
#include "showqrcodewindow.h"
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <memory>
#include "./dialogs/editmusterdialog.h"
#include "./widgets/musterwidget.h"
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void loadAllCateGories(QListWidget* lvData,
                           bool selectFirst = true, bool allowCustomAdd = true, bool enableEvent = true);
    void loadBookMarks(QString sql);
    void loadTheme(int index = 0);
    void addContextMenus();
    void loadMusters();

private slots:
    void onItemClick();
    void on_btnAdd_clicked();

    void on_btnEdit_clicked();

    void on_btnDelete_clicked();

    void on_btnMoveUp_clicked();

    void on_btnAddUrl_clicked();

    void on_btnMoveDown_clicked();

    void on_btnSaveOrder_clicked();
    void onBookmarkClick(QAction* action);

    void on_edtSearch_returnPressed();
    void onLvGroupTriggered();
    void onMusterWidgetClick(int id);
    void onMusterSelectedItem(QListWidgetItem* item);
private:
    Ui::MainWindow* ui;
    QString m_bookmark_sql;
    int m_categoryid;
    ShowQrcodeWindow* showQrcodeWindow = nullptr;
};
#endif // MAINWINDOW_H

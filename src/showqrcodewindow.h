#ifndef SHOWQRCODEWINDOW_H
#define SHOWQRCODEWINDOW_H

#include "api/libqrencode/qrcodewidget.h"
#include <QMainWindow>

namespace Ui {
class ShowQrcodeWindow;
}

class ShowQrcodeWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ShowQrcodeWindow(QWidget* parent = nullptr);
    ~ShowQrcodeWindow();
    void showQrcode(QString qrcode);

private:
    Ui::ShowQrcodeWindow* ui;
    QRCodeWidget* qrcodeWidget;
};

#endif // SHOWQRCODEWINDOW_H

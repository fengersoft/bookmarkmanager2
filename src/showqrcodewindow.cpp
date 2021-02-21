#include "showqrcodewindow.h"
#include "ui_showqrcodewindow.h"

ShowQrcodeWindow::ShowQrcodeWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ShowQrcodeWindow)
{
    ui->setupUi(this);
    qrcodeWidget = new QRCodeWidget(this);
    setCentralWidget(qrcodeWidget);
}

ShowQrcodeWindow::~ShowQrcodeWindow()
{
    delete ui;
}

void ShowQrcodeWindow::showQrcode(QString qrcode)
{
    qrcodeWidget->setQrcode(qrcode);
}

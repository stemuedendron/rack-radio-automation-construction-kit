#include <QPushButton>
#include "mainwindow.h"
#include "fscolordialog.h"
#include "fsgradbutton.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    bt = new FSGradButton(this);
    bt->resize(200,100);

    connect(bt, SIGNAL(pressed()),SLOT(setColor()));

}

void MainWindow::setColor()
{
    bool ok;
    QColor color = FSColorDialog::getColor(this, tr("Choose a color"), &ok);
    if (ok) bt->setColorButton(color);
}

#include <QGridLayout>
#include <QPainter>

#include "fscolordialog.h"
#include "fsgradbutton.h"



FSColorDialog::FSColorDialog(QWidget *parent) : QDialog(parent, Qt::FramelessWindowHint)
{
    setPalette(QPalette(Qt::lightGray));
    setFixedSize(220,300);
    QGridLayout *l = new QGridLayout;
    l->setSpacing(3);
    QList<QColor> colorlist;
    colorlist << "#1D48BB" << "#06829E" << "#006600" << "#990000" << "#800080" << "#FFAA00" << "#A6A6A6"<<"#505050" <<"#000000";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            FSGradButton *bt = new FSGradButton;
            bt->setFixedSize(60,60);
            bt->setColorButton(colorlist.takeAt(0));
            connect(bt, SIGNAL(pressed()),SLOT(setColor()));
            l->addWidget(bt, i, j);
        }
    FSGradButton *closebt = new FSGradButton;
    closebt->setColorButton("#990000");
    closebt->setFixedSize(60,60);
    closebt->setPixmap(QPixmap(":/images/close.png"));
    connect(closebt,SIGNAL(pressed()),SLOT(close()));
    l->addWidget(closebt,3,2);
    setLayout(l);

}

void FSColorDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::red, 6));
    painter.drawRect(2, 2, width()-4, height()-4);
}

void FSColorDialog::setColor()
{
    FSGradButton *senderButton = qobject_cast<FSGradButton *>(sender());
    colorValue = senderButton->getColor();
    done(true);
}

QColor FSColorDialog::getColor(QWidget *parent, const QString &title, bool *ok)
{
    FSColorDialog dialog(parent);
    dialog.setWindowTitle(title);
    int ret = dialog.exec();
    if (ok) *ok = !!ret;
    if (ret) return dialog.colorValue;
}

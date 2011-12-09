#ifndef FSCOLORDIALOG_H
#define FSCOLORDIALOG_H

#include <QDialog>

class FSColorDialog : public QDialog
{
    Q_OBJECT

public:

    explicit FSColorDialog(QWidget *parent = 0);
    static QColor getColor(QWidget *parent, const QString &title, bool *ok = 0);

protected:

    void paintEvent(QPaintEvent *);

private slots:

    void setColor();

private:

    QColor colorValue;

};

#endif // FSCOLORDIALOG_H

#ifndef FSGRADBUTTON_H
#define FSGRADBUTTON_H
#include <QWidget>
#include <QPixmap>

class FSGradButton : public QWidget
{
    Q_OBJECT

public:

    FSGradButton(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    QColor getColor();
    
signals:

    void pressed();

public slots:

    void setColorButton(QColor);
    void setPixmap(QPixmap);

protected:

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:

    bool pushed;
    bool focus;
    QColor colBut;
    QPixmap pixmap;

};

#endif // FSGRADBUTTON_H

#ifndef RSPLITTER_H
#define RSPLITTER_H
#include <QSplitterHandle>
#include <QSplitter>


class RSplitter : public QSplitter
{
    Q_OBJECT

public:

    RSplitter(Qt::Orientation orientation, QWidget *parent = 0);

public slots:

    void setConfigModus(bool);

signals:

    void changeConfigModus(bool);

protected:

    QSplitterHandle *createHandle();

};



class RSplitterHandle : public QSplitterHandle
{
    Q_OBJECT

public:

    RSplitterHandle(Qt::Orientation orientation, RSplitter *parent);

protected:

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:

    QColor m_handleColor;

};


#endif //RSPLITTER_H

#ifndef FSINTERFACE_H
#define FSINTERFACE_H
#include <QWidget>

class FSInterface
{
public:
    virtual ~FSInterface() {}

    virtual QWidget *createFSWidget(QWidget *parent) = 0;

};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(FSInterface, "org.freispiel.Freispiel.FSInterface/0.3")
QT_END_NAMESPACE


#endif

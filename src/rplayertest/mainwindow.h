#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void openFile();


signals:
    void setURI(QString);
private:
    int m_device;

};

#endif // MAINWINDOW_H

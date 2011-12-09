#ifndef RDESKTOP_H
#define RDESKTOP_H
#include <QWidget>

class RSplitter;
class QXmlStreamWriter;


class RDesktop : public QWidget
 {
    Q_OBJECT

public:

    RDesktop(QWidget *parent = 0);

private:

    void saveSplitter(RSplitter *splitter);
    void saveSplitterItem(QObject *obj, QXmlStreamWriter *xml);

    void saveSplittertoXML(RSplitter *splitter);
    void saveSplitterItemtoXML(QObject *obj, QXmlStreamWriter *xml);

private slots:

    void btNewFSWidgetClick(int);
    void btCloseFSWidgetClick();

public slots:

    void setConfigModus(bool);

    //debug this
    void saveFSWidgets();

signals:

    void changeConfigModus(bool);

};

#endif // RDESKTOP_H







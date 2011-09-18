#ifndef RHOTKEYS_H
#define RHOTKEYS_H

#include <QWidget>

class QStringList;
class QStackedWidget;
class QLabel;
class QVBoxLayout;


#include "rblinkbutton.h"

class RHotKeys : public QWidget
{
    Q_OBJECT

public:

    explicit RHotKeys(QWidget *parent = 0);

signals:

    void enableNPButtons(bool);
    void enableIndexButton(bool);
    void editMode(bool);

public slots:

private slots:

    void createHotkeyPage(QString title, const int &rows, const int &cols);
    void showIndexPage();
    void showPreviousPage();
    void showNextPage();
    void indexPageClicked();
    void hotkeyStackChanged(int);

private:

    int sortedInsert(const QString &title);
    void createHotKeys(const int &index, const int &rows, const int &cols);
    void createIndexButton(const int &index, const QString &title, const int &keys);
    void createEditModeLabel(QWidget *widget);
    void editHotkeyPage();
    RBlinkButton *m_btEdit;
    QStringList m_pageList;
    QStackedWidget *m_layout;
    QStackedWidget *m_hotkeyStack;
    QLabel *m_pageTitle;
    QVBoxLayout *m_indexPageLayout;

};

#endif // RHOTKEYS_H

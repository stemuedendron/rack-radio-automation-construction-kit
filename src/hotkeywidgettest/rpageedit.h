#ifndef RPAGEEDIT_H
#define RPAGEEDIT_H

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QTabWidget;
class QLabel;

class RPageEdit : public QDialog
{

    Q_OBJECT

public:

    explicit RPageEdit(QWidget *parent = 0);
    QLineEdit *leEditTitle;
    QLineEdit *leAddTitle;
    QLineEdit *leCloneTitle;
    QSpinBox *sbEditRows;
    QSpinBox *sbEditCols;
    QSpinBox *sbAddRows;
    QSpinBox *sbAddCols;
    QTabWidget *tabWidget;

private slots:

    void okClicked();

private:

    QLabel *m_editEmpty;

};

#endif // RPAGEEDIT_H

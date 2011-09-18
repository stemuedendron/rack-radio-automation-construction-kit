#include "rpageedit.h"
#include <QtGui>

RPageEdit::RPageEdit(QWidget *parent) : QDialog(parent)
{
    setObjectName("rackDialog");
    setWindowTitle(tr("Edit Hotkey Page"));

    //create edit widget:
    leEditTitle = new QLineEdit;
    sbEditRows = new QSpinBox;
    sbEditRows->setRange(1,10);
    sbEditCols = new QSpinBox;
    sbEditCols->setRange(1,10);
    QFormLayout *fl0 = new QFormLayout;
    fl0->setContentsMargins(20,20,20,20);
    fl0->addRow(tr("Page Title:"), leEditTitle);
    fl0->addRow(tr("Rows:"), sbEditRows);
    fl0->addRow(tr("Cols:"), sbEditCols);
    QWidget *wgEdit = new QWidget;
    wgEdit->setLayout(fl0);

    //create add widget:
    leAddTitle = new QLineEdit;
    sbAddRows = new QSpinBox;
    sbAddRows->setRange(1,10);
    sbAddCols = new QSpinBox;
    sbAddCols->setRange(1,10);
    QFormLayout *fl1 = new QFormLayout;
    fl1->setContentsMargins(20,20,20,20);
    fl1->addRow(tr("Page Title:"), leAddTitle);
    fl1->addRow(tr("Rows:"), sbAddRows);
    fl1->addRow(tr("Cols:"), sbAddCols);
    QWidget *wgAdd = new QWidget;
    wgAdd->setLayout(fl1);

    //create clone widget:
    leCloneTitle = new QLineEdit;
    QFormLayout *fl2 = new QFormLayout;
    fl2->setContentsMargins(20,20,20,20);
    fl2->addRow(tr("Page Title:"), leCloneTitle);
    QWidget *wgClone = new QWidget;
    wgClone->setLayout(fl2);

    //create delete widget:
    QLabel *lbDelete = new QLabel(tr("Are you shure to delete this Page?"));
    lbDelete->setWordWrap(true);
    QHBoxLayout *bl = new QHBoxLayout;
    bl->setContentsMargins(20,20,20,20);
    bl->addWidget(lbDelete);
    QWidget *wgDelete = new QWidget;
    wgDelete->setLayout(bl);

    tabWidget = new QTabWidget;
    tabWidget->setObjectName("rackTabWidget");
    tabWidget->addTab(wgEdit, tr("Edit"));
    tabWidget->addTab(wgAdd, tr("Add"));
    tabWidget->addTab(wgClone, tr("Clone"));
    tabWidget->addTab(wgDelete, tr("Delete"));

    //dialog buttons:
    QPushButton *btOK = new QPushButton;
    btOK->setObjectName("rackOkButton");
    btOK->setDefault(true);
    QPushButton *btClose = new QPushButton;
    btClose->setObjectName("rackCloseButton");
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();
    hl->addWidget(btOK);
    hl->addWidget(btClose);

    m_editEmpty = new QLabel(tr("Please Give a Name"));
    m_editEmpty->setObjectName("rackErrorLabel");
    m_editEmpty->setVisible(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    layout->addWidget(m_editEmpty);
    layout->addLayout(hl);
    setLayout(layout);

    connect(btOK, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(btClose, SIGNAL(clicked()), this, SLOT(reject()));

}

void RPageEdit::okClicked()
{
    if ((tabWidget->currentIndex() == 0 && leEditTitle->text().isEmpty()) ||
        (tabWidget->currentIndex() == 1 && leAddTitle->text().isEmpty()) ||
        (tabWidget->currentIndex() == 2 && leCloneTitle->text().isEmpty()))
        m_editEmpty->setVisible(true);
    else accept();
}

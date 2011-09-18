#include "rindexbutton.h"
#include <QtGui>

RIndexButton::RIndexButton(const QString &title, const int &keys, QWidget *parent) : QPushButton(parent)
{
    setObjectName("rackHotkeyIndexButton");
    m_title = new QLabel;
    m_title->setObjectName("rackHotkeyIndexButtonTitleLabel");
    m_keys = new QLabel;
    m_keys->setObjectName("rackHotkeyIndexButtonKeysLabel");
    setTitle(title);
    setKeys(keys);

    m_edit = new QLabel(tr("Edit Mode"));
    m_edit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_edit->setObjectName("rackHotkeyEditMode");
    m_edit->setHidden(true);
    QVBoxLayout * vl = new QVBoxLayout;
    vl->addWidget(m_edit, 0, Qt::AlignJustify | Qt::AlignTop);
    QWidget *lbwidget = new QWidget(this);
    lbwidget->setLayout(vl);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(m_title);
    hl->addWidget(m_keys);
    hl->addStretch();
    QWidget *widget = new QWidget(this);
    widget->setLayout(hl);

    QStackedLayout *sl = new QStackedLayout;
    sl->setStackingMode(QStackedLayout::StackAll);
    sl->addWidget(widget);
    sl->addWidget(lbwidget);
    setLayout(sl);
}

void RIndexButton::setTitle(const QString &title)
{
    if (m_title->text() == title) return;
    m_title->setText(title);
}

void RIndexButton::setKeys(const int &keys)
{
    QString keysStr = "(" + QString::number(keys) + " " + tr("keys") + ")";
    if (m_keys->text() == keysStr) return;
    m_keys->setText(keysStr);
}

QString RIndexButton::title() const
{
    return m_title->text();
}

void RIndexButton::setEditMode(const bool &edit)
{
    m_edit->setVisible(edit);
}

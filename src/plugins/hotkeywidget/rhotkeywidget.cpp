/*
    Copyright (C) 2011, Steffen Müller and the r.a.c.k. team.
    All rights reserved.

    This file is part of r.a.c.k. radio automation construction kit

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author: Steffen Müller
*/

#include "rack.h"
#include "rhotkeywidget.h"
#include "rbutton.h"
#include "rpageedit.h"
#include "rindexbutton.h"
#include <QtGui>


//how to save? after create page or befor remove?

//switch to hotkey page at defined clock (7.00 kaffeesatz) (abfrage + rotes blinken?)

//TODO: show number of currently played hotkeys on index button

RHotKeyWidget::RHotKeyWidget(QWidget *parent, ICore *api)
    : QWidget(parent),
      m_rack(api)
{

    //create header widget:
    QWidget *header = new QWidget;
    header->setObjectName("rackWidgetHeader");
    QLabel *title = new QLabel(tr("Hotkeys"));
    title->setObjectName("rackWidgetHeaderTitle");
    m_pageTitle = new QLabel;
    m_pageTitle->setObjectName("rackWidgetHeaderSubTitle");

    m_btEdit = new RBlinkButton(tr("Edit"));
    m_btEdit->setObjectName("rackButton");
    m_btEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(m_btEdit, SIGNAL(toggled(bool)), this, SIGNAL(editMode(bool)));


    QPushButton *btIndex = new QPushButton(tr("Index"));
    btIndex->setObjectName("rackButton");
    btIndex->setDisabled(true);
    btIndex->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(btIndex,SIGNAL(clicked()),this,SLOT(showIndexPage()));
    connect(this, SIGNAL(enableIndexButton(bool)), btIndex, SLOT(setEnabled(bool)));

    QPushButton *btPrevious = new QPushButton;
    btPrevious->setObjectName("rackLeftArrowButton");
    btPrevious->setDisabled(true);
    btPrevious->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(btPrevious,SIGNAL(clicked()),this,SLOT(showPreviousPage()));
    connect(this, SIGNAL(enableNPButtons(bool)), btPrevious, SLOT(setEnabled(bool)));

    QPushButton *btNext = new QPushButton;
    btNext->setObjectName("rackRightArrowButton");
    btNext->setDisabled(true);
    btNext->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(btNext,SIGNAL(clicked()),this,SLOT(showNextPage()));
    connect(this, SIGNAL(enableNPButtons(bool)), btNext, SLOT(setEnabled(bool)));

    QGridLayout *hl = new QGridLayout;
    hl->addWidget(title,        0, 0);
    hl->addWidget(m_pageTitle,  1, 0);
    hl->addWidget(m_btEdit,       0, 1, 2, 1);
    hl->addWidget(btIndex,      0, 2, 2, 1);
    hl->addWidget(btPrevious,   0, 3, 2, 1);
    hl->addWidget(btNext,       0, 4, 2, 1);
    header->setLayout(hl);

    //create index page:
    QWidget *indexPage = new QWidget;
    indexPage->setObjectName("rackHotkeyIndexPage");
    m_indexPageLayout = new QVBoxLayout;
    m_indexPageLayout->setSpacing(0);
    m_indexPageLayout->setContentsMargins(0,0,0,0);
    m_indexPageLayout->addStretch();
    indexPage->setLayout(m_indexPageLayout);
    QScrollArea *indexPageScrollArea = new QScrollArea;
    indexPageScrollArea->setObjectName("rackHotkeyIndexPage");
    indexPageScrollArea->setWidgetResizable(true);
    indexPageScrollArea->setWidget(indexPage);

    //create the hotkey stack widget:
    m_hotkeyStack = new QStackedWidget;
    connect(m_hotkeyStack,SIGNAL(currentChanged(int)),this,SLOT(hotkeyStackChanged(int)));
    m_layout = new QStackedWidget;
    m_layout->addWidget(indexPageScrollArea);
    m_layout->addWidget(m_hotkeyStack);

    //create main layout:
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(3);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(header);
    layout->addWidget(m_layout);
    setLayout(layout);

    createHotkeyPage("1-Kaffeesatz",2,4);
    createHotkeyPage("2-Kaffeesatz2",2,4);
    createHotkeyPage("3-kaffeesatz3",3,4);
    createHotkeyPage("4-Nowosti",4,6);
    createHotkeyPage("5-V.I.P.",5,6);

}

void RHotKeyWidget::createHotkeyPage(QString title, const int &rows, const int &cols)
{
    //TODO: allow lowercase titles and change the sort?
    title = title.at(0).toUpper() + title.mid(1);
    int index = sortedInsert(title);
    createHotKeys(index, rows, cols);
    createIndexButton(index, title, rows * cols);
}

int RHotKeyWidget::sortedInsert(const QString &title)
{
    int index;
    if (m_pageList.contains(title))
    {
        index = m_pageList.lastIndexOf(title) + 1;
        m_pageList.insert(index, title);
    }
    else
    {
        m_pageList.append(title);
        m_pageList.sort();
        index = m_pageList.indexOf(title);
    }
    return index;
}

void RHotKeyWidget::createHotKeys(const int &index, const int &rows, const int &cols)
{
    QWidget *hotkeyPage = new QWidget;
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(1);
    layout->setContentsMargins(0,0,0,0);
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            RButton *hkb = new RButton(RButton::bkHotKey);
            hkb->setObjectName("rackHotkeyButton");
            createEditModeLabel(hkb);
            layout->addWidget(hkb, row, col);
        }
    }
    hotkeyPage->setLayout(layout);
    m_hotkeyStack->insertWidget(index, hotkeyPage);
}

void RHotKeyWidget::createIndexButton(const int &index, const QString &title, const int &keys)
{
    RIndexButton *button = new RIndexButton(title, keys);
    button->setEditMode(m_btEdit->isChecked());
    connect(m_btEdit, SIGNAL(toggled(bool)), button, SLOT(setEditMode(bool)));
    connect(button, SIGNAL(clicked()), this, SLOT(indexPageClicked()));
    m_indexPageLayout->insertWidget(index, button);
}

void RHotKeyWidget::createEditModeLabel(QWidget *widget)
{
    QLabel *lbEdit = new QLabel(tr("Edit Mode"), this);
    lbEdit->setObjectName("rackHotkeyEditMode");
    lbEdit->setHidden(!m_btEdit->isChecked());
    QObject::connect(m_btEdit, SIGNAL(toggled(bool)), lbEdit, SLOT(setVisible(bool)));
    QVBoxLayout * l = new QVBoxLayout;
    l->addWidget(lbEdit, 0, Qt::AlignJustify | Qt::AlignTop);
    widget->setLayout(l);
}

void RHotKeyWidget::showIndexPage()
{
    emit enableIndexButton(false);
    emit enableNPButtons(false);
    m_layout->setCurrentIndex(0);
}

void RHotKeyWidget::indexPageClicked()
{
    if (m_btEdit->isChecked()) editHotkeyPage();
    else
    {
        RIndexButton *bt = qobject_cast<RIndexButton *>(sender());
        m_hotkeyStack->setCurrentIndex(m_indexPageLayout->indexOf(bt));
        emit enableIndexButton(true);
        m_hotkeyStack->count() == 0 ? emit enableNPButtons(false) : emit enableNPButtons(true);
        m_layout->setCurrentIndex(1);
    }
}

void RHotKeyWidget::showPreviousPage()
{
    m_hotkeyStack->setCurrentIndex(m_hotkeyStack->currentIndex() - 1);
}

void RHotKeyWidget::showNextPage()
{
    m_hotkeyStack->setCurrentIndex(m_hotkeyStack->currentIndex() + 1);
}

void RHotKeyWidget::hotkeyStackChanged(int index)
{
    if (index == -1) m_pageTitle->setText("");
    else m_pageTitle->setText(m_pageList.at(index));
}

void RHotKeyWidget::editHotkeyPage()
{
    RIndexButton *btSender = qobject_cast<RIndexButton *>(sender());
    btSender->setCheckable(true);
    btSender->setChecked(true);
    int index = m_indexPageLayout->indexOf(btSender);
    QWidget *hkPage = m_hotkeyStack->widget(index);
    QGridLayout *hkPageLayout = qobject_cast<QGridLayout *>(hkPage->layout());

    //create the dialog
    RPageEdit dialog(this);
    dialog.leEditTitle->setText(btSender->title());
    dialog.sbEditRows->setValue(hkPageLayout->rowCount());
    dialog.sbEditCols->setValue(hkPageLayout->columnCount());

    if (dialog.exec())
    {
        QString newTitle = dialog.leEditTitle->text();

        switch (dialog.tabWidget->currentIndex())
        {
            case 0: //edit

                //title changed:
                newTitle = newTitle.at(0).toUpper() + newTitle.mid(1);
                if (newTitle != btSender->title())
                {
                    m_pageList.removeAt(index);
                    int newIndex = sortedInsert(newTitle);
                    m_indexPageLayout->removeWidget(btSender);
                    m_indexPageLayout->insertWidget(newIndex, btSender);
                    btSender->setTitle(newTitle);
                    QWidget *hotkeyPage = m_hotkeyStack->widget(index);
                    m_hotkeyStack->removeWidget(hotkeyPage);
                    m_hotkeyStack->insertWidget(newIndex, hotkeyPage);
                }

                //number of rows/cols changed:
                if (hkPageLayout->rowCount() != dialog.sbEditRows->value() || hkPageLayout->columnCount() != dialog.sbEditCols->value())
                {
                    if (hkPageLayout->count() >= dialog.sbEditRows->value() * dialog.sbEditCols->value())
                    {
                        QLayoutItem *child;
                        while ((child = hkPageLayout->takeAt(dialog.sbEditRows->value() * dialog.sbEditCols->value())) != 0)
                        {
                            delete child->widget();
                        }
                        QGridLayout *newLayout = new QGridLayout;
                        newLayout->setSpacing(1);
                        newLayout->setContentsMargins(0,0,0,0);
                        for (int row = 0; row < dialog.sbEditRows->value(); row++)
                        {
                            for (int col = 0; col < dialog.sbEditCols->value(); col++)
                            {
                                child = hkPageLayout->takeAt(0);
                                newLayout->addWidget(child->widget(), row, col);
                            }
                        }
                        delete hkPage->layout();
                        hkPage->setLayout(newLayout);
                    }
                    else if (hkPageLayout->count() < dialog.sbEditRows->value() * dialog.sbEditCols->value())
                    {

                    }
                    btSender->setKeys(dialog.sbEditRows->value() * dialog.sbEditCols->value());
                }
                break;

            case 1: //add
                createHotkeyPage(dialog.leAddTitle->text(), dialog.sbAddRows->value(), dialog.sbAddCols->value());
                break;

            case 2: //clone

                createHotkeyPage(dialog.leCloneTitle->text(), hkPageLayout->rowCount(), hkPageLayout->columnCount());
                //TODO: clone all hotkeys

                break;

            case 3: //delete
                m_pageList.removeAt(index);
                delete m_hotkeyStack->widget(index);
                btSender->deleteLater();
                break;
        }
    }
    btSender->setCheckable(false);
}






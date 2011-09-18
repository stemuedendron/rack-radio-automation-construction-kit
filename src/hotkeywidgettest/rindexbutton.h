#ifndef RINDEXBUTTON_H
#define RINDEXBUTTON_H

#include <QPushButton>

class QLabel;

class RIndexButton : public QPushButton
{
    Q_OBJECT

public:

    explicit RIndexButton(const QString &title, const int &keys, QWidget *parent = 0);
    void setTitle(const QString &title);
    void setKeys(const int &keys);
    QString title() const;

public slots:

    void setEditMode(const bool &);

private:

    QLabel *m_title;
    QLabel *m_keys;
    QLabel *m_edit;

};

#endif // RINDEXBUTTON_H

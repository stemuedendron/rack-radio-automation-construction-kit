#ifndef RSORTFILTERPROXYMODEL_H
#define RSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class RSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    RSortFilterProxyModel(QObject *parent = 0);
    void setSourceRoot(const QModelIndex &sourceRoot);

protected:

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:

    QModelIndex m_sourceRoot;

};

#endif // RSORTFILTERPROXYMODEL_H

#include "rsortfilterproxymodel.h"

RSortFilterProxyModel::RSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_sourceRoot()
{
}

void RSortFilterProxyModel::setSourceRoot(const QModelIndex &sourceRoot)
{
    m_sourceRoot = sourceRoot;
}

bool RSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);

    if (!sourceIndex.isValid() || !source_parent.isValid())
    {
        return true;
    }

    if (m_sourceRoot.isValid() && sourceIndex == m_sourceRoot)
    {
        return true;
    }

    if (source_parent != m_sourceRoot)
    {
        bool childOfRoot = false;
        QModelIndex parent = source_parent;
        for (int depth = 3; depth > 0; depth--)
        {
            if (sourceModel()->data(parent).toString().contains(filterRegExp()))
                return true; // a parent matches the pattern

            parent = parent.parent();
            if (!parent.isValid())
                return true; // tree view handles filtering, and we ascended too far
            if (parent == m_sourceRoot) {
                childOfRoot = true;
                break;
            }
        }
        // OK, we've gone up the tree, did we find our root?
        if (!childOfRoot)
            return true;
    }
    // else, source_parent is a root, so we're good to filter

    // Check if the data for this row matches. If so, the default is to accept
    QString data = sourceModel()->data(sourceIndex).toString();
    if (data.contains(filterRegExp()))
        return true;

    // Now we have to check the child nodes
    // We'll show the row if any child is accepted
    // (i.e., if a file matches, we'll show the directory path to it)
    // Try to fetchMore() first
    sourceModel()->fetchMore(sourceIndex);
    for(int i = 0; i < sourceModel()->rowCount(sourceIndex); ++i) {
        QModelIndex subRow = sourceModel()->index(i, 0, sourceIndex);
        if (!subRow.isValid())
            continue;

        QString rowData = sourceModel()->data(subRow).toString();
        if (rowData.contains(filterRegExp()))
            return true;
    }
    return false; // nothing matched
}

#ifndef RCOUCHDBMODEL_H
#define RCOUCHDBMODEL_H

#include <QStandardItemModel>

class RCouchDBModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit RCouchDBModel(QObject *parent = 0);

signals:

public slots:

};

#endif // RCOUCHDBMODEL_H

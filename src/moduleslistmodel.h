/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef MODULESLISTMODEL_H
#define MODULESLISTMODEL_H


/***
 *
 * unused
 * TODO :template
 */
#include <QAbstractItemModel>
#include "gpumodule.h"

class ModulesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Column
    {
        Title = 0,
        ColumnCount
    };

    ModulesListModel(QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
    void insertData(GPUModule * module) ;

    //List<GPUModule*> * moduleList() const {  return gpuModuleList; }
    GPUModule * get(int row) { return gpuModuleList->at(row); }


private slots:
    void beginInsertItems(int start, int end);
    void endInsertItems();
    void beginRemoveItems(int start, int end);
    void endRemoveItems();
    void changeItems(int start, int end);

private:
    QList<GPUModule*> * gpuModuleList;
    QMap<QModelIndex, QVariant> m_data;
};

#endif // MODULELISTMODEL_H

/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

*/

#include "moduleslistmodel.h"
#include "modulelistitem.h"

ModulesListModel::ModulesListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    gpuModuleList = new QList<GPUModule*>;
}

int ModulesListModel::rowCount(const QModelIndex &parent) const
{
    return gpuModuleList && !parent.isValid() ? gpuModuleList->size() : 0;
}

int ModulesListModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex ModulesListModel::index(int row, int column, const QModelIndex &parent) const
{
    return gpuModuleList && !parent.isValid()
            && row >= 0 && row < gpuModuleList->size()
            && column >= 0 && column < ColumnCount
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex ModulesListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

QVariant ModulesListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        QVariant value = m_data[index];
        if (!value.isValid() && index.column() == Title) {
            return (gpuModuleList->at(index.row()))->name();
            qDebug() << "index rox="<< index.row();
        }
        return value;
    }
    return QVariant();
}


/* substitute to setData */
void ModulesListModel::insertData(GPUModule * module) {
    gpuModuleList->append(module);
    emit dataChanged(QModelIndex(), QModelIndex());
}

bool ModulesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    m_data[index] = value;
    emit dataChanged(index, index);
    return true;
}

void ModulesListModel::beginInsertItems(int start, int end) {
    m_data.clear();
    beginInsertRows(QModelIndex(), start, end);
}

void ModulesListModel::beginRemoveItems(int start, int end){
    m_data.clear();
    beginRemoveRows(QModelIndex(), start, end);
}

void ModulesListModel::changeItems(int start, int end)
{
    m_data.clear();
    emit dataChanged(index(start,0), index(end,ColumnCount));
}

void ModulesListModel::endInsertItems() {  endInsertRows(); }
void ModulesListModel::endRemoveItems() {  endInsertRows(); }

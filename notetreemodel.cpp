#include "NoteTreeModel.h"
#include <qabstractitemmodel.h>
#include <qvariant.h>

NoteTreeModel::NoteTreeModel(QObject *parent) : QAbstractItemModel(parent) {}

QVariant NoteTreeModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    return QVariant();
}

QModelIndex NoteTreeModel::index(int row, int column,
                                 const QModelIndex &parent) const
{
    // FIXME: Implement me!
    if (parent.isValid() && parent.column() != 0) return QModelIndex();
    return createIndex(row, column, nullptr);
}

QModelIndex NoteTreeModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
    return QModelIndex();
}

int NoteTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) return 0;

    // FIXME: Implement me!
    return 0;
}

int NoteTreeModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) return 0;

    return 1;
}

QVariant NoteTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

void NoteTreeModel::setNoteManager(NoteManager *noteManager)
{
    manager = noteManager;
}

NoteManager *NoteTreeModel::getNoteManager() const { return manager; }

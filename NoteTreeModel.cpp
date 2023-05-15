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
    if (!parent.isValid()) {
        if (row == 0) return createIndex(row, column, NoteBooks);
        if (row == 1) return createIndex(row, column, Tags);
    }

    if ((ItemType)parent.internalId() == NoteBooks) {
        if (row >= manager->allDirs().size() || row < 0) return QModelIndex();
        return createIndex(row, column, NoteBook);
    }

    if ((ItemType)parent.internalId() == Tags) {
        if (row >= manager->allTags().size() || row < 0) return QModelIndex();
        return createIndex(row, column, Tag);
    }

    if ((ItemType)parent.internalId() == NoteBook) {
        if (row <= manager->notesOfDir(data(parent).toString()).size() ||
            row < 0)
            return QModelIndex();
        return createIndex(row, column, NoteFromNoteBook);
    }

    if ((ItemType)parent.internalId() == Tag) {
        if (row <= manager->notesOfTag(data(parent).toString()).size() ||
            row < 0)
            return QModelIndex();
        return createIndex(row, column, NoteFromTag);
    }

    if ((ItemType)parent.internalId() == NoteFromNoteBook) return QModelIndex();
    if ((ItemType)parent.internalId() == NoteFromTag) return QModelIndex();

    return QModelIndex();
}

QModelIndex NoteTreeModel::parent(const QModelIndex &index) const
{
    // TODO:
    if (!index.isValid()) return QModelIndex();

    if ((ItemType)index.internalId() == NoteBooks) return QModelIndex();

    if ((ItemType)index.internalId() == Tags) return QModelIndex();

    if ((ItemType)index.internalId() == NoteBook)
        return createIndex(0, 0, NoteBooks);

    if ((ItemType)index.internalId() == Tag) return createIndex(0, 0, Tags);

    if ((ItemType)index.internalId() == NoteFromNoteBook)
        return createIndex(0, 0, NoteBook);

    if ((ItemType)index.internalId() == NoteFromTag)
        return createIndex(0, 0, Tag);

    return QModelIndex();
}

int NoteTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) return 2;

    if ((ItemType)parent.internalId() == NoteBooks) {
        return manager->allDirs().size();
    }

    if ((ItemType)parent.internalId() == Tags) {
        return manager->allTags().size();
    }

    if ((ItemType)parent.internalId() == NoteBook) {
        return manager->notesOfDir(data(parent).toString()).size();
    }

    if ((ItemType)parent.internalId() == Tag) {
        return manager->notesOfTag(data(parent).toString()).size();
    }

    if ((ItemType)parent.internalId() == NoteFromNoteBook) return 0;
    if ((ItemType)parent.internalId() == NoteFromTag) return 0;

    return 0;
}

int NoteTreeModel::columnCount(const QModelIndex &parent) const { return 1; }

QVariant NoteTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (role == Qt::DisplayRole) {
        if ((ItemType)index.internalId() == NoteBooks) {
            return tr("NoteBooks");
        }

        if ((ItemType)index.internalId() == Tags) {
            return tr("Tags");
        }

        if ((ItemType)index.internalId() == NoteBook) {
            return manager->allDirs()[index.row()];
        }

        if ((ItemType)index.internalId() == Tag) {
            return manager->allTags()[index.row()];
        }

        if ((ItemType)index.internalId() == NoteFromNoteBook) {
            QString notebook = data(index.parent()).toString();
            return manager->notesOfDir(notebook)[index.row()].name;
        }

        if ((ItemType)index.internalId() == NoteFromTag) {
            QString tag = data(index.parent()).toString();
            return manager->notesOfTag(tag)[index.row()].name;
        }
    }

    return QVariant();
}

NoteTreeModel::NoteItem *NoteTreeModel::itemFrom(const QModelIndex &index)
{
    if (index.isValid())
        return static_cast<NoteItem *>(index.internalPointer());
    else
        return nullptr;
}

void NoteTreeModel::setNoteManager(NoteManager *noteManager)
{
    manager = noteManager;
}

NoteManager *NoteTreeModel::getNoteManager() const { return manager; }

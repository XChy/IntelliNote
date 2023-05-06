#ifndef NOTETREEMODEL_H
#define NOTETREEMODEL_H

#include <QAbstractItemModel>
#include "NoteManager.h"

class NoteTreeModel : public QAbstractItemModel
{
    Q_OBJECT

   public:
    explicit NoteTreeModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    void setNoteManager(NoteManager *noteManager);
    NoteManager *getNoteManager() const;

   private:
    NoteManager *manager;
};

#endif  // NOTETREEMODEL_H

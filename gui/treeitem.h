#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QVector>

class TreeItem {
public:
    explicit TreeItem(const QVector<QVariant>& data, TreeItem* parent = nullptr);
    ~TreeItem();

    TreeItem* child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    TreeItem* parent();
    int childNumber() const;
    bool setData(int column, const QVariant& value);

private:
    QVector<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem* parentItem;
};

#endif // TREEITEM_H

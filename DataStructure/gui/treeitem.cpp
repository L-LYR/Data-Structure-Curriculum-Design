#include "treeitem.h"

TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
    : itemData(data)
    , parentItem(parent)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

TreeItem* TreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= itemData.size())
        return QVariant();
    return itemData.at(column);
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem* item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for (TreeItem* child : qAsConst(childItems))
        child->insertColumns(position, columns);

    return true;
}

TreeItem* TreeItem::parent()
{
    return parentItem;
}

bool TreeItem::setData(int column, const QVariant& value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

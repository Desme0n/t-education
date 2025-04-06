#ifndef OBJECTTREEMODEL_H
#define OBJECTTREEMODEL_H
#include <QAbstractItemModel>
#include <QObject>
#include <QIcon>

struct TreeItemData {
    QString name;
    QString description;
    QString icon;
};


class ObjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ObjectTreeModel(QObject* parent = nullptr);
    void setColumns(QStringList cols);
    void addItem(const QString &name, const QString &description, const QString &icon, const QModelIndex &parentIdx);
    QObject *_rootItem;  // Явно создаем корень, поскольку объявлен внутри класса он не валиден для присваивания из вне


protected:
    QStringList _columns;
    QObject * objByIndex(const QModelIndex & index) const;      // Это для хранения указателей на элементы дерева
    void saveItems(QJsonObject &jsonObj, QObject *parentObj);   // Вспомогательный метод для рекурсивного сохранения
// QAbstractItemModel interface
public:
virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
virtual QModelIndex parent(const QModelIndex &child) const override;
virtual int rowCount(const QModelIndex &parent) const override;
virtual int columnCount(const QModelIndex &parent) const override;
virtual QVariant data(const QModelIndex &index, int role) const override;
void loadItems(const QJsonObject &jsonObj, QObject *parentObj);
void loadFromFile(const QString &filePath);
void saveToFile(const QString &filePath);
void updateItemIcon(const QModelIndex &index, const QString &newIconPath);
void updateItemDescription(const QModelIndex &index, const QString &newDescription);
void removeItem(const QModelIndex &index);

private:
    QHash<QObject*, TreeItemData> _itemData;

};
#endif


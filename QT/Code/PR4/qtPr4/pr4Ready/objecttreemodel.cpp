#include "mainwindow.h"

//констурктор по умолчанию создает корень
ObjectTreeModel::ObjectTreeModel (QObject *parent)
    : QAbstractItemModel(parent)
{
    _rootItem = new QObject(this); //теперь можно проверить что элемент является корневым
}
//регистрируем количесво колонок в иерархии
void ObjectTreeModel :: setColumns (QStringList cols){
    _columns = cols;
}
void ObjectTreeModel::addItem(const QString &name, const QString &description, const QString &icon, const QModelIndex &parentIdx) {
    QObject* parentObj = objByIndex(parentIdx);
    int newRow = parentObj->children().count();  // Количество до вставки

    beginInsertRows(parentIdx, newRow, newRow);  // Указываем место для нового элемента

    QObject* newItem = new QObject(parentObj);   // Создаем объект
    _itemData[newItem] = {name, description, icon};  // Заполняем данные с иконкой

    endInsertRows();  // Завершаем вставку
}

//метод по преобразованию индекс в указатель QObject
QObject *ObjectTreeModel::objByIndex(const QModelIndex &index) const
{
    //если индекс корень то вернуть корень
    if (!index.isValid()) return _rootItem;
    //иначе вернуть указатель по текущему индексу преобразованного в указатель QOblect
    return static_cast<QObject*>(index.internalPointer());
}
//метод для определения индекса ячейки по переданным строке столбце и индексу родителя
QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    //проверка существует ли такой индекс вообще иначе возвращает пустой индекс
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    QObject * parentObj  = objByIndex(parent);

    return createIndex(row, column, parentObj->children().at(row));
}
//метод по получению индекса родителя элемента по индексу
QModelIndex ObjectTreeModel::parent(const QModelIndex &child) const
{
    QObject * childObj = objByIndex(child);
    QObject * parentObj = childObj->parent();
    if (parentObj == _rootItem)
        return QModelIndex();
    QObject* grandParentObj = parentObj->parent();
    int row = grandParentObj->children().indexOf(parentObj);
    return createIndex(row, 0, parentObj);
}
//количество дочерних строк по родительскому индексу
int ObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    return objByIndex(parent) -> children().count();
}

int ObjectTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1; //
}
//передает пакет данных взятых из ячейки по переданному индесу, где role это роль данных, это отдельная тема Qt,
//короче буквально определяет характер данных

QVariant ObjectTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    QObject *obj = objByIndex(index);
    const TreeItemData &itemData = _itemData[obj];

    if (role == Qt::DisplayRole && index.column() == 0) {
        return itemData.name;
    } else if (role == Qt::UserRole) {
        return itemData.description;
    } else if (role == Qt::DecorationRole && index.column()==0) {
        QIcon icon(itemData.icon);
        if (icon.isNull()){
            qWarning() << "Icon is null for path: " << itemData.icon;
        }
        return icon;
    }

    return QVariant();
}

void ObjectTreeModel::loadFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл:" << filePath;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Неверный формат JSON в файле:" << filePath;
        return;
    }

    beginResetModel();  // сбрасываем модель перед загрузкой новых данных
    _itemData.clear();  // очищаем данные

    QObject* parentObj = _rootItem;  // начинаем с корня
    loadItems(doc.object(), parentObj);

    endResetModel();  // восстанавливаем модель
}

void ObjectTreeModel::loadItems(const QJsonObject &jsonObj, QObject *parentObj) {
  // Создаем новый элемент и добавляем его в родителя
  QString name = jsonObj["name"].toString();
  QString description = jsonObj["description"].toString();
  QString icon = jsonObj["icon"].toString(); // Получаем путь к иконке
  beginInsertRows(createIndex(parentObj->children().count(), 0, parentObj), parentObj->children().count(), parentObj->children().count());
  QObject *newItem = new QObject(parentObj);
  _itemData[newItem] = {name, description, icon};
  endInsertRows();

  if (jsonObj.contains("children") && jsonObj["children"].isArray()) {
      QJsonArray children = jsonObj["children"].toArray();
      for (const QJsonValue &childVal : children) {
          loadItems(childVal.toObject(), newItem);
      }
  }
}

void ObjectTreeModel::saveToFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл:" << filePath;
        return;
    }

    QJsonObject rootObj;

    // Получаем первого ребенка корня, если он существует
    QObject *firstChild = _rootItem->children().isEmpty() ? nullptr : _rootItem->children().at(0);
    if (firstChild) {
        saveItems(rootObj, firstChild); // Передаем первого ребенка
    }

    QJsonDocument doc(rootObj);
    file.write(doc.toJson());
    file.close();
}


void ObjectTreeModel::saveItems(QJsonObject &jsonObj, QObject *parentObj) { //добавить условие что если родитель не корень то поля 152 153 154 не выполняется
    const TreeItemData &itemData = _itemData[parentObj];
    //if (_itemData.contains(parentObj)) {
    if (parentObj != _rootItem) { //не добавляется description
    jsonObj["name"] = itemData.name; // Имя корневого объекта (пустое, если это корень)
    jsonObj["description"] = itemData.description;
    jsonObj["icon"] = itemData.icon;
    }

    QJsonArray childrenArray;
    for (QObject * child : parentObj->children()){
        QJsonObject childObj;
        //if (child!=_rootItem)
        saveItems(childObj, child);
        childrenArray.append(childObj);
    }

    if (!childrenArray.isEmpty()){ //&& parentObj->parent()==_rootItem) {
        jsonObj["children"] = childrenArray; // это хуйня перезаписывается
    }
}
void ObjectTreeModel::updateItemIcon(const QModelIndex &index, const QString &newIconPath) {
    if (!index.isValid())
        return;

    QObject *obj = objByIndex(index);
    if (_itemData.contains(obj)) {
        _itemData[obj].icon = newIconPath; // Обновляем путь к иконке
        emit dataChanged(index, index, {Qt::DecorationRole}); // Уведомляем о изменении данных
    }
}
void ObjectTreeModel::updateItemDescription(const QModelIndex &index, const QString &newDescription) {
    if (!index.isValid())
        return;

    QObject *obj = objByIndex(index);
    if (_itemData.contains(obj)) {
        _itemData[obj].description = newDescription; // Обновляем описание
        emit dataChanged(index, index, {Qt::UserRole}); // Уведомляем о изменении данных
    }
}
void ObjectTreeModel::removeItem(const QModelIndex &index) {
    if (!index.isValid())
        return;

    QObject *itemToRemove = objByIndex(index);
    QObject *parentObj = itemToRemove->parent();
    int row = parentObj->children().indexOf(itemToRemove);

    beginRemoveRows(index.parent(), row, row);
    _itemData.remove(itemToRemove);  // Удаляем элемент из данных
    delete itemToRemove;  // Удаляем объект
    endRemoveRows();
}





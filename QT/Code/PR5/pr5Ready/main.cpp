#include "customitem.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;
    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    scene.setSceneRect(0, 0, 800, 600);
    layout->addWidget(&view);

    QListWidget *itemList = new QListWidget();
    layout->addWidget(itemList);

    QPushButton *addButton = new QPushButton("Добавить объект");
    QPushButton *removeButton = new QPushButton("Удалить объект");
    QPushButton *editButton = new QPushButton("Редактировать объект");
    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(editButton);

    QObject::connect(addButton, &QPushButton::clicked, [&]() {
        bool ok;
        QString type = QInputDialog::getItem(nullptr, "Тип объекта", "Выберите тип объекта:", {"Текст", "Квадрат", "Изображение"}, 0, false, &ok);
        if (!ok) return;

        if (type == "Текст") {
            QString text = QInputDialog::getText(nullptr, "Введите текст", "Текст для объекта:", QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                CustomItem *item = new CustomItem(text);
                item->setPos(0, scene.items().count() * 60);
                scene.addItem(item);
                itemList->addItem(new QListWidgetItem(text));
            }
        } else if (type == "Квадрат") {
            QColor color = QColorDialog::getColor(Qt::green, nullptr, "Выберите цвет квадрата");
            if (color.isValid()) {
                CustomItem *item = new CustomItem(color);
                item->setPos(100, scene.items().count() * 60);
                scene.addItem(item);
                itemList->addItem(new QListWidgetItem("Квадрат"));
            }
        } else if (type == "Изображение") {
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Выберите изображение", FILEPATH, "Images (*.png *.jpg *.bmp)");
            if (!fileName.isEmpty()) {
                CustomItem *item = new CustomItem(fileName, true);
                item->setPos(200, scene.items().count() * 60);
                scene.addItem(item);
                itemList->addItem(new QListWidgetItem("Изображение"));
            }
        }
    });

    QObject::connect(removeButton, &QPushButton::clicked, [&]() {
        QList<QGraphicsItem*> selectedItems = scene.selectedItems();
        for (QGraphicsItem* item : selectedItems) {
            scene.removeItem(item);
            delete item; // Освобождаем память
        }
        // Удаление из списка
        QList<QListWidgetItem*> selectedListItems = itemList->selectedItems();
        for (QListWidgetItem* listItem : selectedListItems) {
            delete listItem;
        }
    });
    QObject::connect(editButton, &QPushButton::clicked, [&]() {
        QListWidgetItem* currentItem = itemList->currentItem();
        if (!currentItem) return;

        int index = itemList->row(currentItem);
        if (index < scene.items().count()) {
            QGraphicsItem* graphicsItem = scene.items().at(index);
            if (auto customItem = dynamic_cast<CustomItem*>(graphicsItem)) {
                if (customItem->isText) {
                    QString newText = QInputDialog::getText(nullptr, "Редактировать текст", "Новый текст:", QLineEdit::Normal, customItem->text);
                    if (!newText.isEmpty()) {
                        customItem->text = newText;
                        currentItem->setText(newText);
                        customItem->update();
                    }
                }
                else if (customItem->isColor) {
                    QColor newColor = QColorDialog::getColor(customItem->color, nullptr, "Выберите новый цвет");
                    if (newColor.isValid()) {
                        customItem->color = newColor;
                        customItem->update();
                    }
                }
                view.update();
            }
        }
    });


    QObject::connect(itemList, &QListWidget::itemClicked, [&](QListWidgetItem* item) {
        int index = itemList->row(item);
        if (index < scene.items().count()) {
            QGraphicsItem* graphicsItem = scene.items().at(index);
            graphicsItem->setSelected(true);
        }
    });

    mainWindow.setWindowTitle("Динамическая сцена");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}

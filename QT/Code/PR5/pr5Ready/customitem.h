#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>

#define FILEPATH "/Users/nikitamakhov/Documents/pr5Ready/icons"

class CustomItem : public QGraphicsItem {
public:
    CustomItem(QString imagePath, bool isImage);
    CustomItem(QString text);            // Для текста
    CustomItem(QColor color);            // Для цвета

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString text;
    QColor color;
    QImage image;
    bool isText;
    bool isColor;
    bool isImage;
};

#endif // CUSTOMITEM_H

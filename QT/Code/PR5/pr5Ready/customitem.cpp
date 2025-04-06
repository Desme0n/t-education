#include "customitem.h"
#include <QFile>
#include <QPainter>

CustomItem::CustomItem(QString text)
    : text(text), isText(true), isColor(false), isImage(false) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

CustomItem::CustomItem(QColor color)
    : color(color), isText(false), isColor(true), isImage(false) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

CustomItem::CustomItem(QString imagePath, bool isImage)
    : isText(false), isColor(false), isImage(true) {
    if (QFile::exists(imagePath)) {
        image.load(imagePath);
        image = image.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        qWarning() << "Файл не найден:" << imagePath;
    }
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}
QRectF CustomItem::boundingRect() const {
    if (isText) {
        return QRectF(-5, -5, 110, 60);
    } else if (isColor) {
        return QRectF(-10, -10, 70, 70);
    } else if (isImage) {
        return QRectF(-10, -10, image.width() + 20, image.height() + 20);
    }
    return QRectF();
}

void CustomItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (isText) {
        painter->drawText(boundingRect(), Qt::AlignCenter, text);
    } else if (isColor) {
        painter->fillRect(boundingRect().adjusted(5, 5, -5, -5), color);
    } else if (isImage) {
        painter->drawImage(0, 0, image);
    }

    // Рисуем выделение с закругленными углами
    if (isSelected()) {
        QPen pen(Qt::cyan, 2, Qt::DashDotDotLine);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(boundingRect(), 10, 10);
    }
}


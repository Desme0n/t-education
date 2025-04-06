#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "objecttreemodel.h"
#include <QtWidgets>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QMainWindow>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QSplitter>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ObjectTreeModel* _model;
    QTreeView *treeView;
    QLineEdit *lineEdit;
    QTextEdit *descriptionView;
    QLabel *labelForDescription;
    QPushButton *addButton;
    QPushButton *exitButton;
    QPushButton *fileButton;
    QPushButton *deleteButton;
    QPushButton *updateIconButton;
    QPushButton *saveChangesButton;
    QLabel *fileLabel;
    QString _currentIconPath;
private slots:
    void onAddButtonClicked();
    void onTreeItemSelected(const QModelIndex &current, const QModelIndex &previous);
    void onExitButtonClicked();
    void onUpdateIconButtonClicked();
    void onSaveChangesButtonClicked();
    void onDeleteButtonClicked();


};

#endif // MAINWINDOW_H

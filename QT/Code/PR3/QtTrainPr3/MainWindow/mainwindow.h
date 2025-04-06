
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadJsonData();
    void onDirectionChanged(int row);
    void onGroupChanged(int row);
    void addButtonClicked();
    void onStudentClicked(QListWidgetItem *item);
    void addStudent();
    void addGroup();
    void addDirection();
    void removeStudent();
    void removeGroup();
    void removeDirection();
    void removeButtonClicked();


private:
    QListWidget *directionsList;
    QListWidget *groupsList;
    QListWidget *studentsList;
    QJsonObject jsonData; // Объект для хранения данных JSON
};

#endif // MAINWINDOW_H

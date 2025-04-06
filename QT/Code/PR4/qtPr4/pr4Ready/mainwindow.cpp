#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создаем центральный виджет и основной лейаут
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);    // Основной лейаут

    // Лейауты для левой и правой частей
    QVBoxLayout *LayoutLeft = new QVBoxLayout();                 // Левая половина экрана
    QVBoxLayout *LayoutRight = new QVBoxLayout();                // Правая половина экрана
    QVBoxLayout *LayoutExit = new QVBoxLayout();
    QSplitter *splitter = new QSplitter(Qt::Vertical, centralWidget);

    // ЛЕВАЯ ЧАСТЬ: Виджет TreeView и ввод данных
    QLabel *labelTreeView = new QLabel("Космос");
    LayoutLeft->addWidget(labelTreeView);

    treeView = new QTreeView;
    LayoutLeft->addWidget(treeView);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    lineEdit = new QLineEdit;
    addButton = new QPushButton("Добавить объект");
    deleteButton = new QPushButton("Удалить объект");
    inputLayout->addWidget(lineEdit);
    inputLayout->addWidget(addButton);
    inputLayout->addWidget(deleteButton);
    LayoutLeft->addLayout(inputLayout);

    // ЛЕВАЯ ЧАСТЬ: Виджеты для выбора иконки
    fileButton = new QPushButton("Выбрать изображение");
    fileLabel = new QLabel("Нет выбранного файла");
    updateIconButton = new QPushButton("Обновить иконку");



    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->addWidget(fileButton);
    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(updateIconButton);

    LayoutLeft->addLayout(inputLayout);

    // Подключаем сигнал для удаления
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
    LayoutLeft->addLayout(fileLayout); // Добавляем выбор иконки в левую часть интерфейса

    // Добавляем левую часть в основной лейаут
    mainLayout->addLayout(LayoutLeft);

    // ПРАВАЯ ЧАСТЬ: Описание и кнопки управления
    labelForDescription = new QLabel("Описание");
    LayoutRight->addWidget(labelForDescription);

    descriptionView = new QTextEdit();
    LayoutRight->addWidget(descriptionView);

    saveChangesButton = new QPushButton("Сохранить изменения");
    LayoutRight->addWidget(saveChangesButton); // Добавляем кнопку в правую часть

    // Добавляем правую часть в основной лейаут
    mainLayout->addLayout(LayoutRight);

    // Кнопка "Выйти" в основной лейаут (располагается отдельно)
    exitButton = new QPushButton("Выйти");
    LayoutExit->addWidget(splitter);
    LayoutExit->addWidget(exitButton);
    mainLayout->addLayout(LayoutExit);

    // Устанавливаем центральный виджет
    setCentralWidget(centralWidget);

    // Подключаем сигналы и слоты
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(updateIconButton, &QPushButton::clicked, this, &MainWindow::onUpdateIconButtonClicked);
    connect(fileButton, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "/Users", "Images (*.png *.xpm *.jpg)");
        if (!fileName.isEmpty()) {
            fileLabel->setText("Выбранный файл: " + fileName);
            _currentIconPath = fileName;
        }
    });
    connect(saveChangesButton, &QPushButton::clicked, this, &MainWindow::onSaveChangesButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);

    // Модель и дерево объектов
    _model = new ObjectTreeModel(this);
    QStringList cols;
    cols << "Name" << "Description";
    _model->setColumns(cols);

    _model->loadFromFile("/Users/nikitamakhov/Documents/qtPr4/pr4Ready/base.json");
    treeView->setModel(_model);

    // Обработка выбора элемента в TreeView
    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onTreeItemSelected);
}


MainWindow::~MainWindow()
{}

void MainWindow::onAddButtonClicked()
{
    if (lineEdit->text().isEmpty() || descriptionView->toPlainText().isEmpty()) return;

    QString iconPath = _currentIconPath.isEmpty()
                           ? "/Users/nikitamakhov/Documents/qtPr4/pr4Ready/icons/unknown planet.png"
                           : _currentIconPath;

    // Добавление элемента с выбранной иконкой
    _model->addItem(lineEdit->text(), descriptionView->toPlainText(), iconPath, treeView->currentIndex());

    lineEdit->clear();
    descriptionView->clear();
    _currentIconPath.clear();
}

void MainWindow::onExitButtonClicked() {
    _model->saveToFile("/Users/nikitamakhov/Documents/qtPr4/pr4Ready/base.json");
    close();
}
void MainWindow::onTreeItemSelected(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    // Получаем описание выбранного элемента и устанавливаем его в descriptionView
    QString description = _model->data(current, Qt::UserRole).toString();
    descriptionView->setText(description);
}
void MainWindow::onUpdateIconButtonClicked() {
    QModelIndex currentIndex = treeView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите элемент для обновления иконки.");
        return;
    }

    if (_currentIconPath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала выберите изображение.");
        return;
    }
    _model->updateItemIcon(currentIndex, _currentIconPath);
}
void MainWindow::onSaveChangesButtonClicked() {
    QModelIndex currentIndex = treeView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите элемент для сохранения изменений.");
        return;
    }

    QString newDescription = descriptionView->toPlainText();
    _model->updateItemDescription(currentIndex, newDescription);
}


void MainWindow::onDeleteButtonClicked() {
    QModelIndex currentIndex = treeView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите элемент для удаления.");
        return;
    }

    // Удаление элемента из модели
    _model->removeItem(currentIndex);

    // Сохранение изменений
    _model->saveToFile("/Users/nikitamakhov/Documents/qt/ShablonPodPr4/base.json");
}



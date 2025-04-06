QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Укажите пути к исходным файлам
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h


# Дополнительные настройки компиляции, если нужно
# Например, для использования C++11:
CONFIG += c++17

# Укажите путь к файлу ресурсов, если он есть
# RESOURCES += resources.qrc

# Добавьте зависимости, если нужны дополнительные библиотеки или модули

# Default rules for deployment.

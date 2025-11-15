#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "regwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Подключаем SQLite базу
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть базу данных.");
        return -1;
    }
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)");
    RegWindow w;
    w.show();
    return a.exec();
}

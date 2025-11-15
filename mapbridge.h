#ifndef MAPBRIDGE_H
#define MAPBRIDGE_H

#include <QObject>
#include <QVariantList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

class MapBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged)

public:
    explicit MapBridge(QObject *parent = nullptr);

    QString getUsername() const;                // Добавляем getter
public slots:

    void setUsername(const QString &username); // Лучше сделать слотом

    // Установка расстояния (например, для отображения на UI)
    void setDistance(const QString &distance);

    // Сохраняет маршрут в JSON файл
    void saveLastRoute(const QVariantList &points);

    // Загружает маршрут из JSON файла
    void getLastRoute();

signals:
    // Сигнал для отображения расстояния в интерфейсе
    void distanceChanged(QString distance);

    // Сигнал, отправляющий маршрут в JS после загрузки
    void lastRouteLoaded(QVariantList points);

    // Сигнал при изменении имени пользователя
    void usernameChanged(const QString &username);

private:
    QString currentUsername;
};

#endif // MAPBRIDGE_H

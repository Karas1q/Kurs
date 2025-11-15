#include "mapbridge.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

MapBridge::MapBridge(QObject *parent) : QObject(parent) {}

QString MapBridge::getUsername() const {
    return currentUsername;
}

void MapBridge::setUsername(const QString &username) {
    if (currentUsername != username) {
        currentUsername = username;
        emit usernameChanged(username);
    }
}

void MapBridge::setDistance(const QString &distance) {
    emit distanceChanged(distance);
}

void MapBridge::saveLastRoute(const QVariantList &points) {
    QJsonArray jsonArray;

    for (const QVariant &point : points) {
        QVariantList coords = point.toList();
        if (coords.size() == 2) {
            QJsonArray coordArray;
            coordArray.append(coords[0].toDouble());
            coordArray.append(coords[1].toDouble());
            jsonArray.append(coordArray);
        }
    }

    QString filename = currentUsername + "Last.json";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(QJsonDocument(jsonArray).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Маршрут сохранён в" << filename;
    } else {
        qWarning() << "Не удалось открыть файл для записи маршрута:" << filename;
    }
}

void MapBridge::getLastRoute() {
    QString filename = currentUsername + "Last.json";
    QFile file(filename);
    QVariantList result;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();
            for (const QJsonValue &val : jsonArray) {
                if (val.isArray()) {
                    QJsonArray coords = val.toArray();
                    if (coords.size() == 2) {
                        QVariantList point;
                        point << coords[0].toDouble() << coords[1].toDouble();
                        result << QVariant(point);
                    }
                }
            }
        }
    } else {
        qWarning() << "Не удалось открыть файл для чтения маршрута:" << filename;
    }

    emit lastRouteLoaded(result);
}

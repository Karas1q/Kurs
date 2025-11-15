#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include "mapbridge.h"

class MenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWindow(const QString &username, bool isAdmin, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onUsernameChanged(const QString &username);
    void onCallTaxiClicked();
    void onProfileClicked();
    void onAdminClicked();
    void onOtzivClicked();

private:
    void setupRoleUI();
    QString m_username;
    bool m_isAdmin;

    QPushButton *callTaxiButton;
    QPushButton *profileButton;
    QPushButton *adminButton;
    QPushButton *otzivButton;
    QPushButton *exitButton;
    MapBridge *mapbridge;
    QPixmap backgroundPixmap;
};

#endif // MENUWINDOW_H

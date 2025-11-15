#include "menuwindow.h"
#include "profilewindow.h"
#include "mainwindow.h"
#include "adminmenuwindow.h"
#include "otzivwindow.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QApplication>
#include "mapbridge.h"

MenuWindow::MenuWindow(const QString &username, bool isAdmin, QWidget *parent)
    : QWidget(parent), m_username(username), m_isAdmin(isAdmin)
{
    MapBridge* mapbridge = new MapBridge(this);
    connect(mapbridge, &MapBridge::usernameChanged, this, &MenuWindow::onUsernameChanged);
    mapbridge->setUsername(m_username);
    setWindowTitle("Меню");
    resize(400, 350);

    backgroundPixmap = QPixmap(":/icons/background.jpg");
    callTaxiButton = new QPushButton("Меню вызова такси", this);
    profileButton = new QPushButton("Личный кабинет", this);
    adminButton = new QPushButton("Кабинет администратора", this);
    otzivButton = new QPushButton("Оставить отзыв", this);
    exitButton = new QPushButton("Выход", this);

    for (QPushButton *btn : {callTaxiButton, profileButton, adminButton, otzivButton, exitButton}) {
        btn->setFixedWidth(200);
        btn->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(callTaxiButton, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(profileButton, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(adminButton, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(otzivButton, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(exitButton, 0, Qt::AlignCenter);
    layout->addStretch();

    setLayout(layout);

    connect(callTaxiButton, &QPushButton::clicked, this, &MenuWindow::onCallTaxiClicked);
    connect(profileButton, &QPushButton::clicked, this, &MenuWindow::onProfileClicked);
    connect(adminButton, &QPushButton::clicked, this, &MenuWindow::onAdminClicked);
    connect(otzivButton, &QPushButton::clicked, this, &MenuWindow::onOtzivClicked);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    setupRoleUI();
}

void MenuWindow::setupRoleUI()
{
    if (m_isAdmin) {
        profileButton->setEnabled(false);
        profileButton->setStyleSheet("background-color: rgba(180, 180, 180, 200); font-weight: bold; font-size: 14px;");

        adminButton->setEnabled(true);
        adminButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");
    } else {
        profileButton->setEnabled(true);
        profileButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");

        adminButton->setEnabled(false);
        adminButton->setStyleSheet("background-color: rgba(180, 180, 180, 200); font-weight: bold; font-size: 14px;");
    }
}

void MenuWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundPixmap);
    QWidget::paintEvent(event);
}

void MenuWindow::onCallTaxiClicked()
{
    MainWindow *main = new MainWindow(m_username,this);
    main->show();
    this->close();
}

void MenuWindow::onProfileClicked()
{
    if (!m_isAdmin) {
        ProfileWindow *profile = new ProfileWindow(m_username, this);
        profile->show();
        this->close();
    }
}

void MenuWindow::onAdminClicked()
{
    AdminMenuWindow *adminMenu = new AdminMenuWindow(this);
    adminMenu->show();
    this->close();
}

void MenuWindow::onOtzivClicked()
{
    OtzivWindow *otziv = new OtzivWindow(m_username,m_isAdmin,this);
    otziv->show();
    this->close();
}
void MenuWindow::onUsernameChanged(const QString &username)
{
    qDebug() << "Сигнал usernameChanged сработал! Имя пользователя:" << username;
    // Можешь добавить любую дополнительную обработку
}

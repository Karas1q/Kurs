#include "adminmenuwindow.h"
#include "adminwindow.h"
#include "checkwindow.h"
#include "menuwindow.h"
#include <QVBoxLayout>
#include <QPainter>

AdminMenuWindow::AdminMenuWindow(MenuWindow *menuWindow, QWidget *parent)
    : QWidget(parent), menuWindow(menuWindow)
{
    setWindowTitle("Кабинет администратора");
    resize(400, 350);

    backgroundPixmap = QPixmap(":/icons/background.jpg");

    openAdminWindowButton = new QPushButton("Изменение тарифов", this);
    openCheckWindowButton = new QPushButton("Отзывы", this);  // Новая кнопка
    backButton = new QPushButton("Назад", this);

    openAdminWindowButton->setFixedWidth(200);
    openCheckWindowButton->setFixedWidth(200);  // Устанавливаем ширину новой кнопки
    backButton->setFixedWidth(200);

    QString buttonStyle = "font-weight: bold; font-size: 14px;";
    openAdminWindowButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); " + buttonStyle);
    openCheckWindowButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); " + buttonStyle);
    backButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); " + buttonStyle);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(openAdminWindowButton, 0, Qt::AlignCenter);
    layout->addSpacing(15);
    layout->addWidget(openCheckWindowButton, 0, Qt::AlignCenter);  // Добавляем кнопку в макет
    layout->addSpacing(15);
    layout->addWidget(backButton, 0, Qt::AlignCenter);
    layout->addStretch();

    setLayout(layout);

    connect(openAdminWindowButton, &QPushButton::clicked, this, &AdminMenuWindow::onOpenAdminWindowClicked);
    connect(openCheckWindowButton, &QPushButton::clicked, this, &AdminMenuWindow::onOpenCheckWindowClicked);
    connect(backButton, &QPushButton::clicked, this, &AdminMenuWindow::onBackClicked);
}

void AdminMenuWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundPixmap);
    QWidget::paintEvent(event);
}

void AdminMenuWindow::onOpenAdminWindowClicked()
{
    AdminWindow *admin = new AdminWindow(this);
    admin->show();
    this->close();
}

void AdminMenuWindow::onOpenCheckWindowClicked()
{
    CheckWindow *check = new CheckWindow(this);  // Открытие окна CheckWindow
    check->show();
    this->close();
}

void AdminMenuWindow::onBackClicked()
{
    if (menuWindow) {
        menuWindow->show();
    }
    this->close();
}

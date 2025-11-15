#include "profilewindow.h"
#include "menuwindow.h"  // для использования MenuWindow
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPainter>
#include <QCryptographicHash>

ProfileWindow::ProfileWindow(const QString &username, MenuWindow *menuWnd, QWidget *parent)
    : QWidget(parent), currentUsername(username), menuWindow(menuWnd)
{
    setWindowTitle("Личный кабинет");
    resize(400, 300);

    backgroundPixmap = QPixmap(":/icons/background.jpg");

    // Кнопка назад
    backButton = new QPushButton("Назад", this);
    backButton->setFixedSize(40, 30);
    backButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 255, 255, 180);
            border: 1px solid #aaa;
            border-radius: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: rgba(245, 245, 245, 220);
        }
    )");
    connect(backButton, &QPushButton::clicked, this, &ProfileWindow::onBackClicked);

    // Layout для кнопки назад в правом верхнем углу
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 18, 0, 0);  // отступ сверху
    topLayout->addStretch();
    topLayout->addWidget(backButton);

    // Все остальные элементы
    QLabel *userLabel = new QLabel("Пользователь: " + currentUsername, this);
    userLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: black;");

    QLabel *oldPassLabel = new QLabel("Старый пароль:", this);
    oldPassLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: black;");
    oldPasswordEdit = new QLineEdit(this);
    oldPasswordEdit->setEchoMode(QLineEdit::Password);
    oldPasswordEdit->setFixedWidth(280);
    oldPasswordEdit->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");

    QLabel *newPassLabel = new QLabel("Новый пароль:", this);
    newPassLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: black;");
    newPasswordEdit = new QLineEdit(this);
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit->setFixedWidth(280);
    newPasswordEdit->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");

    changePasswordButton = new QPushButton("Изменить пароль", this);
    changePasswordButton->setFixedWidth(160);
    changePasswordButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");
    connect(changePasswordButton, &QPushButton::clicked, this, &ProfileWindow::onChangePasswordClicked);

    // Layout для центрированного контента
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(userLabel, 0, Qt::AlignCenter);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(oldPassLabel, 0, Qt::AlignCenter);
    centerLayout->addWidget(oldPasswordEdit, 0, Qt::AlignCenter);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(newPassLabel, 0, Qt::AlignCenter);
    centerLayout->addWidget(newPasswordEdit, 0, Qt::AlignCenter);
    centerLayout->addSpacing(5);
    centerLayout->addWidget(changePasswordButton, 0, Qt::AlignCenter);

    // Главный layout: сначала кнопка назад, затем центрированный контент
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void ProfileWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundPixmap);
    QWidget::paintEvent(event);
}

void ProfileWindow::onChangePasswordClicked()
{
    QString oldPass = oldPasswordEdit->text();
    QString newPass = newPasswordEdit->text();

    if (oldPass.isEmpty() || newPass.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите старый и новый пароль.");
        return;
    }

    // Хеширование паролей
    QByteArray oldHash = QCryptographicHash::hash(oldPass.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray newHash = QCryptographicHash::hash(newPass.toUtf8(), QCryptographicHash::Sha256).toHex();

    // Подтверждение
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Подтверждение",
        "Вы уверены, что хотите изменить пароль?",
        QMessageBox::Yes | QMessageBox::No
        );
    if (reply != QMessageBox::Yes)
        return;

    // Проверка старого пароля
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(currentUsername);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Ошибка", "Пользователь не найден.");
        return;
    }

    QString storedPassword = query.value(0).toString();
    if (storedPassword != QString(oldHash)) {
        QMessageBox::warning(this, "Ошибка", "Старый пароль неверен.");
        return;
    }

    // Обновление пароля
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE users SET password = ? WHERE username = ?");
    updateQuery.addBindValue(QString(newHash));
    updateQuery.addBindValue(currentUsername);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось изменить пароль.");
        return;
    }

    QMessageBox::information(this, "Готово", "Пароль успешно изменён.");
    oldPasswordEdit->clear();
    newPasswordEdit->clear();
}


void ProfileWindow::onBackClicked()
{
    if (menuWindow) {
        menuWindow->show();
    }
    this->close();
}

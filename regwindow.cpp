#include "regwindow.h"
#include "menuwindow.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCryptographicHash>

RegWindow::RegWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Регистрация / Вход");
    resize(400, 250);

    backgroundPixmap = QPixmap(":/icons/background.jpg");

    QLabel *usernameLabel = new QLabel("Логин:", this);
    usernameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setFixedWidth(280);
    usernameLineEdit->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");

    QLabel *passwordLabel = new QLabel("Пароль:", this);
    passwordLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setFixedWidth(280);
    passwordLineEdit->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");

    // Кнопка показать/скрыть пароль под полем
    togglePasswordButton = new QToolButton(this);
    togglePasswordButton->setText("Показать");
    togglePasswordButton->setCursor(Qt::PointingHandCursor);
    togglePasswordButton->setStyleSheet(
        "background-color: rgba(255, 255, 200, 230);"
        "font-weight: bold;"
        "font-size: 14px;"
        "border: 1px solid #ccc;"
        "border-radius: 4px;"
        "padding: 3px 8px;"
        );
    togglePasswordButton->setFixedHeight(30);
    togglePasswordButton->setFixedWidth(100);

    connect(togglePasswordButton, &QToolButton::clicked, this, &RegWindow::on_togglePasswordButton_clicked);

    registerButton = new QPushButton("Зарегистрироваться", this);
    loginButton = new QPushButton("Войти", this);

    registerButton->setFixedWidth(160);
    loginButton->setFixedWidth(160);

    QString buttonStyle = "background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;";
    registerButton->setStyleSheet(buttonStyle);
    loginButton->setStyleSheet(buttonStyle);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(5);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(loginButton);

    QWidget *buttonContainer = new QWidget(this);
    buttonContainer->setLayout(buttonLayout);
    buttonContainer->setFixedWidth(340);
    buttonContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(usernameLabel, 0, Qt::AlignCenter);
    layout->addWidget(usernameLineEdit, 0, Qt::AlignCenter);
    layout->addSpacing(8);
    layout->addWidget(passwordLabel, 0, Qt::AlignCenter);
    layout->addWidget(passwordLineEdit, 0, Qt::AlignCenter);
    layout->addWidget(togglePasswordButton, 0, Qt::AlignCenter);  // Кнопка под полем
    layout->addSpacing(8);
    layout->addWidget(buttonContainer, 0, Qt::AlignCenter);
    layout->addStretch();

    setLayout(layout);

    connect(registerButton, &QPushButton::clicked, this, &RegWindow::on_registerButton_clicked);
    connect(loginButton, &QPushButton::clicked, this, &RegWindow::on_loginButton_clicked);
}

RegWindow::~RegWindow() {}

void RegWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundPixmap);
    QDialog::paintEvent(event);
}

void RegWindow::on_togglePasswordButton_clicked()
{
    if (passwordLineEdit->echoMode() == QLineEdit::Password) {
        passwordLineEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordButton->setText("Скрыть");
    } else {
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        togglePasswordButton->setText("Показать");
    }
}

void RegWindow::on_registerButton_clicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль.");
        return;
    }

    if (password.length() < 5) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен содержать минимум 5 символов.");
        return;
    }

    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    checkQuery.addBindValue(username);
    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Ошибка при проверке базы данных.");
        return;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::information(this, "Уже существует", "Пользователь с таким логином уже зарегистрирован.");
        return;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    insertQuery.addBindValue(username);
    insertQuery.addBindValue(hash);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить пользователя.");
        return;
    }

    QMessageBox::information(this, "Готово", "Вы успешно зарегистрированы!");
}


void RegWindow::on_loginButton_clicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

    if (username.isEmpty() || hash.toHex().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);
    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка", "Ошибка при обращении к базе данных.");
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == hash.toHex()) {
            // Сравнение с заранее заданным хешем для admin
            QByteArray adminHash = QCryptographicHash::hash("admin", QCryptographicHash::Sha256).toHex();
            bool isAdmin = (username == "admin" && storedPassword == adminHash);

            QMessageBox::information(this, "Успешный вход", "Добро пожаловать!");

            MenuWindow *menu = new MenuWindow(username, isAdmin);
            menu->show();

            accept();
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный пароль.");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден.");
    }
}

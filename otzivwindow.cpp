#include "otzivwindow.h"
#include "menuwindow.h"
#include <QMessageBox>
#include <QSettings>
#include <QDateTime>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>

OtzivWindow::OtzivWindow(const QString &username, bool isAdmin, MenuWindow *menuWindow, QWidget *parent)
    : QWidget(parent), m_username(username), m_isAdmin(isAdmin), m_menuWindow(menuWindow)
{
    setWindowTitle("Отзывы");
    resize(400, 300);
    backgroundPixmap = QPixmap(":/icons/background.jpg");

    QLabel *label = new QLabel("Напишите ваш отзыв:", this);
    label->setStyleSheet("font-weight: bold; font-size: 14px;");

    reviewEdit = new QTextEdit(this);
    reviewEdit->setFixedWidth(320);  // Оставляем ширину 320
    reviewEdit->setFixedHeight(100);  // Уменьшаем высоту до 80
    reviewEdit->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");

    saveButton = new QPushButton("Сохранить", this);
    backButton = new QPushButton("Назад", this);

    QString buttonStyle = "font-weight: bold; font-size: 14px;";
    saveButton->setStyleSheet("background-color: green; color: white; " + buttonStyle);
    backButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); " + buttonStyle);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label, 0, Qt::AlignCenter);
    mainLayout->addWidget(reviewEdit, 0, Qt::AlignCenter);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(backButton);
    buttonLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(saveButton, &QPushButton::clicked, this, &OtzivWindow::onSaveClicked);
    connect(backButton, &QPushButton::clicked, this, &OtzivWindow::onBackClicked);
}

void OtzivWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundPixmap);
    QWidget::paintEvent(event);
}

void OtzivWindow::onSaveClicked()
{
    QString reviewText = reviewEdit->toPlainText().trimmed();
    if (reviewText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Отзыв не может быть пустым.");
        return;
    }

    QSettings settings(m_username + ".ini", QSettings::IniFormat);
    settings.beginGroup("Reviews");
    QString currentDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    settings.setValue(currentDateTime, reviewText);
    settings.endGroup();
    settings.sync();

    QMessageBox::information(this, "Сохранено", "Спасибо за ваш отзыв!");
    reviewEdit->clear();
}

void OtzivWindow::onBackClicked()
{
    if (m_menuWindow) {
        m_menuWindow->show();
    }
    this->close();
}

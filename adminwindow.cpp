#include "adminwindow.h"
#include "adminmenuwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>

AdminWindow::AdminWindow(AdminMenuWindow *adminMenuWindow, QWidget *parent)
    : QWidget(parent), adminMenuWindow(adminMenuWindow)
{
    setWindowTitle("Изменение тарифов");
    resize(400, 300);
    backgroundPixmap = QPixmap(":/icons/background.jpg");

    // --- Кнопка Назад ---
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
    connect(backButton, &QPushButton::clicked, this, &AdminWindow::onBackClicked);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 20, 0, 0);
    topLayout->addStretch();
    topLayout->addWidget(backButton);

    // --- Все остальные элементы ---
    QLabel *titleLabel = new QLabel("Изменение тарифов:", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: black;");

    QLabel *economLabel = new QLabel("Эконом (BYN/км):", this);
    QLabel *comfortLabel = new QLabel("Комфорт (BYN/км):", this);
    QLabel *businessLabel = new QLabel("Бизнес (BYN/км):", this);

    QFont labelFont;
    labelFont.setBold(true);
    economLabel->setFont(labelFont);
    comfortLabel->setFont(labelFont);
    businessLabel->setFont(labelFont);

    tariffEconom = new QLineEdit("0.6", this);
    tariffComfort = new QLineEdit("1.0", this);
    tariffBusiness = new QLineEdit("1.5", this);

    for (auto edit : {tariffEconom, tariffComfort, tariffBusiness}) {
        edit->setFixedWidth(280);
        edit->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");
    }

    saveButton = new QPushButton("Сохранить", this);
    saveButton->setFixedSize(160, 20);
    saveButton->setStyleSheet("background-color: green; color: white; font-weight: bold; font-size: 14px;");
    connect(saveButton, &QPushButton::clicked, this, &AdminWindow::onSaveClicked);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    centerLayout->addWidget(economLabel, 0, Qt::AlignCenter);
    centerLayout->addWidget(tariffEconom, 0, Qt::AlignCenter);
    centerLayout->addWidget(comfortLabel, 0, Qt::AlignCenter);
    centerLayout->addWidget(tariffComfort, 0, Qt::AlignCenter);
    centerLayout->addWidget(businessLabel, 0, Qt::AlignCenter);
    centerLayout->addWidget(tariffBusiness, 0, Qt::AlignCenter);
    centerLayout->addWidget(saveButton, 0, Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void AdminWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundPixmap);
    QWidget::paintEvent(event);
}

void AdminWindow::onBackClicked()
{
    if (adminMenuWindow) {
        adminMenuWindow->show();
    }
    this->close();
}

void AdminWindow::onSaveClicked()
{
    bool ok1, ok2, ok3;
    double val1 = tariffEconom->text().toDouble(&ok1);
    double val2 = tariffComfort->text().toDouble(&ok2);
    double val3 = tariffBusiness->text().toDouble(&ok3);

    if (!ok1 || !ok2 || !ok3 || val1 <= 0 || val2 <= 0 || val3 <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения тарифов.");
        return;
    }

    QSettings settings("tariffs.ini", QSettings::IniFormat);
    settings.beginGroup("Tariffs");
    settings.setValue("Econom", val1);
    settings.setValue("Comfort", val2);
    settings.setValue("Business", val3);
    settings.endGroup();
    settings.sync();

    QMessageBox::information(this, "Сохранено", "Тарифы успешно обновлены.");
}

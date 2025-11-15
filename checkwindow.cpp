#include "checkwindow.h"
#include "adminmenuwindow.h"

#include <QPainter>
#include <QSettings>
#include <QFileInfoList>
#include <QTextStream>
#include <QDir>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>

CheckWindow::CheckWindow(AdminMenuWindow *adminMenu, QWidget *parent)
    : QWidget(parent), m_adminMenu(adminMenu)
{
    setWindowTitle("Отзывы пользователей");
    resize(500, 400);  // Размер окна остался прежним

    backgroundPixmap = QPixmap(":/icons/background.jpg");

    textView = new QTextEdit(this);
    textView->setReadOnly(true);
    textView->setFixedHeight(250);  // Уменьшенная высота области отзывов
    textView->setFixedWidth(450);   // Ширина осталась прежней

    textView->setStyleSheet("background-color: rgba(255, 255, 255, 200); font-family: Consolas;");

    backButton = new QPushButton("Назад", this);
    backButton->setStyleSheet("background-color: rgba(255, 255, 200, 230); font-weight: bold; font-size: 14px;");
    backButton->setFixedWidth(120);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addSpacing(30);  // Смещаем textView немного вниз
    layout->addWidget(textView, 0, Qt::AlignCenter);
    layout->addWidget(backButton, 0, Qt::AlignCenter);
    setLayout(layout);

    connect(backButton, &QPushButton::clicked, this, &CheckWindow::onBackClicked);

    loadReviewFiles();
}

void CheckWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundPixmap);
    QWidget::paintEvent(event);
}

void CheckWindow::onBackClicked()
{
    if (m_adminMenu) {
        m_adminMenu->show();
    }
    this->close();
}

void CheckWindow::loadReviewFiles()
{
    QDir dir(QDir::currentPath());
    QStringList filters;
    filters << "*.ini";
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);

    QString result;

    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.fileName();
        if (fileName.toLower() == "tariffs.ini")
            continue;

        QSettings settings(fileInfo.filePath(), QSettings::IniFormat);
        settings.beginGroup("Reviews");
        QStringList keys = settings.allKeys();
        settings.endGroup();

        if (keys.isEmpty())
            continue;

        QString user = fileInfo.baseName();
        result += "----- " + user + " -----\n";

        settings.beginGroup("Reviews");
        for (const QString &key : keys) {
            QString review = settings.value(key).toString();
            result += review + "\n";
        }
        settings.endGroup();

        result += "\n";
    }

    if (result.isEmpty()) {
        textView->setText("Нет отзывов для отображения.");
    } else {
        textView->setText(result);
    }
}

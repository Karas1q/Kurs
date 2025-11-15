#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapbridge.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QWebChannel>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include "menuwindow.h"

MainWindow::MainWindow(const QString &username, MenuWindow *menuWindow, QWidget *parent)
    : QMainWindow(parent)
    , Username(username)
    , ui(new Ui::MainWindow)
    , menuWindow(menuWindow)
{
    ui->setupUi(this);
    setWindowTitle("Заказ такси");
    // --- Загрузка тарифов ---
    QSettings settings("tariffs.ini", QSettings::IniFormat);
    settings.beginGroup("Tariffs");
    double economTariff = settings.value("Econom", 0.6).toDouble();
    double comfortTariff = settings.value("Comfort", 1.0).toDouble();
    double businessTariff = settings.value("Business", 1.5).toDouble();
    currentTariff = settings.value("Econom", 0.6).toDouble();
    settings.endGroup();

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(15);

    // --- WebView ---
    webView = new QWebEngineView(this);
    webView->load(QUrl("qrc:/map.html"));
    webView->setMinimumHeight(500);
    webView->setMaximumHeight(500);
    webView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(webView);
    mainLayout->addSpacing(10);

    // --- Панель управления ---
    QWidget *controlPanel = new QWidget(this);
    controlPanel->setMinimumHeight(220);

    QVBoxLayout *panelLayout = new QVBoxLayout(controlPanel);
    panelLayout->setContentsMargins(20, 15, 20, 20);
    panelLayout->setSpacing(15);

    // --- Заголовок и кнопка Назад ---
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *label = new QLabel("Выберите тариф:");
    QFont labelFont = label->font();
    labelFont.setPointSize(14);
    labelFont.setBold(true);
    label->setFont(labelFont);

    QPushButton *backButton = new QPushButton("Назад", this);
    backButton->setFixedSize(40, 30);
    backButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 255, 200, 230);
            border: 1px solid #aaa;
            border-radius: 10px;
            color: #222;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 200, 255);
        }
    )");
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);

    headerLayout->addWidget(label, 0, Qt::AlignLeft);
    headerLayout->addStretch();
    headerLayout->addWidget(backButton);
    panelLayout->addLayout(headerLayout);

    // --- Кнопки тарифов ---
    QPushButton *btnEconom = new QPushButton(QIcon(":/icons/yellow.png"), "Эконом", this);
    QPushButton *btnComfort = new QPushButton(QIcon(":/icons/white.png"), "Комфорт", this);
    QPushButton *btnBusiness = new QPushButton(QIcon(":/icons/black.png"), "Бизнес", this);

    QSize buttonSize(160, 65);
    QSize iconSize(48, 48);
    QFont btnFont = btnEconom->font();
    btnFont.setPointSize(13);

    QString baseStyle = R"(
        QPushButton {
            background-color: rgba(255, 255, 200, 230);
            border: 2px solid #ccc;
            border-radius: 10px;
            color: #222;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 200, 255);
        }
        QPushButton:checked {
            background-color: rgba(245, 230, 150, 255);
            border-color: #bbb;
            color: #111;
        }
    )";

    for (auto btn : {btnEconom, btnComfort, btnBusiness}) {
        btn->setMinimumSize(buttonSize);
        btn->setIconSize(iconSize);
        btn->setFont(btnFont);
        btn->setCheckable(true);
        btn->setStyleSheet(baseStyle);
    }

    QButtonGroup *tariffGroup = new QButtonGroup(this);
    tariffGroup->addButton(btnEconom);
    tariffGroup->addButton(btnComfort);
    tariffGroup->addButton(btnBusiness);
    tariffGroup->setExclusive(true);
    btnEconom->setChecked(true);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(25);
    btnLayout->addWidget(btnEconom);
    btnLayout->addWidget(btnComfort);
    btnLayout->addWidget(btnBusiness);
    panelLayout->addLayout(btnLayout);

    connect(tariffGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, [this, economTariff, comfortTariff, businessTariff](QAbstractButton *btn){
                if (btn->text() == "Эконом")
                    currentTariff = economTariff;
                else if (btn->text() == "Комфорт")
                    currentTariff = comfortTariff;
                else if (btn->text() == "Бизнес")
                    currentTariff = businessTariff;
                onDistanceChanged(lastDistanceStr);
            });

    // --- Метка цены ---
    priceLabel = new QLabel("Цена: 0.00 BYN.");
    QFont priceFont = priceLabel->font();
    priceFont.setPointSize(14);
    priceFont.setBold(true);
    priceLabel->setFont(priceFont);
    panelLayout->addSpacing(20);
    panelLayout->addWidget(priceLabel, 0, Qt::AlignLeft);

    // --- Кнопка Заказать ---
    QPushButton *orderButton = new QPushButton("Заказать");
    orderButton->setMinimumHeight(50);
    QFont orderFont = orderButton->font();
    orderFont.setPointSize(13);
    orderFont.setBold(true);
    orderButton->setFont(orderFont);
    orderButton->setStyleSheet("background-color: green; color: white;");
    panelLayout->addWidget(orderButton);

    mainLayout->addWidget(controlPanel);
    setCentralWidget(centralWidget);
    setMinimumSize(900, 740);

    // --- WebChannel ---
    mapBridge = new MapBridge(this);
    mapBridge->setUsername(Username);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("mapBridge"), mapBridge);
    webView->page()->setWebChannel(channel);

    connect(mapBridge, &MapBridge::distanceChanged, this, &MainWindow::onDistanceChanged);

    connect(orderButton, &QPushButton::clicked, this, [this]() {
        QString priceText = priceLabel->text();
        QString numberStr = priceText;
        numberStr.remove("Цена:").remove("BYN.").remove(" ").trimmed();

        bool ok = false;
        double price = numberStr.toDouble(&ok);

        if (!ok || price <= 0.0) {
            QMessageBox::warning(this, "Ошибка", "Точки не выбраны.");
            return;
        }
        // Перед сбросом маршрута:
        webView->page()->runJavaScript(R"(
            (function() {
            const route = multiRoute.model.getReferencePoints();
                if (route.length === 2 && window.mapBridge) {
                    mapBridge.saveLastRoute(route);
                }
            })();
        )");

        // 👉 Вызов JavaScript-кода, который нажимает кнопку "Сброс"
        webView->page()->runJavaScript("document.getElementById('resetBtn').click();");

        QMessageBox::information(this, "Заказ", "Такси заказано и прибудет через 5 минут.");
    });
}

void MainWindow::onDistanceChanged(QString distanceStr)
{
    lastDistanceStr = distanceStr;
    bool ok = false;
    double distance = distanceStr.toDouble(&ok);
    if (ok) {
        double price = distance * currentTariff;
        priceLabel->setText(QString("Цена: %1 BYN.").arg(QString::number(price, 'f', 2)));
    } else {
        priceLabel->setText("Цена: 0.00 BYN.");
    }
}

void MainWindow::onBackClicked()
{
    if (menuWindow) {
        menuWindow->show();
    }
    this->close();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#fed000"));
    QMainWindow::paintEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

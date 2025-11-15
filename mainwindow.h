#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QLabel>
#include "mapbridge.h"

// Добавляем forward declaration
class MenuWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Обновлённый конструктор — принимает MenuWindow*
    explicit MainWindow(const QString &username, MenuWindow *menuWindow, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDistanceChanged(QString distance);
    void onBackClicked();  // слот для кнопки "Назад"


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    QWebEngineView *webView;
    QLabel *priceLabel;
    MapBridge *mapBridge;
    double currentTariff;
    QString lastDistanceStr;
    QString Username;


    MenuWindow *menuWindow;  // указатель на окно меню
};

#endif // MAINWINDOW_H

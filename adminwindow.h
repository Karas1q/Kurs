#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QPixmap>

class AdminMenuWindow;

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(AdminMenuWindow *adminMenuWindow = nullptr, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onBackClicked();
    void onSaveClicked();

private:
    AdminMenuWindow *adminMenuWindow;

    QPixmap backgroundPixmap;
    QPushButton *backButton;
    QPushButton *saveButton;
    QLineEdit *tariffEconom;
    QLineEdit *tariffComfort;
    QLineEdit *tariffBusiness;
};

#endif // ADMINWINDOW_H

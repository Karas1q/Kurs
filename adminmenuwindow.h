#ifndef ADMINMENUWINDOW_H
#define ADMINMENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QPixmap>

class MenuWindow;

class AdminMenuWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMenuWindow(MenuWindow *menuWindow, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onOpenAdminWindowClicked();
    void onOpenCheckWindowClicked();  // Новый слот
    void onBackClicked();

private:
    QPushButton *openAdminWindowButton;
    QPushButton *openCheckWindowButton;  // Новая кнопка
    QPushButton *backButton;
    QPixmap backgroundPixmap;
    MenuWindow *menuWindow;
};

#endif // ADMINMENUWINDOW_H

#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>

// Вперед объявим класс MenuWindow, чтобы использовать указатель
class MenuWindow;

class ProfileWindow : public QWidget
{
    Q_OBJECT

public:
    // Добавлен указатель на MenuWindow для возврата
    explicit ProfileWindow(const QString &username, MenuWindow *menuWindow = nullptr, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onChangePasswordClicked();
    void onBackClicked();  // слот для кнопки Назад

private:
    QString currentUsername;
    QPixmap backgroundPixmap;

    QLineEdit *oldPasswordEdit;
    QLineEdit *newPasswordEdit;
    QPushButton *changePasswordButton;

    QPushButton *backButton;  // кнопка Назад

    MenuWindow *menuWindow;  // указатель на меню для возврата
};

#endif // PROFILEWINDOW_H

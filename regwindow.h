#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QDialog>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>  // для кнопки показать/скрыть пароль

class RegWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegWindow(QWidget *parent = nullptr);
    ~RegWindow();

signals:
    void loginSuccess(const QString &username, bool isAdmin);

private slots:
    void on_registerButton_clicked();
    void on_loginButton_clicked();
    void on_togglePasswordButton_clicked();  // слот для переключения видимости пароля

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap backgroundPixmap;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *registerButton;
    QPushButton *loginButton;
    QToolButton *togglePasswordButton;  // кнопка показать/скрыть пароль
};

#endif // REGWINDOW_H

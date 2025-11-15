#ifndef OTZIVWINDOW_H
#define OTZIVWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QTextEdit>
#include <QPushButton>

class MenuWindow;

class OtzivWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OtzivWindow(const QString &username, bool isAdmin, MenuWindow *menuWindow, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onSaveClicked();
    void onBackClicked();

private:
    QString m_username;
    bool m_isAdmin;
    MenuWindow *m_menuWindow;

    QPixmap backgroundPixmap;
    QTextEdit *reviewEdit;
    QPushButton *saveButton;
    QPushButton *backButton;
};

#endif // OTZIVWINDOW_H

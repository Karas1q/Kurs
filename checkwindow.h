#ifndef CHECKWINDOW_H
#define CHECKWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QPixmap>
#include <QVBoxLayout>
#include <QDir>

class AdminMenuWindow;

class CheckWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CheckWindow(AdminMenuWindow *adminMenu, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onBackClicked();

private:
    QTextEdit *textView;
    QPushButton *backButton;
    QPixmap backgroundPixmap;
    AdminMenuWindow *m_adminMenu;

    void loadReviewFiles();
};

#endif // CHECKWINDOW_H

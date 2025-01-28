#ifndef WIDGET_H
#define WIDGET_H
#include<QDebug>
#include <QDateTime>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPlainTextEdit>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_getButton_clicked();

    void on_postButton_clicked();

    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::Widget *ui;
    QNetworkAccessManager *manager;
    QVector<QPlainTextEdit*> keys;
    QVector<QPlainTextEdit*> values;
};
#endif // WIDGET_H

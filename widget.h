#ifndef WIDGET_H
#define WIDGET_H
#include<QDebug>
#include <QDateTime>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebSocket>
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

    void on_WSaddButton_clicked();

    void on_WSdeleteButton_clicked();

    void on_WSLinkButton_clicked();

    void onWSConnected();

    void onWSDisConnected();

    void onWSErrorOccurred(QAbstractSocket::SocketError error);

    void addSysInfo(QString message,QTextEdit *target);

    void addServerInfo(QString message,QTextEdit *target);
    void addClientInfo(QString message,QTextEdit *target);
    void onWSMessageReceived(const QString &message);
    void on_WSSubmitPureTextButton_clicked();

    void on_WSSubmitJSONButton_clicked();

private:
    Ui::Widget *ui;
    QNetworkAccessManager *manager;
    QVector<QPlainTextEdit*> keys;
    QVector<QPlainTextEdit*> values;
    QVector<QPlainTextEdit*> WSkeys;
    QVector<QPlainTextEdit*> WSvalues;

    QWebSocket socket;
};
#endif // WIDGET_H

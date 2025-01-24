#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    manager=new QNetworkAccessManager(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_getButton_clicked()
{
    QString url=ui->getOuputer->toPlainText();
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 通常不用设置 Content-Type，除非服务器明确要求
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    // 处理响应
    QByteArray response = reply->readAll();
    ui->getInputer->append("response :"+response+"\n");
}


void Widget::on_postButton_clicked()
{
    QString num1=ui->postData_1->toPlainText();
    QString num2=ui->postData_2->toPlainText();
    QUrl url=ui->postOuputer->toPlainText();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jsonObj;
    jsonObj["num1"]=num1;
    jsonObj["num2"]=num2;
    QJsonDocument doc(jsonObj);
    QByteArray postData = doc.toJson();
    QNetworkReply *reply = manager->post(request, postData);
    /*QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();*/
    QByteArray response = reply->readAll();
    ui->postInputer->append("respond :"+response);
}


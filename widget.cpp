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
    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->getInputer->append(currentDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz")+"response :"+response+"\n");
}


void Widget::on_postButton_clicked()
{
    QVector<QString> str_keys;
    QVector<QString> str_values;
    for(int i=0;i<keys.size();++i){
        str_keys.push_back(keys[i]->toPlainText());
        str_values.push_back(values[i]->toPlainText());
    }
    QString url=ui->postOuputer->toPlainText();
    qDebug()<<url<<Qt::endl;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jsonObj;
    for(int i=0;i<str_keys.size();++i){
        jsonObj[str_keys[i]]=str_values[i];
    }
    QJsonDocument doc(jsonObj);
    QByteArray postData = doc.toJson();
    QNetworkReply *reply = manager->post(request, postData);
    //等待完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray response = reply->readAll();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->postInputer->append(currentDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz")+"respond :"+response);
}


void Widget::on_addButton_clicked()
{
    QPlainTextEdit *newKey=new QPlainTextEdit(this);
    QPlainTextEdit *newValue=new QPlainTextEdit(this);
    ui->formLayout->addRow(newKey,newValue);
    newKey->setMaximumWidth(100);
    keys.push_back(newKey);
    values.push_back(newValue);
}


void Widget::on_deleteButton_clicked()
{
    for(int i=0;i<keys.size();++i){
        ui->formLayout->removeRow(0);
        delete keys[i];
        delete values[i];
    }
    keys.clear();
    values.clear();
}


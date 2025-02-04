#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    manager=new QNetworkAccessManager(this);
    //socket=new QWebSocket();
    connect(&socket, &QWebSocket::connected, this, &Widget::onWSConnected);
    connect(&socket, &QWebSocket::disconnected, this, &Widget::onWSDisConnected);
    connect(&socket, &QWebSocket::errorOccurred, this, &Widget::onWSErrorOccurred);
    connect(&socket, &QWebSocket::textMessageReceived, this, &Widget::onWSMessageReceived);

}

Widget::~Widget()
{
    if(manager)
        delete manager;
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
    addServerInfo(response,ui->getInputer);
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
    addClientInfo(postData,ui->postInputer);
    QNetworkReply *reply = manager->post(request, postData);
    //等待完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray response = reply->readAll();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    addServerInfo(response,ui->postInputer);
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


void Widget::on_WSaddButton_clicked()
{
    QPlainTextEdit *newKey=new QPlainTextEdit(this);
    QPlainTextEdit *newValue=new QPlainTextEdit(this);
    ui->WSformLayout->addRow(newKey,newValue);
    newKey->setMaximumWidth(100);
    WSkeys.push_back(newKey);
    WSvalues.push_back(newValue);
}


void Widget::on_WSdeleteButton_clicked()
{
    for(int i=0;i<WSkeys.size();++i){
        ui->WSformLayout->removeRow(0);
        delete WSkeys[i];
        delete WSvalues[i];
    }
    WSkeys.clear();
    WSvalues.clear();
}


void Widget::on_WSLinkButton_clicked()
{
    QString url=ui->WSOuputer->toPlainText();
    socket.open(QUrl(url));

    qDebug() << "尝试连接 WebSocket: " << url;
}
void Widget::onWSConnected(){
    socket.sendTextMessage("你好，Flask！");
    ui->WSLinkButton->setStyleSheet("background-color: SpringGreen  ;");
    addSysInfo("Connected",ui->WSInputer);
}
void Widget::onWSDisConnected(){
    ui->WSLinkButton->setStyleSheet("background-color: MistyRose  ;");
    addSysInfo("DISConnected",ui->WSInputer);
}
void Widget::onWSErrorOccurred(QAbstractSocket::SocketError error){
    addSysInfo("LINK ERROR :"+socket.errorString(),ui->WSInputer);
}
void Widget::addSysInfo(QString message,QTextEdit *target){
    QDateTime currentDateTime = QDateTime::currentDateTime();
    target->append("[SYS]"+currentDateTime.toString("HH:mm:ss ")+message);
}

void Widget::addServerInfo(QString message,QTextEdit *target){\
    QDateTime currentDateTime = QDateTime::currentDateTime();
    target->append("[s]"+currentDateTime.toString("HH:mm:ss ")+message);
}
void Widget::addClientInfo(QString message,QTextEdit *target){\
        QDateTime currentDateTime = QDateTime::currentDateTime();
    target->append("[c]"+currentDateTime.toString("HH:mm:ss ")+message);
}
void Widget::onWSMessageReceived(const QString &message){
    qDebug() << "收到消息:" << message;
    addServerInfo(message,ui->WSInputer);
}

void Widget::on_WSSubmitPureTextButton_clicked()
{
    QString message=ui->WSPureTextOuputer->toPlainText();
    socket.sendTextMessage(message);
    addClientInfo(message,ui->WSInputer);
}


void Widget::on_WSSubmitJSONButton_clicked()
{
    QVector<QString> str_keys;
    QVector<QString> str_values;
    for(int i=0;i<WSkeys.size();++i){
        str_keys.push_back(WSkeys[i]->toPlainText());
        str_values.push_back(WSvalues[i]->toPlainText());
    }
    QJsonObject jsonObj;
    for(int i=0;i<str_keys.size();++i){
        jsonObj[str_keys[i]]=str_values[i];
    }
    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson();
    socket.sendTextMessage(data);
    //等待完成
    QEventLoop loop;
    addClientInfo(data,ui->WSInputer);
}


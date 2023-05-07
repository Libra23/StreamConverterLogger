#ifndef WEBSOCKETSTREAM_H
#define WEBSOCKETSTREAM_H

#include "streambase.h"
#include <QWebSocket>
#include <QWebSocketServer>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class WebSocketOptions : public QWidget
{
public:
    WebSocketOptions()
    {
        // create option widget
        url_label_ = new QLabel(this);
        url_label_->setText("URL");
        url_edit_ = new QLineEdit(this);
        url_edit_->setText("ws://localhost");
        port_label_ = new QLabel(this);
        port_label_->setText("Port");
        port_edit_ = new QLineEdit(this);
        port_edit_->setText("9871");
        QVBoxLayout* vbox = new QVBoxLayout;
        vbox->addWidget(url_label_);
        vbox->addWidget(url_edit_);
        vbox->addWidget(port_label_);
        vbox->addWidget(port_edit_);
        QWidget::setLayout(vbox);
    }
    ~WebSocketOptions()
    {
    }
    QString GetURL()
    {
        return url_edit_->text();
    }
    QString GetPort()
    {
        return port_edit_->text();
    }
private:
    QLabel* url_label_;
    QLineEdit* url_edit_;
    QLabel* port_label_;
    QLineEdit* port_edit_;
};

class WebSocketStream : public StreamBase
{
public:
    WebSocketStream();
    ~WebSocketStream();
    bool BeginReceive() override;
    bool BeginTransmit() override;
    void End() override;
    void Tramsmit(const QByteArray& data) override;
    void RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback = [](const QByteArray& data) {}) override;
    QWidget* GetOptionsWidget() override;
    static QString GetStreamType();
private:
    QWebSocketServer* server_;
    QWebSocket* client_;
    WebSocketOptions* option_widget_;
    bool is_connected_;
private slots:
    void OnNewConnection();
    void OnDisconnected();
    void ReceiveText(QString text);
};

#endif // WEBSOCKETSTREAM_H

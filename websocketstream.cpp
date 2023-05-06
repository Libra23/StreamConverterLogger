#include "websocketstream.h"

WebSocketStream::WebSocketStream()
    : server_(new QWebSocketServer("SteamConverter", QWebSocketServer::NonSecureMode))
    , client_(new QWebSocket("SteamConverter"))
    , option_widget_(new WebSocketOptions())
    , is_connected_(false)
{}

WebSocketStream::~WebSocketStream()
{

}

bool WebSocketStream::BeginReceive()
{
    if (server_->listen(QHostAddress::Any, (option_widget_->GetPort()).toInt())) {
        connect(server_, &QWebSocketServer::newConnection, this, &WebSocketStream::OnNewConnection);
        return true;
    } else {
        return false;
    }
}

bool WebSocketStream::BeginTransmit()
{
    client_->open(QUrl(option_widget_->GetURL() + ':' + option_widget_->GetPort()));
    return true;
}

void WebSocketStream::End()
{
    OnDisconnected();
    server_->close();
}

void WebSocketStream::Tramsmit(const QByteArray& data)
{
    client_->sendTextMessage(QString(data));
}

void WebSocketStream::RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback) {
    callback_ = std::move(callback);
}

QWidget* WebSocketStream::GetOptionsWidget()
{
    return option_widget_;
}

QString WebSocketStream::GetStreamType()
{
    return "WebSocket Stream";
}

void WebSocketStream::OnNewConnection()
{
    if (!is_connected_)
    {
        is_connected_ = true;
        client_ = server_->nextPendingConnection();
        connect(client_, &QWebSocket::textMessageReceived, this, &WebSocketStream::ReceiveText);
        connect(client_, &QWebSocket::disconnected, this, &WebSocketStream::OnDisconnected);
    }

}

void WebSocketStream::OnDisconnected()
{
    if (is_connected_)
    {
        is_connected_ = false;
        disconnect(client_, &QWebSocket::textMessageReceived, this, &WebSocketStream::ReceiveText);
        disconnect(client_, &QWebSocket::disconnected, this, &WebSocketStream::OnDisconnected);
        client_->deleteLater();
    }
}

void WebSocketStream::ReceiveText(QString text)
{
    QByteArray data = text.toLocal8Bit();
    callback_(data);
}

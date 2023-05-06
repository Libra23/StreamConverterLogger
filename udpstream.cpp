#include "udpstream.h"
#include <QNetworkDatagram>

UdpStream::UdpStream()
    : udpsocket_(new QUdpSocket())
    , option_widget_(new UdpOptions())
{}

UdpStream::~UdpStream()
{

}

bool UdpStream::BeginReceive()
{
    if (udpsocket_->bind(QHostAddress::Any, option_widget_->GetPort())) {
       connect(udpsocket_, &QUdpSocket::readyRead, this, &UdpStream::Receive);
        return true;
    } else {
        return false;
    }
}

bool UdpStream::BeginTransmit()
{
    return true;
}

void UdpStream::End()
{
    udpsocket_->close();
}

void UdpStream::Tramsmit(const QByteArray& data)
{
    udpsocket_->writeDatagram(data, QHostAddress::LocalHost, option_widget_->GetPort());
}

void UdpStream::RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback) {
    callback_ = std::move(callback);
}

QWidget* UdpStream::GetOptionsWidget()
{
    return option_widget_;
}

QString UdpStream::GetStreamType()
{
    return "UDP Stream";
}

void UdpStream::Receive() {
    while (udpsocket_->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = udpsocket_->receiveDatagram();
        QByteArray data = datagram.data();
        callback_(data);
    }
}

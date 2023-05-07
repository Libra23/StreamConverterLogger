#include "serialstream.h"
#include <QMessageBox>

SerialStream::SerialStream()
    : serialport_(new QSerialPort())
    , option_widget_(new SerialOptions())
{}


SerialStream::~SerialStream()
{
    if (serialport_->isOpen()) serialport_->close();
}

bool SerialStream::BeginReceive()
{
    serialport_->setPortName(option_widget_->GetPort());
    serialport_->setBaudRate(option_widget_->GetBaudrate());
    if (serialport_->isOpen()) serialport_->close(); // @todo
    if (serialport_->open(QIODevice::ReadWrite)) {
        connect(serialport_, &QSerialPort::readyRead, this, &SerialStream::Receive);
        return true;
    } else {
        return false;
    }
}

bool SerialStream::BeginTransmit()
{
    serialport_->setPortName(option_widget_->GetPort());
    serialport_->setBaudRate(option_widget_->GetBaudrate());
    if (serialport_->isOpen()) serialport_->close(); // @todo
    if (serialport_->open(QIODevice::ReadWrite)) {
        return true;
    } else {
        return false;
    }
}

void SerialStream::End()
{
    if (serialport_->isOpen()) serialport_->close();
}

void SerialStream::Tramsmit(const QByteArray& data)
{
    serialport_->write(data);
}

void SerialStream::RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback)
{
    callback_ = std::move(callback);
}

QWidget* SerialStream::GetOptionsWidget()
{
    return option_widget_;
}

QString SerialStream::GetStreamType()
{
    return "Serial Stream";
}
void SerialStream::Receive()
{
    const QByteArray data = serialport_->readAll();
    callback_(data);
}

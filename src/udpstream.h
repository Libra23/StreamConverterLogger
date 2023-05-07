#ifndef UDPSTREAM_H
#define UDPSTREAM_H

#include "streambase.h"
#include <QUdpSocket>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class UdpOptions : public QWidget
{
public:
    UdpOptions()
    {
        // create option widget
        port_label_ = new QLabel(this);
        port_label_->setText("Port");
        port_edit_ = new QLineEdit(this);
        port_edit_->setText("9870");
        QVBoxLayout* vbox = new QVBoxLayout;
        vbox->addWidget(port_label_);
        vbox->addWidget(port_edit_);
        QWidget::setLayout(vbox);
    }
    ~UdpOptions()
    {
    }
    int GetPort()
    {
        return (port_edit_->text()).toInt();
    }
private:
    QLabel* port_label_;
    QLineEdit* port_edit_;
};

class UdpStream : public StreamBase
{
public:
    UdpStream();
    ~UdpStream();
    bool BeginReceive() override;
    bool BeginTransmit() override;
    void End() override;
    void Tramsmit(const QByteArray& data) override;
    void RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback = [](const QByteArray& data) {}) override;
    QWidget* GetOptionsWidget() override;
    static QString GetStreamType();
private:
    QUdpSocket* udpsocket_;
    UdpOptions* option_widget_;
private slots:
    void Receive() override;
};

#endif // UDPSTREAM_H

#ifndef SERIALSTREAM_H
#define SERIALSTREAM_H

#include "streambase.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

class SerialOptions : public QWidget
{
public:
    SerialOptions()
    {
        // add port
        port_label_ = new QLabel(this);
        port_label_->setText("Port");
        port_comboBox_ = new QComboBox(this);
        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
            port_comboBox_->addItem(info.portName());
        }
        // add baudrate
        baudrate_label_ = new QLabel(this);
        baudrate_label_->setText("Baudrate");
        baudrate_comboBox_ = new QComboBox(this);
        QStringList baudrate_list;
        baudrate_list.append("600");
        baudrate_list.append("1200");
        baudrate_list.append("2400");
        baudrate_list.append("4800");
        baudrate_list.append("9600");
        baudrate_list.append("14400");
        baudrate_list.append("19200");
        baudrate_list.append("38400");
        baudrate_list.append("57600");
        baudrate_list.append("115200");
        baudrate_list.append("230400");
        baudrate_list.append("460800");
        baudrate_list.append("921600");
        baudrate_comboBox_->addItems(baudrate_list);
        baudrate_comboBox_->setEditable(true);
        QVBoxLayout* vbox = new QVBoxLayout;
        vbox->addWidget(port_label_);
        vbox->addWidget(port_comboBox_);
        vbox->addWidget(baudrate_label_);
        vbox->addWidget(baudrate_comboBox_);
        QWidget::setLayout(vbox);
    }
    ~SerialOptions()
    {
    }
    QString GetPort()
    {
        return port_comboBox_->currentText();
    }
    int GetBaudrate()
    {
        return (baudrate_comboBox_->currentText()).toInt();
    }
private:
    QLabel* port_label_;
    QComboBox* port_comboBox_;
    QLabel* baudrate_label_;
    QComboBox* baudrate_comboBox_;
};

class SerialStream : public StreamBase
{
public:
    SerialStream();
    ~SerialStream();
    bool BeginReceive() override;
    bool BeginTransmit() override;
    void End() override;
    void Tramsmit(const QByteArray& data) override;
    void RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback = [](const QByteArray& data) {}) override;
    QWidget* GetOptionsWidget() override;
    static QString GetStreamType();
private:
    QSerialPort* serialport_;
    SerialOptions* option_widget_;
private slots:
    void Receive() override;
};

#endif // SERIALSTREAM_H

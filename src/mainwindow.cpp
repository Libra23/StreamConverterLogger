#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardPaths>

#include "serialstream.h"
#include "udpstream.h"
#include "websocketstream.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , file_(new QFile(this))
    , input_stream_dase_(new StreamBase())
    , output_stream_dase_(new StreamBase())
    , convert_running_(false)
    , log_running_(false)
{
    // ui
    ui_->setupUi(this);
    ui_->log_pushButton->setEnabled(false);
    // slot
    connect(ui_->input_comboBox, &QComboBox::currentTextChanged, this, &MainWindow::OnSelect_InputStream);
    connect(ui_->output_comboBox, &QComboBox::currentTextChanged, this, &MainWindow::OnSelect_OutputStream);
    connect(ui_->convert_pushButton, &QPushButton::clicked, this, &MainWindow::OnClick_Convert);
    connect(ui_->log_pushButton, &QPushButton::clicked, this, &MainWindow::OnClick_Log);
    // map
    input_stream_map_.insert(StreamBase::GetStreamType(), new StreamBase());
    input_stream_map_.insert(SerialStream::GetStreamType(), new SerialStream());
    input_stream_map_.insert(UdpStream::GetStreamType(), new UdpStream());
    input_stream_map_.insert(WebSocketStream::GetStreamType(), new WebSocketStream());
    output_stream_map_.insert(StreamBase::GetStreamType(), new StreamBase());
    output_stream_map_.insert(SerialStream::GetStreamType(), new SerialStream());
    output_stream_map_.insert(UdpStream::GetStreamType(), new UdpStream());
    output_stream_map_.insert(WebSocketStream::GetStreamType(), new WebSocketStream());
    for (auto itr = input_stream_map_.begin(); itr != input_stream_map_.end(); itr++) {
        ui_->input_comboBox->addItem(itr.key());
    }
    for (auto itr = output_stream_map_.begin(); itr != output_stream_map_.end(); itr++) {
        ui_->output_comboBox->addItem(itr.key());
    }
}

MainWindow::~MainWindow()
{
    delete ui_;
    if (file_->isOpen()) file_->close();
    input_stream_dase_->End();
}

void MainWindow::OnSelect_InputStream(const QString &text)
{
    qDebug() << "OnSelect_InputStream Selected : " << text;
    if (input_stream_dase_->GetOptionsWidget()) {
        input_stream_dase_->GetOptionsWidget()->setVisible(false);
    }
    input_stream_dase_ = input_stream_map_[text];
    if (input_stream_dase_->GetOptionsWidget()) {
        input_stream_dase_->GetOptionsWidget()->setVisible(true);
    }
    ui_->input_options->addWidget(input_stream_dase_->GetOptionsWidget());
}

void MainWindow::OnSelect_OutputStream(const QString &text)
{
    qDebug() << "OnSelect_OutputStream Selected : " << text;
    if (output_stream_dase_->GetOptionsWidget()) {
        output_stream_dase_->GetOptionsWidget()->setVisible(false);
    }
    output_stream_dase_ = output_stream_map_[text];
    if (output_stream_dase_->GetOptionsWidget()) {
        output_stream_dase_->GetOptionsWidget()->setVisible(true);
    }
    ui_->output_options->addWidget(output_stream_dase_->GetOptionsWidget());
}

void MainWindow::OnClick_Convert()
{
    qDebug() << "OnClick_Convert Clicked";

    if (!convert_running_) {
        /* Start Convert */
        // prepare receive
        if (input_stream_dase_->BeginReceive()) {
            input_stream_dase_->RegisterReceiveCallback([this](const QByteArray& data) {
                if (log_running_) file_->write(data);
                output_stream_dase_->Tramsmit(data);
            });
        } else {
            QMessageBox::critical(this, tr("Input Stream Error"), "Input Stream Error");
            return;
        }
        // prepare transmit
        if (!output_stream_dase_->BeginTransmit()) {
            QMessageBox::critical(this, tr("Input Stream Error"), "Input Stream Error");
            return;
        }
        convert_running_ = true;
        ui_->convert_pushButton->setText("Stop");
        ui_->log_pushButton->setEnabled(true);
        ui_->input_comboBox->setEnabled(false);
        ui_->output_comboBox->setEnabled(false);
    } else {
        /* Stop Convert */
        input_stream_dase_->End();
        convert_running_ = false;
        ui_->convert_pushButton->setText("Start");
        ui_->log_pushButton->setEnabled(false);
        ui_->input_comboBox->setEnabled(true);
        ui_->output_comboBox->setEnabled(true);
    }
}

void MainWindow::OnClick_Log()
{
    qDebug() << "OnClick_Log Clicked";

    if (!log_running_) {
        /* Start Log */
        // open file to log
        QString dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        QString file_name = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        file_->setFileName(dir + '/' + file_name + "_log.log");
        qDebug() << file_->fileName();
        if (file_->open(QIODevice::WriteOnly)) {
            QTextStream outStream(file_);
            log_running_ = true;
            ui_->log_pushButton->setText("Stop");
            // disable stream stop
            ui_->convert_pushButton->setEnabled(false);
        } else {
            QMessageBox::critical(this, tr("File Error"), file_->errorString());
        }
    } else {
        /* Stop Log */
        if (file_->isOpen()) {
            file_->close();
            log_running_ = false;
            ui_->log_pushButton->setText("Start");
            // enable stream stop
            ui_->convert_pushButton->setEnabled(true);
        }
    }
}

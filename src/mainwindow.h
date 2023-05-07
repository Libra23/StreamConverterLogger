#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFile>
#include "streambase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OnSelect_InputStream(const QString &text);
    void OnSelect_OutputStream(const QString &text);
    void OnClick_Convert();
    void OnClick_Log();

private:
    Ui::MainWindow* ui_;
    QFile* file_;
    StreamBase* input_stream_dase_;
    StreamBase* output_stream_dase_;
    QMap<QString, StreamBase*> input_stream_map_;
    QMap<QString, StreamBase*> output_stream_map_;
    bool convert_running_;
    bool log_running_;
};
#endif // MAINWINDOW_H

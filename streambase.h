#ifndef STREAMBASE_H
#define STREAMBASE_H

#include <QString>
#include <QWidget>

class StreamBase : public QObject
{
    Q_OBJECT
public:
    StreamBase();
    virtual ~StreamBase();
    virtual bool BeginReceive();
    virtual bool BeginTransmit();
    virtual void End();
    virtual void Tramsmit(const QByteArray& data);
    virtual void RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback = [](const QByteArray& data) {});
    virtual QWidget* GetOptionsWidget();
    static QString GetStreamType();
private slots:
    virtual void Receive();
protected:
    std::function<void(const QByteArray& data)> callback_;
};

#endif // STREAMBASE_H

#include "streambase.h"

StreamBase::StreamBase()
{

}

StreamBase::~StreamBase()
{

}

bool StreamBase::BeginReceive()
{
    return false;
}

bool StreamBase::BeginTransmit()
{
    return false;
}

void StreamBase::End()
{

}

void StreamBase::Tramsmit(const QByteArray& data)
{

}

void StreamBase::RegisterReceiveCallback(std::function<void(const QByteArray& data)> callback)
{

}

QWidget* StreamBase::GetOptionsWidget()
{
    return nullptr;
}

QString StreamBase::GetStreamType()
{
    return "No Stream";
}

void StreamBase::Receive()
{

}

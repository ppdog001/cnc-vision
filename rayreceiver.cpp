#include "rayreceiver.h"
#include <QDebug>

RayReceiver::RayReceiver(QObject *parent) : QObject(parent)
{
    m_udp = new QUdpSocket(this);
    m_udp->bind(QHostAddress::Any, 45454);
    connect(m_udp, &QUdpSocket::readyRead,
            this,  &RayReceiver::onReadyRead);
}

void RayReceiver::onReadyRead()
{
    while (m_udp->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udp->receiveDatagram();
        processPayload(datagram);
    }
}

void RayReceiver::processPayload(QNetworkDatagram datagram)
{
    QByteArray data = datagram.data();
    if (data.size() != sizeof(ray_payload_t))
        return;
    quint8 *p = (quint8 *)data.data();
    memcpy((void *)&m_payload, p, sizeof(ray_payload_t));
    qDebug() << "x:" << m_payload.mcs_x << "\ty:" << m_payload.mcs_y << "\tz:" << m_payload.mcs_z << "\tb:" << m_payload.mcs_b;

}

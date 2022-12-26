// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dactivevpnconnection_p.h"
#include "dnmutils.h"

DNETWORKMANAGER_BEGIN_NAMESPACE

DActiveVpnConnectionPrivate::DActiveVpnConnectionPrivate(const quint64 id, DActiveVpnConnection *parent)
    : DActiveConnectionPrivate(id, parent)
#ifdef USE_FAKE_INTERFACE
    , m_vpn(new DActiveVpnConnectionInterface("/com/deepin/FakeNetworkManager/ActiveConnection/" + QByteArray::number(id), this))
#else
    , m_vpn(new DActiveVpnConnectionInterface("/org/freedesktop/NetworkManager/ActiveConnection/" + QByteArray::number(id), this))
#endif
{
}

DActiveVpnConnection::DActiveVpnConnection(const quint64 id, QObject *parent)
    : DActiveConnection(*new DActiveVpnConnectionPrivate(id, this), parent)
{
    Q_D(const DActiveVpnConnection);

    // connect to ActiveConnection signals,because DActiveVpnConnection is devired by DActiveConnection
    connect(d->m_activeConn, &DActiveConnectionInterface::devicesChanged, this, [this](const QList<QDBusObjectPath> &devices) {
        QList<quint64> ret;
        for (const auto &it : devices)
            ret.append(getIdFromObjectPath(it));
        emit this->devicesChanged(ret);
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::vpnChanged, this, &DActiveConnection::vpnChanged);
    connect(d->m_activeConn, &DActiveConnectionInterface::connectionChanged, this, [this](const QDBusObjectPath &connection) {
        emit this->connectionChanged(getIdFromObjectPath(connection));
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::DHCP4ConfigChanged, this, [this](const QDBusObjectPath &config) {
        emit this->DHCP4ConfigChanged(getIdFromObjectPath(config));
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::DHCP6ConfigChanged, this, [this](const QDBusObjectPath &config) {
        emit this->DHCP6ConfigChanged(getIdFromObjectPath(config));
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::IP4ConfigChanged, this, [this](const QDBusObjectPath &config) {
        emit this->IP4ConfigChanged(getIdFromObjectPath(config));
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::IP6ConfigChanged, this, [this](const QDBusObjectPath &config) {
        emit this->IP6ConfigChanged(getIdFromObjectPath(config));
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::specificObjectChanged, this, [this](const QDBusObjectPath &specObj) {
        emit this->specificObjectChanged(specObj.path().toUtf8());
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::idChanged, this, &DActiveConnection::connecionIdChanged);
    connect(d->m_activeConn, &DActiveConnectionInterface::uuidChanged, this, [this](const QString &uuid) {
        emit this->UUIDChanged(uuid.toUtf8());
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::typeChanged, this, [this](const QString &type) {
        emit this->connectionTypeChanged(type.toUtf8());
    });
    connect(d->m_activeConn, &DActiveConnectionInterface::stateChanged, this, [this](const quint32 state, const quint32 reason) {
        emit this->connectionStateChanged(static_cast<NMActiveConnectionState>(state),
                                          static_cast<NMActiveConnectionStateReason>(reason));
    });

    // connect to vpn connection specific signals
    connect(d->m_vpn, &DActiveVpnConnectionInterface::VpnStateChanged, this, [this](const quint32 state, const quint32 reason) {
        emit this->VpnStateChanged(static_cast<NMVpnConnectionState>(state), static_cast<NMActiveConnectionStateReason>(reason));
    });

    connect(d->m_vpn, &DActiveVpnConnectionInterface::BannerChanged, this, &DActiveVpnConnection::bannerChanged);
}

NMVpnConnectionState DActiveVpnConnection::vpnState() const
{
    Q_D(const DActiveVpnConnection);
    return static_cast<NMVpnConnectionState>(d->m_vpn->vpnState());
}

QString DActiveVpnConnection::banner() const
{
    Q_D(const DActiveVpnConnection);
    return d->m_vpn->banner();
}

DNETWORKMANAGER_END_NAMESPACE

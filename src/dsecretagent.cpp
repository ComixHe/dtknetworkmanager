// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsecretagent.h"
#include "dsecretagent_p.h"

#undef signals
#include <NetworkManager.h>
#include <libnm/NetworkManager.h>

#define signals Q_SIGNALS

#include <QDBusConnection>

DNETWORKMANAGER_BEGIN_NAMESPACE

<<<<<<< HEAD
constexpr const char *connPath = "/org/freedesktop/NetworkManager/Settings/";

using DCORE_NAMESPACE::DUnexpected;
using DCORE_NAMESPACE::emplace_tag;
=======
DSecretAgentPrivate::DSecretAgentPrivate(const QString &id, DSecretAgent::Capabilities caps, DSecretAgent *parent)
    : m_agentId(id)
    , m_caps(caps)
    , q_ptr(parent)
    , m_agent(new DSecretAgentAdaptor(parent))
    , m_manager(new DAgentManagerInterface(this))
{
    Q_Q(DSecretAgent);
    qRegisterMetaType<SettingDesc>("SettingDesc");
    qDBusRegisterMetaType<SettingDesc>();
    auto managerConnection = m_manager->getConnection();
    managerConnection.connect("org.freedesktop.NetworkManager",
                              "/org/freedesktop",
                              "org.freedesktop.DBus.ObjectManager",
                              QLatin1String("InterfacesAdded"),
                              q,
                              SLOT(dbusInterfacesAdded(QDBusObjectPath, QVariantMap)));
    managerConnection.registerObject(QLatin1String(NM_DBUS_PATH_SECRET_AGENT), m_agent, QDBusConnection::ExportAllSlots);
    registerAgent(caps);
}
>>>>>>> master

DSecretAgentPrivate::~DSecretAgentPrivate()
{
    m_manager->unregisterAgent();
}

void DSecretAgentPrivate::dbusInterfacesAdded(const QDBusObjectPath &path, const QVariantMap &interfaces)
{
    Q_UNUSED(path);
    if (!interfaces.contains(QString::fromLatin1("org.freedesktop.NetworkManager.AgentManager"))) {
        return;
    }

    registerAgent(m_caps);
}

<<<<<<< HEAD
DExpected<SettingDesc> DSecretAgent::secrets(const SettingDesc &conn,
                                             const quint64 connId,
                                             const QByteArray &settingName,
                                             const QList<QByteArray> &hints,
                                             const SecretFlags &flags) const
{
    Q_D(const DSecretAgent);
    auto reply = d->m_secret->getSecrets(conn, connPath + QByteArray::number(connId), settingName, hints, flags);
    reply.waitForFinished();
    if (!reply.isValid())
        return DUnexpected{emplace_tag::USE_EMPLACE, reply.error().type(), reply.error().message()};
    return reply.value();
}

DSecretAgent::~DSecretAgent() = default;

DExpected<void> DSecretAgent::cancelSecrets(const quint64 connId, const QByteArray &settingName) const
{
    Q_D(const DSecretAgent);
    auto reply = d->m_secret->cancelGetSecrets(connPath + QByteArray::number(connId), settingName);
    reply.waitForFinished();
    if (!reply.isValid())
        return DUnexpected{emplace_tag::USE_EMPLACE, reply.error().type(), reply.error().message()};
    return {};
}

DExpected<void> DSecretAgent::saveSecret(const SettingDesc &connSettigns, const quint64 connId) const
{
    Q_D(const DSecretAgent);
    auto reply = d->m_secret->saveSecrets(connSettigns, connPath + QByteArray::number(connId));
    reply.waitForFinished();
    if (!reply.isValid())
        return DUnexpected{emplace_tag::USE_EMPLACE, reply.error().type(), reply.error().message()};
    return {};
}

DExpected<void> DSecretAgent::deleteSecret(const SettingDesc &connSettigns, const quint64 connId) const
{
    Q_D(const DSecretAgent);
    auto reply = d->m_secret->deleteSecrets(connSettigns, connPath + QByteArray::number(connId));
    reply.waitForFinished();
    if (!reply.isValid())
        return DUnexpected{emplace_tag::USE_EMPLACE, reply.error().type(), reply.error().message()};
    return {};
=======
void DSecretAgentPrivate::registerAgent()
{
    m_manager->registerAgent(m_agentId);
}

void DSecretAgentPrivate::registerAgent(const DSecretAgent::Capabilities capabilities)
{
    m_manager->registerWithCapabilities(m_agentId, capabilities);
}

DSecretAgent::DSecretAgent(const QString &id, QObject *parent)
    : QObject(parent)
    , QDBusContext()
    , d_ptr(new DSecretAgentPrivate(id, NMSecretAgentCapabilities::None, this))
{
}

DSecretAgent::DSecretAgent(const QString &id, Capabilities caps, QObject *parent)
    : QObject(parent)
    , QDBusContext()
    , d_ptr(new DSecretAgentPrivate(id, caps, this))
{
>>>>>>> master
}

DSecretAgent::~DSecretAgent() = default;

#include "moc_dsecretagent.cpp"

DNETWORKMANAGER_END_NAMESPACE

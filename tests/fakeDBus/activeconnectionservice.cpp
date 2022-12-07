// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "activeconnectionservice.h"
#include <QDBusConnection>
#include <QDBusError>

FakeActiveConncetionService::FakeActiveConncetionService(QObject *parent)
    : QObject(parent)
{
}

FakeActiveConncetionService::~FakeActiveConncetionService() {}

/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "rcthttpserver.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QDebug>
#include <QJsonDocument>
#include <QByteArray>
#include <QVariantMap>

namespace {
struct RegisterQMLMetaType {
    RegisterQMLMetaType() {
        qRegisterMetaType<RCTHttpServer*>();
    }
} registerMetaType;
} // namespace

class RCTHttpServerPrivate {
public:
    Bridge* bridge = nullptr;
};

RCTHttpServer::RCTHttpServer(QObject* parent) : QObject(parent), d_ptr(new RCTHttpServerPrivate) {}

RCTHttpServer::~RCTHttpServer() {}

void RCTHttpServer::setBridge(Bridge* bridge) {
    Q_D(RCTHttpServer);
    d->bridge = bridge;
}

QString RCTHttpServer::moduleName() {
    return "RCTHttpServer";
}

QList<ModuleMethod*> RCTHttpServer::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap RCTHttpServer::constantsToExport() {
    return QVariantMap();
}

void RCTHttpServer::start(int port, QString serviceName) {

}

void RCTHttpServer::stop() {

}

void RCTHttpServer::respond(QString requestId, int code, QString type, QString body) {

}

/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef RCTHTTPSERVER_H
#define RCTHTTPSERVER_H

#include "moduleinterface.h"

#include <QVariantMap>

class RCTHttpServerPrivate;
class RCTHttpServer : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(RCTHttpServer)

public:
    Q_INVOKABLE RCTHttpServer(QObject* parent = 0);
    ~RCTHttpServer();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;
    QList<ModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

    Q_INVOKABLE void start(int port, QString serviceName);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void respond(QString requestId, int code, QString type, QString body);

private:
    QScopedPointer<RCTHttpServerPrivate> d_ptr;
};

#endif // RCTHTTPSERVER_H
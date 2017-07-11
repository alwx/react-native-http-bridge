/**
 * @flow
 * @providesModule react-native-http-server
 */
'use strict';

import { NativeEventEmitter, NativeModules } from 'react-native';
const { HttpServer } = NativeModules;
const HttpServerEventEmitter = new NativeEventEmitter(HttpServer);

export function start(
  port: number,
  serviceName: string,
  callback: (json: Object) => {}
) {
  if (port == 80) {
    throw 'Invalid server port specified. Port 80 is reserved.';
  }

  HttpServerEventEmitter.addListener('httpServerResponseReceived', callback);
  HttpServer.start(port, serviceName);
}

export function stop() {
  HttpServer.stop();
  HttpServerEventEmitter.removeListener('httpServerResponseReceived');
}

export function respond(
  code: string = '200',
  type: string = 'text/html',
  body: string = ''
) {
  HttpServer.respond(code, type, body);
}

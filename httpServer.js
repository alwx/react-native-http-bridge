/**
 * @providesModule react-native-http-server
 */
'use strict';

import {DeviceEventEmitter} from 'react-native';
import {NativeModules} from 'react-native';
var Server = NativeModules.HttpServer;

module.exports = {
    start: function (port, serviceName, callback) {
        if (port == 80) {
            throw "Invalid server port specified. Port 80 is reserved.";
        }

        Server.start(port, serviceName);
        DeviceEventEmitter.addListener('httpServerResponseReceived', callback);
    },

    stop: function () {
        Server.stop();
        DeviceEventEmitter.removeListener('httpServerResponseReceived');
    },

    respond: function (code, type, body) {
        Server.respond(code, type, body);
    }
}

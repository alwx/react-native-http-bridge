/**
 * @providesModule react-native-http-server
 */
'use strict';

import {DeviceEventEmitter} from 'react-native';
import {NativeModules} from 'react-native';
var Server = NativeModules.HttpServer;

module.exports = {
    start: function (port, callback) {
        if (port == 80) {
            throw "Invalid server port specified. Port 80 is reserved.";
        }

        Server.start(port);
        DeviceEventEmitter.addListener('httpServerResponseReceived', callback);
    },

    stop: function () {
        Server.stop();
        DeviceEventEmitter.removeListener('httpServerResponseReceived');
    }
}

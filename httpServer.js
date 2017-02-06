/**
 * @providesModule react-native-http-server
 */
'use strict';

import {DeviceEventEmitter} from 'react-native';
import {NativeModules} from 'react-native';
var Server = NativeModules.HttpServer;

var validStatusCodes = ['ACCEPTED',
    'BAD_REQUEST',
    'CREATED',
    'FORBIDDEN',
    'INTERNAL_ERROR',
    'METHOD_NOT_ALLOWED',
    'NO_CONTENT',
    'NOT_FOUND',
    'NOT_MODIFIED',
    'OK',
    'PARTIAL_CONTENT',
    'RANGE_NOT_SATISFIABLE',
    'REDIRECT',
    'UNAUTHORIZED'
];

module.exports = {

    create: function (options, callback) {
        if (options.port == 80) {
            throw "Invalid server port specified. Port 80 is reserved.";
        }

        Server.init(options, function () {
            DeviceEventEmitter.addListener('httpServerResponseReceived', function (request) {
                var success = true;
                var promise = new Promise(function (resolve, reject) {
                    callback(request, resolve);
                }).then(function (response) {
                    if (validStatusCodes.indexOf(response.status) === 0) {
                        success = false;
                        throw "Invalid response status code specified in HttpServer options.";
                    }

                    if (response.type === null) {
                        response.type = "text/plain";
                    }
                    if (response.data === null) {
                        response.data = "";
                    }
                    if (response.headers === null) {
                        response.headers = {};
                    }
                    if (success) {
                        Server.setResponse(request.url, response);
                    }
                });
            });
        }, function (e) {
            throw "Could not initialize server: " + e;
        });

    },

    // attempt to start the instance of the server - returns a promise object that will be rejected or approved
    start: function () {
        var promise = new Promise(function (resolve, reject) {
            Server.start(function () {
                resolve();
            }, function () {
                reject();
            });
        });

        return promise;
    },

    // effectively pause the instance of the server
    stop: function () {
        Server.stop();
    }
}

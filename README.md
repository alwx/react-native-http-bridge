# react-native-http-bridge

HTTP Server for [React Native](https://github.com/facebook/react-native)

Supports only POST-requests and one-way communication. Created for [Status.im](https://github.com/status-im)

## Install

```shell
npm install --save react-native-http-bridge
```

## Automatically link

#### With React Native 0.27+

```shell
react-native link react-native-http-bridge
```

## Example

First import/require react-native-http-server:

```js

    var httpBridge = require('react-native-http-bridge');

```


Initalise the server in the `componentWillMount` lifecycle method. You need to provide a `port` and a callback where requests will be captured. Currently there is no way to return responses.

```js

    componentWillMount(){

      // initalize the server (now accessible via localhost:1234)
      httpBridge.start(5561, function(request) {

          // request.url
          // request.postData

      });

    }

```

Finally, ensure that you disable the server when your component is being unmounted.

```js

  componentWillUnmount() {
    httpBridge.stop();
  }

```

# react-native-http-server

HTTP Server for [React Native](https://github.com/facebook/react-native)

Android only for now.

Built on top of the NanoHttpd library: https://github.com/NanoHttpd/nanohttpd

## Install

```shell
npm install --save react-native-http-server
```

## Automatically link

#### With React Native 0.27+

```shell
react-native link react-native-http-server
```

#### With older versions of React Native

You need [`rnpm`](https://github.com/rnpm/rnpm) (`npm install -g rnpm`)

```shell
rnpm link react-native-http-server
```

## Manually link

- in `android/app/build.gradle`:

```diff
dependencies {
    ...
    compile "com.facebook.react:react-native:+"  // From node_modules
+   compile project(':react-native-http-server')
}
```

- in `android/settings.gradle`:

```diff
...
include ':app'
+ include ':react-native-http-server'
+ project(':react-native-http-server').projectDir = new File(rootProject.projectDir, '../node_modules/react-native-http-server/android')
```

#### With React Native 0.29+

- in `MainApplication.java`:

```diff
+ import com.strainy.RNHttpServer.RNHttpServer;

  public class MainApplication extends Application implements ReactApplication {
    //......

    @Override
    protected List<ReactPackage> getPackages() {
      return Arrays.<ReactPackage>asList(
+         new RNHttpServer(),
          new MainReactPackage()
      );
    }

    ......
  }
```

#### With older versions of React Native:

- in `MainActivity.java`:

```diff
+ import com.strainy.RNHttpServer.RNHttpServer;

  public class MainActivity extends ReactActivity {
    ......

    @Override
    protected List<ReactPackage> getPackages() {
      return Arrays.<ReactPackage>asList(
+       new RNHttpServer(),
        new MainReactPackage()
      );
    }
  }
```


## Release Notes

See [CHANGELOG.md](https://github.com/strainy/react-native-http-server/blob/master/CHANGELOG.md)

## Example

First import/require react-native-http-server:

```js

    var httpServer = require('react-native-http-server');

```


Initalise the server in the `componentWillMount` lifecycle method. When initalising, you'll provide an options object (only the `port` property is accepted for now) and a callback where requests will be captured and responses returned.

```js

    componentWillMount(){

      var options = {
        port: 1234, // note that 80 is reserved on Android - an exception will be thrown
      };

      // initalise the server (now accessible via localhost:1234)
      httpServer.create(options, function(request, send){

          // interpret the url
          let url = request.url.split('/');
          let ext = url[1];
          let data = JSON.stringify({data: "hello world!", extension: ext});

          //Build our response object (you can specify status, mime_type (type), data, and response headers)
          let res = {};
          res.status = "OK";
          res.type = "application/json";
          res.data = data;
          res.headers = {
            "Access-Control-Allow-Credentials": "true",
            "Access-Control-Allow-Headers": "Authorization, Content-Type, Accept, Origin, User-Agent, Cache-Control, Keep-Alive, If-Modified-Since, If-None-Match",
            "Access-Control-Allow-Methods": "GET, HEAD",
            "Access-Control-Allow-Origin": "*",
            "Access-Control-Expose-Headers": "Content-Type, Cache-Control, ETag, Expires, Last-Modified, Content-Length",
            "Access-Control-Max-Age": "3000",
            "Cache-Control": "max-age=300",
            "Connection": "keep-alive",
            "Content-Encoding": "gzip",
            "Content-Length": data.length.toString(),
            "Date": (new Date()).toUTCString(),
            "Last-Modified": (new Date()).toUTCString(),
            "Server": "Fastly",
            "Vary": "Accept-Encoding"
          };

          send(res);

      });

    }

```

Finally, ensure that you disable the server when your component is being unmounted.

```js

  componentWillUnmount() {
    httpServer.stop();
  }

```

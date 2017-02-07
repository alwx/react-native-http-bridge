package me.alwx.HttpServer;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactMethod;

import java.io.IOException;

import android.util.Log;

public class HttpServerModule extends ReactContextBaseJavaModule implements LifecycleEventListener {
    ReactApplicationContext reactContext;

    private static final String MODULE_NAME = "HttpServer";
    private static final int DEFAULT_PORT = 5561;

    private int port;
    private Server server = null;

    public HttpServerModule(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;

        port = DEFAULT_PORT;

        reactContext.addLifecycleEventListener(this);
    }

    @Override
    public String getName() {
        return MODULE_NAME;
    }

    @ReactMethod
    public void start(int port) {
        Log.d(MODULE_NAME, "Initializing server...");
        this.port = port;

        start();
    }

    private void start() {
        if (port == 0) {
            return;
        }

        server = new Server(reactContext, port);
        try {
            server.start();
        } catch (IOException e) {
            Log.e(MODULE_NAME, e.getMessage());
        }
    }

    @ReactMethod
    public void stop() {
        Log.d(MODULE_NAME, "Server stopped");
        server.stop();
        server = null;
    }

    @Override
    public void onHostResume() {
        start();
    }

    @Override
    public void onHostPause() {
        stop();
    }

    @Override
    public void onHostDestroy() {
        stop();
    }

}

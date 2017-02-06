package me.alwx.HttpServer;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.ReadableMap;
import me.alwx.HttpServer.Server;

import java.util.Map;
import java.util.HashMap;

import android.support.annotation.Nullable;
import android.util.Log;

public class HttpServerModule extends ReactContextBaseJavaModule implements LifecycleEventListener {
    ReactApplicationContext reactContext;

    private static final String TAG = "HttpServer";
    private static final String DEFAULT_PORT_KEY = "DEFAULT_PORT";
    private static final String DEFAULT_TIMEOUT_KEY = "DEFAULT_TIMEOUT";
    private static final String SERVER_EVENT_ID_KEY = "SERVER_EVENT";

    private static final int DEFAULT_PORT = 5561;
    private static final int DEFAULT_TIMEOUT = 5000;
    private static final String SERVER_EVENT_ID = "ReactNativeHttpServerResponse";

    private int port;
    private int timeout;
    private Server server = null;

    public HttpServerModule(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;

        port = DEFAULT_PORT;
        timeout = DEFAULT_TIMEOUT;

        reactContext.addLifecycleEventListener(this);
    }

    @Override
    public String getName() {
        return "HttpServer";
    }

    @Override
    public Map<String, Object> getConstants() {
        final Map<String, Object> constants = new HashMap<>();
        constants.put(DEFAULT_PORT_KEY, DEFAULT_PORT);
        constants.put(DEFAULT_TIMEOUT_KEY, DEFAULT_TIMEOUT);
        constants.put(SERVER_EVENT_ID_KEY, SERVER_EVENT_ID);
        return constants;
    }

    @ReactMethod
    public void init(ReadableMap options, Callback success, @Nullable Callback failure) {
        Log.d(TAG, "Initializing server...");

        if (options.hasKey("port")) {
            port = options.getInt("port");
        }

        if (options.hasKey("timeout")) {
            timeout = options.getInt("timeout");
        }

        start(success, failure);
    }

    @ReactMethod
    public void start(@Nullable Callback success, @Nullable Callback failure) {
        try {
            server = new Server(reactContext, port, timeout);
            server.start();

            if (success != null) {
                success.invoke();
            }
        } catch (Exception e) {
            Log.e(TAG, e.getMessage());

            if (failure != null) {
                failure.invoke(e.getMessage());
            }
        }

    }

    @ReactMethod
    public void stop() {
        Log.d(TAG, "Server Stopped.");
        server.stop();
        server = null;
    }

    @ReactMethod
    public void setResponse(String uri, ReadableMap response) {
        if (server != null) {
            server.setResponse(uri, response);
        }
    }

    @ReactMethod
    public String getHostName() {
        if (server != null) {
            Log.d(TAG, server.getHostname());
            return server.getHostname();
        } else {
            return "not defined";
        }
    }

    /* Shut down the server if app is destroyed or paused */
    @Override
    public void onHostResume() {
        //we can restart the server here as the success callback is not needed since an event is registered
        start(null, null);
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

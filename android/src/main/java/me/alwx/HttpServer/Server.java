package me.alwx.HttpServer;

import fi.iki.elonen.NanoHTTPD;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;

import java.util.Map;
import java.util.Set;
import java.util.HashMap;

import android.support.annotation.Nullable;
import android.util.Log;

public class Server extends NanoHTTPD {
    private static final String TAG = "HttpServer";
    private static final String SERVER_EVENT_ID = "httpServerResponseReceived";

    private Map<String, ReadableMap> response;
    private ReactContext reactContext;

    public Server(ReactContext context, int port) {
        super(port);
        reactContext = context;
        response = new HashMap<>();

        Log.d(TAG, "Server started");
    }

    @Override
    public Response serve(IHTTPSession session) {
        Log.d(TAG, "Request received!");

        WritableMap request;
        try {
            request = fillRequestMap(session);
        } catch (Exception e) {
            return newFixedLengthResponse(
                    Response.Status.INTERNAL_ERROR, MIME_PLAINTEXT, e.getMessage()
            );
        }

        this.sendEvent(reactContext, SERVER_EVENT_ID, request);
        return newFixedLengthResponse(Response.Status.OK, MIME_PLAINTEXT, "OK");
    }

    private WritableMap fillRequestMap(IHTTPSession session) throws Exception {
        Method method = session.getMethod();
        WritableMap request = Arguments.createMap();
        request.putString("url", session.getUri());

        Map<String, String> files = new HashMap<>();
        if (Method.POST.equals(method)) {
            session.parseBody(files);
            if (files.size() > 0) {
                request.putString("postData", files.get("postData"));
            }
        }
        return request;
    }

    private void sendEvent(ReactContext reactContext, String eventName, @Nullable WritableMap params) {
        reactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class).emit(eventName, params);
    }
}

package me.alwx.HttpServer;

import fi.iki.elonen.NanoHTTPD;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;

import java.io.IOException;
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

    private int timeout;

    public Server(ReactContext context, int port, int timeout) {
        super(port);
        reactContext = context;
        this.timeout = timeout;
        response = new HashMap<>();

        Log.d(TAG, "Server started");
    }

    public void setResponse(String uri, ReadableMap response) {
        this.response.put(uri, response);
    }

    @Override
    public Response serve(IHTTPSession session) {
        Log.d(TAG, "Request received!");

        WritableMap request;
        try {
            request = fillRequestMap(session);
        } catch (IOException e) {
            return newFixedLengthResponse(
                    Response.Status.INTERNAL_ERROR, MIME_PLAINTEXT,
                    "SERVER INTERNAL ERROR: IOException: " + e.getMessage()
            );
        } catch (ResponseException e) {
            return newFixedLengthResponse(e.getStatus(), MIME_PLAINTEXT, e.getMessage());
        }

        this.sendEvent(reactContext, SERVER_EVENT_ID, request);
        return waitForResponse(session);
    }

    private WritableMap fillRequestMap(IHTTPSession session) throws IOException, ResponseException {
        Method method = session.getMethod();
        WritableMap request = Arguments.createMap();
        request.putString("url", session.getUri());
        request.putString("method", method.toString());
        request.putMap("headers", this.convertToWritableMap(session.getHeaders()));

        Map<String, String> files = new HashMap<>();
        if (Method.POST.equals(method)) {
            session.parseBody(files);
            if (files.size() > 0) {
                request.putString("postData", files.get("postData"));
            }
        }
        return request;
    }

    private Response waitForResponse(IHTTPSession session) {
        Response.Status errorStatus = null;
        String errorText = "";
        int timer = 0;
        int interval = 500;
        ReadableMap response;

        while (!this.response.containsKey(session.getUri()) && (timer < timeout)) {
            try {
                Thread.sleep(interval);
            } catch (InterruptedException e) {
                Log.e(TAG, e.getMessage());
                errorStatus = Response.Status.INTERNAL_ERROR;
                errorText = e.getMessage();
                break;
            }

            timer = timer + interval;
        }

        if (!this.response.containsKey(session.getUri())) {
            errorStatus = Response.Status.NOT_FOUND;
            errorText = "Resource not found";
        }

        if (errorStatus != null) {
            return newFixedLengthResponse(errorStatus, MIME_PLAINTEXT, errorText);
        } else {
            response = this.response.get(session.getUri());
            this.response.remove(session.getUri()); // clear responses
        }

        Log.d(TAG, "Sending response for " + session.getUri());

        Response res = newFixedLengthResponse(
                Response.Status.valueOf(response.getString("status")),
                response.getString("type"),
                response.getString("data"));

        ReadableMap headers = response.getMap("headers");
        com.facebook.react.bridge.ReadableMapKeySetIterator iterator = headers.keySetIterator();
        if (!iterator.hasNextKey()) {
            return null;
        }
        while (iterator.hasNextKey()) {
            String key = iterator.nextKey();
            Log.d(TAG, key + "=" + headers.getString(key));
            res.addHeader(key, headers.getString(key));
        }

        return res;
    }

    private void sendEvent(ReactContext reactContext, String eventName, @Nullable WritableMap params) {
        reactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class).emit(eventName, params);
    }

    private WritableMap convertToWritableMap(Map map) {
        WritableMap request = Arguments.createMap();

        Set<Map.Entry<String, String>> entrySet = map.entrySet();
        for (Map.Entry entry : entrySet) {
            switch (entry.getValue().getClass().getName()) {
                case "java.lang.Boolean":
                    request.putBoolean(entry.getKey().toString(), (Boolean) entry.getValue());
                    break;
                case "java.lang.Integer":
                    request.putInt(entry.getKey().toString(), (Integer) entry.getValue());
                    break;
                case "java.lang.Double":
                    request.putDouble(entry.getKey().toString(), (Double) entry.getValue());
                    break;
                case "java.lang.String":
                    request.putString(entry.getKey().toString(), (String) entry.getValue());
                    break;
            }
        }

        return request;
    }
}

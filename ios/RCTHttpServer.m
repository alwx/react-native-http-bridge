#import "RCTHttpServer.h"
#import "React/RCTBridge.h"
#import "React/RCTLog.h"
#import "React/RCTEventDispatcher.h"

#import "WGCDWebServer.h"
#import "WGCDWebServerDataResponse.h"
#import "WGCDWebServerDataRequest.h"
#import "WGCDWebServerPrivate.h"
#include <stdlib.h>

@interface RCTHttpServer : NSObject <RCTBridgeModule> {
    WGCDWebServer* _webServer;
    NSMutableDictionary* _requestResponses;
}
@end

static RCTBridge *bridge;

@implementation RCTHttpServer

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE();


- (void)initResponseReceivedFor:(WGCDWebServer *)server forType:(NSString*)type {
    [server addDefaultHandlerForMethod:type requestClass:[WGCDWebServerDataRequest class] processBlock:^WGCDWebServerResponse *(WGCDWebServerRequest* request) {
        
        long long milliseconds = (long long)([[NSDate date] timeIntervalSince1970] * 1000.0);
        int r = arc4random_uniform(1000000);
        NSString *requestId = [NSString stringWithFormat:@"%lld:%d", milliseconds, r];

        // it's a weird way of doing it, fix it
        @try {
            if ([WGCDWebServerTruncateHeaderValue(request.contentType) isEqualToString:@"application/json"]) {
                WGCDWebServerDataRequest* dataRequest = (WGCDWebServerDataRequest*)request;
                [self.bridge.eventDispatcher sendAppEventWithName:@"httpServerResponseReceived"
                                                             body:@{@"requestId": requestId,
                                                                    @"postData": dataRequest.jsonObject,
                                                                    @"type": type,
                                                                    @"url": request.URL.relativeString}];
            } else {
                [self.bridge.eventDispatcher sendAppEventWithName:@"httpServerResponseReceived"
                                                             body:@{@"requestId": requestId,
                                                                    @"type": type,
                                                                    @"url": request.URL.relativeString}];
            }
        } @catch (NSException *exception) {
            [self.bridge.eventDispatcher sendAppEventWithName:@"httpServerResponseReceived"
                                                         body:@{@"requestId": requestId,
                                                                @"type": type,
                                                                @"url": request.URL.relativeString}];
        }
        
        while ([_requestResponses objectForKey:requestId] == NULL) {
            [NSThread sleepForTimeInterval:0.01f];
        }
    
        WGCDWebServerDataResponse* response = [_requestResponses objectForKey:requestId];
        [_requestResponses removeObjectForKey:requestId];
        return response;
    }];
}

RCT_EXPORT_METHOD(start:(NSInteger) port
                  serviceName:(NSString *) serviceName)
{
    RCTLogInfo(@"Running HTTP bridge server: %ld", port);
    _requestResponses = [[NSMutableDictionary alloc] init];
    
    dispatch_sync(dispatch_get_main_queue(), ^{
        _webServer = [[WGCDWebServer alloc] init];
        
        [self initResponseReceivedFor:_webServer forType:@"POST"];
        [self initResponseReceivedFor:_webServer forType:@"PUT"];
        [self initResponseReceivedFor:_webServer forType:@"GET"];
        [self initResponseReceivedFor:_webServer forType:@"DELETE"];
        
        [_webServer startWithPort:port bonjourName:serviceName];
    });
}

RCT_EXPORT_METHOD(stop)
{
    RCTLogInfo(@"Stopping HTTP bridge server");
    
    if (_webServer != nil) {
        [_webServer stop];
        [_webServer removeAllHandlers];
        _webServer = nil;
    }
}

RCT_EXPORT_METHOD(respond: (NSString *) requestId
                  code: (NSInteger) code
                  type: (NSString *) type
                  body: (NSString *) body)
{
    NSData* data = [body dataUsingEncoding:NSUTF8StringEncoding];
    WGCDWebServerDataResponse* requestResponse = [[WGCDWebServerDataResponse alloc] initWithData:data contentType:type];
    requestResponse.statusCode = code;
    
    [_requestResponses setObject:requestResponse forKey:requestId];
}

@end

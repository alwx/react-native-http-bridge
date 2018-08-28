#import "RCTHttpServer.h"
#import "React/RCTBridge.h"
#import "React/RCTLog.h"
#import "React/RCTEventDispatcher.h"

#import "WGCDWebServer.h"
#import "WGCDWebServerDataResponse.h"
#import "WGCDWebServerDataRequest.h"

@interface RCTHttpServer : NSObject <RCTBridgeModule> {
    WGCDWebServer* _webServer;
    WGCDWebServerDataResponse* _requestResponse;
}
@end

static RCTBridge *bridge;

@implementation RCTHttpServer

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE();


- (void)initResponseReceivedFor:(WGCDWebServer *)server forType:(NSString*)type {
    [server addDefaultHandlerForMethod:type requestClass:[WGCDWebServerDataRequest class] processBlock:^WGCDWebServerResponse *(WGCDWebServerRequest* request) {
        WGCDWebServerDataRequest* dataRequest = (WGCDWebServerDataRequest*)request;
        _requestResponse = NULL;
        
        [self.bridge.eventDispatcher sendAppEventWithName:@"httpServerResponseReceived"
                                                     body:@{@"postData": dataRequest.jsonObject,
                                                            @"type": type,
                                                            @"url": dataRequest.URL.relativeString}];
        while (_requestResponse == NULL) {
            [NSThread sleepForTimeInterval:0.01f];
        }
        return _requestResponse;
    }];
}

RCT_EXPORT_METHOD(start:(NSInteger) port
                  serviceName:(NSString *) serviceName)
{
    RCTLogInfo(@"Running HTTP bridge server: %d", port);
    
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

RCT_EXPORT_METHOD(respond:(NSInteger) code
                  type: (NSString *) type
                  body: (NSString *) body)
{
    NSData* data = [body dataUsingEncoding:NSUTF8StringEncoding];
    _requestResponse = [[WGCDWebServerDataResponse alloc] initWithData:data contentType:type];
    _requestResponse.statusCode = code;
}

@end

#import "RCTHttpServer.h"
#import "RCTBridge.h"
#import "RCTLog.h"
#import "RCTEventDispatcher.h"

#import "WGCDWebServer.h"
#import "WGCDWebServerDataResponse.h"
#import "WGCDWebServerDataRequest.h"

@interface RCTHttpServer : NSObject <RCTBridgeModule> {
    WGCDWebServer* _webServer;
}
@end

static RCTBridge *bridge;

@implementation RCTHttpServer

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE();

RCT_EXPORT_METHOD(start:(NSInteger) port)
{
    RCTLogInfo(@"Running HTTP bridge server: %d", port);
    
    dispatch_sync(dispatch_get_main_queue(), ^{
        _webServer = [[WGCDWebServer alloc] init];
        
        [_webServer addDefaultHandlerForMethod:@"POST"
                    requestClass:[WGCDWebServerDataRequest class]
                    processBlock:^WGCDWebServerResponse *(WGCDWebServerRequest* request) {
                        
            WGCDWebServerDataRequest* dataRequest = (WGCDWebServerDataRequest*)request;
                        
            [self.bridge.eventDispatcher sendAppEventWithName:@"httpServerResponseReceived"
                                                         body:@{@"postData": dataRequest.jsonObject,
                                                                     @"url": dataRequest.URL.relativeString}];
                        
            return [WGCDWebServerDataResponse responseWithStatusCode:200];
        }];
        
        [_webServer startWithPort:port bonjourName:nil];
    });
}

RCT_EXPORT_METHOD(stop)
{
    RCTLogInfo(@"Stopping HTTP bridge server");
    
    dispatch_sync(dispatch_get_main_queue(), ^{
        if (_webServer) {
            [_webServer stop];
        }
    });
}

@end

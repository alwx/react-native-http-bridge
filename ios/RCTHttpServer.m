#import "RCTHttpServer.h"
#import "React/RCTLog.h"
#import "WGCDWebServerDataRequest.h"

@implementation RCTHttpServer {
    WGCDWebServer* _webServer;
    WGCDWebServerDataResponse* _requestResponse;
}

RCT_EXPORT_MODULE();

- (NSArray<NSString *> *)supportedEvents
{
    return @[@"httpServerResponseReceived"];
}


RCT_EXPORT_METHOD(start:(NSInteger) port
                  serviceName:(NSString *) serviceName)
{
    RCTLogInfo(@"Running HTTP bridge server: %ld", (long)port);
    
    __weak __typeof(self) weakSelf = self;

    dispatch_sync(dispatch_get_main_queue(), ^{
        _webServer = [[WGCDWebServer alloc] init];
    

        [_webServer addDefaultHandlerForMethod:@"POST"
                    requestClass:[WGCDWebServerDataRequest class]
                    processBlock:^WGCDWebServerResponse *(WGCDWebServerRequest* request) {
                        
            id payload = nil;
            _requestResponse = NULL;
            if (request.hasBody) {
                WGCDWebServerDataRequest* dataRequest = (WGCDWebServerDataRequest*)request;
                payload = dataRequest.jsonObject;
            }

            [weakSelf sendEventWithName:@"httpServerResponseReceived"
                               body:@{
                                 @"postData" : payload,
                                 @"url" : request.URL.relativeString
                               }];

            while (_requestResponse == NULL) {
              [NSThread sleepForTimeInterval:0.1f];
            }

            return _requestResponse;
        }];

        [_webServer startWithPort:port bonjourName:serviceName];
    });
}

RCT_EXPORT_METHOD(stop)
{
    RCTLogInfo(@"Stopping HTTP bridge server");

    //dispatch_sync(dispatch_get_main_queue(), ^{
        if (_webServer != nil) {
            [_webServer stop];
            [_webServer removeAllHandlers];
            _webServer = nil;
        }
    //});
}

RCT_EXPORT_METHOD(respond:(NSInteger) code
                  type: (NSString *) type
                  body: (NSString *) body)
{
    NSData* data = [body dataUsingEncoding:NSUTF8StringEncoding];
    _requestResponse = [[WGCDWebServerDataResponse alloc] initWithData:data contentType:type];
}

@end

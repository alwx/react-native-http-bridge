/*
 Copyright (c) 2012-2015, Pierre-Olivier Latour
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * The name of Pierre-Olivier Latour may not be used to endorse
 or promote products derived from this software without specific
 prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL PIERRE-OLIVIER LATOUR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <os/object.h>
#import <sys/socket.h>

/**
 *  All WGCDWebServer headers.
 */

#import "WGCDWebServerHTTPStatusCodes.h"
#import "WGCDWebServerFunctions.h"

#import "WGCDWebServer.h"
#import "WGCDWebServerConnection.h"

#import "WGCDWebServerDataRequest.h"
#import "WGCDWebServerFileRequest.h"
#import "WGCDWebServerMultiPartFormRequest.h"
#import "WGCDWebServerURLEncodedFormRequest.h"

#import "WGCDWebServerDataResponse.h"
#import "WGCDWebServerErrorResponse.h"
#import "WGCDWebServerFileResponse.h"
#import "WGCDWebServerStreamedResponse.h"

/**
 *  Check if a custom logging facility should be used instead.
 */

#if defined(__WGCDWEBSERVER_LOGGING_HEADER__)

#define __WGCDWEBSERVER_LOGGING_FACILITY_CUSTOM__

#import __WGCDWEBSERVER_LOGGING_HEADER__

/**
 *  Automatically detect if XLFacility is available and if so use it as a
 *  logging facility.
 */

#elif defined(__has_include) && __has_include("XLFacilityMacros.h")

#define __WGCDWEBSERVER_LOGGING_FACILITY_XLFACILITY__

#undef XLOG_TAG
#define XLOG_TAG @"gcdwebserver.internal"

#import "XLFacilityMacros.h"

#define GWS_LOG_DEBUG(...) XLOG_DEBUG(__VA_ARGS__)
#define GWS_LOG_VERBOSE(...) XLOG_VERBOSE(__VA_ARGS__)
#define GWS_LOG_INFO(...) XLOG_INFO(__VA_ARGS__)
#define GWS_LOG_WARNING(...) XLOG_WARNING(__VA_ARGS__)
#define GWS_LOG_ERROR(...) XLOG_ERROR(__VA_ARGS__)

#define GWS_DCHECK(__CONDITION__) XLOG_DEBUG_CHECK(__CONDITION__)
#define GWS_DNOT_REACHED() XLOG_DEBUG_UNREACHABLE()

/**
 *  Automatically detect if CocoaLumberJack is available and if so use
 *  it as a logging facility.
 */

#elif defined(__has_include) && __has_include("CocoaLumberjack/CocoaLumberjack.h")

#import <CocoaLumberjack/CocoaLumberjack.h>

#define __WGCDWEBSERVER_LOGGING_FACILITY_COCOALUMBERJACK__

#undef LOG_LEVEL_DEF
#define LOG_LEVEL_DEF WGCDWebServerLogLevel
extern DDLogLevel WGCDWebServerLogLevel;

#define GWS_LOG_DEBUG(...) DDLogDebug(__VA_ARGS__)
#define GWS_LOG_VERBOSE(...) DDLogVerbose(__VA_ARGS__)
#define GWS_LOG_INFO(...) DDLogInfo(__VA_ARGS__)
#define GWS_LOG_WARNING(...) DDLogWarn(__VA_ARGS__)
#define GWS_LOG_ERROR(...) DDLogError(__VA_ARGS__)

/**
 *  If all of the above fail, then use WGCDWebServer built-in
 *  logging facility.
 */

#else

#define __WGCDWEBSERVER_LOGGING_FACILITY_BUILTIN__

typedef NS_ENUM(int, WGCDWebServerLoggingLevel) {
  kWGCDWebServerLoggingLevel_Debug = 0,
  kWGCDWebServerLoggingLevel_Verbose,
  kWGCDWebServerLoggingLevel_Info,
  kWGCDWebServerLoggingLevel_Warning,
  kWGCDWebServerLoggingLevel_Error
};

extern WGCDWebServerLoggingLevel WGCDWebServerLogLevel;
extern void WGCDWebServerLogMessage(WGCDWebServerLoggingLevel level, NSString* format, ...) NS_FORMAT_FUNCTION(2, 3);

#if DEBUG
#define GWS_LOG_DEBUG(...) do { if (WGCDWebServerLogLevel <= kWGCDWebServerLoggingLevel_Debug) WGCDWebServerLogMessage(kWGCDWebServerLoggingLevel_Debug, __VA_ARGS__); } while (0)
#else
#define GWS_LOG_DEBUG(...)
#endif
#define GWS_LOG_VERBOSE(...) do { if (WGCDWebServerLogLevel <= kWGCDWebServerLoggingLevel_Verbose) WGCDWebServerLogMessage(kWGCDWebServerLoggingLevel_Verbose, __VA_ARGS__); } while (0)
#define GWS_LOG_INFO(...) do { if (WGCDWebServerLogLevel <= kWGCDWebServerLoggingLevel_Info) WGCDWebServerLogMessage(kWGCDWebServerLoggingLevel_Info, __VA_ARGS__); } while (0)
#define GWS_LOG_WARNING(...) do { if (WGCDWebServerLogLevel <= kWGCDWebServerLoggingLevel_Warning) WGCDWebServerLogMessage(kWGCDWebServerLoggingLevel_Warning, __VA_ARGS__); } while (0)
#define GWS_LOG_ERROR(...) do { if (WGCDWebServerLogLevel <= kWGCDWebServerLoggingLevel_Error) WGCDWebServerLogMessage(kWGCDWebServerLoggingLevel_Error, __VA_ARGS__); } while (0)

#endif

/**
 *  Consistency check macros used when building Debug only.
 */

#if !defined(GWS_DCHECK) || !defined(GWS_DNOT_REACHED)

#if DEBUG

#define GWS_DCHECK(__CONDITION__) \
  do { \
    if (!(__CONDITION__)) { \
      abort(); \
    } \
  } while (0)
#define GWS_DNOT_REACHED() abort()

#else

#define GWS_DCHECK(__CONDITION__)
#define GWS_DNOT_REACHED()

#endif

#endif

/**
 *  WGCDWebServer internal constants and APIs.
 */

#define kWGCDWebServerDefaultMimeType @"application/octet-stream"
#define kWGCDWebServerErrorDomain @"WGCDWebServerErrorDomain"

static inline BOOL WGCDWebServerIsValidByteRange(NSRange range) {
  return ((range.location != NSUIntegerMax) || (range.length > 0));
}

static inline NSError* WGCDWebServerMakePosixError(int code) {
  return [NSError errorWithDomain:NSPOSIXErrorDomain code:code userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithUTF8String:strerror(code)]}];
}

extern void WGCDWebServerInitializeFunctions();
extern NSString* WGCDWebServerNormalizeHeaderValue(NSString* value);
extern NSString* WGCDWebServerTruncateHeaderValue(NSString* value);
extern NSString* WGCDWebServerExtractHeaderValueParameter(NSString* header, NSString* attribute);
extern NSStringEncoding WGCDWebServerStringEncodingFromCharset(NSString* charset);
extern BOOL WGCDWebServerIsTextContentType(NSString* type);
extern NSString* WGCDWebServerDescribeData(NSData* data, NSString* contentType);
extern NSString* WGCDWebServerComputeMD5Digest(NSString* format, ...) NS_FORMAT_FUNCTION(1,2);
extern NSString* WGCDWebServerStringFromSockAddr(const struct sockaddr* addr, BOOL includeService);

@interface WGCDWebServerConnection ()
- (id)initWithServer:(WGCDWebServer*)server localAddress:(NSData*)localAddress remoteAddress:(NSData*)remoteAddress socket:(CFSocketNativeHandle)socket;
@end

@interface WGCDWebServer ()
@property(nonatomic, readonly) NSArray* handlers;
@property(nonatomic, readonly) NSString* serverName;
@property(nonatomic, readonly) NSString* authenticationRealm;
@property(nonatomic, readonly) NSDictionary* authenticationBasicAccounts;
@property(nonatomic, readonly) NSDictionary* authenticationDigestAccounts;
@property(nonatomic, readonly) BOOL shouldAutomaticallyMapHEADToGET;
@property(nonatomic, readonly) dispatch_queue_priority_t dispatchQueuePriority;
- (void)willStartConnection:(WGCDWebServerConnection*)connection;
- (void)didEndConnection:(WGCDWebServerConnection*)connection;
@end

@interface WGCDWebServerHandler : NSObject
@property(nonatomic, readonly) WGCDWebServerMatchBlock matchBlock;
@property(nonatomic, readonly) WGCDWebServerAsyncProcessBlock asyncProcessBlock;
@end

@interface WGCDWebServerRequest ()
@property(nonatomic, readonly) BOOL usesChunkedTransferEncoding;
@property(nonatomic, readwrite) NSData* localAddressData;
@property(nonatomic, readwrite) NSData* remoteAddressData;
- (void)prepareForWriting;
- (BOOL)performOpen:(NSError**)error;
- (BOOL)performWriteData:(NSData*)data error:(NSError**)error;
- (BOOL)performClose:(NSError**)error;
- (void)setAttribute:(id)attribute forKey:(NSString*)key;
@end

@interface WGCDWebServerResponse ()
@property(nonatomic, readonly) NSDictionary* additionalHeaders;
@property(nonatomic, readonly) BOOL usesChunkedTransferEncoding;
- (void)prepareForReading;
- (BOOL)performOpen:(NSError**)error;
- (void)performReadDataWithCompletion:(WGCDWebServerBodyReaderCompletionBlock)block;
- (void)performClose;
@end

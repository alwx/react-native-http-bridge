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

// http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
// http://www.iana.org/assignments/http-status-codes/http-status-codes.xhtml

#import <Foundation/Foundation.h>

/**
 *  Convenience constants for "informational" HTTP status codes.
 */
typedef NS_ENUM(NSInteger, WGCDWebServerInformationalHTTPStatusCode) {
  kWGCDWebServerHTTPStatusCode_Continue = 100,
  kWGCDWebServerHTTPStatusCode_SwitchingProtocols = 101,
  kWGCDWebServerHTTPStatusCode_Processing = 102
};

/**
 *  Convenience constants for "successful" HTTP status codes.
 */
typedef NS_ENUM(NSInteger, WGCDWebServerSuccessfulHTTPStatusCode) {
  kWGCDWebServerHTTPStatusCode_OK = 200,
  kWGCDWebServerHTTPStatusCode_Created = 201,
  kWGCDWebServerHTTPStatusCode_Accepted = 202,
  kWGCDWebServerHTTPStatusCode_NonAuthoritativeInformation = 203,
  kWGCDWebServerHTTPStatusCode_NoContent = 204,
  kWGCDWebServerHTTPStatusCode_ResetContent = 205,
  kWGCDWebServerHTTPStatusCode_PartialContent = 206,
  kWGCDWebServerHTTPStatusCode_MultiStatus = 207,
  kWGCDWebServerHTTPStatusCode_AlreadyReported = 208
};

/**
 *  Convenience constants for "redirection" HTTP status codes.
 */
typedef NS_ENUM(NSInteger, WGCDWebServerRedirectionHTTPStatusCode) {
  kWGCDWebServerHTTPStatusCode_MultipleChoices = 300,
  kWGCDWebServerHTTPStatusCode_MovedPermanently = 301,
  kWGCDWebServerHTTPStatusCode_Found = 302,
  kWGCDWebServerHTTPStatusCode_SeeOther = 303,
  kWGCDWebServerHTTPStatusCode_NotModified = 304,
  kWGCDWebServerHTTPStatusCode_UseProxy = 305,
  kWGCDWebServerHTTPStatusCode_TemporaryRedirect = 307,
  kWGCDWebServerHTTPStatusCode_PermanentRedirect = 308
};

/**
 *  Convenience constants for "client error" HTTP status codes.
 */
typedef NS_ENUM(NSInteger, WGCDWebServerClientErrorHTTPStatusCode) {
  kWGCDWebServerHTTPStatusCode_BadRequest = 400,
  kWGCDWebServerHTTPStatusCode_Unauthorized = 401,
  kWGCDWebServerHTTPStatusCode_PaymentRequired = 402,
  kWGCDWebServerHTTPStatusCode_Forbidden = 403,
  kWGCDWebServerHTTPStatusCode_NotFound = 404,
  kWGCDWebServerHTTPStatusCode_MethodNotAllowed = 405,
  kWGCDWebServerHTTPStatusCode_NotAcceptable = 406,
  kWGCDWebServerHTTPStatusCode_ProxyAuthenticationRequired = 407,
  kWGCDWebServerHTTPStatusCode_RequestTimeout = 408,
  kWGCDWebServerHTTPStatusCode_Conflict = 409,
  kWGCDWebServerHTTPStatusCode_Gone = 410,
  kWGCDWebServerHTTPStatusCode_LengthRequired = 411,
  kWGCDWebServerHTTPStatusCode_PreconditionFailed = 412,
  kWGCDWebServerHTTPStatusCode_RequestEntityTooLarge = 413,
  kWGCDWebServerHTTPStatusCode_RequestURITooLong = 414,
  kWGCDWebServerHTTPStatusCode_UnsupportedMediaType = 415,
  kWGCDWebServerHTTPStatusCode_RequestedRangeNotSatisfiable = 416,
  kWGCDWebServerHTTPStatusCode_ExpectationFailed = 417,
  kWGCDWebServerHTTPStatusCode_UnprocessableEntity = 422,
  kWGCDWebServerHTTPStatusCode_Locked = 423,
  kWGCDWebServerHTTPStatusCode_FailedDependency = 424,
  kWGCDWebServerHTTPStatusCode_UpgradeRequired = 426,
  kWGCDWebServerHTTPStatusCode_PreconditionRequired = 428,
  kWGCDWebServerHTTPStatusCode_TooManyRequests = 429,
  kWGCDWebServerHTTPStatusCode_RequestHeaderFieldsTooLarge = 431
};

/**
 *  Convenience constants for "server error" HTTP status codes.
 */
typedef NS_ENUM(NSInteger, WGCDWebServerServerErrorHTTPStatusCode) {
  kWGCDWebServerHTTPStatusCode_InternalServerError = 500,
  kWGCDWebServerHTTPStatusCode_NotImplemented = 501,
  kWGCDWebServerHTTPStatusCode_BadGateway = 502,
  kWGCDWebServerHTTPStatusCode_ServiceUnavailable = 503,
  kWGCDWebServerHTTPStatusCode_GatewayTimeout = 504,
  kWGCDWebServerHTTPStatusCode_HTTPVersionNotSupported = 505,
  kWGCDWebServerHTTPStatusCode_InsufficientStorage = 507,
  kWGCDWebServerHTTPStatusCode_LoopDetected = 508,
  kWGCDWebServerHTTPStatusCode_NotExtended = 510,
  kWGCDWebServerHTTPStatusCode_NetworkAuthenticationRequired = 511
};

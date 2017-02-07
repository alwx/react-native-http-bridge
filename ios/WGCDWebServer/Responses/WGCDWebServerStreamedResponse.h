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

#import "WGCDWebServerResponse.h"

/**
 *  The WGCDWebServerStreamBlock is called to stream the data for the HTTP body.
 *  The block must return either a chunk of data, an empty NSData when done, or
 *  nil on error and set the "error" argument which is guaranteed to be non-NULL.
 */
typedef NSData* (^WGCDWebServerStreamBlock)(NSError** error);

/**
 *  The WGCDWebServerAsyncStreamBlock works like the WGCDWebServerStreamBlock
 *  except the streamed data can be returned at a later time allowing for
 *  truly asynchronous generation of the data.
 *
 *  The block must call "completionBlock" passing the new chunk of data when ready,
 *  an empty NSData when done, or nil on error and pass a NSError.
 *
 *  The block cannot call "completionBlock" more than once per invocation.
 */
typedef void (^WGCDWebServerAsyncStreamBlock)(WGCDWebServerBodyReaderCompletionBlock completionBlock);

/**
 *  The WGCDWebServerStreamedResponse subclass of WGCDWebServerResponse streams
 *  the body of the HTTP response using a WGCD block.
 */
@interface WGCDWebServerStreamedResponse : WGCDWebServerResponse

/**
 *  Creates a response with streamed data and a given content type.
 */
+ (instancetype)responseWithContentType:(NSString*)type streamBlock:(WGCDWebServerStreamBlock)block;

/**
 *  Creates a response with async streamed data and a given content type.
 */
+ (instancetype)responseWithContentType:(NSString*)type asyncStreamBlock:(WGCDWebServerAsyncStreamBlock)block;

/**
 *  Initializes a response with streamed data and a given content type.
 */
- (instancetype)initWithContentType:(NSString*)type streamBlock:(WGCDWebServerStreamBlock)block;

/**
 *  This method is the designated initializer for the class.
 */
- (instancetype)initWithContentType:(NSString*)type asyncStreamBlock:(WGCDWebServerAsyncStreamBlock)block;

@end

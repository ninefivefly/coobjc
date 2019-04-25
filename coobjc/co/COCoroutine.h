//
//  COCoroutine.h
//  coobjc
//
//  Copyright © 2018 Alibaba Group Holding Limited All rights reserved.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#import <Foundation/Foundation.h>
#import <cocore/coroutine.h>
#import <cocore/CODispatch.h>
#import <coobjc/COChan.h>
#import <coobjc/COPromise.h>

NS_ASSUME_NONNULL_BEGIN

@class COCoroutine;

/**
 Wait a  `COPromise` or `COChan` object, until Promise is fulfilled/rejected, or Channel has value send.

 @param awaitable `COPromise` or `COChan` object
 @return If await Promise, return fulfilled value; elseif Channel, return sent value.
 */
id _Nullable co_await(id  awaitable);

/**
 Await a list of awaitable object asynchronous. The tasks is concurrent.

 @param awaitableArray a list of awaitable
 @return the value result list. `nil` is replaced by `NSNull`.
 */
NSArray *_Nullable co_batch_await(NSArray *awaitableArray);

/**
 Get the Coroutine ObjC object from the struct.

 @param co the coroutine struct.
 @return `COCorutine` instance.
 */
COCoroutine *co_get_obj(coroutine_t  *co);

/**
 Get the lastError generated by Promise.

 @return NSError object.
 */
NSError *co_getError(void);

/**
 `co_setspecific` and `co_getspecific` help you to store a value in the context.
 */
BOOL co_setspecific(NSString * key, id _Nullable value);
id _Nullable co_getspecific(NSString * key);

extern NSString *const COInvalidException;

/**
 COCoroutine is the object owned coroutine.
 */
@interface COCoroutine: NSObject

/**
 Call back when coroutine is finished.
 */
@property(nonatomic, copy, nullable) dispatch_block_t finishedBlock;

/**
 The code body of the coroutine.
 */
@property(nonatomic, copy) void (^execBlock) (void);

/**
 The `dispatch_queue_t` coroutine will run on it.
 */
@property(nonatomic, strong) CODispatch* dispatch;

/**
 The struct pointer of coroutine_t
 */
@property(nonatomic, assign, nullable) coroutine_t  *co;

/**
 If `COCoroutine is suspend by a Channel, this pointer mark it.
 */
@property(nonatomic, strong, nullable) COChan *currentChan;

/**
 The lastError marked in the Coroutine.
 */
@property(nonatomic, strong, nullable) NSError *lastError;

/**
 Get the current running coroutine object.

 @return The coroutine object.
 */
+ (COCoroutine *_Nullable)currentCoroutine;

/**
 Tell if the coroutine is cancelled or finished.

 @return isActive
 */
+ (BOOL)isActive;

/**
 Create coroutine instance.

 @param block exec code body
 @param queue the `dispatch_queue_t`
 @return The coroutine object.
 */
+ (instancetype)coroutineWithBlock:(void(^)(void))block onQueue:(dispatch_queue_t _Nullable)queue;
    

/**
 Create coroutine instance with custom stack size, in case stackSize not enough.
 The stack size is 65536 by default.

 @param block : the code
 @param queue : the queue code run
 @param stackSize : stackSize of the coroutine.
 @return The coroutine object.
 */
+ (_Nullable instancetype)coroutineWithBlock:(void(^)(void))block onQueue:(dispatch_queue_t _Nullable)queue stackSize:(NSUInteger)stackSize;

/**
 Create coroutine instance with custom stack size, in case stackSize not enough.
 The stack size is 65536 by default.
 
 @param block : the code
 @param queue : the queue code run
 @param stackSize : stackSize of the coroutine.
 @return The coroutine object.
 */
- (instancetype)initWithBlock:(void (^)(void))block onQueue:(dispatch_queue_t _Nullable)queue stackSize:(NSUInteger)stackSize;

/**
 The coroutine is Finished.
 */
- (BOOL)isFinished;

/**
 The coroutine is cancelled.
 */
- (BOOL)isCancelled;

/**
 The coroutine is resumed.
 */
- (BOOL)isResume;

/**
 Cancel the coroutine.
 */
- (void)cancel;

/**
 Calling this method in another coroutine. wait the coroutine to be finished.
 */
- (void)join;

/**
 Calling this method in another coroutine. Cancel the coroutine, and wait the coroutine to be finished.
 */
- (void)cancelAndJoin;

/**
 Resume the coroutine.

 @return The coroutine object.
 */
- (COCoroutine *_Nonnull)resume;

/**
 Resume the coroutine immediately, in current runloop or another queue.
 */
- (void)resumeNow;

/**
 Add to scheduler, if scheduler is idle, resume it.
 */
- (void)addToScheduler;

@end

NS_ASSUME_NONNULL_END

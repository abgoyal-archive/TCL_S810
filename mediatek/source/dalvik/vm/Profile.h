

#ifndef DALVIK_PROFILE_H_
#define DALVIK_PROFILE_H_

#ifndef NOT_VM      /* for utilities that sneakily include this file */

#include <stdio.h>

struct Thread;      // extern


/* boot init */
bool dvmProfilingStartup(void);
void dvmProfilingShutdown(void);

struct MethodTraceState {
    /* active state */
    pthread_mutex_t startStopLock;
    pthread_cond_t  threadExitCond;
    FILE*   traceFile;
    bool    directToDdms;
    int     bufferSize;
    int     flags;

    int     traceEnabled;
    u1*     buf;
    volatile int curOffset;
    u8      startWhen;
    int     overflow;

    int     traceVersion;
    size_t  recordSize;
};

struct AllocProfState {
    bool    enabled;            // is allocation tracking enabled?

    int     allocCount;         // #of objects allocated
    int     allocSize;          // cumulative size of objects

    int     failedAllocCount;   // #of times an allocation failed
    int     failedAllocSize;    // cumulative size of failed allocations

    int     freeCount;          // #of objects freed
    int     freeSize;           // cumulative size of freed objects

    int     gcCount;            // #of times an allocation triggered a GC

    int     classInitCount;     // #of initialized classes
    u8      classInitTime;      // cumulative time spent in class init (nsec)
};


void dvmMethodTraceStart(const char* traceFileName, int traceFd, int bufferSize,
        int flags, bool directToDdms);
bool dvmIsMethodTraceActive(void);
void dvmMethodTraceStop(void);

void dvmEmulatorTraceStart(void);
void dvmEmulatorTraceStop(void);

void dvmStartInstructionCounting();
void dvmStopInstructionCounting();

enum {
    TRACE_ALLOC_COUNTS      = 0x01,
};

#define TRACE_METHOD_ENTER(_self, _method)                                  \
    do {                                                                    \
        if (_self->interpBreak.ctl.subMode & kSubModeMethodTrace)           \
            dvmMethodTraceAdd(_self, _method, METHOD_TRACE_ENTER);          \
        if (_self->interpBreak.ctl.subMode & kSubModeEmulatorTrace)         \
            dvmEmitEmulatorTrace(_method, METHOD_TRACE_ENTER);              \
    } while(0);
#define TRACE_METHOD_EXIT(_self, _method)                                   \
    do {                                                                    \
        if (_self->interpBreak.ctl.subMode & kSubModeMethodTrace)           \
            dvmMethodTraceAdd(_self, _method, METHOD_TRACE_EXIT);           \
        if (_self->interpBreak.ctl.subMode & kSubModeEmulatorTrace)         \
            dvmEmitEmulatorTrace(_method, METHOD_TRACE_EXIT);               \
    } while(0);
#define TRACE_METHOD_UNROLL(_self, _method)                                 \
    do {                                                                    \
        if (_self->interpBreak.ctl.subMode & kSubModeMethodTrace)           \
            dvmMethodTraceAdd(_self, _method, METHOD_TRACE_UNROLL);         \
        if (_self->interpBreak.ctl.subMode & kSubModeEmulatorTrace)         \
            dvmEmitEmulatorTrace(_method, METHOD_TRACE_UNROLL);             \
    } while(0);

void dvmMethodTraceAdd(struct Thread* self, const Method* method, int action);
void dvmEmitEmulatorTrace(const Method* method, int action);

void dvmMethodTraceGCBegin(void);
void dvmMethodTraceGCEnd(void);
void dvmMethodTraceClassPrepBegin(void);
void dvmMethodTraceClassPrepEnd(void);

extern "C" void dvmFastMethodTraceEnter(const Method* method, struct Thread* self);
extern "C" void dvmFastMethodTraceExit(struct Thread* self);
extern "C" void dvmFastNativeMethodTraceExit(const Method* method, struct Thread* self);

void dvmStartAllocCounting(void);
void dvmStopAllocCounting(void);

#endif


enum {
    METHOD_TRACE_ENTER = 0x00,      // method entry
    METHOD_TRACE_EXIT = 0x01,       // method exit
    METHOD_TRACE_UNROLL = 0x02,     // method exited by exception unrolling
    // 0x03 currently unused
};

#define TOKEN_CHAR      '*'

#define METHOD_ACTION_MASK      0x03            /* two bits */
#define METHOD_ID(_method)      ((_method) & (~METHOD_ACTION_MASK))
#define METHOD_ACTION(_method)  (((unsigned int)(_method)) & METHOD_ACTION_MASK)
#define METHOD_COMBINE(_method, _action)    ((_method) | (_action))

#endif  // DALVIK_PROFILE_H_

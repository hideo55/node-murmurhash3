/*!
 * node-murmurhash3
 * Copyright(c) 2013 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <v8.h>
#include <node.h>
#include <node_version.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "MurmurHash3.h"

#define REQ_ARG_COUNT_AND_TYPE(I, TYPE) \
  if (args.Length() < (I + 1) ) { \
      return ThrowException(Exception::RangeError(String::New("A least " #I " arguments are required"))); \
  } else if (!args[I]->Is##TYPE()) { \
      return ThrowException(Exception::TypeError(String::New("Argument " #I " must be a " #TYPE))); \
  }

#define REQ_FUN_ARG(I, VAR) \
  REQ_ARG_COUNT_AND_TYPE(I, Function) \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

#define REQ_STR_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, String)

#define REQ_BOOL_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, Boolean)

#define REQ_UINT32_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, Uint32)

#define TRY_CATCH_CALL(context, callback, argc, argv)                          \
    TryCatch try_catch;                                                        \
    (callback)->Call((context), (argc), (argv));                               \
    if (try_catch.HasCaught()) {                                               \
        FatalException(try_catch);                                             \
    }

using namespace v8;
using namespace node;

struct Baton {
    uv_work_t request;
    v8::Persistent<v8::Function> callback;

    std::string key;
    uint32_t seed;
    bool isHexMode;
    uint32_t result[4];

    Baton(std::string key_, uint32_t seed_, bool isHexMode_, v8::Handle<v8::Function> cb_) :
            key(key_), seed(seed_), isHexMode(isHexMode_) {
        request.data = this;
        callback = v8::Persistent < v8::Function > ::New(cb_);
    }
    virtual ~Baton() {
        callback.Dispose();
    }
};

//Initialize
void Initialize(Handle<Object> target);

//uv async functions
void Work_murmur32(uv_work_t* req);
void Work_murmur128(uv_work_t* req);
#if NODE_VERSION_AT_LEAST(0,9,4)
void Work_After_murmur32(uv_work_t* req, int status);
void Work_After_murmur128(uv_work_t* req, int status);
#else
void Work_After_murmur32(uv_work_t* req);
void Work_After_murmur128(uv_work_t* req);
#endif

Handle<Value> murmur32_async(Arguments& args);
Handle<Value> murmur128_async(Arguments& args);
Handle<Value> murmur32_sync(Arguments& args);
Handle<Value> murmur128_sync(Arguments& args);

Handle<Value> murmur32_async(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);
    REQ_FUN_ARG(3, callback);

    std::string key = *String::Utf8Value(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();

    Baton* baton = new Baton(key, seed, args[2]->ToBoolean()->Value(), callback);

    uv_queue_work(uv_default_loop(), &baton->request, Work_murmur32, Work_After_murmur32);

    return scope.Close(Undefined());
}

Handle<Value> murmur128_async(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);
    REQ_FUN_ARG(3, callback);

    std::string key = *String::Utf8Value(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();
    Baton* baton = new Baton(key, seed, args[2]->ToBoolean()->Value(), callback);

    uv_queue_work(uv_default_loop(), &baton->request, Work_murmur128, Work_After_murmur128);

    return scope.Close(Undefined());
}

Handle<Value> murmur32_sync(const Arguments& args) {
    HandleScope scope;
    uint32_t out;

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);

    String::Utf8Value key(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();
    bool isHexMode = args[2]->ToBoolean()->Value();

    MurmurHash3_x86_32(reinterpret_cast<const char *>(*key), (int) key.length(), seed, &out);

    if (isHexMode) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << out;
        return scope.Close(String::New((ss.str()).c_str()));
    } else {
        return scope.Close(Integer::New(out));
    }
}

Handle<Value> murmur128_sync(const Arguments& args) {
    HandleScope scope;
    uint32_t out[4];

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);

    String::Utf8Value key(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();
    bool isHexMode = args[2]->ToBoolean()->Value();

    MurmurHash3_x86_128(reinterpret_cast<const char *>(*key), (int) key.length(), seed, &out);

    if (isHexMode) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 4; i++) {
            ss << std::setw(8) << out[i];
        }
        return scope.Close(String::New((ss.str()).c_str()));
    } else {
        Local < Array > ret = Array::New(4);
        for (int i = 0; i < 4; i++) {
            ret->Set(Integer::New(i), Integer::New(out[i]));
        }
        return scope.Close(ret);
    }
}

void Work_murmur32(uv_work_t* req) {
    Baton *baton = static_cast<Baton *>(req->data);
    uint32_t out;

    MurmurHash3_x86_32(baton->key.c_str(), (int) baton->key.length(), baton->seed, &out);
    baton->result[0] = out;
}

#if NODE_VERSION_AT_LEAST(0,9,4)
void Work_After_murmur32(uv_work_t* req, int status) {
#else
void Work_After_murmur32(uv_work_t* req) {
#endif
    HandleScope scope;
    std::auto_ptr<Baton> baton(static_cast<Baton *>(req->data));

    Local < Value > res[2];

    res[0] = Local < Value > ::New(Null());

    if (baton->isHexMode) {
        Local < Value > tmp = Integer::New(baton->result[0]);
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << tmp->Uint32Value();
        res[1] = String::New((ss.str()).c_str());
    } else {
        res[1] = Integer::NewFromUnsigned(baton->result[0]);
    }

    if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
        TRY_CATCH_CALL(Context::GetCurrent()->Global(), baton->callback, 2, res);
    }

    scope.Close(Undefined());
}

void Work_murmur128(uv_work_t *req) {

    Baton *baton = static_cast<Baton *>(req->data);
    uint32_t out[4];

    MurmurHash3_x86_128(baton->key.c_str(), (int) baton->key.length(), baton->seed, &out);
    for (int i = 0; i < 4; i++) {
        baton->result[i] = out[i];
    }
}

#if NODE_VERSION_AT_LEAST(0,9,4)
void Work_After_murmur128(uv_work_t *req, int status) {
#else
void Work_After_murmur128(uv_work_t *req) {
#endif
    HandleScope scope;
    std::auto_ptr<Baton> baton(static_cast<Baton *>(req->data));

    uint32_t *result = baton->result;

    Local < Value > res[2];
    res[0] = Local < Value > ::New(Null());

    if (baton->isHexMode) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 4; i++) {
            ss << std::setw(8) << result[i];
        }
        res[1] = String::New((ss.str()).c_str());
    } else {
        Local < Array > ret = Array::New(4);
        for (int i = 0; i < 4; i++) {
            ret->Set(Integer::New(i), Integer::NewFromUnsigned(result[i]));
        }
        res[1] = ret;

    }

    if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
        TRY_CATCH_CALL(Context::GetCurrent()->Global(), baton->callback, 2, res);
    }

    scope.Close(Undefined());
}

void Initialize(Handle<Object> target) {
    NODE_SET_METHOD(target, "murmur32", murmur32_async);
    NODE_SET_METHOD(target, "murmur128", murmur128_async);
    NODE_SET_METHOD(target, "murmur32Sync", murmur32_sync);
    NODE_SET_METHOD(target, "murmur128Sync", murmur128_sync);
}

NODE_MODULE(murmurhash3, Initialize)

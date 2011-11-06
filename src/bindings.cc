/*!
 * murmurhash3
 * Copyright(c) 2011 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */

#include <v8.h>
#include <node.h>
#include <node_version.h>
#include <iostream>
#include <sstream>
#include "Murmurhash3.h"

using namespace v8;
using namespace node;

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

typedef struct {
    std::string key;
    int length;
    bool isHexMode;
    uint32_t result[4];
    Persistent<Function> callback;
} MurmurParams;

#if NODE_VERSION_AT_LEAST(0,5,4)
static void EIO_murmur32(eio_req *req) {
#else
static int EIO_murmur32(eio_req *req) {
#endif
    MurmurParams *params = reinterpret_cast<MurmurParams *> (req->data);
    uint32_t seed = 0;
    uint32_t out;

    MurmurHash3_x86_32(params->key.c_str(), (int) params->key.length(), seed,
            &out);
    params->result[0] = out;
#if !NODE_VERSION_AT_LEAST(0,5,4)
    return 0;
#endif
}

static int EIO_murmur32After(eio_req *req) {
    HandleScope scope;
    MurmurParams *params = reinterpret_cast<MurmurParams *> (req->data);

    Handle < Value > res[2];
    res[0] = Undefined();

    if (params->isHexMode) {
        Local < Value > tmp = Integer::New(params->result[0]);
        std::stringstream ss;
        ss << std::hex << tmp->Uint32Value();
        res[1] = String::New((ss.str()).c_str());
    } else {
        res[1] = Integer::NewFromUnsigned(params->result[0]);
    }

    params->callback->Call(Context::GetCurrent()->Global(), 2, res);
    params->callback.Dispose();
    delete params;

    ev_unref( EV_DEFAULT_UC);

    return 0;
}

#if NODE_VERSION_AT_LEAST(0,5,4)
static void EIO_murmur128(eio_req *req) {
#else
static int EIO_murmur128(eio_req *req) {
#endif
    MurmurParams *params = reinterpret_cast<MurmurParams *> (req->data);
    uint32_t seed = 0;
    uint32_t out[4];

    MurmurHash3_x86_128(params->key.c_str(), (int) params->key.length(), seed,
            &out);
    for (int i = 0; i < 4; i++) {
        params->result[i] = out[i];
    }
#if !NODE_VERSION_AT_LEAST(0,5,4)
    return 0;
#endif
}

static int EIO_murmur128After(eio_req *req) {
    HandleScope scope;
    MurmurParams *params = reinterpret_cast<MurmurParams *> (req->data);

    uint32_t *result = params->result;

    Handle < Value > res[2];
    res[0] = Undefined();

    if (params->isHexMode) {
        std::stringstream ss;
        for (int i = 0; i < 4; i++) {
            ss << std::hex << result[i];
        }
        res[1] = String::New((ss.str()).c_str());
    } else {
        Local < Array > ret = Array::New(4);
        for (int i = 0; i < 4; i++) {
            ret->Set(Integer::New(i), Integer::NewFromUnsigned(result[i]));
        }
        res[1] = ret;

    }

    params->callback->Call(Context::GetCurrent()->Global(), 2, res);
    params->callback.Dispose();
    delete params;

    ev_unref( EV_DEFAULT_UC);

    return 0;
}

static Handle<Value> murmur32_async(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0);
    REQ_BOOL_ARG(1);
    REQ_FUN_ARG(2,callback);

    String::Utf8Value key(args[0]->ToString());

    MurmurParams *params = new MurmurParams;
    params->key.assign(reinterpret_cast<const char *> (*key));
    params->isHexMode = args[1]->ToBoolean()->Value();
    params->callback = Persistent<Function>::New(callback);

    eio_custom(EIO_murmur32, EIO_PRI_DEFAULT, EIO_murmur32After,
            (eio_req *) params);
    ev_ref( EV_DEFAULT_UC);

    return scope.Close(Undefined());
}

static Handle<Value> murmur128_async(const Arguments& args) {
    HandleScope scope;

    REQ_STR_ARG(0);
    REQ_BOOL_ARG(1);
    REQ_FUN_ARG(2,callback);

    String::Utf8Value key(args[0]->ToString());

    MurmurParams *params = new MurmurParams;
    params->key.assign(reinterpret_cast<const char *> (*key));
    params->isHexMode = args[1]->ToBoolean()->Value();
    params->callback = Persistent<Function>::New(callback);

    eio_custom(EIO_murmur128, EIO_PRI_DEFAULT, EIO_murmur128After,
            (eio_req *) params);
    ev_ref( EV_DEFAULT_UC);

    return scope.Close(Undefined());
}

static Handle<Value> murmur32_sync(const Arguments& args) {
    HandleScope scope;
    uint32_t seed = 0;
    uint32_t out;

    REQ_STR_ARG(0);
    REQ_BOOL_ARG(1);

    String::Utf8Value key(args[0]->ToString());
    bool isHexMode = args[1]->ToBoolean()->Value();

    MurmurParams *params = new MurmurParams;
    MurmurHash3_x86_32(reinterpret_cast<const char *> (*key),
            (int) key.length(), seed, &out);

    if (isHexMode) {
        std::stringstream ss;
        ss << std::hex << out;
        return scope.Close(String::New((ss.str()).c_str()));
    } else {
        return scope.Close(Integer::New(out));
    }
}

static Handle<Value> murmur128_sync(const Arguments& args) {
    HandleScope scope;
    uint32_t seed = 0;
    uint32_t out[4];

    REQ_STR_ARG(0);
    REQ_BOOL_ARG(1);

    String::Utf8Value key(args[0]->ToString());
    bool isHexMode = args[1]->ToBoolean()->Value();

    MurmurParams *params = new MurmurParams;
    MurmurHash3_x86_128(reinterpret_cast<const char *> (*key),
            (int) key.length(), seed, &out);

    if (isHexMode) {
        std::stringstream ss;
        for (int i = 0; i < 4; i++) {
            ss << std::hex << out[i];
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

extern "C" void init(Handle<Object> target) {
    HandleScope scope;
    NODE_SET_METHOD(target, "murmur32", murmur32_async);
    NODE_SET_METHOD(target, "murmur128", murmur128_async);
    NODE_SET_METHOD(target, "murmur32Sync", murmur32_sync);
    NODE_SET_METHOD(target, "murmur128Sync", murmur128_sync);
}
NODE_MODULE(murmurhash3, init);

/*!
 * node-murmurhash3
 * Copyright(c) 2013 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include "nan.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "MurmurHash3.h"

#define REQ_ARG_COUNT_AND_TYPE(I, TYPE) \
  if (args.Length() < (I + 1) ) { \
      ThrowException(Exception::RangeError(String::New("A least " #I " arguments are required"))); \
  } else if (!args[I]->Is##TYPE() || args[I]->IsUndefined()) { \
      ThrowException(Exception::TypeError(String::New("Argument " #I " must be a " #TYPE))); \
  }

#define REQ_FUN_ARG(I, VAR) \
  REQ_ARG_COUNT_AND_TYPE(I, Function) \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

#define REQ_STR_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, String)

#define REQ_BOOL_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, Boolean)

#define REQ_UINT32_ARG(I) REQ_ARG_COUNT_AND_TYPE(I, Uint32)

using namespace v8;
using namespace node;


static NAN_INLINE(void MakeReturnValue_murmur32(Local<Value>& ret, uint32_t value, bool hexMode)) {
    if (hexMode) {
        Local < Value > tmp = Integer::New(value);
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << tmp->Uint32Value();
        ret = String::New((ss.str()).c_str());
    } else {
        ret = Integer::NewFromUnsigned(value);
    }
}

static NAN_INLINE(void MakeReturnValue_murmur128(Local<Value>& ret, uint32_t* value, bool hexMode)) {
    if (hexMode) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 4; i++) {
            ss << std::setw(8) << value[i];
        }
        ret = String::New((ss.str()).c_str());
    } else {
        Local<Array> tmp = Array::New(4);
        for (int i = 0; i < 4; i++) {
            tmp->Set(Integer::New(i), Integer::NewFromUnsigned(value[i]));
        }
        ret = tmp;
    }
}

class Murmur32Worker : public NanAsyncWorker {
public:
    Murmur32Worker(NanCallback *callback, std::string& key, uint32_t seed, bool hexMode)
        : NanAsyncWorker(callback), key_(key), seed_(seed), hexMode_(hexMode) {
    }

    void Execute() {
        MurmurHash3_x86_32(key_.c_str(), (int) key_.length(), seed_, &hashValue_);
    }

    void HandleOKCallback() {
        NanScope();
        Local < Value > res[2];
        res[0] = NanNewLocal(Null());
        MakeReturnValue_murmur32(res[1], hashValue_, hexMode_);
        callback->Call(2, res);
    }

private:
    std::string key_;
    uint32_t seed_;
    uint32_t hashValue_;
    bool hexMode_;
};

class Murmur128Worker : public NanAsyncWorker {
public:
    Murmur128Worker(NanCallback *callback, std::string& key, uint32_t seed, bool hexMode)
        : NanAsyncWorker(callback), key_(key), seed_(seed), hexMode_(hexMode) {
    }

    void Execute() {
        MurmurHash3_x86_128(key_.c_str(), (int) key_.length(), seed_, &hashValue_);
    }

    void HandleOKCallback() {
        NanScope();
        Local < Value > res[2];
        res[0] = NanNewLocal(Null());
        MakeReturnValue_murmur128(res[1], hashValue_, hexMode_);
        callback->Call(2, res);
    }

private:
    std::string key_;
    uint32_t seed_;
    uint32_t hashValue_[4];
    bool hexMode_;
};

//Initialize
void Initialize(Handle<Object> target);

NAN_METHOD(murmur32_async) {
    NanScope();

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);
    REQ_FUN_ARG(3, cb);

    std::string key = *String::Utf8Value(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();

    NanCallback *callback = new NanCallback(cb);
    NanAsyncQueueWorker(new Murmur32Worker(callback, key, seed, args[2]->ToBoolean()->Value()));

    NanReturnUndefined();
}

NAN_METHOD(murmur128_async) {
    NanScope();

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);
    REQ_FUN_ARG(3, cb);

    std::string key = *String::Utf8Value(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();

    NanCallback *callback = new NanCallback(cb);
    NanAsyncQueueWorker(new Murmur128Worker(callback, key, seed, args[2]->ToBoolean()->Value()));

    NanReturnUndefined();
}

NAN_METHOD(murmur32_sync) {
    NanScope();
    uint32_t out;

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);

    String::Utf8Value key(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();
    bool hexMode = args[2]->ToBoolean()->Value();

    MurmurHash3_x86_32(reinterpret_cast<const char *>(*key), (int) key.length(), seed, &out);

    Local<Value> ret;
    MakeReturnValue_murmur32(ret, out, hexMode);
    NanReturnValue(ret);

}

NAN_METHOD(murmur128_sync) {
    NanScope();
    uint32_t out[4];

    REQ_STR_ARG(0);
    REQ_UINT32_ARG(1);
    REQ_BOOL_ARG(2);

    String::Utf8Value key(args[0]->ToString());
    uint32_t seed = args[1]->ToUint32()->Value();
    bool hexMode = args[2]->ToBoolean()->Value();

    MurmurHash3_x86_128(reinterpret_cast<const char *>(*key), (int) key.length(), seed, &out);

    Local<Value> ret;
    MakeReturnValue_murmur128(ret, out, hexMode);
    NanReturnValue(ret);
}

void Initialize(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "murmur32", murmur32_async);
    NODE_SET_METHOD(exports, "murmur128", murmur128_async);
    NODE_SET_METHOD(exports, "murmur32Sync", murmur32_sync);
    NODE_SET_METHOD(exports, "murmur128Sync", murmur128_sync);
}

NODE_MODULE(murmurhash3, Initialize)

/*!
 * murmurhash3
 * Copyright(c) 2011 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */

#include <sstream>
#include <node.h>
#include "Murmurhash3.h"

using namespace v8;
using namespace node;

namespace {

Handle<Value> murmur32_binding(const Arguments& args) {
    HandleScope scope;
    uint32_t seed = 0;
    uint32_t out;
    String::Utf8Value key(args[0]->ToString());
    MurmurHash3_x86_32((unsigned char *) *key, (int) key.length(), seed, &out);
    return scope.Close(Integer::New(out));
}

Handle<Value> murmur128_binding(const Arguments& args) {
    HandleScope scope;
    Local < Array > ret = Array::New(4);
    uint32_t seed = 0;
    uint32_t out[4];
    String::Utf8Value key(args[0]->ToString());
    MurmurHash3_x86_128((unsigned char *) *key, (int) key.length(), seed, &out);
    for (int i = 0; i < 4; i++) {
        ret->Set(Integer::New(i), Integer::New(out[i]));
    }
    return scope.Close(ret);
}

Handle<Value> murmur32_hex(const Arguments& args) {
    HandleScope scope;
    Local < Integer > hashVal = murmur32_binding(args)->ToInteger();
    std::stringstream ss;
    ss << std::hex << hashVal->Uint32Value();
    return scope.Close(String::New((ss.str()).c_str()));
}

Handle<Value> murmur128_hex(const Arguments& args) {
    HandleScope scope;
    Local < Object > hashVals = murmur128_binding(args)->ToObject();
    std::stringstream ss;
    for (int i = 0; i < 4; i++) {
        ss << std::hex << hashVals->Get(i)->Uint32Value();
    }
    return scope.Close(String::New((ss.str()).c_str()));
}

}

extern "C" void init(Handle<Object> target) {
    HandleScope scope;
    NODE_SET_METHOD(target, "murmur32", murmur32_binding);
    NODE_SET_METHOD(target, "murmur32Hex", murmur32_hex);
    NODE_SET_METHOD(target, "murmur128", murmur128_binding);
    NODE_SET_METHOD(target, "murmur128Hex", murmur128_hex);
}

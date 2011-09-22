#include <node.h>
#include <iostream>
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

}

extern "C" void init(Handle<Object> target) {
	HandleScope scope;
	NODE_SET_METHOD(target, "murmur32", murmur32_binding);
	NODE_SET_METHOD(target, "murmur128", murmur128_binding);
}

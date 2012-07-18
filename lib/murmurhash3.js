/*!
* murmurhash3
* Copyright(c) 2011 Hideaki Ohno <hide.o.j55{at}gmail.com>
* MIT Licensed
*/

/**
 * Library version.
 */

exports.version = '0.1.1';

var binding;
try {
  binding = require(__dirname + '/../build/Release/murmurhash3');
} catch(e) {
  binding = require(__dirname + '/../build/default/murmurhash3');
}

//Aync interface
exports.murmur32 = function(key, cb) {
  // Cast signed integer to unsigned integer
  var res = binding.murmur32(key, false, function(err, res) {
    cb(err, res >>> 0);
  });
  return res;
};
exports.murmur32Hex = function(key, cb) {
  return binding.murmur32(key, true, cb);
};
exports.murmur128 = function(key, cb) {
  binding.murmur128(key, false, function(err, res) {
    for(var i = 0; i < res.length; i++) {
      // Cast signed integer to unsigned integer
      res[i] = res[i] >>> 0;
    }
    cb(err, res);
  });
};
exports.murmur128Hex = function(key, cb) {
  return binding.murmur128(key, true, cb);
};
//Sync interface

exports.murmur32Sync = function(key) {
  // Cast signed integer to unsigned integer
  var res = binding.murmur32Sync(key, false) >>> 0;
  return res;
};
exports.murmur32HexSync = function(key) {
  return binding.murmur32Sync(key, true);
};
exports.murmur128Sync = function(key) {
  var res = binding.murmur128Sync(key, false);
  for(var i = 0; i < res.length; i++) {
    // Cast signed integer to unsigned integer
    res[i] = res[i] >>> 0;
  }
  return res;
};
exports.murmur128HexSync = function(key) {
  return binding.murmur128Sync(key, true);
};

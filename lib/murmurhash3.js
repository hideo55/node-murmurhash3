/*!
* murmurhash3
* Copyright(c) 2011 Hideaki Ohno <hide.o.j55{at}gmail.com>
* MIT Licensed
*/

/**
 * Library version.
 */

exports.version = '0.4.1';

var binding;
try {
  binding = require(__dirname + '/../build/Release/murmurhash3');
} catch(e) {
  binding = require(__dirname + '/../build/default/murmurhash3');
}

var E_CALLBACK_MISSING = "Callback function is missing";

//Aync interface

exports.murmur32 = function(key, seed, cb) {
  if ( typeof seed == 'function') {
    cb = seed;
    seed = 0;
  }
  if (cb === undefined || typeof cb != 'function') {
    throw new Error(E_CALLBACK_MISSING);
  }
  var res = binding.murmur32(key, seed, false, function(err, res) {
    // Cast signed integer to unsigned integer
    cb(err, res >>> 0);
  });
  return res;
};

exports.murmur32Hex = function(key, seed, cb) {
  if ( typeof seed == 'function') {
    cb = seed;
    seed = 0;
  }
  if (cb === undefined || typeof cb != 'function') {
    throw new Error(E_CALLBACK_MISSING);
  }
  return binding.murmur32(key, seed, true, cb);
};

exports.murmur128 = function(key, seed, cb) {
  if ( typeof seed == 'function') {
    cb = seed;
    seed = 0;
  }
  if (cb === undefined || typeof cb != 'function') {
    throw new Error(E_CALLBACK_MISSING);
  }
  binding.murmur128(key, seed, false, function(err, res) {
    for (var i = 0; i < res.length; i++) {
      // Cast signed integer to unsigned integer
      res[i] = res[i] >>> 0;
    }
    cb(err, res);
  });
};

exports.murmur128Hex = function(key, seed, cb) {
  if ( typeof seed == 'function') {
    cb = seed;
    seed = 0;
  }
  if (cb === undefined || typeof cb != 'function') {
    throw new Error(E_CALLBACK_MISSING);
  }
  return binding.murmur128(key, seed, true, cb);
};


//Sync interface

exports.murmur32Sync = function(key, seed) {
  // Cast signed integer to unsigned integer
  if (seed === undefined) {
    seed = 0;
  }
  var res = binding.murmur32Sync(key, seed, false) >>> 0;
  return res;
};

exports.murmur32HexSync = function(key, seed) {
  if (seed === undefined) {
    seed = 0;
  }
  return binding.murmur32Sync(key, seed, true);
};

exports.murmur128Sync = function(key, seed) {
  if (seed === undefined) {
    seed = 0;
  }
  var res = binding.murmur128Sync(key, seed, false);
  for (var i = 0; i < res.length; i++) {
    // Cast signed integer to unsigned integer
    res[i] = res[i] >>> 0;
  }
  return res;
};

exports.murmur128HexSync = function(key, seed) {
  if (seed === undefined) {
    seed = 0;
  }
  return binding.murmur128Sync(key, seed, true);
};

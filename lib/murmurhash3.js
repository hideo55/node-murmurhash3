/*!
 * murmurhash3
 * Copyright(c) 2011 Hideaki Ohno <hide.o.j55{at}gmail.com>
 * MIT Licensed
 */

/**
 * Library version.
 */

exports.version = '0.0.1';

var binding = require('../murmurhash3_binding');

exports.murmur32 = function(key) {
  // Cast signed integer to unsigned integer
  var res = binding.murmur32(key) >>> 0;
  return res;
};

exports.murmur32Hex = function(key) {
  return binding.murmur32Hex(key);
};

exports.murmur128 = function(key) {
  var res = binding.murmur128(key);
  for ( var i = 0; i < res.length; i++) {
    // Cast signed integer to unsigned integer
    res[i] = res[i] >>> 0;
  }
  return res;
};

exports.murmur128Hex = function(key) {
  return binding.murmur128Hex(key);
};
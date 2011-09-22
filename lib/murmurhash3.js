
/*!
 * murmurhash3
 * Copyright(c) 2011 Hideaki Ohno <hide.o.j55@gmail.com>
 * MIT Licensed
 */

/**
 * Library version.
 */

exports.version = '0.0.1';

var binding = require('../murmurhash3_binding');

exports.murmur32 = function(key){
	var res = binding.murmur32(key) >>> 0;
	return res;
};

exports.murmur128 = function(key){
	var res = binding.murmur128(key);
	for( var i = 0; i < res.length; i++ ){
		res[i] = res[i] >>> 0;
	}
	return res;
}

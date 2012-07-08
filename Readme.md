
# murmurhash3

  Node.js binding of [MurmurHash3](http://code.google.com/p/smhasher/wiki/MurmurHash3)

 [![Build Status](https://secure.travis-ci.org/hideo55/node-murmurhash3.png)](http://travis-ci.org/hideo55/node-murmurhash3)

## Requirement

### Node v0.4

You must use node-murmurhash3 0.0.6.

### Node v0.6 (v0.6.13 or later)

You must use node-murmurhash3 0.0.8 or later.

### Node v0.8

You must use node-murmurhash3 0.0.8 or later.

## Install

    #>npm install murmurhash3

## Usage

    var mmh3 = require('murmurhash3');
    var hashVal32 = mmh3.murmur32('key', function(err,hashvalue){
      ...
   	});
    var hashVal128 = mmh3.murmur128('key', function(err,hashvalue){
      ...
    });

## Functions

###Async intefeaces

    murmur32(key, callback);    // return 32bit integer value
    murmur32Hex(key, callback); // return 32bit hexadecimal string
    murmur128(key, callback);   // return array that have 4 elements of 32bit integer
    murmur128Hex(key, callback);// return 128bit hexadecimal string

###Sync interfaces

    murmur32Sync(key);    // return 32bit integer value
    murmur32HexSync(key); // return 32bit hexadecimal string
    murmur128Sync(key);   // return array that have 4 elements of 32bit integer
    murmur128HexSync(key);// return 128bit hexadecimal string

## License 

(The MIT License)

Copyright (c) 2012 Hideaki Ohno &lt;hide.o.j55{at}gmail.com&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

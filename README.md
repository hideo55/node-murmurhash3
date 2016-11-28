# murmurhash3

  Node.js binding of [MurmurHash3](http://code.google.com/p/smhasher/wiki/MurmurHash3) C++ implementaion.

[![NPM](https://nodei.co/npm/murmurhash3.png)](https://nodei.co/npm/murmurhash3/)

[![Build Status](https://travis-ci.org/hideo55/node-murmurhash3.svg?branch=master)](http://travis-ci.org/hideo55/node-murmurhash3)

## Installation

    $ npm install murmurhash3
    
or

    $ git clone https://github.com/hideo55/node-murmurhash3.git
    $ npm install ./node-murmurhash3

## Usage

```javascript
var mmh3 = require('murmurhash3');
    
mmh3.murmur32('key', function(err, hashValue){
  if (err) throw err;
  ...
});
mmh3.murmur128Hex('key', function(err, hashValue){
  if (err) throw err;
  ...
});
```

## Functions

###Async interfaces

    murmur32(key [,seed], callback);    // return 32bit integer value
    murmur32Hex(key [,seed], callback); // return 32bit hexadecimal string
    murmur128(key [,seed], callback);   // return array that have 4 elements of 32bit integer
    murmur128Hex(key [,seed], callback);// return 128bit hexadecimal string

- `seed` is optional argument. (unsigned integer)
- The callback gets two arguments `(error, hashValue)`. 

###Sync interfaces

    murmur32Sync(key [,seed]);    // return 32bit integer value
    murmur32HexSync(key, [,seed]); // return 32bit hexadecimal string
    murmur128Sync(key [,seed]);   // return array that have 4 elements of 32bit integer
    murmur128HexSync(key [,seed]);// return 128bit hexadecimal string

- `seed` is optional argument. (unsigned integer)

## Requirement

- Node v0.12 or later

## Author

Hideaki Ohno  &lt;hide.o.j55{at}gmail.com&gt;

MurmurHash3(C++ implementaion) by Austin Appleby

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

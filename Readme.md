
# murmurhash3

  Node binding of MurmurHash3

## Install

    #>npm install murmurhash3

## Usage

    var mmh3 = require('murmurhash3');
    var hashVal32 = mmh3.murmur32('key');
    var hashVal128 = mmh3.murmur128('key');

## Functions

    murmur32(key);    // return 32bit integer value
    murmur32Hex(key); // return 32bit hexadecimal string
    murmur128(key);   // return array that have 4 elements of 32bit integer
    murmur128Hex(key);// return 128bit hexadecimal string

## License 

(The MIT License)

Copyright (c) 2011 Hideaki Ohno &lt;hide.o.j55{at}gmail.com&gt;

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

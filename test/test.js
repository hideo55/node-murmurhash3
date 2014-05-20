var mmh3 = require('../lib/murmurhash3');
var testData = [["Hello", 0x12da77c8, [0x2360ae46, 0x5e6336c6, 0xad45b3f4, 0xad45b3f4]], ["Hello1", 0x6357e0a6, [0x8eb0cf41, 0x641b2401, 0xbc4c0dfc, 0xbc4c0dfc]], ["Hello2", 0xe5ce223e, [0xd3bcfc45, 0x66782162, 0x4beab2d1, 0x4beab2d1]], ["2", 0x0129e217, [0x9dd4f4e7, 0x3df769b3, 0x3df769b3, 0x3df769b3]], ["88", 0x7a0040a5, [0x28979fa9, 0x0b1b1a58, 0x0b1b1a58, 0x0b1b1a58]]];

function zeroFill(number, width) {
  width -= number.toString().length;
  if (width > 0) {
    return new Array(width + (/\./.test(number) ? 2 : 1)).join('0') + number;
  }
  return number + "";
  // always return a string
}

var assert = require('assert');
var i = 0;
for (; i < testData.length; i++) {
  var val = testData[i];
  describe('Pattern:' + (i + 1 ), function() {
    describe("Async Interface", function() {
      describe('murmur32', function() {
        it('integer', function(done) {
          mmh3.murmur32(val[0], function(err, res) {
            assert.equal(res, val[1]);
            done();
          });
        });
        it('integer - with seed', function(done) {
          mmh3.murmur32(val[0], 0, function(err, res) {
            assert.equal(res, val[1]);
            done();
          });
        });
        it('hex', function(done) {
          mmh3.murmur32Hex(val[0], function(err, res) {
            assert.equal(res, zeroFill(val[1].toString(16), 8));
            done();
          });
        });
        it('hex - with seed', function(done) {
          mmh3.murmur32Hex(val[0], 0, function(err, res) {
            assert.equal(res, zeroFill(val[1].toString(16), 8));
            done();
          });
        });
      });
      describe("murmur128", function() {
        it('integer', function(done) {
          mmh3.murmur128(val[0], function(err, res) {
            assert.deepEqual(res, val[2]);
            done();
          });
        });
        it('integer - with seed', function(done) {
          mmh3.murmur128(val[0], 0, function(err, res) {
            assert.deepEqual(res, val[2]);
            done();
          });
        });
        it('hex', function(done) {
          mmh3.murmur128Hex(val[0], function(err, res) {
            var expect = '';
            for (var j = 0; j < 4; j++) {
              expect += zeroFill(val[2][j].toString(16), 8);
            }
            assert.equal(res, expect);
            done();
          });
        });
        it('hex - with seed', function(done) {
          mmh3.murmur128Hex(val[0], 0, function(err, res) {
            var expect = '';
            for (var j = 0; j < 4; j++) {
              expect += zeroFill(val[2][j].toString(16), 8);
            }
            assert.equal(res, expect);
            done();
          });
        });
      });
    });

    describe("Sync Interface", function() {
      describe('murmur32', function() {
        it('integer', function(done) {
          var res = mmh3.murmur32Sync(val[0]);
          assert.equal(res, val[1]);
          done();
        });
        it('integer - with seed', function(done) {
          var res = mmh3.murmur32Sync(val[0], 0);
          assert.equal(res, val[1]);
          done();
        });
        it('hex', function(done) {
          var res = mmh3.murmur32HexSync(val[0]);
          assert.equal(res, zeroFill(val[1].toString(16), 8));
          done();
        });
        it('hex - with seed ', function(done) {
          var res = mmh3.murmur32HexSync(val[0], 0);
          assert.equal(res, zeroFill(val[1].toString(16), 8));
          done();
        });
      });
      describe('murmur128', function() {
        it('integer', function(done) {
          var res = mmh3.murmur128Sync(val[0]);
          assert.deepEqual(res, val[2]);
          done();
        });
        it('integer - with seed', function(done) {
          var res = mmh3.murmur128Sync(val[0], 0);
          assert.deepEqual(res, val[2]);
          done();
        });
        it('hex', function(done) {
          var res = mmh3.murmur128HexSync(val[0]);
          var expect = '';
          for (var j = 0; j < 4; j++) {
            expect += zeroFill(val[2][j].toString(16), 8);
          }
          assert.equal(res, expect);
          done();
        });
        it('hex - with seed', function(done) {
          var res = mmh3.murmur128HexSync(val[0], 0);
          var expect = '';
          for (var j = 0; j < 4; j++) {
            expect += zeroFill(val[2][j].toString(16), 8);
          }
          assert.equal(res, expect);
          done();
        });
      });
    });
  });
}

describe('Exception', function() {
  describe('Argument', function() {
    describe('murmur32', function() {
      it('key must be string', function(done) {
        try {
          mmh3.murmur32(undefined, function() {
          });
        } catch(e) {
          assert.ok((new RegExp(/^Argument 1 must be a String/)).test(e.message));
          done();
        }
      });
      it('seed must be unsigned integer', function(done) {
        try {
          mmh3.murmur32('key', -1, function() {
          });
        } catch(e) {
          assert.ok((new RegExp(/^Argument 2 must be a Uint32/)).test(e.message));
          done();
        }
      });
      it('callback missing', function(done) {
        try {
          mmh3.murmur32('key', 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
      it('not function', function(done) {
        try {
          mmh3.murmur32('key', 0, 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
      it('callback missing - hex', function(done) {
        try {
          mmh3.murmur32Hex('key', 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
      it('not function - hex', function(done) {
        try {
          mmh3.murmur32Hex('key', 0, 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
    });

    describe('murmur128', function() {
      it('key must be string', function(done) {
        try {
          mmh3.murmur128(undefined, function() {
          });
        } catch(e) {
          assert.ok((new RegExp(/^Argument 1 must be a String/)).test(e.message));
          done();
        }
      });
      it('seed must be unsigned integer', function(done) {
        try {
          mmh3.murmur128('key', -1, function() {
          });
        } catch(e) {
          assert.ok((new RegExp(/^Argument 2 must be a Uint32/)).test(e.message));
          done();
        }
      });
      it('callback missing', function(done) {
        try {
          mmh3.murmur128('key', 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
      it('not function', function(done) {
        try {
          mmh3.murmur128('key', 0, 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
      it('callback missing - hex', function(done) {
        try {
          mmh3.murmur128Hex('key', 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
      it('not function - hex', function(done) {
        try {
          mmh3.murmur128Hex('key', 0, 0);
        } catch(e) {
          assert.ok((new RegExp(/^Callback function is missing/)).test(e.message));
          done();
        }
      });
    });

    describe('murmur32Sync', function() {
      it('key must be string', function(done) {
        try {
          mmh3.murmur32Sync();
        } catch(e) {
          assert.ok((new RegExp(/^Argument 1 must be a String/)).test(e.message));
          done();
        }
      });
      it('seed must be unsigned integer', function(done) {
        try {
          mmh3.murmur32Sync('key', -1);
        } catch(e) {
          assert.ok((new RegExp(/^Argument 2 must be a Uint32/)).test(e.message));
          done();
        }
      });
    });

    describe('murmur128Sync', function() {
      it('key must be string', function(done) {
        try {
          mmh3.murmur128Sync();
        } catch(e) {
          assert.ok((new RegExp(/^Argument 1 must be a String/)).test(e.message));
          done();
        }
      });
      it('seed must be unsigned integer', function(done) {
        try {
          mmh3.murmur128Sync('key', -1);
        } catch(e) {
          assert.ok((new RegExp(/^Argument 2 must be a Uint32/)).test(e.message));
          done();
        }
      });
    });
  });
});

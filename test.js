var mmh = mmh3 = require('./lib/murmurhash3');
var testData = [["Hello", 0x12da77c8, [0x2360ae46, 0x5e6336c6, 0xad45b3f4, 0xad45b3f4]], ["Hello1", 0x6357e0a6, [0x8eb0cf41, 0x641b2401, 0xbc4c0dfc, 0xbc4c0dfc]], ["Hello2", 0xe5ce223e, [0xd3bcfc45, 0x66782162, 0x4beab2d1, 0x4beab2d1]], ["2", 0x0129e217, [0x9dd4f4e7, 0x3df769b3, 0x3df769b3, 0x3df769b3]], ["88", 0x7a0040a5, [0x28979fa9, 0x0b1b1a58, 0x0b1b1a58, 0x0b1b1a58]]];

function zeroFill(number, width) {
  width -= number.toString().length;
  if ( width > 0 )
  {
    return new Array( width + (/\./.test( number ) ? 2 : 1) ).join( '0' ) + number;
  }
  return number + ""; // always return a string
}

exports.setUp = function(cb) {
  cb();
};
for(var i = 0; i < testData.length; i++) {
  var val = testData[i];
  exports["testAsyncInterface32_" + i] = function(test) {
    mmh3.murmur32(val[0], function(err, res) {
      test.equal(res, val[1], 'murmur32-' + i);
      test.done();
    });
  };
  exports["testAsyncInterface32Hex_" + i] = function(test) {
    mmh3.murmur32Hex(val[0], function(err, res) {
      test.equal(res, zeroFill(val[1].toString(16), 8), 'murmur32Hex-' + i);
      test.done();
    });
  };
  exports["testAsyncInterface128_" + i] = function(test) {
    mmh3.murmur128(val[0], function(err, res) {
      test.deepEqual(res, val[2], 'murmur128-' + i);
      test.done();
    });
  };
  exports["testAsyncInterface128Hex_" + i] = function(test) {
    mmh3.murmur128Hex(val[0], function(err, res) {
      var expect = '';
      for(var j = 0; j < 4; j++) {
        expect += zeroFill(val[2][j].toString(16), 8);
      }
      test.equal(res, expect, 'murmur128Hex-' + i);
      test.done();
    });
  };
  exports["testSyncInterface_" + i] = function(test){
      var res;
      res = mmh3.murmur32Sync(val[0]);
      test.equal(res, val[1], 'murmur32-' + i);
      res = mmh3.murmur32HexSync(val[0]);
      test.equal(res, zeroFill(val[1].toString(16), 8), 'murmur32Hex-' + i);
      res = mmh3.murmur128Sync(val[0]);
      test.deepEqual(res, val[2], 'murmur128-' + i);
      res = mmh3.murmur128HexSync(val[0]);
      var expect = '';
      for(var j = 0; j < 4; j++) {
        expect += zeroFill(val[2][j].toString(16), 8);
      }
      test.equal(res, expect, 'murmur128Hex-' + i);
      test.done();   
  };
}

{
  'targets': [
    {
      'target_name': 'murmurhash3',
      'sources': ['src/MurmurHash3.cpp', 'src/node_murmurhash3.cc'],
      'cflags': ['-fexceptions'],
      'cflags_cc': ['-fexceptions'],
      'cflags!': ['-fno-exceptions'],
      'cflags_cc!': ['-fno-exception'],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ],
      'conditions': [
        ['OS=="win"', {
            'msvs_settings': {
              'VCCLCompilerTool': {
                'AdditionalOptions': [ '/EHsc' ]
              }
            }
          }
        ],
        ['OS=="mac"', {
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            }
          }
        ]
      ]
    }
  ]
}
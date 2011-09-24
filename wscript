import Options, Utils
from os import unlink, symlink, popen
from os.path import exists, lexists

srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt): 
    opt.tool_options('compiler_cxx')

def configure(conf):
    conf.check_tool('compiler_cxx')
    conf.check_tool('node_addon')

def build(bld):
    obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
    obj.target = 'murmurhash3_binding'
    obj.source = './src/MurmurHash3.cpp ./src/bindings.cc'

def test(tst):
  test_binary = 'node'
  Utils.exec_command(test_binary + ' test.js')

def shutdown():
  t = 'murmurhash3_binding.node'
  if Options.commands['clean']:
    if lexists(t): unlink(t)
  else:
    if exists('build/default/' + t) and not lexists(t):
      symlink('build/default/' + t, t)
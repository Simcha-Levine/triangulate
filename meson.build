project('triangulate', 'cpp', version : '0.1', 
  default_options : ['warning_level=3', 'cpp_std=c++20'])

sfml = [dependency('sfml-graphics')]

src = ['main.cpp']

exe = executable('triangulate', src, dependencies : sfml)

test('basic', exe)
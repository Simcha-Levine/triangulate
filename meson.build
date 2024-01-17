project('line_art', 'cpp', version : '0.1', 
  default_options : ['warning_level=3', 'cpp_std=c++20'])

sfml = [dependency('sfml-graphics'),
 dependency('sfml-window'),
  dependency('sfml-system'),
  dependency('sfml-audio')]

src = ['main.cpp']

exe = executable('line_art', src, install : true, dependencies : [sfml])

test('basic', exe)
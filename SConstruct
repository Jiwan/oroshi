import os

env = Environment() # Initialize the SCons environment.

env['ENV']['TERM'] = os.environ['TERM'] # Do accept clang color.

env['CXXFLAGS'] = '-std=c++11 -g'

Export('env')

SConscript('libs/common/SConscriptCommon', variant_dir='build/libs/common', duplicate=0, exports='env')

SConscript('login/SConscriptLogin', variant_dir='build/login', duplicate=0, exports='env')

def FlagsForFile( filename, **kwargs ):
  return {
    'flags': [ '-x', 'c++', 'sdl2-config --cflags --libs', '-lSDL2_image', '-std=c++11'],
  }

# Lua Dash!

A engine built in C, that you can create a game with lua scripts.  mainly to learn C, and lua.  The goal is to build a similar to monogamedash that I did in c# and c++
![Picture didn't load idiot](https://github.com/kjblanchard/lua_c_dash/blob/master/img/game.gif?raw=true)


- Using minimal IDE, 100% vim and lldb for debugging.
- Tracking the project in github projects

**[Doxygen Documentation](https://docs.supergoon.com)**

Currently Compiling on  Mac (apple silicon/arm64), eventually I'll compile to others once the project gets big enough.


## License

All of the licenses that I'm using will be listed here.

### Libraries
- [SDL2](https://www.libsdl.org) - ZLIB
- [Lua](https://www.lua.org/license.html) - MIT
- [MojoAL](https://github.com/icculus/mojoAL) - ZLIB
- [Ogg/Vorbis](https://xiph.org/vorbis/) - BSD

### Art
- [Pixel art](https://pixelfrog-assets.itch.io/pixel-adventure-1) - Creative Commons License
- [Trampoline Sound](https://freesound.org/people/Breviceps/sounds/493161/) - Creative Commons License
- [Jump Sound](https://freesound.org/people/se2001/sounds/528568/) - Creative Commons License
- [Footsteps](https://freesound.org/people/EVRetro/sounds/501102/) - Creative Commons License
- [End of level Music](https://freesound.org/people/maxmakessounds/sounds/353546) - Creative Commons License

## Acknowledgements
- **SDL2** - Used for low level handling of windows, timers, events, and input.
- **Make** - Build system
- **OpenAl** - audio (Using mojoal C wrapper on openal 1.1)
- **Tiled** - Used for creating the maps and outputting data for it
- **Ogg/Vorbis** - Used for audio decoding
- **Lua** - Scripting / Configuration
- **[Audacity](https://www.audacityteam.org)** - Music edits
- **[Doxygen](https://doxygen.nl/index.html)** - Generating documentation

## Building
- All dependencies are in the external folder, and if you need any of them, pull the submodule and build/install them with 
- - git submodule update --init --recursive

## Authors

- [@Kevin Blanchard](https://www.github.com/kjblanchard)

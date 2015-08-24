# CosmoScroll

A space-based shoot'em up game, written in C++ using the SFML library.

## How to play?

- Move spaceship:	<kbd>Up</kbd>, <kbd>Down</kbd>, <kbd>Left</kbd>, <kbd>Right</kbd>
- Shoot: <kbd>Space</kbd>
- Launch missile: <kbd>A</kbd>
- User cooler power-up: <kbd>LeftCtrl</kbd>
- Pause the game:	<kbd>Escap</kbd>
- Place HUD at the top of the screen: <kbd>Page Up</kbd>
- Place HUD at the bottom of the screen: <kbd>Page Down</kbd>
-  Take a screenshot (saved in `screenshots`): <kbd>F1</kbd>

## How to compile?

CosmoScroll should compile on any system supported by SFML (Windows, Linux, MacOS).

First, you must install the following libraries:

- [SFML](http://www.sfml-dev.org/), version >= 2.1
- [libdumb](http://dumb.sourceforge.net/) (for Debian/Ubuntu, simply install the `libdumb1-dev` package)

Run the `make` command if you are using a UNIX-based system (gcc), or you can use the Code::Blocks project file (`cosmoscroll.cbp`) to compile the game with this IDE.

If you want to use another compiler than gcc:
- Make sure your compiler is c++11 compliant
- Add the `src` directory in your compiler search path
- Link against the aforementioned libraries

## Configuration file

CosmoScroll saves your configuration in your application settings directory, using your operating system's convention:

- Windows: `Application Data/cosmoscroll/cosmoscroll.ini`
- Linux: `~/.config/cosmoscroll/cosmoscroll.ini`
- Mac OS: `~/Library/Application Support/cosmoscroll/cosmoscroll.ini`


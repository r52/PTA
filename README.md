# PTA

[![License](https://img.shields.io/github/license/r52/PTA)](https://github.com/r52/PTA/blob/master/LICENSE)
![Windows Build](https://github.com/r52/PTA/workflows/Windows%20Build/badge.svg)
[![Build status](https://ci.appveyor.com/api/projects/status/m0mh6snl47uxdcs1?svg=true)](https://ci.appveyor.com/project/r52/pta)

**PTA** (PoE Trade Assistant) is currently a [PoE-TradeMacro](https://github.com/PoE-TradeMacro/POE-TradeMacro) clone (with fewer features) that operates on the [Official Path of Exile Trade](https://www.pathofexile.com/trade) website instead of third-party alternatives. It hopes to become a full fledged trade assistant tool in the future.

**NOTE:** As with any pricing tools, never trust or rely on its results entirely. If something seems off, confirm with a manual search. It could very well be an issue with the app itself.

PTA is licensed under GPL-3.0

## Installation and Usage

[Download the latest release from Releases](https://github.com/r52/PTA/releases/latest/) **OR** a development build from [here](https://github.com/r52/PTA/actions?query=workflow%3A%22Windows+Build%22) or [here](https://ci.appveyor.com/project/r52/pta/build/artifacts).

Extract the archive and run **PTA.exe**

**If your PC is not running Windows 10 with the latest feature updates or you are getting a VC runtime error, you MUST download and install the [Microsoft Visual C++ Redistributable](https://aka.ms/vs/16/release/VC_redist.x64.exe)!**

## Features

- Simple and advanced item searches (**Ctrl+D** and **Ctrl+Alt+D** ala [PoE-TradeMacro](https://github.com/PoE-TradeMacro/POE-TradeMacro)) on www.pathofexile.com/trade
- Custom macros with Client.txt support. See [wiki](https://github.com/r52/PTA/wiki) for more information.

All shortcuts and macros can be re-configured in the settings.

## Building

### Requirements

- [Visual Studio 2019](https://www.visualstudio.com/)
- [Qt 5.12](http://www.qt.io/) (other versions such as Qt 5.14 are **NOT** supported)
- [Node.js 12](https://nodejs.org/)

Only Windows x64 is supported.

### Instructions

1. Build the PTA executable using `PTA.sln`.
2. `cd ./PTA/search/`
3. `npm ci && npm run build`

## Credits

- [Grinding Gear Games](http://www.grindinggear.com/) for [Path of Exile](https://www.pathofexile.com/)
- [brather1ng](https://github.com/brather1ng) for [RePoE](https://github.com/brather1ng/RePoE).
- [PoE-TradeMacro](https://github.com/PoE-TradeMacro/POE-TradeMacro)
- [poeprices.info](https://poeprices.info/)

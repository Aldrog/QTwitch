# QTwitch

This library is intended to make development of Twitch-powered applications for any platform as easy as possible.
Although it's primarily designed to simplify [TwitchTube](https://github.com/Aldrog/TwitchTube) and to make it's porting to other platforms easier, you will probably find QTwitch useful for various other applications from a chat bot or statistics collector to embedding a stream player into an app.

## Dependencies

* Qt 5
* C++17-capable compiler

## C++11

If for some reason you cannot use a recent compiler, QTwitch provides a C++11 compatibility mode that can be enabled by setting CXX11COMPATIBILITY environment variable.

This mode doesn't disable any of QTwitch features but will decrease performance and may be less stable as it relies on custom backported implementations of several functions of the standard library.

## Build

Twitch requires a Client-ID for all requests so before being able to use QTwitch you'll have to register your app on [developer's dashboard](https://dev.twitch.tv/), get it's Client-ID and set it to TWITCH_CLIENT_ID environment variable, e.g.:
```bash
export TWITCH_CLIENT_ID=your-client-id
```
If you need authorization you also have to set redirect url (also set on developer's dashboard) in TWITCH_REDIRECT_URI.
```bash
export TWITCH_REDIRECT_URI=http://localhost
```
If you use Qt Creator, you can also set these variables in Projects -> Build -> Build Environment section.

Then, you should be able to build the root project QTwitch.pro, e.g.:
```bash
qmake QTwitch.pro
make
```
This will generate a shared library for each module.  
Unfortunately, there're no installation rules nor any qmake or CMake helpers at the moment.

## Documentation

As the library matures, it's going to get a complete doxygen-like documentation. For now, there's only a sketch of it's general structure.

## Modules

The library consists of the following modules:

### Api

This module includes a basic framework for sending requests to Twitch APIs and authorization as well as descriptions for API endpoints.

All classes in the Api module are defined inside QTwitch::Api namespace.

**Works**: OAuth2 implicit flow, significant part of the New API, APIv5 search and follow/unfollow endpoints and Usher API.
**TODO**: describe remaining endpoints of the New API.

### Models

This module will include models for visual representation of various data provided by Twitch.
The models will be QAbstractListModel-inherited classes thus usable by both QtQuick and QWidgets, wrapping *Api* functionality.

**Status**: WIP.

### Chat

This module will include an implementation of IRC with support for Twitch extensions as well as a model for visual representation of the chat (Note: the latter might actually better suit `Models` module).

**Status**: High priority in TODO list.

### Player

Embeddable GStreamer-based stream/vod player.

**Status**: Low priority in TODO list.

### tests

Contains tests for functionality of each module.

**Status**: Has basic functional tests for most implemented API endpoints. All tests pass successfully.

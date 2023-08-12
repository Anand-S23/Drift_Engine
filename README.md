# Drift Engine

Low level game engine being developed in C.

## Status Update

Drift Engine was pretty far along it's development, though it was not completed
and a lot of functionality was missing (like audio), users are able to make 2D 
games with Drift. Despite this, I have decided to rewrite the engine to provide
better cross platform support, 3D rendering support and overall better API design.

That being said this is the following features that have been completed for the rewrite:
- Core Platform Layer (Window Creation, Core Engine Update Loop)
- Input Handling
- Enforce Frame rate
- Internal Math Library (Quatarion support needs to be added)

Currently being developed:
- 2D and 3D renderer

Still needs to be implemented:
- Logging
- Hot Reloading
- Windows (and possible Mac) port
- Audio
- UI system

Note: The old engine still exists in the `old` branch. The new changes are
made to the `rewrite` branch and merged into main once features are finalized

## Quick Start

For the rewrite only Linux is supported, the apporiate Windows functions call
have yet to be implemented.

1. Clone the Drift Engine respository
2. Run `./build.sh` to compile the program
3. The output will go to the build directory, so run `./build/app` to run the program

## Additional Information

There is a respository ([Drift Exaple Projects](https://github.com/Anand-S23/drift-example-projects)) with some example programs,
that can be used as reference. This is will be using the old version of the engine until rendering is completed in Drift.

If you want to get into engine programming the following are some links to get you started:
- [Handmade Hero](https://handmadehero.org/): Casey Muratori's series on building a game engine from scratch (biggest inspiration for drift)
- [Sokol](https://github.com/floooh/sokol): Simple set of STB-style cross-platform libraries for C and C++, useful for game development
- [DigiPen Game Engine Architecture Club](https://www.youtube.com/@GameEngineArchitects): Set of talks able various game engine development topics


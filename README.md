# minesweeper-interface
A C++ interface for Minesweeper that handles game logic.

## Usage

For an example of usage (how to set up your game loop, etc), see [`test.cpp`](test.cpp).

**C++ usage:**

Mostly just drag and drop and include. The example linked above should show you all you need to know. You will need to compile with C++14 support or else you will get a bunch of warnings and errors.

**Java usage:**

Look into how to use the JNI. You will need to take the board and convert it from `std::vector<std::vector<tile_t>>` to something like a char array. In the future, I might make a Java wrapper for this, although you'll likely still need to compile the C++ library yourself still.

**Objective-C usage:**

You can use this natively by simply dragging and dropping it into your Objective-C project and using Objective-C++. Again, make sure C++14 is enabled or else you will get warnings and errors.

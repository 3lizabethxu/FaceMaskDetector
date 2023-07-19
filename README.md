Group 32 - 3307

# Requirements

This project is designed to run on a Mac running OSX 11.4

Install dependancies using homebrew:
```
brew install opencv
brew install libtensorflow
brew install qt
```

We shipped it with cppflow so you do not need to install this library yourself.

Ensure packages (opencv and tensorflow) are linked with pkg-config.
If it is your first time using `pkg-config` then please read this guide for configuration instructions.

[https://people.freedesktop.org/~dbn/pkg-config-guide.html](https://people.freedesktop.org/~dbn/pkg-config-guide.html)

# Setup Environment

Update the environment variables in `environment.hpp`

`FACE_MODEL_LOCATION`, `MASK_MODEL_LOCATION`, and `OUTPUT_FOLDER` 

to reflect your system pointing to the model files.

### Building

To setup your environment please use the build_helper script provided by running `./build_helper` from the terminal, this file will ensure the xCode files are created for the environment also making sure an initial build is created for the app.

# How to run this thing

### To run the project:

Open `BigBrother.xcodeproj` in xCode and and run/build


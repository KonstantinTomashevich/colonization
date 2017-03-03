# Colonization
TODO: description

**Engine:** Urho3D.

**Programming languages:** C++ (main library, tests, launchers) and AngelScript (client, custom victory types, *in future: AIs*), AngelScript **!=** ActionScript.

## Features:
* TODO: features

## How to build
Step 1. Download Urho3D SDK binary release from [SourceForge](https://sourceforge.net/projects/urho3d/files/Urho3D/). Recommended version -- **1.6**. Also you can build Urho3D from source, for more information visit [Urho3D Site](https://urho3d.github.io).

Step 2. Set `URHO3D_HOME` environment var equal to the unpacked sdk dir.

Step 3. Clone repository:
```bash
git clone https://github.com/KonstantinTomashevich/colonization.git
```
Step 4. Generate build directory via CMake:
```bash
cd colonization && mkdir build && cd build && cmake ..
```
Step 5. Build and run tests.
```bash
make && make test
```
Step 6. Executables will be in ${BINARY_DIR}/bin folder.

## Screenshots
TODO: screenshots

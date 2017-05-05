## Colonization [Work In Progress]
Multiplayer RTS game in colonization setting.

**Engine:** Urho3D.

**Programming languages:** C++ (main library, tests, launchers) and AngelScript (client, custom victory types, *in future: AIs*), AngelScript **!=** ActionScript.

## Features:
- [x] Districts system like Europa Universalis.
- [x] Different victory types like Civilization (victory types can be added by separate script, specified in map info xml).
- [x] Colonies evolution system (for now, evolution of farms, mines, industry, logistics and defense).
- [x] Trade areas system (district with good logistics unite to trade area and it will give you additional profit).
- [x] Diplomacy system (basic, you can declare war and send peace treaty).
- [ ] Colony level of life system.
- [ ] Natives and relations with them.
- [x] Multiplayer support (scene replication is one-directional, the server always has authority, the client responds with network messages).

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
Step 6. Executables will be in `${CMAKE_BINARY_DIR}/bin` in `Tools`, `Tests` and `GameLauncher` folders.

## Screenshots
![0](https://s12.postimg.org/4ipmrhf25/image.png)
![1](https://s12.postimg.org/drrt1lny5/image.png)
![2](https://s12.postimg.org/fxm3w3rel/image.png)
![3](https://s12.postimg.org/dhovvzypp/image.png)
![4](https://s12.postimg.org/lbphhe6il/image.png)
![5](https://s12.postimg.org/ade7z7hx9/image.png)
![6](https://s12.postimg.org/8zml3winx/image.png)

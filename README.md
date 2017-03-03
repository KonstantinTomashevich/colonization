## Colonization [Work In Progress]
Multiplayer RTS game in colonization setting. 

**Engine:** Urho3D.

**Programming languages:** C++ (main library, tests, launchers) and AngelScript (client, custom victory types, *in future: AIs*), AngelScript **!=** ActionScript.

## Features:
- [x] Districts system like Europa Universalis.
- [x] Different victory types like Civilization (victory types can be added by separate script, specified in map info xml).
- [x] Colonies evolution system (for now, evolution of farms, mines, industry, logistics and defense).
- [x] Trade areas system (district with good logistics unite to trade area and it will give you additional profit).
- [ ] Diplomacy system.
- [ ] Colony level of life system.
- [ ] Natives and relations with them.
- [x] Multiplayer support (basic multiplayer works at commit 07448146c1a85e2766adfa3ce62613260807918d, now working on normal game start ui).

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

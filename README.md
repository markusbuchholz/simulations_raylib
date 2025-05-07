# Simulations using Raylib

Sample simulations using the [raylib](https://github.com/raysan5/raylib).

Check [here](https://www.raylib.com/index.html) for more examples. <br>

---


## Install RayLib

```bash
sudo apt update

sudo apt install -y build-essential git cmake \
  libasound2-dev libx11-dev libxrandr-dev libxi-dev \
  libxinerama-dev libxcursor-dev libgl1-mesa-dev \
  libglu1-mesa-dev libwayland-dev libxkbcommon-dev

git clone https://github.com/raysan5/raylib.git

cd raylib

mkdir build && cd build

cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release ..

make -j$(nproc)

sudo make install

sudo ldconfig
```

## Build and run simulations

```bash

git clone https://github.com/markusbuchholz/simulations_raylib.git

cd simulations_raylib/src

gcc robot_ray.c -o robot -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

./robot

```


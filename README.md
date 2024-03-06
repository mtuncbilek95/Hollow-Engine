# How to run Hollow Engine MiniVk Branch

Download Python 3.12 and Vulkan SDK (v1.3.275.0). While installing Vulkan, choose the features below.

- Shader Toolchain Debug Symbols - 64bit
- SDL2 libraries and headers
- Vulkan Memory Allocator header.

You also need couple packages in Python as well.
```terminal
pip install colorama
pip install requests
pip install glob
pip install time
pip install os
pip install time
```

I dont exactly remember which packages are built-in so I wrote all the packages I used. After installing everything, you can use the batch file called __InstallExternal.bat__ to install External libraries to the Engine. Then you are good to go.

Currently writing the descriptors. So no constant buffer system.

# Quirk Engine [![Group 1](https://github.com/user-attachments/assets/7c07abde-6e3a-4d22-b4fc-849b8c9e728c)](https://github.com/Jayantkumar56/Quirk/blob/master/LICENSE)

Quirk is primarily an early-stage interactive application and rendering engine. Currently not much is implemented and works on windows only. It uses OpenGl as the rendering api.

# Getting Started

Visual Studio 2022 is recommended, Quirk is not tested on other development environments, while it uses Premake to generate project files for various platforms and IDEs.

<ins>**1. Downloading the repository:**</ins>

```git clone --recursive https://github.com/Jayantkumar56/Quirk```

If the repository was cloned non-recursively, clone the necessary submodules with ```git submodule update --init```

<ins>**2. Configuring the dependencies:**</ins>

To generate project files for Visual Studio 2022 run the [GenerateProject.bat](https://github.com/Jayantkumar56/Quirk/blob/master/GenerateProject.bat) file found in the main workspace directory.

<ins>**3. Ready to Go!**</ins>

Project files are generated now you can open Quirk in your IDE.

# Main features to come:

* Fast 2D rendering (UI, particles, sprites, etc.).
* Support for Mac and Linux.
* Native rendering API support (DirectX, Vulkan, Metal).
* Fully featured viewer and editor applications.
* Fully scripted interaction and behavior.
* Integrated 3rd party 2D and 3D physics engine.
* Procedural terrain and world generation.
* Audio system.

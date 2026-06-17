# Las Vegas Sphere Simulation

![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl)
![C++](https://img.shields.io/badge/C++17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)

A real-time 3D graphics and spatial audio simulation inspired by the Las Vegas Sphere.

https://github.com/user-attachments/assets/d647c63c-b06a-4bb4-882f-96e7c8adb492

## Tech Stack

* **Core:** C++17
* **Graphics:** OpenGL (Core Profile 4.1), GLAD, GLFW3
* **Math:** GLM (OpenGL Mathematics)
* **Audio:** OpenAL, dr_mp3 (Single-header MP3 decoder)
* **Assets:** stb_image (Texture loading)
* **Build System:** CMake

## Controls
The simulation includes a fully interactive fly-through camera and real-time environment controls to demonstrate dynamic lighting and audio updates.

| Keybinds | How it works |
| --- | --- |
| **W / A / S / D** | Move the camera Forward, Left, Backward, and Right |
| **Spacebar** | Move the camera Up |
| **Left Shift** | Move the camera Down |
| **Key 1,2,3,4,5** | Select a specific pattern |
| **Left/Right Arrow** | Select Previous/Next Patterns |
| **Up/Down Arrow** | Increase/Decrease the Brightness |
| **X** | Toggle light on/off |


## Dynamic Textures & Shaders
I wrote custom fragment shaders so the sphere can dynamically cycle between 5 distinct visual states in real-time. It also calculates ambient, diffuse, and specular lighting against the concrete base to ensure the glowing patterns interact realistically with the environment.

https://github.com/user-attachments/assets/7a1d9236-5a22-4cf2-b514-c447daf5a9a6


## Audio
To handle the spatial audio, I built a custom OpenAL hardware wrapper that decodes compressed MP3s into 16-bit PCM buffers on the fly. The engine tracks the camera's exact 3D vectors and applies an Inverse Distance Clamped math model, meaning the music naturally and aggressively fades out as you back away from the sphere.

https://github.com/user-attachments/assets/014c4df6-793c-4b6d-8838-ce1ff47ebaf5





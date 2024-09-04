# 3D Rasterizer in Windows Terminal

## Project Description
This project is a solo side project that implements a 3D rasterizer within the Windows terminal. The rasterizer renders a rotating cube in 3D space and allows for basic camera movement to navigate around the cube. Although the rasterizer is limited to displaying this single mesh type, it demonstrates the core principles of 3D rendering, including perspective projection and real-time rotation.

## Features
- **Rotating 3D Cube:** The cube continuously rotates on all axes, showcasing basic 3D transformations.
- **Camera Movement:** Users can move the camera horizontally, vertically, and zoom in and out, providing different perspectives of the cube.
- **Terminal Viewport:** The entire terminal window acts as the viewport for the rendered scene.

## Requirements
- **GCC:** Ensure GCC is installed on your system to build and run the project.
- **Windows OS:** This project is designed to run in the Windows terminal.

## Build and Run Instructions
1. **Download the Repository:**
   - Download the repository as a ZIP file and extract it to your desired location.

2. **Build the Project:**
   - Open a terminal window in the root directory of the project.
   - Run the command: `./build`

3. **Run the Project:**
   - After building, run the project using the command: `./run`
   - The entire terminal will transform into a viewport for the 3D rasterizer.

## Camera Controls
Use the following keys to move the camera:

- **Move Right:** Press the **Right Arrow (→)** key.
- **Move Left:** Press the **Left Arrow (←)** key.
- **Tilt Up:** Press the **Up Arrow (↑)** key.
- **Tilt Down:** Press the **Down Arrow (↓)** key.
- **Zoom In:** Press the **Right Shift** key.
- **Zoom Out:** Press the **Left Shift** key.

These controls allow you to navigate the 3D space and observe the cube from different angles.

#include "canvas.h"
#include <cmath>

const int FPS = 100;

int main() {
	Canvas canvas(2, 0);    
	std::vector<Vector3f> vertices = {
		{-0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f}
	};
	
	std::vector<int> indices = {
		0, 1, 2, 2, 3, 0,
		1, 5, 6, 6, 2, 1,
		5, 4, 7, 7, 6, 5,
		4, 0, 3, 3, 7, 4,
		4, 5, 1, 1, 0, 4,
		3, 2, 6, 6, 7, 3
	};

	std::vector<char> colors = {
		COLORS[9], COLORS[9],
		COLORS[9], COLORS[9],
		COLORS[9], COLORS[9],
		COLORS[9], COLORS[9],
		COLORS[9], COLORS[9],
		COLORS[9], COLORS[9]
	};

	float z = 0;

	canvas.camera.position.y = 50;
	canvas.camera.rotation.x = M_PI / 6.0f;

	while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
		canvas.clear();
		
		canvas.drawShape(vertices, indices, colors, createTransformationMatrix(Vector3f(0, 0, 100), Vector3f(z, z, z), Vector3f(8, 8, 8)));
		z += 0.03f;

		if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
			canvas.camera.position.x += 0.1f;
		else if(GetAsyncKeyState(VK_LEFT) & 0x8000)
			canvas.camera.position.x -= 0.1f;
		else if(GetAsyncKeyState(VK_UP) & 0x8000)
			canvas.camera.rotation.x += 0.01f;
		else if(GetAsyncKeyState(VK_DOWN) & 0x8000)
			canvas.camera.rotation.x -= 0.01f;
		else if(GetAsyncKeyState(VK_LSHIFT) & 0x8000)
			canvas.camera.position.z -= 0.4f;
		else if(GetAsyncKeyState(VK_RSHIFT) & 0x8000)
			canvas.camera.position.z += 0.4f;

		canvas.render();
		Sleep(1000/FPS);
	}

	return 0;
}
#include <windows.h>
#include <iostream>
#include <cmath>
#include "canvas.h"
#include "math.h"
#include <algorithm>

Canvas::Canvas(const Rect& rect, int xpad, int ypad):
	console(GetStdHandle(STD_OUTPUT_HANDLE)), rect(rect), xpad(xpad), ypad(ypad) {

	init();
}

Canvas::Canvas(int xpad, int ypad):
	console(GetStdHandle(STD_OUTPUT_HANDLE)), xpad(xpad), ypad(ypad) {
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(console, &csbi);
	COORD consoleSize;
	consoleSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	consoleSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	this->rect = { 0, 0, consoleSize.X / (xpad + 1), (consoleSize.Y / (ypad + 1)) - 1 };
	
	init();
}

Canvas::~Canvas() {
	delete[] content;
	delete[] colorBuffer;
	showCurser(TRUE);
}

void Canvas::init() {
	int xp = xpad + 1;
	int yp = ypad + 1;
	int numPixels = rect.w * xp * rect.h;
	int numLinebreaks = rect.h * yp;
	int numXOffsets = rect.h * rect.x * xp;

	contentLength = numPixels + numLinebreaks + numXOffsets + 1;
	
	colorBuffer = new char[rect.h * rect.w];
	content = new char[contentLength];

	projection = createProjectionMatrix((float) rect.w / (float) rect.h, camera.fov * (M_PI / 180.0f), camera.znear, camera.zfar);

	system("clear");
	showCurser(FALSE);
	clear();
}

void Canvas::clear() {
	int contentIndex = 0;
	for(int i = 0; i < rect.h; i++) {
		for(int ox = 0; ox < rect.x * (xpad + 1); ox++) {
			content[contentIndex + ox] = ' ';
		}
		contentIndex += rect.x * (xpad + 1);
		for(int j = 0; j < rect.w; j++) {
			colorBuffer[i * rect.w + j] = COLORS[0];
			content[contentIndex++] = COLORS[0];
			for(int x = 0; x < xpad; x++) {
				content[contentIndex + x] = ' ';
			}
			contentIndex += xpad;
		}
		content[contentIndex++] = '\n';
		for(int y = 0; y < ypad; y++) {
			content[contentIndex + y] = '\n';
		}
		contentIndex += ypad;   
	}
	content[contentIndex] = '\0';
}

void Canvas::plotPixel(int x, int y, char p) {
	if(x >= 0 && x < rect.w && y >= 0 && y < rect.h) {
		colorBuffer[y * rect.w + x] = p;

		int xp = xpad + 1;
		int yp = ypad + 1;
		int w = rect.w;
		int h = rect.h;
		int ox = rect.x * xp;

		int contentIndex = (x * xp + ox) + (((w * xp + ox) + yp) * y);
		content[contentIndex] = p;
	}
}

void Canvas::drawTriangle(const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, char c) {
	drawLine(v1.x, v1.y, v2.x, v2.y, c);
	drawLine(v2.x, v2.y, v3.x, v3.y, c);
	drawLine(v3.x, v3.y, v1.x, v1.y, c);
}

void Canvas::fillTriangle(const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, char c) {
	int xmin = std::min(std::min(v1.x, v2.x), v3.x);
	int ymin = std::min(std::min(v1.y, v2.y), v3.y);
	int xmax = std::max(std::max(v1.x, v2.x), v3.x);
	int ymax = std::max(std::max(v1.y, v2.y), v3.y);

	for(int y = ymin; y <= ymax; y++){
		for(int x = xmin; x <= xmax; x++){
			Vector2f currentPixel(x, y);
			if(isInsideTriangle(currentPixel, Vector2f(v1.x, v1.y), Vector2f(v2.x, v2.y), Vector2f(v3.x, v3.y))) {
				plotPixel(currentPixel.x, currentPixel.y, c);
			}
		}   
	}
}

void Canvas::render() {
	SetConsoleCursorPosition(console, {0, (short) (rect.y * (ypad + 1))});
	DWORD charsWritten;
	WriteConsole(console, content, contentLength - 1, &charsWritten, NULL);
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, char p) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int slope = dy > dx;

	if (slope) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int dx2 = x2 - x1;
	int dy2 = abs(y2 - y1);

	int error = dx2 / 2;
	int ystep = (y1 < y2) ? 1 : -1;
	int y = y1;

	for (int x = x1; x <= x2; x++) {
		if (slope) {
			plotPixel(y, x, p);
		} else {
			plotPixel(x, y, p);
		}

		error -= dy2;
		if (error < 0) {
			y += ystep;
			error += dx2;
		}
	}
}

void Canvas::drawShape(std::vector<Vector3f> vertices, const std::vector<int>& indices, const std::vector<char> colors, const Matrix4f& transformation) {
	if(vertices.empty()) return; 

	view = createTransformationMatrix(camera.position, camera.rotation, Vector3f(1, 1, 1));

	for(std::size_t i = 0; i < vertices.size(); i++) {
		Vector4f transformed = projection * view * transformation * Vector4f(vertices[i].x, vertices[i].y, vertices[i].z, 1);

		vertices[i].x = transformed.x / transformed.w;
		vertices[i].y = transformed.y / transformed.w;
		vertices[i].z = transformed.z / transformed.w;
	}

	for(int i = 0; i < indices.size(); i += 3) {
		int index0 = indices[i];
		int index1 = indices[i + 1];
		int index2 = indices[i + 2];

		float i0x = (rect.w / 2.0f) * vertices[index0].x + rect.w / 2.0f;
		float i0y = (rect.h / 2.0f) * -vertices[index0].y + rect.h / 2.0f;

		float i1x = (rect.w / 2.0f) * vertices[index1].x + rect.w / 2.0f;
		float i1y = (rect.h / 2.0f) * -vertices[index1].y + rect.h / 2.0f;

		float i2x = (rect.w / 2.0f) * vertices[index2].x + rect.w / 2.0f;
		float i2y = (rect.h / 2.0f) * -vertices[index2].y + rect.h / 2.0f;

		drawTriangle(Vector2f(i0x, i0y), Vector2f(i1x, i1y), Vector2f(i2x, i2y), colors[i / 3]);
	}
}

void Canvas::showCurser(BOOL flag) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(console, &cursorInfo);
}
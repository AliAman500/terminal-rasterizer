#pragma once

#include <windows.h>
#include <vector>
#include "math.h"
#include <array>

class Canvas {

	private:
		char* colorBuffer;
		char* content;

		Matrix4f projection;
		Matrix4f view;

		void init();
	public:
		Canvas(const Rect& rect, int xpad, int ypad);
		Canvas(int xpad, int ypad);
		~Canvas();

		HANDLE console;        
		Rect rect;
		Camera camera;

		int xpad;
		int ypad;
		int contentLength;

		void clear();
		void plotPixel(int x, int y, char p);
		void drawTriangle(const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, char c);
		void fillTriangle(const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, char c);
		void render();
		void drawLine(int x1, int y1, int x2, int y2, char p);
		void drawShape(std::vector<Vector3f> vertices, const std::vector<int>& indices, const std::vector<char> colors, const Matrix4f& transformation);
		void showCurser(BOOL flag);
};
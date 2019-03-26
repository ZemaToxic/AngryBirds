#ifndef __UTILS_H__
#define __UTILS_H__

// Definitions
#define BUTTON_UP 0;
#define BUTTON_DOWN 1;

// Open GL Header files etc
#include "../Dependencies/soil/SOIL.h"
#include "../Dependencies/glm/glm.hpp"
#include "../Dependencies/glm/gtc/matrix_transform.hpp"
#include "../Dependencies/glm/gtc/type_ptr.hpp"
#include "../Dependencies/include/glew.h"
#include "../Dependencies/include/freeglut.h"

// Main includes
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include "input.h"

// Model Type
enum ModelType
{
	kTriangle = 0,
	kQuad = 1,
	kCube = 2,
	kSphere = 3
};

// Vertex Format
struct Position
{
	float x, y, z;

	Position(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Position()
	{
	}
};

struct TexCoord
{
	float u, v;

	TexCoord(float _u, float _v)
	{
		u = _u;
		v = _v;
	}

	TexCoord()
	{
	}
};

struct Normals
{
	float x, y, z;

	Normals(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Normals()
	{
	}
};

struct VertexFormat
{
	Position pos;
	TexCoord texCoord;
	Normals normal;

	VertexFormat(Position _pos, TexCoord _texCoord, Normals _normal)
	{
		pos = _pos;
		texCoord = _texCoord;
		normal = _normal;
	}

	VertexFormat()
	{
	}
};

struct IndexFormat
{
	int x;
	int y;
	int z;

	IndexFormat(int _x, int _y, int _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

namespace utils
{
	const static float window_pos_x = 250;
	const static float window_pos_y = 150;
	const static float window_width = 1000;
	const static float window_height = 500;

	static float ambientStrength = 1.0f;
	static float specularStrength = 1.0f;

	static bool DEBUG = false;

	static unsigned char key_state[255];

	// Handle window resizing
	inline void handle_resize(int w, int h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, static_cast<double>(w) / static_cast<double>(h), 1.0, 200.0);
	}

	// Setting Model Data
	inline void setTriData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices)
	{
		std::vector<VertexFormat> Vertices;

		Vertices.emplace_back(Position(0.0f, 1.0f, 0.0f), TexCoord(1, 1), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(1.0, 0.0, 0.0), TexCoord(0, 1), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(-1.0, 0.0, 0.0), TexCoord(1, 0), Normals(0.0f, 0.0f, 1.0));


		std::vector<GLuint> Indices;
		Indices.push_back(0);
		Indices.push_back(1);
		Indices.push_back(2);

		vertices.clear();
		indices.clear();

		vertices = Vertices;
		indices = Indices;
	}

	inline void setQuadData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices)
	{
		std::vector<VertexFormat> Vertices;
		Vertices.emplace_back(Position(-1.0f, -1.0f, 1.0f), TexCoord(1.0, 1.0), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(1.0f, -1.0f, 1.0f), TexCoord(0.0, 1.0), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(1.0f, 1.0f, 1.0f), TexCoord(0.0, 0.0), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(-1.0f, 1.0f, 1.0f), TexCoord(1.0, 0.0), Normals(0.0f, 0.0f, 1.0));

		std::vector<GLuint> Indices;
		Indices.push_back(0);
		Indices.push_back(1);
		Indices.push_back(2);
		Indices.push_back(0);
		Indices.push_back(2);
		Indices.push_back(3);
		vertices.clear();
		indices.clear();
		vertices = Vertices;
		indices = Indices;
	}

	inline void setCubeData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices)
	{
		glm::vec3 color;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		std::vector<VertexFormat> Vertices;
		//back
		Vertices.emplace_back(Position(-1.0f, -1.0, -1.0), TexCoord(0.0, 1.0), Normals(0.0f, 0.0f, -1.0));
		Vertices.emplace_back(Position(1.0, -1.0, -1.0), TexCoord(0.0, 0.0), Normals(0.0f, 0.0f, -1.0));
		Vertices.emplace_back(Position(1.0, 1.0, -1.0), TexCoord(1.0, 0.0), Normals(0.0f, 0.0f, -1.0));
		Vertices.emplace_back(Position(-1.0, 1.0, -1.0), TexCoord(1.0, 1.0), Normals(0.0f, 0.0f, -1.0));

		//front
		Vertices.emplace_back(Position(-1.0f, -1.0f, 1.0f), TexCoord(1.0, 1.0), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(1.0f, -1.0f, 1.0f), TexCoord(0.0, 1.0), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(1.0f, 1.0f, 1.0f), TexCoord(0.0, 0.0), Normals(0.0f, 0.0f, 1.0));
		Vertices.emplace_back(Position(-1.0f, 1.0f, 1.0f), TexCoord(1.0, 0.0), Normals(0.0f, 0.0f, 1.0));

		//left
		Vertices.emplace_back(Position(-1.0, 1.0, 1.0), TexCoord(0.0, 0.0), Normals(-1.0f, 0.0f, 0.0));
		Vertices.emplace_back(Position(-1.0, 1.0, -1.0), TexCoord(1.0, 0.0), Normals(-1.0f, 0.0f, 0.0));
		Vertices.emplace_back(Position(-1.0, -1.0, -1.0), TexCoord(1.0, 1.0), Normals(-1.0f, 0.0f, 0.0));
		Vertices.emplace_back(Position(-1.0f, -1.0, 1.0), TexCoord(0.0, 1.0), Normals(-1.0f, 0.0f, 0.0));

		//right
		Vertices.emplace_back(Position(1.0, 1.0, 1.0), TexCoord(1.0, 0.0), Normals(1.0f, 0.0f, 0.0));
		Vertices.emplace_back(Position(1.0, 1.0, -1.0), TexCoord(0.0, 0.0), Normals(1.0f, 0.0f, 0.0));
		Vertices.emplace_back(Position(1.0f, -1.0, -1.0), TexCoord(0.0, 1.0), Normals(1.0f, 0.0f, 0.0));
		Vertices.emplace_back(Position(1.0, -1.0, 1.0), TexCoord(1.0, 1.0), Normals(1.0f, 0.0f, 0.0));

		//bottom
		Vertices.emplace_back(Position(-1.0f, -1.0, -1.0), TexCoord(1.0, 1.0), Normals(0.0f, -1.0f, 0.0));
		Vertices.emplace_back(Position(1.0, -1.0, -1.0), TexCoord(0.0, 1.0), Normals(0.0f, -1.0f, 0.0));
		Vertices.emplace_back(Position(1.0, -1.0, 1.0), TexCoord(0.0, 0.0), Normals(0.0f, -1.0f, 0.0));
		Vertices.emplace_back(Position(-1.0, -1.0, 1.0), TexCoord(1.0, 0.0), Normals(0.0f, -1.0f, 0.0));

		//top
		Vertices.emplace_back(Position(-1.0f, 1.0f, -1.0f), TexCoord(0.0, 1.0), Normals(0.0f, 1.0f, 0.0));
		Vertices.emplace_back(Position(1.0f, 1.0f, -1.0f), TexCoord(1.0, 1.0), Normals(0.0f, 1.0f, 0.0));
		Vertices.emplace_back(Position(1.0f, 1.0f, 1.0f), TexCoord(1.0, 0.0), Normals(0.0f, 1.0f, 0.0));
		Vertices.emplace_back(Position(-1.0f, 1.0f, 1.0f), TexCoord(0.0, 0.0), Normals(0.0f, 1.0f, 0.0));

		std::vector<GLuint> Indices;

		Indices.push_back(0);
		Indices.push_back(1);
		Indices.push_back(2);
		Indices.push_back(2);
		Indices.push_back(3);
		Indices.push_back(0);

		Indices.push_back(4);
		Indices.push_back(5);
		Indices.push_back(6);
		Indices.push_back(6);
		Indices.push_back(7);
		Indices.push_back(4);

		Indices.push_back(8);
		Indices.push_back(9);
		Indices.push_back(10);
		Indices.push_back(10);
		Indices.push_back(11);
		Indices.push_back(8);

		Indices.push_back(12);
		Indices.push_back(13);
		Indices.push_back(14);
		Indices.push_back(12);
		Indices.push_back(14);
		Indices.push_back(15);

		Indices.push_back(16);
		Indices.push_back(17);
		Indices.push_back(18);
		Indices.push_back(18);
		Indices.push_back(19);
		Indices.push_back(16);

		Indices.push_back(20);
		Indices.push_back(21);
		Indices.push_back(22);
		Indices.push_back(22);
		Indices.push_back(23);
		Indices.push_back(20);

		vertices.clear();
		indices.clear();
		vertices = Vertices;
		indices = Indices;
	}

	inline void setSphereData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices)
	{
		std::vector<VertexFormat> Vertices;
		std::vector<GLuint> Indices;

		float latitudeBands = 30;
		float longitudeBands = 30;
		float radius = 1.0f;

		for (float latNumber = 0; latNumber <= latitudeBands; latNumber++)
		{
			float theta = latNumber * (float)3.14 / latitudeBands;
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);

			for (float longNumber = 0; longNumber <= longitudeBands; longNumber++)
			{
				float phi = (float)(longNumber * 2 * 3.14 / longitudeBands);
				float sinPhi = sin(phi);
				float cosPhi = cos(phi);

				VertexFormat vs;

				vs.texCoord.u = 1 - (longNumber / longitudeBands); // u
				vs.texCoord.v = 1 - (latNumber / latitudeBands); // v

				vs.normal.x = cosPhi * sinTheta; // x
				vs.normal.y = cosTheta; // y
				vs.normal.z = sinPhi * sinTheta; // z

				vs.pos.x = radius * vs.normal.x;
				vs.pos.y = radius * vs.normal.y;
				vs.pos.z = radius * vs.normal.z;

				Vertices.push_back(vs);
			}
		}


		for (GLuint latNumber = 0; latNumber < latitudeBands; latNumber++)
		{
			for (GLuint longNumber = 0; longNumber < longitudeBands; longNumber++)
			{
				GLuint first = (GLuint)((latNumber * (longitudeBands + 1)) + longNumber);
				GLuint second = (GLuint)(first + longitudeBands + 1);

				Indices.push_back(first);
				Indices.push_back(second);
				Indices.push_back(first + 1);

				Indices.push_back(second);
				Indices.push_back(second + 1);
				Indices.push_back(first + 1);
			}
		}

		vertices.clear();
		indices.clear();
		vertices = Vertices;
		indices = Indices;
	}
}


#endif // !__UTILS_H__

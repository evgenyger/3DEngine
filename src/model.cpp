#include "stdafx.h"
#include "model.h"

void Model::loadOBJ (std::string path)
{
	vertexCount = 0;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::ifstream rm;
	rm.open(path, std::ios::in);
	if (!rm.is_open())
	{
		std::cout << ">> " << path << " >> Failed to load model!" << std::endl;
		return;
	}

	for (std::string line; std::getline(rm, line);)
	{
		std::istringstream iss(line);
		std::string cmd;
		iss >> cmd;

		/* Vertex */
		if (cmd == "v")
		{
			glm::vec3 v;
			iss >> v.x >> v.y >> v.z;
			temp_vertices.push_back(v);
		}

		/* UV */
		else if (cmd == "vt")
		{
			glm::vec2 uv;
			iss >> uv.x >> uv.y;
			uv.y = 1 - uv.y;
			temp_uvs.push_back(uv);
		}

		/* Normal */
		else if (cmd == "vn")
		{
			glm::vec3 n;
			iss >> n.x >> n.y >> n.z;
			temp_normals.push_back(n);
		}

		/* Face */
		else if (cmd == "f")
		{
			std::string v1, v2, v3;
			char d; // dummy
			std::vector<int> vertexIndex(3), uvIndex(3), normalIndex(3);
			iss >> vertexIndex[0] >> d >> uvIndex[0] >> d >> normalIndex[0]
				>> vertexIndex[1] >> d >> uvIndex[1] >> d >> normalIndex[1]
				>> vertexIndex[2] >> d >> uvIndex[2] >> d >> normalIndex[2];

			Vertex buff;
			for (int i = 0; i < 3; i++)
			{
				buff.pos	= temp_vertices[vertexIndex[i] - 1];
				buff.uv		= temp_uvs[uvIndex[i] - 1];
				buff.normal = temp_normals[normalIndex[i] - 1];
				
				vertexCount++;
				vertices.push_back(buff);
			}
		}

		else {} // ignore
	}

	rm.close();

	//for (int i = 0; i < vertices.size() * sizeof(Vertex) / sizeof(float); i++)
	//	std::cout << ((float*)&vertices[0])[i] << " ";

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

glm::mat4 ModelInstance::getModelMatrix() 
{
	return glm::translate(glm::mat4(1.0f), pos) * glm::mat4_cast(orientation);
}

void ModelInstance::move(glm::vec3 vec)
{
	pos += vec;
}

void ModelInstance::rotate(float angle, const glm::vec3 &axis)
{
	orientation = glm::quat(axis * angle) * orientation;
	glm::normalize(orientation);
}

void ModelInstance::rotate(glm::quat rotation)
{
	orientation = rotation * orientation;
}

void ModelInstance::turn(float angle)
{
	rotate(glm::angleAxis(angle, orientation * glm::vec3(0.0f, 1.0f, 0.0f)));
}

void ModelInstance::pitch(float angle)
{
	rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void ModelInstance::yaw(float angle)
{
	rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ModelInstance::roll(float angle)
{
	rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
}
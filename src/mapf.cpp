#include "stdafx.h"
#include "mapf.h"
#include "model.h"
#include "renderer.h"
#include "log.h"

std::vector<std::unique_ptr<ModelInstance>> objects;
std::map<std::string, Model*> models;

Model *suzanne = new Model;
Model *ground = new Model;
Model *msky = new Model;

ModelInstance *cannon = new ModelInstance;
ModelInstance *sky = new ModelInstance;

void initModels()
{
	models["suzanne"] = suzanne;
	models["ground"] = ground;

	msky->loadOBJ("sky.obj");
	msky->texture.load("sky.png");

	ground->loadOBJ("ground.obj");
	ground->texture.load("ground2.jpg");

	// -----

	sky->type = msky;
}

bool mRead()
{
	std::ifstream im;
	im.open("map.txt", std::ios::in);

	if (!im.is_open())
	{
		error("Map file wasn't found, exiting");

		im.close();
		return 1;
	}

	std::string line;
	while (std::getline(im, line))
	{
		std::istringstream iss(line);
		std::string type;
		auto buff = std::make_unique<ModelInstance>();

		iss >> type;
		auto it = models.find(type);

		if (it == models.end())
		{
			warning(fmt::format("[Warning] Unknown model name {}", type));
			continue;
		}

		buff->type = models[type];

		glm::vec3 v;
		iss >> v.x >> v.y >> v.z;
		buff->pos = v;

		objects.push_back(std::move(buff));
	}

	im.close();
	return 0;
}
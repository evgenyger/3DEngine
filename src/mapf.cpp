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
	msky->loadOBJ("sky.obj");
	msky->texture.load("sky.png");

	sky->type = msky;
}

bool mRead()
{
	std::ifstream im;
	im.open("map.txt", std::ios::in);

	if (!im.is_open())
	{
		error("No map found");

		im.close();
		return 1;
	}

	std::string line;
	while (std::getline(im, line))
	{
		std::istringstream iss(line);
		std::string type;

		/* define the type of object */
		iss >> type;

		if (type == "model")
		{
			std::string name, model, tex;
			Model *buff = new Model;
			iss >> name >> model >> tex;

			buff->loadOBJ(model);
			buff->texture.load(tex);
			models[name] = buff;
		}

		else if (type == "object")
		{
			std::string mtype;
			auto buff = std::make_unique<ModelInstance>();

			iss >> mtype;
			auto it = models.find(mtype);

			if (it == models.end())
			{
				warning(fmt::format("Unknown model name {}", mtype));
				continue;
			}

			buff->type = models[mtype];

			glm::vec3 v;
			iss >> v.x >> v.y >> v.z;
			buff->pos = v;

			objects.push_back(std::move(buff));
		}
		else
			warning(fmt::format("Unknown map object type: {}", type));
	}

	im.close();
	return 0;
}
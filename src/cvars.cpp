#include "stdafx.h"
#include "cvars.h"
#include "math.h"
#include "log.h"

CVar *CVars = nullptr;

std::map<std::string, CVar*> CVarsByName;

void initCVars()
{
	for (CVar *cvar = CVars; cvar != nullptr; cvar = cvar->PrevCVar())
		CVarsByName[cvar->Name()] = cvar;
}

void configRead()
{
	log("Loading configuration file: config.cfg...");

	std::ifstream rc;
	rc.open("config.cfg", std::ios::in);

	if (!rc.is_open())
	{
		log("Configuration file was not found, a default one will be made on exit.");
		return;
	}

	while (!rc.eof())
		configReadLine(&rc);

	log("Finished loading configuration file!");
	rc.close();
}

void configReadLine(std::ifstream *c)
{
	std::string cvar, value;
	*c >> cvar;

	if (!cvar.size() || c->eof())
		return;

	*c >> value;

	if (!value.size() || c->eof())
	{
		warning(fmt::format("CVar {}: expected value!", cvar.c_str()));
		return;
	}

	if (!checkCVar(cvar))
		return;

	getCVar(cvar)->SetString(value);
	log(fmt::format("config >> {} = {}", cvar.c_str(), value));
}

void configWrite()
{
	log("<< Writting configuration file: config.cfg...");

	std::ofstream wc;
	wc.open("config.cfg", std::ios::out | std::ios::trunc);

	for (auto &i : CVarsByName)
		wc << i.first << " " << i.second->ToString() << std::endl;

	log("<< Writting configuration file: config.cfg...");
}

bool checkCVar(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (CVarsByName.find(name) == CVarsByName.end())
	{
		warning(fmt::format("Unknown CVar {}!", name.c_str()));
		return false;
	}
	return true;
}

CVar* getCVar(std::string name)
{
	if (!checkCVar(name))
		return nullptr;

	return CVarsByName[name];
}

CVar::CVar(std::string name)
{
	_PrevCVar = CVars;
	CVars = this;

	_Name = name;
	std::transform(_Name.begin(), _Name.end(), _Name.begin(), ::tolower);
}

IntCVar::IntCVar(std::string name, int value, int lrange, int rrange) : CVar(name)
{
	_Value = clip(value, lrange, rrange);
}

FloatCVar::FloatCVar(std::string name, float value, float lrange, float rrange) : CVar(name)
{
	_Value = clip(value, lrange, rrange);
}

void IntCVar::SetString(std::string value)
{
	std::stringstream s(value);

	if (!(s >> _Value))
		_Value = 0;

	clip(_Value, LRange, RRange);
}

std::string IntCVar::ToString()
{
	std::stringstream s;
	s << _Value;

	return s.str();
}

void FloatCVar::SetString(std::string value)
{
	std::stringstream s(value);

	if (!(s >> _Value))
		_Value = 0;

	clip(_Value, LRange, RRange);
}

std::string FloatCVar::ToString()
{
	std::stringstream s;
	s << _Value;

	return s.str();
}

void StringCVar::SetString(std::string value)
{
	_Value = value;
	std::transform(_Value.begin(), _Value.end(), _Value.begin(), ::tolower);
}

std::string StringCVar::ToString()
{
	return _Value;
}

// system

INT_CVAR(l_type, 0, 0, 3)

// window

INT_CVAR(w_width, 800, 320, 8192)
INT_CVAR(w_height, 600, 240, 8192)
BOOL_CVAR(w_fullscreen, 0)

// render

INT_CVAR(r_fpslimit, 60, 1, 300)
INT_CVAR(r_antialiasing, 4, 0, 16)
BOOL_CVAR(r_vsync, 0)
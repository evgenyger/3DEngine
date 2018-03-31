#include "stdafx.h"
#include "cvars.h"
#include "math.h"

CVar *CVars = nullptr;

std::map<std::string, CVar*> CVarsByName;

void initCVars()
{
	for (CVar *cvar = CVars; cvar != nullptr; cvar = cvar->PrevCVar())
		CVarsByName[cvar->Name()] = cvar;
}

void configRead()
{
	std::cout << "<< Loading configuration file: config.cfg..." << std::endl;

	std::ifstream rc;
	rc.open("config.cfg", std::ios::in);

	if (!rc.is_open())
	{
		std::cout << "<< Configuration file wasn't found, a new one will be made on exit..." << std::endl;
		return;
	}

	while (!rc.eof())
		configReadLine(&rc);

	std::cout << "<< Finished loading configuration file!" << std::endl;
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
		std::cout << "<< [Warning] CVar " << cvar.c_str() << ": expected value!" << std::endl;
		return;
	}

	if (!checkCVar(cvar))
		return;

	getCVar(cvar)->SetString(value);
	std::cout << ">> config >> " << cvar.c_str() << " = " << value << std::endl;
}

void configWrite()
{
	std::cout << "<< Writting configuration file: config.cfg..." << std::endl;

	std::ofstream wc;
	wc.open("config.cfg", std::ios::out | std::ios::trunc);

	for (auto &i : CVarsByName)
		wc << i.first << " " << i.second->ToString() << std::endl;

	std::cout << "<< Finished writting configuration file!" << std::endl;
}

bool checkCVar(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (CVarsByName.find(name) == CVarsByName.end())
	{
		std::cout << "<< [Warning] Unknown CVar " << name.c_str() << "!" << std::endl;
		CVarsByName.erase(CVarsByName.end());
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

// window

INT_CVAR(w_width, 800, 320, 8192)
INT_CVAR(w_height, 600, 240, 8192)
BOOL_CVAR(w_fullscreen, 0)

// render

INT_CVAR(r_fpslimit, 60, 1, 300)
INT_CVAR(r_antialiasing, 4, 0, 16)
BOOL_CVAR(r_vsync, 0)
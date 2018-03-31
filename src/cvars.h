#pragma once

#define INT_CVAR(Name, Value, LeftRange, RightRange) IntCVar Name(# Name, Value, LeftRange, RightRange);
#define BOOL_CVAR(Name, Value) IntCVar Name(# Name, Value, 0, 1);
#define FLOAT_CVAR(Name, Value, LeftRange, RightRange) FloatCVar Name(# Name, Value, LeftRange, RightRange);
#define STRING_CVAR(Name, Value, LeftRange, RightRange) IntCVar Name(# Name, Value);

class CVar
{
public:
	CVar(std::string name);

	GETTER(std::string, Name)
		GETTER(CVar*, PrevCVar)

		virtual std::string ToString() = 0;

	virtual void SetString(std::string value) = 0;

private:
	std::string _Name;
	CVar *_PrevCVar = nullptr;
};

class IntCVar : public CVar
{
public:
	IntCVar(std::string name, int value, int lrange, int rrange);

	GETSET(int, Value)

		std::string ToString();
	void SetString(std::string value);

	operator int() const { return _Value; }
	IntCVar& operator=(int value) { _Value = value; return *this; }

private:
	int _Value;
	int LRange = INT_MIN;
	int RRange = INT_MAX;
};

class FloatCVar : public CVar
{
public:
	FloatCVar(std::string name, float value, float lrange, float rrange);

	GETSET(float, Value)

		std::string ToString();
	void SetString(std::string value);

	operator float() const { return _Value; }
	FloatCVar& operator=(int value) { _Value = value; return *this; }

private:
	float _Value;
	float LRange = FLT_MIN;
	float RRange = FLT_MAX;
};

class StringCVar : public CVar
{
public:
	GETSET(std::string, Value)

		std::string ToString();
	void SetString(std::string value);

	operator std::string() const { return _Value; }
	StringCVar& operator=(int value) { _Value = value; return *this; }

private:
	std::string _Value;
};

void initCVars();
void configRead();
void configReadLine(std::ifstream *c);
void configWrite();
bool checkCVar(std::string name);
CVar* getCVar(std::string name);
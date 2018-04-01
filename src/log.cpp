#include "stdafx.h"
#include "log.h"

std::ofstream lout;
bool blog = FALSE;

std::string timestamp()
{
	sf::Time t = dclock.getElapsedTime();
	sf::Int64	seconds = t.asSeconds(),
				minutes = seconds / 60,
				hours = minutes / 60;

	return fmt::format("[{:02}:{:02}:{:02}]", hours, minutes % 60, seconds % 60);
}

const void error(std::string &s)
{
	std::string outp = timestamp() + " [ERROR] " + s;

	std::cout << outp << std::endl;
	if (blog)
		lout << outp << std::endl;
}

const void error(const char* s)
{
	std::string outp = timestamp() + " [ERROR] " + std::string(s);

	std::cout << outp << std::endl;
	if (blog)
		lout << outp << std::endl;
}

const void warning(std::string &s)
{
	std::string outp = timestamp() + " [WARNING] " + s;

	std::cout << outp << std::endl;
	if (blog)
		lout << outp << std::endl;
}

const void warning(const char* s)
{
	std::string outp = timestamp() + " [WARNING] " + std::string(s);

	std::cout << outp << std::endl;
	if (blog)
		lout << outp << std::endl;
}

const void log(std::string &s)
{
	std::string outp = timestamp() + " " + s;

	std::cout << outp << std::endl;
	if (blog)
		lout << outp << std::endl;
}

const void log(const char* s)
{
	std::string outp = timestamp() + " " + std::string(s);

	std::cout << outp << std::endl;
	if (blog)
		lout << outp << std::endl;
}

void logInit()
{
	lout.open("log.txt", std::ofstream::out | std::ofstream::trunc);
	if(!lout.is_open())
		std::cout << " << [ERROR] Failed opening log file!" << std::endl;
}
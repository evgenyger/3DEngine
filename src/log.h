#pragma once

extern sf::Clock dclock;

std::string timestamp();
const void error(std::string &s);
const void error(const char* s);
const void warning(std::string &s);
const void warning(const char* s);
const void log(std::string &s);
const void log(const char* s);
void logInit();
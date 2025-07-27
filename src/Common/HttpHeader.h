#pragma once

#include <string>
#include <vector>

struct StartLine
{
	std::vector<std::string> parts;
};

struct HttpHeader
{
	StartLine m_startLine;
};
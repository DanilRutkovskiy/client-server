#pragma once

#include <string>
#include <vector>

struct StartLine
{
	std::vector<std::string> parts;
};

struct HeaderField
{
	std::string m_name;
	std::string m_value;
};

struct HttpHeader
{
	StartLine m_startLine;
	void Add(HeaderField field)
	{
		m_headers.push_back(std::move(field));
	}
	std::vector<HeaderField> m_headers;
};
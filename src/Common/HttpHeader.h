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

	HeaderField* Get(const std::string& name) const
	{
		const auto loc = std::find_if(std::begin(m_headers), std::end(m_headers), 
			[&name](const HeaderField& field) { return field.m_name == name; });

		return std::end(m_headers) == loc ? nullptr : &(*loc);
	}

	std::vector<HeaderField> m_headers;
};
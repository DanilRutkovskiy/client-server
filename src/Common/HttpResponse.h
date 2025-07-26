#pragma once

class HttpResponse
{
public:
	int m_statusCode = 400;
	std::string m_body;
	std::string m_data;
};
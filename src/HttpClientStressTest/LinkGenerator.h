#pragma once
#include <random>
#include <string>
#include <vector>

struct LinkGenerator
{
	std::vector<std::string> m_links
	{
		"http://127.0.0.1/a.txt",
		"http://127.0.0.1/b.txt",
		"http://127.0.0.1/c.txt",
	};

	std::random_device m_randomDevice;
	std::mt19937 m_generator;
	std::uniform_int_distribution<> m_distribution;

	LinkGenerator()
		:
		m_generator{ m_randomDevice() },
		m_distribution{ 0,  static_cast<int>(m_links.size() - 1)}
	{

	}

	std::string operator()()
	{
		int index = m_distribution(m_generator);
		return m_links[index];
	}
};
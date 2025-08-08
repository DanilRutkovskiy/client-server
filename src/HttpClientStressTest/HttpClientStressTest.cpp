#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "StressTester.h"
#include "../Common/SllUtils.h"

void test()
{
	int concurrency = 500;
	int numberOfThreads = 4;

	boost::asio::io_context ioContext;
	std::cout << "started" << std::endl;
	{
		auto sslContext = PrepareSslContext();
		auto workGuard = boost::asio::make_work_guard(ioContext);

		StressTester tester(ioContext, sslContext, concurrency);
		tester();

		for (int i = 0; i < numberOfThreads; i++)
		{
			std::jthread th([&ioContext]()
				{
					try
					{
						ioContext.run();
					}
					catch (const std::exception& ex)
					{
						std::cout << "exception: " << ex.what() << std::endl;
					}
					catch (...)
					{
						std::cout << "unknown error" << std::endl;
					}
				});

			th.detach();
		}

		std::cout << "Press enter to stop the app" << std::endl;
		std::cin.get();

		tester.PrintResult();

		workGuard.reset();
		ioContext.stop();
	}

	std::cout << "done" << std::endl;
}

int main()
{
	try
	{
		test();
	}
	catch (const std::exception& ex)
	{
		std::cout << "exception: " << ex.what() << std::endl;
	}

	return 0;
}
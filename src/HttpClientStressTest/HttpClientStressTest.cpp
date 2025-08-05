#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "StressTester.h"

int main()
{
	boost::asio::io_context ioContext;
	{
		auto workGuard = boost::asio::make_work_guard(ioContext);

		StressTester tester(ioContext);
		tester();

		std::jthread th([&ioContext]()
			{
				try
				{
					ioContext.run();
					std::cout << "ioContext is out of work" << std::endl;
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
		std::cout << "Press enter to stop the app" << std::endl;
		std::cin.get();

		workGuard.reset();
		ioContext.stop();
	}

	std::cout << "done" << std::endl;
}
#pragma once
#include <memory>
#include <string>
#include "HttpClientParameters.h"
#include "ConnectionParameters.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"
#include "HttpResponsePopulator.h"
#include "HttpResponseStreamParser.h"

class HttpClient : public std::enable_shared_from_this<HttpClient>
{
public:
	static std::shared_ptr<HttpClient> Make(HttpClientParameters parameters)
	{
		return std::shared_ptr<HttpClient>(new HttpClient(std::move(parameters)));
	}

	template<typename Callable>
	void ConnectAsync(ConnectionParameters connectionParams, Callable callable)
	{
		m_resolver.async_resolve	
		(
			connectionParams.m_host, connectionParams.m_port,
			[callable = std::move(callable), sharedThis = this->shared_from_this(), this]
			(boost::system::error_code err, const boost::asio::ip::tcp::resolver::results_type& endpoints) mutable
			{
				if (err)
				{
					std::cout << "resolve error oocured: " << err.message() << std::endl;
					callable(err);
					DeferDeletion();
					return;
				}

				boost::asio::ip::tcp::socket& socket = sharedThis->m_socket;
				boost::asio::async_connect(socket, endpoints,
					[callable = std::move(callable), sharedThis = std::move(sharedThis), this]
					(boost::system::error_code err, boost::asio::ip::tcp::endpoint ep)
					{
						if (err)
						{
							std::cout << "connect error occured: " << err.message() << std::endl;
							callable(err);
							DeferDeletion();

						}

						callable(std::error_code{});
						DeferDeletion();
					});

				//std::cout << "successfully resolved" << std::endl;
				//callable(std::error_code());
			}
		);
	}

	template<typename Callable>
	void SendAsync(const std::string& hhtpMessage, Callable callable)
	{
		ReadResponseAsync(m_socket, std::move(callable));
		SendMessageAsync(m_socket, hhtpMessage);
		/*
		post(m_parameters.m_executor, 
			[callable = std::move(callable), sharedThis = this->shared_from_this(), this]() mutable
			{


				//callable(std::error_code(), std::move(response));
				DeferDeletion();
			});
		*/
	}

protected:
	explicit HttpClient(HttpClientParameters parameters)
		:
		m_parameters(std::move(parameters)),
		m_resolver(m_parameters.m_executor),
		m_socket(m_parameters.m_executor),
		m_parser(&m_populator)
	{

	}

	template<typename Callable>
	void ReadResponseAsync(boost::asio::ip::tcp::socket& socket, Callable callable)
	{
		boost::asio::async_read_until(socket, m_response, m_parser, 
			[sharedThis = this->shared_from_this(), callable = std::move(callable), this]
			(boost::system::error_code err, size_t byteContt) mutable
			{
				if (err)
				{
					callable(err, HttpResponse{});
				}
				std::string data{ std::istreambuf_iterator<char>(&m_response), std::istreambuf_iterator<char>() };

				HttpResponse response;
				response.m_header = m_populator.ReadHeaderData();

				callable(std::error_code{}, std::move(response));
			});
	}

	void SendMessageAsync(boost::asio::ip::tcp::socket& socket, const std::string& content)
	{
		std::ostream os(&m_request);
		os << content;

		boost::asio::async_write(socket, m_request, 
			[sharedThis = this->shared_from_this()]
			(boost::system::error_code err, std::size_t bytes_transfered)
			{
				//TODO handle error here
			});
	}

	void DeferDeletion()
	{
		post(m_parameters.m_executor,
			[sharedThis = this->shared_from_this()]() mutable
			{
				sharedThis.reset();
			});
	}

protected:
	HttpClientParameters m_parameters;
	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::streambuf m_request;
	boost::asio::streambuf m_response;
	HttpResponsePopulator m_populator;
	HttpResponseStreamParser m_parser;
};
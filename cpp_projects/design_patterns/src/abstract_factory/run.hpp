#pragma once

#include "client.hpp"
#include "factory.hpp"

namespace dp
{

void Run()
{
	Client* client = nullptr;

	client = new Client(new Factory1);
	client->Run();
	delete client;

	client = new Client(new Factory2);
	client->Run();
	delete client;
}

} // end namespace dp

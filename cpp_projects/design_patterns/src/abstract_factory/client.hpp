#pragma once

#include <memory>
#include <utility>
#include "factory.hpp"
#include "product.hpp"

namespace dp
{

class Client
{
private:
	std::unique_ptr<AbstractProductA> productA;
	std::unique_ptr<AbstractProductB> productB;
public:
	Client(AbstractFactory* factory)
	{
		productA = std::move(std::unique_ptr<AbstractProductA>(factory->CreateProductA()));
		productB = std::move(std::unique_ptr<AbstractProductB>(factory->CreateProductB()));
	}

	void Run() const	{ productB->Interact(productA.get()); };
};

} // end namespace dp

#pragma once

#include "product.hpp"

namespace dp
{

class AbstractFactory
{
public:
	virtual ~AbstractFactory() {}
	virtual AbstractProductA* CreateProductA() const = 0;
	virtual AbstractProductB* CreateProductB() const = 0;
};

class Factory1 : public AbstractFactory
{
public:
	AbstractProductA* CreateProductA() const	{ return new ProductA1; }
	AbstractProductB* CreateProductB() const	{ return new ProductB1; }
};

class Factory2 : public AbstractFactory
{
public:
	AbstractProductA* CreateProductA() const	{ return new ProductA2; }
	AbstractProductB* CreateProductB() const	{ return new ProductB2; }
};

} // end namespace dp

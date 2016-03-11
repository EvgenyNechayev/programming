#pragma once

#include <iostream>
#include <typeinfo>

namespace dp
{

class AbstractProductA
{
public:
	virtual ~AbstractProductA() {}
	virtual const char* GetName() const = 0;
};

class ProductA1 : public AbstractProductA
{
public:
	const char* GetName() const	{ return typeid(this).name(); }
};

class ProductA2 : public AbstractProductA
{
public:
	const char* GetName() const	{ return typeid(this).name(); }
};

class AbstractProductB
{
public:
	virtual ~AbstractProductB() {}
	virtual const char* GetName() const = 0;
	virtual void Interact(AbstractProductA*) = 0;
};

class ProductB1 : public AbstractProductB
{
public:
	const char* GetName() const	{ return typeid(this).name(); }
	virtual void Interact(AbstractProductA* product)
	{
		std::cout << typeid(this).name() << " interacts with " << product->GetName() << std::endl;
	}
};

class ProductB2 : public AbstractProductB
{
public:
	const char* GetName() const	{ return typeid(this).name(); }
	virtual void Interact(AbstractProductA* product)
	{
		std::cout << typeid(this).name() << " interacts with " << product->GetName() << std::endl;
	}
};

} // end namespace dp

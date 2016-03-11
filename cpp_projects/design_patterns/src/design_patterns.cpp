//============================================================================
// Name        : design_patterns.cpp
// Author      : nechayev
// Version     :
// Copyright   : Your copyright notice
// Description : Design Patterns
//============================================================================

//#include "abstract_factory/run.hpp"
#include "mycollection/mycollection.hpp"

int main()
{
//	dp::Run();	// for abstract factory

	int const NUMTYPES = 3;
	int const RANGE = 100;
	Collection<Prototype> MyCollection(NUMTYPES, RANGE);
	MyCollection.CreateCollection(new A(), new B(), new C());

	for (int i = 0; i < NUMTYPES; ++i)
		cout << "count = " << MyCollection.collection[i].size() << endl;

	Client clt;
	clt.EqualizeCollection(MyCollection);

	for (int i = 0; i < NUMTYPES; ++i)
		cout << "count = " << MyCollection.collection[i].size() << endl;

}

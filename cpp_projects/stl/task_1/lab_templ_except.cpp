//Исключения

//Шаблоны:
	//		шаблоны функций,
	//		шаблоны классов,

#include <iostream>

#include "swap.hpp"
#include "mystring.hpp"
#include "mystack.hpp"
#include "mystack2.hpp"
#include "myqueue.hpp"

int main()
{
////////////////////////////////////////////////////////////////////////////
	//Тема. Шаблоны функций.
	//Создайте шаблон функции перестановки местами двух
	//значений - Swap(). Проверьте работоспособность созданного
	//шаблона с помощью приведенного ниже фрагмента кода.
	//Подсказка 1: объявление шаблона корректнее поместить в .h-файл.

    std::cout << "Introduction:" << std::endl;

    int iX = 1, iY = -1;
    Swap(iX, iY);
    std::cout << "iX=" << iX << " iY=" << iY << std::endl;

    double dX = 0.5, dY = -5.5;
    Swap(dX, dY);
    std::cout << "dX=" << dX << " dY=" << dY << std::endl;

    //Подсказка 2: подумайте, что нужно реализовать, для того,
    //			чтобы следующий вызов работал с объектами MyString
    //			не только корректно, но и эффективно
    MyString str1("One"), str2("Two");
    Swap(str1, str2);
    std::cout << "str1=" << str1.Get() << " str2=" << str2.Get() << std::endl;
/////////////////////////////////////////////////////////////////////
	
	//Тема. Шаблоны классов.
	//Задание 1.
	//Создайте шаблон класса MyStack для хранения элементов любого типа T.
	//Подсказка: 1.элементы нужно где-то хранить - простым и удобным средством
	//			для хранения последовательно расположенных в памяти элементов
	//			является массив,
	//			2.для задания максимального размера стека может быть использован
	//			параметр-константа шаблона
	//			3.обязательными операциями со стеком являются "push" и "pop". Для
	//			того, чтобы гарантировать корректное выполнение этих операций
	//			хорошо было бы генерировать исключение в аварийной ситуации

	
	//С помощью шаблона MyStack создайте стек из "N" элементов int - iStack и
	//стек из "M" элементов MyString - strStack и поэкспериментируйте с функциями
	//push() и pop()

    std::cout << std::endl << "Task 1:" << std::endl;

//-- iStack
    int const N = 10;
    MyStack<int, N> iStack;

    for (int i = 0; !iStack.isFull(); ++i)
        try { iStack.push(i); }
        catch (MyStack<int>::MyStackErr& err) { std::cerr << err.what() << std::endl; }

    std::cout << "iStack: ";
    while (!iStack.isEmpty())
    {
        std::cout << iStack.top() << ' ';
        try { iStack.pop(); }
        catch (MyStack<MyString>::MyStackErr& err) { std::cerr << err.what() << std::endl; }
    }
    std::cout << std::endl;

//-- strStack
    int const M = 5;
    MyStack<MyString, M> strStack;

    char const* str[] =
    {
        "One",
        "Two",
        "Three",
        "Four",
        "Five"
    };

    for (int i = 0; !strStack.isFull(); ++i)
        try { strStack.push(str[i]); }
        catch (MyStack<int>::MyStackErr& err) { std::cerr << err.what() << std::endl; }

    std::cout << "strStack: ";
    while (!strStack.isEmpty())
    {
        std::cout << strStack.top().Get() << ' ';
        try { strStack.pop(); }
        catch (MyStack<MyString>::MyStackErr& err) { std::cerr << err.what() << std::endl; }
    }
    std::cout << std::endl;

	//Добавьте в шаблон стека возможность обращения к элементам
	//по индексу. Для этого переопределите оператор [] таким
    //образом, чтобы он возвращал значение n-ого элемента или генерировал
	//исключение, если требуемый элемент находится вне "корректного"
	//диапазона.

    for (int i = 0; !iStack.isFull(); ++i)
        try { iStack.push(i); }
        catch (MyStack<int>::MyStackErr& err) { std::cerr << err.what() << std::endl; }

    //распечатайте стек в порядке ЗАПОЛНЕНИЯ

    for (int i = 0; i < iStack.capacity(); ++i)
        try { std::cout << iStack[i] << ' '; }
        catch (MyStack<int>::MyStackErr& err) { std::cerr << err.what() << std::endl; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Задание 2. Реализуйте шаблон стека - MyStack2 таким образом, чтобы
    //для хранения элементов использовался ОДНОСВЯЗНЫЙ список.

    std::cout << std::endl << "Task 2:" << std::endl;

    MyStack2<int> iStack2;
    for (int i = 0; !iStack2.isFull(); ++i)
        try { iStack2.push(i); }
        catch (MyStack2<int>::MyStack2Err& err) { std::cerr << err.what() << std::endl; }

    //распечатайте стек в порядке ЗАПОЛНЕНИЯ

    MyStack2<int> iStackTemp(iStack2);
    for (int i = 0; i < iStackTemp.capacity(); ++i)
        try { std::cout << iStackTemp.top() << ' '; iStackTemp.pop(); }
        catch (MyStack2<int>::MyStack2Err& err) { std::cerr << err.what() << std::endl; }
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Задание 3. Реализуйте шаблон очереди - MyQueue таким образом, чтобы
    //для хранения элементов использовался динамический массив.
    //Очередь должна быть кольцевой => если достигнут конец массива, то скорее
    //всего в начале уже есть свободные элементы

    std::cout << std::endl << "Task 3:" << std::endl;

    MyQueue<int, 5> iMyQueue;
    for (int i = 0; !iMyQueue.isFull(); ++i)
        try { iMyQueue.push(i); }
        catch (MyQueue<int>::MyQueueErr& err) { std::cerr << err.what() << std::endl; }

    //Распечатайте очередь

    while (!iMyQueue.isEmpty())
        try { std::cout << iMyQueue.front() << ' '; iMyQueue.pop(); }
        catch (MyQueue<int>::MyQueueErr& err) { std::cerr << err.what() << std::endl; }
    std::cout << std::endl;

    MyQueue<int, 5> iMyQueue_1;
    iMyQueue_1.push(1);
    iMyQueue_1.push(2);
    iMyQueue_1.push(3);
    iMyQueue_1.push(4);
    iMyQueue_1.push(5);
    iMyQueue_1.pop();
    iMyQueue_1.pop();
    iMyQueue_1.push(6);
    iMyQueue_1.push(7);

    while (!iMyQueue_1.isEmpty())
        try { std::cout << iMyQueue_1.front() << ' '; iMyQueue_1.pop(); }
        catch (MyQueue<int>::MyQueueErr& err) { std::cerr << err.what() << std::endl; }
    std::cout << std::endl;

    return 0;
}


//Стандартная библиотека шаблонов - STL
	//Контейнеры стандартной библиотеки - vector
	//Итераторы

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <deque>
#include <cctype>

#include "mylist.hpp"
#include "myalgorithms.hpp"

using namespace MySpace;

int main()
{
	// Контейнер стандартной библиотеки vector

//////////////////////////////////////////////////////////////////////
	//Создание векторов.

	//Создайте следующие вектора:
	//---------------------------
	//пустой вектор целых чисел - vInt и проверьте с помощью функции size(),
	//что его размер нулевой.

    MyVector<int> vInt;

    std::cout << "vInt size=" << vInt.size() << std::endl;
	
	//Раскомментируйте следующий фрагмент, подумайте - все ли корректно
	//Если есть некорректности, - исправьте

    vInt.resize(1);
    vInt.front() = 1;

    std::cout << "vInt front=" << vInt.front() << std::endl;

	//С помощью функции push_back() в цикле заполните вектор какими-либо значениями.
	//На каждой итерации цикла следите за размером вектора с помощью методов
	//size(), capacity(), max_size() и выводите значения элементов.
	//
	//Подсказка: так как такая проверка понадобится Вам и в следующих заданиях,
	//напишите шаблон функции, которая для вектора, содержащего элементы любого типа
	//выводит его "реквизиты" и значения элементов на консоль.
	
    std::cout << "vInt" << std::endl;
    for (int i = 10; i < 20; ++i)
    {
        vInt.push_back(i);
        DisplayParam(vInt);
    }

	//вектор вещественных - vDouble1 с заданным Вами начальным размером  и
	//проверьте результат с помощью созданного Вами шаблона. Как будут
	//проинициализированы элементы вектора?
	
    MyVector<double> vDouble1(5);
    std::cout << "vDouble1" << std::endl;
    DisplayParam(vDouble1);
    std::cout << vDouble1;

	//вектор объектов типа MyString с начальным размером - 5 элементов
	//и инициализацией каждого элемента строкой "A"
	//C помощью функции at() а также с помощью оператора
	//индексирования []  измените значения каких-либо элементов.
	//Попробуйте "выйти" за границы вектора с помощью at() и
    //с помощью [].

    MyVector<MyString> myString1(5, "A");

    try { myString1[0] = "B"; }
    catch (std::range_error& err) { std::cerr << err.what() << std::endl; }
    try { myString1.at(1) = "C"; }
    catch (std::range_error& err) { std::cerr << err.what() << std::endl; }

//    try { myString1[6] = "D"; }
//    catch (range_error& err) { std::cerr << err.what() << std::endl; }
//    try { myString1.at(7) = "E"; }
//    catch (range_error& err) { std::cerr << err.what() << std::endl; }

    std::cout << "myString1" << std::endl;
    DisplayParam(myString1);
    std::cout << myString1;

	//вектор вещественных - vDouble3, который является копией элементов
	// [0,5) массива вещественных чисел dMas. Предворительно массив dMas
	//нужно создать и проинициализировать!

    double dMas[] { 23, 10, 62, 3, 45, 35, 56, 77, 88, 99 };

    MyVector<double> vDouble3(&dMas[0], &dMas[4]);
    std::cout << "vDouble3" << std::endl;
    DisplayParam(vDouble3);
    std::cout << vDouble3;

	//вектор вещественных - vDouble4, который является копией элементов
	// [2,5) вектора vDouble3.

    MyVector<double> vDouble4(&vDouble3[2], &vDouble3[4]);
    std::cout << "vDouble4" << std::endl;
    DisplayParam(vDouble4);
    std::cout << vDouble4;

    //вектор элементов типа MyPoint - vMyPoint1 а) с начальным размером 3. Какой конструктор
	//будет вызван для каждого элемента?

    MyVector<MyPoint> vMyPoint1(3);
    std::cout << "vMyPoint1" << std::endl;
    DisplayParam(vMyPoint1);
    std::cout << vMyPoint1;

    //b) vMyPoint2 с начальным размером 5 и проинициализируйте каждый элемент координатами (1,1).

    MyVector<MyPoint> vMyPoint2(5, MyPoint(1, 1));
    std::cout << "vMyPoint2" << std::endl;
    DisplayParam(vMyPoint2);
    std::cout << vMyPoint2;

    //вектор указателей на MyPoint - vpMyPoint с начальным размером 5
    //Подумайте: как корректно заставить эти указатели "указывать" на объекты MyPoint
//	{
	//Подсказка: для вывода на печать значений скорее всего Вам понадобится
		//а) специализация Вашей шаблонной функции
        //б) или перегрузка operator<< для MyPoint*

        MyPoint MyPoints[] { MyPoint(0, 0), MyPoint(1, 1), MyPoint(2, 2), MyPoint(3, 3), MyPoint(4, 4), MyPoint(5, 5) };
        MyVector<MyPoint*> vpMyPoint(5);

        MyPoint* p = MyPoints;
        for (auto& vr : vpMyPoint)
        {
            vr = p++;
        }

        std::cout << "vpMyPoint" << std::endl;
        std::cout << vpMyPoint;

//	}//Какие дополнительные действия нужно предпринять для такого вектора?


	///////////////////////////////////////////////////////////////////////
    //Резервирование памяти.
    //Подумайте, всегда ли верны приведенные ниже проверки?
//        {
//        size_t n = 10;
//        vector<int> v(n);
//        v.resize(n/2);
//        if(v.capacity() == n) //true?
//          // Всегда верно
//        }



//        {
//        int n=...
//        size_t m=...
//        vector<int> v(n);
//        v.reserve(m);
//        if(v.capacity() == m) //true?
//          // Верно при условии: m >= n
//        }



//        {
//        vector<int> v(3,5);
//        // v: 5 5 5
//        v.resize(4,10); //значения?
//        // v: 5 5 5 10
//                     |_> инициализируется вторым параметром, передаваемым в resize
//        v.resize(5); //значения?
//        // v: 5 5 5 10 0
//                       |_> инициализируется конструктором по умолчанию
//        }


	//Создайте два "пустых" вектора с элементами
	//любого (но одного и того же типа) типа.
	//В первом векторе зарезервируйте память под 5 элементов, а потом заполните
	//его значениями с помощью push_back.
	//Второй вектор просто заполните значениями посредством push_back.
	//
	//Сравните размер, емкость векторов и значения элементов

    MyVector<int> vInt1, vInt2;
    vInt1.resize(5);

    for (int i = 10; i < 20; ++i)
    {
        vInt1.push_back(i);
    }

    std::cout << "vInt1" << std::endl;
    DisplayParam(vInt1);
    std::cout << vInt1;

    for (int i = 10; i < 20; ++i)
    {
        vInt2.push_back(i);
    }

    std::cout << "vInt2" << std::endl;
    DisplayParam(vInt2);
    std::cout << vInt2;

    // Уменьшение емкости вектора.
	//Для любого вектора из предыдущего задания требуется уменьшить емкость
	//до size.

    vInt1.shrink_to_fit();
	
    std::cout << "vInt1" << std::endl;
    DisplayParam(vInt1);
    std::cout << vInt1;

	//Создание "двухмерного вектора" - вектора векторов
	//Задан одномерный массив int ar[] = {11,2,4,3,5};
	//Создайте вектор векторов следующим образом:
	//вектор vv[0] - содержит 11 элементов со значением 11
	//vv[1] - содержит 2,2
	//vv[2] - содержит 4,4,4,4
	//...
	//Распечатайте содержимое такого двухмерного вектора по строкам
	
    int ar[] = { 11, 2, 4, 3, 5 };
    MyVector<MyVector<int>> vv;

    const int NUM_ROWS = sizeof(ar)/sizeof(ar[0]);

    vv.resize(NUM_ROWS);
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        vv[i].resize(ar[i]);
        MySpace::MyFill(vv[i].begin(), vv[i].end(), ar[i]);
    }

    std::cout << "2D vector" << std::endl;
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        std::cout << "vv[" << i << "] - ";

        int const NUM_COLS = vv[i].size();
        for (int j = 0; j < NUM_COLS-1; ++j)
        {
            std::cout << vv[i][j] << ", ";

        }
        std::cout << vv[i][NUM_COLS-1] << std::endl;
    }

    std::cout << "==================================" << std::endl;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Вставка элемента последовательности insert().
	//В вектор vChar2 вставьте в начало вектора символ только при
	//условии, что в векторе такого еще нет.

    char chMas[] { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };
    MyVector<char> vChar2(chMas, chMas + sizeof(chMas)/sizeof(chMas[0]));

    std::cout << "vChar2" << std::endl;
    DisplayParam(vChar2);
    std::cout << vChar2;

    if (!std::count(vChar2.begin(), vChar2.end(), 'q'))
        vChar2.insert(vChar2.begin(), 'q');

    std::cout << "vChar2:insert" << std::endl;
    DisplayParam(vChar2);
    std::cout << vChar2;
	
    //Вставьте перед каждым элементом вектора vChar2 букву 'W'
	
    for (int i = vChar2.size(); i >= 0; --i)
    {
        vChar2.insert(i, 'W');
    }

    std::cout << "vChar2:insert W" << std::endl;
    DisplayParam(vChar2);
    std::cout << vChar2;

///////////////////////////////////////////////////////////////////
	//Напишите функцию, которая должна удалять только повторяющиеся последовательности.
	//Например: было - "qwerrrrty122223", стало - "qwety13"

    char const* str = "qwerrrrty122223";
    MyVector<char> vSeq1(str, str + strlen(str));

    std::cout << "vSeq1" << std::endl;
    DisplayParam(vSeq1);
    std::cout << vSeq1;

    vSeq1.erase(MyUnique(vSeq1.begin(), vSeq1.end()), vSeq1.end());

    std::cout << "vSeq1: after unique" << std::endl;
    DisplayParam(vSeq1);
    std::cout << vSeq1;

///////////////////////////////////////////////////////////////////

	//Удаление элемента последовательности erase()
	//Напишите функцию удаления из любого вектора всех дублей

    char const* str_rand = "qwrrerrqdty1223323";
    MyVector<char> vSeq2(str_rand, str_rand + strlen(str_rand));

    std::cout << "vSeq2" << std::endl;
    DisplayParam(vSeq2);
    std::cout << vSeq2;

    MyQuickSort(vSeq2.begin(), vSeq2.end(),
                [](MyVector<char>::iterator a, MyVector<char>::iterator b)
                {
                    return *a <= *b;
                }
    );

    std::cout << "vSeq2 after sort" << std::endl;
    DisplayParam(vSeq2);
    std::cout << vSeq2;

    vSeq2.erase(MyUnique(vSeq2.begin(), vSeq2.end()), vSeq2.end());

    std::cout << "vSeq2: after unique" << std::endl;
    DisplayParam(vSeq2);
    std::cout << vSeq2;

///////////////////////////////////////////////////////////////////
	//Создайте новый вектор таким образом, чтобы его элементы стали
	//копиями элементов любого из созданных ранее векторов, но расположены
	//были бы в обратном порядке

    MyVector<char> vSeq3(vSeq1.crbegin(), vSeq1.crend());

    std::cout << "vSeq3 is reverse vSeq1" << std::endl;
    DisplayParam(vSeq3);
    std::cout << vSeq3;

///////////////////////////////////////////////////////////////////

	//Задание 1. Списки. Операции, характерные для списков.
    //Создайте пустой список из элементов MyPoint - ptList1 и наполните
	//его значениями с помощью методов push_back(),
	//push_front, insert()
	
    MyList<MyPoint> ptList1;

    ptList1.push_back(&MyPoints[0]);
    ptList1.push_back(&MyPoints[1]);

    ptList1.push_front(&MyPoints[2]);
    ptList1.push_front(&MyPoints[3]);

    ptList1.insert(ptList1.begin() + 1, &MyPoints[4]);
    ptList1.insert(ptList1.begin() + 2, &MyPoints[5]);

	//Напишите шаблон функции, которая будет выводить элементы
	//ЛЮБОГО КОНТЕЙНЕРА на печать. Проверьте работу шаблона на контейнерах
	//vector и list. Подсказка - хотелось бы увидеть тип контейнера.

    std::cout << "ptList1" << std::endl;
    DisplayParam(ptList1);
    std::cout << ptList1;

    //Сделайте любой из списков "реверсивным" - reverse()

    ptList1.reverse();

    std::cout << "ptList1 after reverse" << std::endl;
    DisplayParam(ptList1);
    std::cout << ptList1;

    //Создайте список ptList2 из элементов MyPoint таким образом, чтобы он стал
    //копией вектора элементов типа MyPoint, но значения элементов списка располагались
	//бы в обратном порядке

    MyList<MyPoint> ptList2(ptList1.crbegin(), ptList1.crend());

    ptList2.push_back(MyPoint(10, -10));
    ptList2.push_back(MyPoint(7, 7));

    std::cout << "ptList2" << std::endl;
    DisplayParam(ptList2);
    std::cout << ptList2;

	//Отсортируйте списки  ptList1 и ptList2 - методом класса list - sort()
	//по возрастанию.
    //Подумайте: что должно быть перегружено в классе MyPoint для того, чтобы
	//работала сортировка

    auto less = [](MyList<MyPoint>::iterator a, MyList<MyPoint>::iterator b) { return *a <= *b; };

    ptList1.sort(less);

    std::cout << "ptList1 after sort" << std::endl;
    DisplayParam(ptList1);
    std::cout << ptList1;

    ptList2.sort(less);

    std::cout << "ptList2 after sort" << std::endl;
    DisplayParam(ptList2);
    std::cout << ptList2;

	//Объедините отсортированные списки - merge(). Посмотрите: что
	//при этом происходит с каждым списком.

    ptList1.merge(ptList2);

    std::cout << "ptList1 after merge" << std::endl;
    DisplayParam(ptList1);
    std::cout << ptList1;

	//Исключение элемента из списка - remove()
	//Исключите из списка элемент с определенным значением.
    //Подумайте: что должно быть перегружено в классе MyPoint?
	
    ptList1.remove(MyPoint(7, 7));

    std::cout << "ptList1 after remove" << std::endl;
    DisplayParam(ptList1);
    std::cout << ptList1;

	//Исключение элемента из списка, удовлетворяющего заданному условию:
	//любая из координат отрицательна - remove_if().

    ptList1.remove_if([](MyPoint point) { return point.GetX() < 0 || point.GetY() < 0; });

    std::cout << "ptList1 after remove_if" << std::endl;
    DisplayParam(ptList1);
    std::cout << ptList1;

	//Исключение из списка подряд расположенных дублей - unique().

    ptList1.unique();

    std::cout << "ptList1 after unique" << std::endl;
    DisplayParam(ptList1);
    std::cout << ptList1;

///////////////////////////////////////////////////////////////////
	//Задание 2.Очередь с двумя концами - контейнер deque

    //Создайте пустой deque с элементами типа MyPoint. С помощью
	//assign заполните deque копиями элементов вектора. С помощью
	//разработанного Вами в предыдущем задании универсального шаблона
	//выведите значения элементов на печать

    std::deque<MyPoint> ptDeque;

    ptDeque.assign(vMyPoint2.begin(), vMyPoint2.end());

    std::cout << "ptDeque after assign" << std::endl;
    std::cout << ptDeque;

	//Создайте deque с элементами типа MyString. Заполните его значениями
	//с помощью push_back(), push_front(), insert()
	//С помощью erase удалите из deque все элементы, в которых строчки
	//начинаются с 'A' или 'a'

    std::deque<MyString> myStrDeque;

    myStrDeque.push_back(MyString("Ant"));
    myStrDeque.push_back(MyString("Bomb"));

    myStrDeque.push_back(MyString("alf"));
    myStrDeque.push_back(MyString("dog"));

    myStrDeque.push_back(MyString("Ant"));
    myStrDeque.push_back(MyString("God"));

    std::cout << "myStrDeque" << std::endl;
    std::cout << myStrDeque;

    for (std::deque<MyString>::iterator it = myStrDeque.begin(); it != myStrDeque.end(); ++it)
    {
        if ('a' == std::tolower((*it)[0]))
        {
            myStrDeque.erase(it);
        }
    }

    std::cout << "myStrDeque after erase" << std::endl;
    std::cout << myStrDeque;

    std::cout << std::endl << "DONE";

	return 0;
}

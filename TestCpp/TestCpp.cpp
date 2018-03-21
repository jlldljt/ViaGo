#include <typeinfo>
#include <iostream>
#include <string>
#include "TestTemplate.h"

class BaseClass
{
protected:
	char name[80];
public:
	BaseClass() {
		strcpy_s(name, "BaseClass");
	};
	~BaseClass() {
		;
	};

	virtual void Display() {
		std::cout << "display base" << std::endl;
	}
	
	char* GetName() {
		return name;
	}

};

class DeriveOneClass:public BaseClass
{
public:
	DeriveOneClass() {
		strcpy_s(name , "DeriveOneClass");
	}

	~DeriveOneClass();

	void Display() {
		std::cout << "display derive one" << std::endl;
	}
private:

};

class DeriveTwoClass:public BaseClass
{
public:
	DeriveTwoClass() {
		strcpy_s(name, "DeriveTwoClass");
	}
	~DeriveTwoClass();

	void Display() {
		std::cout << "display derive two" << std::endl;
	}
private:

};

void main() {
	//test rtti
	BaseClass *a = new DeriveOneClass();
	//BaseClass *a = new DeriveTwoClass();
	//test RTTI 's typeid
	if (typeid(DeriveOneClass) == typeid(*a)) {
		((DeriveOneClass*)a)->Display();
	}
	else if (typeid(DeriveTwoClass) == typeid(*a)) {
		((DeriveTwoClass*)a)->Display();
	}
	else {
		std::cout << "unknown type" << (typeid(*a)).name() << std::endl;
	}

	getchar();

	//test T function
	int i = power(5, 4);
	long l = power(10L, 3);
	short iil = power(5L, 4);
	CTestTemplate ta(5), tb(0);
	tb = power(ta, 4);
	std::cout << "i=" << i << std::endl;
	std::cout << "l=" << l << std::endl;
	std::cout << "iil=" << iil << std::endl;
	std::cout << "tb=" << tb.i << std::endl;
	getchar();

	//test
}





#include <iostream>

int main() {
  
}



#if 0
#include <iostream>
using namespace std;
class CBase
{
public:
  virtual void display1() { cout << "base \n"; }
};

class CDerive : public CBase
{
public:
  virtual void display2() { cout << "derive2 \n"; }
  virtual void display1() { cout << "derive1 \n"; }
};

void main()
{
	CBase oBase , *pBase;
	CDerive oDerive , *pDerive;

	oBase.display1();//1 base
	oDerive.display2();//2 derive2
	oBase = oDerive;
	oBase.display1();//3 base
	pBase = &oDerive;
	pBase->display1();//4 base
	pDerive = (CDerive*)&oBase;//从基类转换到派生类的唯一选择，用指针显式转换，一般不允许这样做
	pDerive->display1();//5 derive1
	//pDerive->display2();//6 derive2
	((CBase)oDerive).display1();//7 base //object slice 
	//((CDerive)oBase)
	getchar();
}

#endif
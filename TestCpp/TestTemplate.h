#pragma once
class CTestTemplate
{
protected:
	
public:
	CTestTemplate(int num) { i = num; };
	~CTestTemplate() {};
	int i;
	bool operator == (CTestTemplate &a) {
		if (this->i == a.i) {
			return true;
		}
		else {
			return false;
		}
	}

	CTestTemplate& operator = (const int a) {
		this->i = a;
		return *this;
	}

	bool operator < (CTestTemplate &a) {
		if (this->i < a.i) {
			return true;
		}
		else {
			return false;
		}
	}

	CTestTemplate& operator *= (CTestTemplate &a) {
		this->i *= a.i;
		return *this;
	}

	CTestTemplate operator * (CTestTemplate &a) {
		CTestTemplate b = *this;
		b.i *= a.i;
		return b;
	}
};

//T function
template <class T>
T power(T base, int exponent) {
	T result = base;
	if (exponent == 0) result = 1;
	else if (exponent < 0) result = 0;
	else
	while (--exponent) result *= base;
	return result;
}

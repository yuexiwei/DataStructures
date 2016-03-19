#ifndef __LARGEINT_H__
#define __LARGEINT_H__

#include "LinkList.h"

/// 非负大整数类
class BigNum
{
protected:
///  非负大整数类的数据成员:
	DblLinkList<unsigned int> num;					// 存储非负整数的值

/// 辅助函数
	BigNum Multi10Power(unsigned int exponent) const;// 乘10的阶幂10^exponent
	BigNum operator *(unsigned int digit) const;	// 乘法运算符重载(乘以1位数)
	void TrimLeftZero();							// 去掉非负整数最左侧的0

public:
///  方法声明及重载编译系统默认方法声明:
	BigNum(unsigned int n);						// 构造函数
	BigNum &operator =(const BigNum &copy);	// 赋值运算符重载
	friend BigNum operator +(const BigNum &a, const BigNum &b);	// 加法运算符+重载
	friend BigNum operator *(const BigNum &a, const BigNum &b);	// 乘法运算符*重载
	friend bool operator <(const BigNum &a, const BigNum &b);			// 关系运算符<重载
	friend ostream &operator <<(ostream &outStream, const BigNum &outLargeInt); // 重载运算符<<
	friend BigNum Power(const BigNum &base,	const BigNum &exp);

};

/// 非负大整数类的实现部分
// 操作结果: 构造值为num的非负大整数——构造函数
BigNum::BigNum(unsigned int num)
{
	int r, q;					// r余数，q商

	q = num;
	while (q != 0)
	{	//求lNum的各位
		r = q % 10;				// r为q的当前个位数
		this->num.Insert(1, r);	// 插入到双向链表中
		q = q / 10;				// 进一步求高位
	}
}

void BigNum::TrimLeftZero()
// 操作结果：去掉最左侧的0
{
	unsigned int digit;							// 当前非负大整数的一位
	for (num.GetElem(1, digit); digit == 0; num.GetElem(1, digit));	// 去掉最左侧的0
}

BigNum &BigNum::operator =(const BigNum &copy)
// 操作结果：将非负大整数copy赋值给当前非负大整数——赋值语句重载
{
	if (&copy != this)
	{
		this->num = copy.num;			// num用于存储非负大整数
	}

	return *this;
}

// 操作结果：加法运算符+重载
BigNum operator +(const BigNum &a, const BigNum &b)
{
	BigNum bigNum(0);
	unsigned int carry = 0;				// 进位
	unsigned int digit1, digit2;		// 表示非负大整数的各位
	unsigned int pos1, pos2;			// 表示非负大整数的各位的位置

	pos1 = a.num.Length();				// 被加数a的个位位置
	pos2 = b.num.Length();				// 加数b的个位位置
	while (pos1 > 0 && pos2 > 0)
	{	// 从个位开始求和
		a.num.GetElem(pos1--, digit1);	// 被加数的一位
		b.num.GetElem(pos2--, digit2);	// 加数的一位
		bigNum.num.Insert(1, (digit1 + digit2 + carry) % 10);	// 插入和的新的一位
		carry = (digit1 + digit2 + carry) / 10;	// 新的进位
	}

	while (pos1 > 0)
	{	// 被加数还有位没有求和
		a.num.GetElem(pos1--, digit1);	// a的一位
		bigNum.num.Insert(1, (digit1 + carry) % 10);		// 插入和的新的一位
		carry = (digit1 + carry) / 10;	// 新的进位
	}

	while (pos2 > 0)
	{	// 加数还有位没有求和
		b.num.GetElem(pos2--, digit2);	// 加数的一位
		bigNum.num.Insert(1, (digit2 + carry) % 10);		// 插入和的新的一位
		carry = (digit2 + carry) / 10;			// 新的进位
	}

	if (carry > 0)
	{	// 存在进位
		bigNum.num.Insert(1, carry);		// 向高位进位
	}

	return bigNum;
}

// 操作结果：乘法运算符*重载
BigNum operator *(const BigNum &a, const BigNum &b)
{
	BigNum bigNum(0);
	unsigned int digit;							// 表示一位数字
	unsigned int len = b.num.Length();			// 乘数位数

	for (int pos = len; pos > 0; pos--)
	{	// 用乘数b的每一位与被乘数a相乘
		b.num.GetElem(pos, digit);				// 取出乘数的一位
		bigNum = bigNum + a.Multi10Power(len - pos) * digit;
			// 累加乘数b的每一位与被乘数a的乘积
	}

	return bigNum;
}

// 操作结果：乘10的阶幂
BigNum BigNum::Multi10Power(unsigned int exponent) const
{
	BigNum bigNum = *this;;

	for (unsigned int i = 0; i < exponent; i++)
	{	// 低exponent位为0
		bigNum.num.Insert(bigNum.num.Length() + 1, 0);
	}
	return bigNum;
}

// 操作结果：乘法运算符重载(乘以一位数)
BigNum BigNum::operator *(unsigned int digit) const
{
	BigNum bigNum(0);
	unsigned int carry = 0;					// 进位
	unsigned int tmpDigit;					// 当前非负大整数的某位
	for (int pos = this->num.Length(); pos > 0; pos--)
	{	// 用digit依次乘当前非负大整数的各位
		this->num.GetElem(pos, tmpDigit);	// 取出一位
		bigNum.num.Insert(1, (tmpDigit * digit + carry) % 10);	// tmpDigit * digit的一位
		carry = (tmpDigit * digit + carry) / 10;	// tmpDigit * digit的进位
	}

	if (carry > 0)
	{	// 存在进位
		bigNum.num.Insert(1, carry);
	}

	return bigNum;
}


// 操作结果：关系运算符<重载
bool operator <(const BigNum &a, const BigNum &b)
{
	BigNum first = a, second = b;		// 暂存a与b, 以便进行处理
	first.TrimLeftZero();	second.TrimLeftZero();		// 去掉最左侧的0
	int len1 = first.num.Length(), len2 = second.num.Length();	// first(a)与second(b)的位数
	if (len1 < len2) return true;			// first(a)位数更小
	else if (len1 > len2) return false;		// first(a)位数更大
	else
	{	// first(a)与second(b)的位数相等
		for (int pos = 1; pos <= len1; pos++)
		{	// 从高位到低位依次比较first(a)与second(b)的各位
			unsigned int digit1, digit2;			// 表示非负大整数的各位
			first.num.GetElem(pos, digit1);			// first(a)的1位
			second.num.GetElem(pos, digit2);		// second(b)的1位
			if (digit1 < digit2) return true;		// first(a)更小
			else if  (digit1 > digit2) return false;// first(a)更大
		}
	}
	return false;									// 此时first(a)与second(b)的各位相等
}

// 重载运算符<<
ostream &operator <<(ostream &outStream, const BigNum &outLargeInt)
{
	int len = outLargeInt.num.Length();				// outLargeInt位数
	if (len == 0)
	{	// outLargeInt为0
		cout << 0;									// 输出0
	}
	else
	{	// outLargeInt非0
		for (int pos = 1; pos <= outLargeInt.num.Length(); pos++)
		{	// 依次输出各位
			unsigned int digit;						// 非负大整数的某位
			outLargeInt.num.GetElem(pos, digit);	// 取出一位
			outStream << digit;						// 显示一位
		}
	}

	return outStream;
}


// 文件路径名:power\power.h
BigNum Power(const BigNum &base,	const BigNum &exp)
// 操作结果: 计算大非负整数数的大非负整数数次方
{
	BigNum tmpLargInt(1);
	for (int i = 0; i < exp; i = i + 1)
	{	// 连乘求大非负整数数的大非负整数数次方
		tmpLargInt = tmpLargInt * base;
	}

	return tmpLargInt;
}


#endif

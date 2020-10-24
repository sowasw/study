#include "mathClass.h"
#include<iostream>
#include<string>
#include <cstdlib>

int gcd(int x, int y);
void errorInMathClass(std::string msg);


Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

Point::~Point()
{
	//std::cout <<"xxxx"<< std::endl;
}

void Point::set(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Point::setX(int x)
{
	this->x = x;
}

void Point::setY(int y)
{
	this->y = y;
}


Point Point::operator()(Point p1)
{
	return p1;
}

Point Point::operator()(int x, int y)
{
	return Point(x,y);
}

int Point::getX() const
{
	return x;
}

int Point::getY() const
{
	return y;
}

std::string Point::toString() const
{
	return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

std::ostream & operator<<(std::ostream & os, Point pt)
{
	// TODO: 在此处插入 return 语句
	return os << pt.toString();
}

Point operator-(Point p1, Point p2)
{
	return Point(p1.x-p2.x,p1.y-p2.y);
}

Point operator+(Point p1, Point p2)
{
	return Point(p1.x + p2.x, p1.y + p2.y);
}

bool operator==(Point p1, Point p2)
{
	return p1.x == p2.x&&p1.y == p2.y;
}

bool operator!=(Point p1, Point p2)
{
	return !(p1 == p2);
}

Rational::Rational()
{
	num = 0;
	den = 1;
}

Rational::Rational(int n)
{
	num = n;
	den = 1;
}

Rational::Rational(int x, int y)
{
	if(y==0){
		errorInMathClass("分母不能为0，退出程序。");
	}
	if (x == 0) {
		num = 0;
		den = 1;
	}
	else {
		int g = gcd(abs(x), abs(y));
		num = x / g;
		den = abs(y) / g;
		if (y < 0)num = -num;
	}
}

Rational::~Rational()
{
}

std::string Rational::toString()
{
	if (den == 1) {
		return std::to_string(num);
	}
	else {
		return std::to_string(num) + "/" + std::to_string(den);
	}
}

std::ostream & operator<<(std::ostream &os, Rational rat)
{
	// TODO: 在此处插入 return 语句
	return os << rat.toString();
}

Rational operator+(Rational r1, Rational r2)
{
	return Rational(r1.num*r2.den + r2.num*r1.den, r1.den*r2.den);
}

Rational operator-(Rational r1, Rational r2)
{
	return Rational(r1.num*r2.den - r2.num*r1.den, r1.den*r2.den);
}

Rational operator*(Rational r1, Rational r2)
{
	return Rational(r1.num*r2.num, r1.den*r2.den);
}

Rational operator/(Rational r1, Rational r2)
{
	return Rational(r1.num*r2.den, r1.den*r2.num);
}


int gcd(int x, int y)
{
	int r = x % y;
	while (r != 0) {
		x = y;
		y = r;
		r = x % y;
	}

	return y;
}

void errorInMathClass(std::string msg)                 //#include<string>
{
	std::cerr << msg << std::endl;
	exit(EXIT_FAILURE);                      //#include <cstdlib>
}

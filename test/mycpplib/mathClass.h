//数学类

#ifndef mathClass_h
#define mathClass_h

#include<iostream>
#include<string>

//二维点
class Point 
{
public:
	Point();
	Point(int x, int y);

	~Point();

	void set(int x, int y);
	void setX(int x);
	void setY(int y);

	friend Point operator-(Point p1, Point p2);
	friend Point operator+(Point p1, Point p2);
	Point operator()(Point p1);
	Point operator()(int x,int y);

	int getX()const;
	int getY()const;

	std::string toString()const;

	friend bool operator==(Point p1, Point p2);

private:
	int x;
	int y;
};

std::ostream &operator<<(std::ostream &os,Point pt);//<<操作符返回输出流

Point operator-(Point p1, Point p2);
Point operator+(Point p1, Point p2);

bool operator==(Point p1, Point p2);
bool operator!=(Point p1, Point p2);

//有理数
class Rational 
{
public:
	Rational();
	Rational(int n);
	Rational(int x, int y);
	~Rational();

	std::string toString();

	friend Rational operator+(Rational r1, Rational r2);
	friend Rational operator-(Rational r1, Rational r2);
	friend Rational operator*(Rational r1, Rational r2);
	friend Rational operator/(Rational r1, Rational r2);

private:
	int num;
	int den;
};

std::ostream &operator<<(std::ostream &os, Rational rat);

Rational operator+(Rational r1, Rational r2);
Rational operator-(Rational r1, Rational r2);
Rational operator*(Rational r1, Rational r2);
Rational operator/(Rational r1, Rational r2);


#endif

#include "stringW.h"

#include<iostream>
#include<string>
#include<cctype>
#include<cstdlib>


bool endWith(std::string str, std::string suffix)
{
	if (str.length() < suffix.length())return false;
	int j = str.length() - 1;
	for (int i = suffix.length() - 1; i >= 0; i--)
	{
		if (suffix[i] != str[j])return false;
		j--;
	}
	return true;
}

bool endWith(std::string str, char suffix)
{
	return str.length() > 0 && str[str.length() - 1] == suffix;
}

bool startWith(std::string str, std::string prefix)
{
	if (str.length() < prefix.length()) return false;
	int nChars = prefix.length();
	for (int i = 0; i < nChars; i++) {
		if (str[i] != prefix[i]) return false;
	}
	return true;
}

bool startWith(std::string str, char prefix)
{
	return str.length() > 0 && str[0] == prefix;
}

std::string trim(std::string str)
{
	std::string str2 = "";
	for (int i = 0; i < str.length(); i++)
		if (!isspace(str[i]))str2 += str[i];
	return str2;
}

std::string substrMy(std::string str, char pos, int n)
{
	if (str.length() < 1) error("输入错误，不存在子串");
	int nChar = str.length();
	std::string str1 = "";
	for (int i = 0; i < nChar; i++)
	{
		if (str[i] == pos)
		{
			//switch (n)
			{
				if (n <= nChar)
					for (int j = i; j < i + n; j++)
						str1 += str[j];
				if (n > nChar)
					for (int j = i; j < nChar; j++)
						str1 += str[j];
				break;
			}
		}
	}
	return str1;
}

std::string substrMy(std::string str, char pos)
{
	if (str.length() < 1) error("输入错误，不存在子串");
	int nChar = str.length();
	std::string str1 = "";
	for (int i = 0; i < nChar; i++)
	{
		if (str[i] == pos)
		{
			for (int j = i; j < nChar; j++)
				str1 += str[j];
			break;
		}
	}
	return str1;
}

std::string substrMy(std::string str, int pos, int n)
{
	if (str.length() < pos) error("输入错误，不存在子串");
	int nChar = str.length();
	std::string str1 = "";
	if (n <= nChar)
		for (int i = pos; i < pos + n; i++)
		{
			str1 += str[i];
		}
	if (n > nChar)
		for (int i = pos; i < nChar; i++)
			str1 += str[i];
	return str1;
}

std::string substrMy(std::string str, int pos)
{
	if (str.length() < pos) error("输入错误，不存在子串");
	int nChar = str.length();
	std::string str1 = "";
	for (int i = pos; i < nChar; i++)
		str1 += str[i];
	return str1;
}

std::string capitalize(std::string str)
{
	int n = str.length();
	for (int i = 0; i < n; i++)
	{
		if (isalpha(str[i]))
		{
			str[i] = toupper(str[i]);
			for (int j = i + 1; j < n; j++)
				str[j] = tolower(str[j]);
			break;
		}
	}
	return str;
}

std::string acronym(std::string str)
{
	std::string str1 = "";//临时字符串
	std::string str2 = "";//结果
	for (int i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]))
		{
			str1 += str[i];
		}
		else
		{
			if (str1 != "")
			{
				str2 += str1[0];
				str1 = "";
			}
		}
	}
	str2 += str1[0];
	return str2;
}

std::string removeCharacters(std::string str, std::string remove)
{
	//int m = 0;
	for (int i = 0; i < remove.length(); i++)
		for (int j = 0; j < str.length(); j++)
		{
			//m++;
			//std::cout << m << "循环次数" << std::endl;
			if (str[j] == remove[i])
			{
				str.erase(j, 1);
				j--;
			}
		}

	return str;
}

std::string removeCharactersInPlace(std::string str, int n)
{
	if (n <= 0)return str;
	for (int i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]))
		{
			str.erase(i, 1);
			i--;
			n--;
			if (n == 0)return str;
		}
	}
	return str;
}

std::string removeDoubledLetters(std::string str)
{
	if (str.length() <= 0)return str;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == str[i + 1])//不能为i-1？？？
		{
			str.erase(i, 1);
			i--;
		}
	}
	return str;
}

std::string replaceAll(std::string str, char c1, char c2)
{
	for (int i = 0; i < str.length(); i++)
		if (str[i] == c1)
			str[i] = c2;
	return str;
}

std::string replaceAll(std::string str, std::string str1, std::string str2)
{
	//if (str == str1)return str2;
	if (str.length() < str1.length())return str;
	std::string str3 = "";
	for (int i = 0; i < str.length(); i++)//遍历str
	{
		if (str[i] == str1[0])//如果str 与str1第一个字符相同
		{
			for (int j = 0; j < str1.length(); j++)//检测str接下来按顺序是否有与str1一样的相同字符
			{
				if (str[i + j] == str1[j])
				{
					str3 += str[i + j];//如果str，str1有相同字符，存入中间变量str3中
				}
				else
					break;
			}

			if (str3 == str1)//检测str子串str3是否跟str1一样
			{
				str.replace(i, str1.length(), str2);//替换
				i += str2.length() - 1;//遍历位置变更为i加上替换为字符串str2的长度再减1的位置
			}
			str3.erase(0);//清空str3以便检测后面内容
		}
	}
	return str;
}

bool isSentencePalindrome(std::string str)
{
	std::string str1 = "";
	std::string str2 = "";
	for (int i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]))
			str1 += toupper(str[i]);
	}
	//std::cout << str1 << std::endl;
	if (str1 == "")return true;
	for (int j = str1.length() - 1; j >= 0; j--)
	{
		str2 += str1[j];
	}

	if (str1 == str2)
		return true;

	return false;

}

std::string creatRegularPlural(std::string word)
{
	int n = word.length() - 1;
	switch (word[n])
	{
	case 's':
	case 'x':
	case 'z':
		return word + "es";
	case 'h':
		if (word[n - 1] == 'c' || word[n - 1] == 's')
			return word + "es";
		return word + "s";
	case 'y':
		switch (word[n - 1])
		{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
			return word + "s";
		default:
			return word.replace(n, 1, "ies");
		}
	default:
		return word + "s";
	}
}

std::string addCommas(std::string digits)
{
	for (int i = digits.length() - 3; i > 0; i -= 3)
		digits.insert(i, ",");
	return digits;
}

std::string lineToPigLatin(std::string line) {
	std::string result;
	std::string str = "";//改进后
	int start = -1;
	for (int i = 0; i < line.length(); i++) {
		char ch = line[i];
		if (isalpha(ch))
		{
			str += ch;//改进后
			if (start == -1)
			{
				start = i;
			}

		}
		else
		{
			if (start >= 0)
			{
				if (isupper(str[0]))//改进后
				{
					str[0] = tolower(str[0]);
					str = wordToPigLatin(str);
					str[0] = toupper(str[0]);
				}
				else {
					str = wordToPigLatin(str);//改进后
				}
				result += str;//改进后
							  //result += wordToPigLatin(line.substr(start, i - start));//改进前
				start = -1;
				str = "";//改进后
			}
			result += ch;
		}
	}
	if (start >= 0)
	{//此大括号内为改进后
		if (isupper(str[0]))
		{
			str[0] = tolower(str[0]);
			str = wordToPigLatin(str);
			str[0] = toupper(str[0]);
		}
		else {
			str = wordToPigLatin(str);
		}
		result += str;
		start = -1;
		str = "";
	}
	//result += wordToPigLatin(line.substr(start));//改进前
	return result;
}

std::string wordToPigLatin(std::string word) {
	int vp = findFirstVowel(word);
	if (vp == -1) {
		return word;
	}
	else if (vp == 0) {
		return word + "way";
	}
	else {
		std::string head = word.substr(0, vp);
		std::string tail = word.substr(vp);
		return tail + head + "ay";
	}
}

int findFirstVowel(std::string word) {
	for (int i = 0; i < word.length(); i++) {
		if (isVowel(word[i])) return i;
	}
	return -1;
}

bool isVowel(char ch) {
	switch (ch) {
	case 'A': case 'E': case 'I': case 'O': case 'U':
	case 'a': case 'e': case 'i': case 'o': case 'u':
		return true;
	default:
		return false;
	}
}


void error(std::string msg)
{
	std::cerr << msg << std::endl;
	exit(EXIT_FAILURE);
}

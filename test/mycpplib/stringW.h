//字符串处理
//!testThree_h

#ifndef stringW_h
#define stringW_h

#include<string>

//3-1
bool endWith(std::string str, std::string suffix);//判断特定结尾
bool endWith(std::string str, char suffix);

bool startWith(std::string str, std::string prefix);//判断特定开始
bool startWith(std::string str, char prefix);

std::string trim(std::string str);//3-2删除空白字符

								  //3-3
std::string substrMy(std::string str, char pos, int n); //返回从第一个pos字符起，n个字符的子串
std::string substrMy(std::string str, char pos);
std::string substrMy(std::string str, int pos, int n);//3-3正确题意 返回从第pos+1个字符起，n个字符的子串
std::string substrMy(std::string str, int pos);//3-3正确题意

std::string capitalize(std::string str);//3-4首字母大写，其余小写，非字母不受影响

std::string acronym(std::string str);//3-6//返回首字母缩略词
std::string removeCharacters(std::string str, std::string remove);//3-7返回删除remove中的字符后的字符串

//3-8删除前n个字母
std::string removeCharactersInPlace(std::string str, int n);

std::string removeDoubledLetters(std::string str);//3-9删除重复字母，未考虑更多情况，比如该字符串不是单词

//3-10 字符（串）c2替换c1
std::string replaceAll(std::string str, char c1, char c2);
std::string replaceAll(std::string str, std::string str1, std::string str2);

bool isSentencePalindrome(std::string str);//3-11回文识别，忽略符号 大小写

std::string creatRegularPlural(std::string word);//3-12 返回单词复数形式  只限题中规则，小写情况

std::string addCommas(std::string digits);//3-14十进制字符串每三位数字插入一个逗号

										  //斯坦福儿童黑话
std::string lineToPigLatin(std::string line);//3-15 整行翻译为儿童黑话，改进后的函数（如果首字母大写）
std::string wordToPigLatin(std::string word);//单词翻译为儿童黑话
int findFirstVowel(std::string word);//找到第一个元音字母
bool isVowel(char ch);//判断元音字母


void error(std::string msg);


#endif 

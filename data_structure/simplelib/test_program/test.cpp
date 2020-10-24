#include <iostream>
#include <string>

#include "String.h"
#include "HashMap.h"
#include "Queue.h"
#include "QueueL.h"

/*
 *用std::string与自定义的String类型结合哈希表调试结果一样
 */
#define STRING String
//#define STRING std::string

template<typename K,typename V>
void printKeyValue(const K &key,const V &value){
	std::cout<<key<<":"<<value<<std::endl;
}

void initHashMap(HashMap<STRING,STRING>& map);//美国50个州的字符映射

int main(int argc, char** argv){

	String str("string");
	std::cout<<str<<std::endl;

	HashMap<STRING,STRING> map;
	initHashMap(map);
	map.mapAll(printKeyValue);

	QueueL<int> qli;
	qli.enqueue(1);
	qli.enqueue(3);
	qli.enqueue(6);

	Queue<int> qi;
	qi.enqueue(20);
	qi.enqueue(30);
	qi.enqueue(50);
	qi.enqueue(40);

	//std::cout<<"qli.peek():"<<qli.peek()<<std::endl;
	//std::cout<<"qi.peek():"<<qi.peek()<<std::endl;
	std::cout<<"qli.size():"<<qli.size()<<std::endl;
	std::cout<<"qi.size():"<<qi.size()<<std::endl;
	
	while(!qi.isEmpty()){
		std::cout<<"qi.peek():"<<qi.peek()<<std::endl;
		
		//std::cout<<"qi.dequeue();"<<std::endl;
		qi.dequeue();
	}

	while(!qli.isEmpty()){
		std::cout<<"qli.peek():"<<qli.peek()<<std::endl;
		
		//std::cout<<"qli.dequeue();"<<std::endl;
		qli.dequeue();
	}

	std::cout<<"测试ok！"<<std::endl;
 	return 0;
}

void initHashMap(HashMap<STRING,STRING>& map){
	map.put("AK", "Alaska");
	map.put("AL", "Alabama");
	map.put("AR", "Arkansas");
	map.put("AZ", "Arizona");
	map.put("CA", "California");
	map.put("CO", "Colorado");
	map.put("CT", "Connecticut");
	map.put("DE", "Delaware");
	map.put("FL", "Florida");
	map.put("GA", "Georgia");
	map.put("HI", "Hawaii");
	map.put("IA", "Iowa");
	map.put("ID", "Idaho");
	map.put("IL", "Illinois");
	map.put("IN", "Indiana");
	map.put("KS", "Kansas");
	map.put("KY", "Kentucky");
	map.put("LA", "Louisiana");
	map.put("MA", "Massachusetts");
	map.put("MD", "Maryland");
	map.put("ME", "Maine");
	map.put("MI", "Michigan");
	map.put("MN", "Minnesota");
	map.put("MO", "Missouri");
	map.put("MS", "Mississippi");
	map.put("MT", "Montana");
	map.put("NC", "North Carolina");
	map.put("ND", "North Dakota");
	map.put("NE", "Nebraska");
	map.put("NH", "New Hampshire");
	map.put("NJ", "New Jersey");
	map.put("NM", "New Mexico");
	map.put("NV", "Nevada");
	map.put("NY", "New York");
	map.put("OH", "Ohio");
	map.put("OK", "Oklahoma");
	map.put("OR", "Oregon");
	map.put("PA", "Pennsylvania");
	map.put("RI", "Rhode Island");
	map.put("SC", "South Carolina");
	map.put("SD", "South Dakota");
	map.put("TN", "Tennessee");
	map.put("TX", "Texas");
	map.put("UT", "Utah");
	map.put("VA", "Virginia");
	map.put("VT", "Vermont");
	map.put("WA", "Washington");
	map.put("WI", "Wisconsin");
	map.put("WV", "West Virginia");
	map.put("WY", "Wyoming");
}

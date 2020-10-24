#!/bin/bash

#单行命令

echo "this is a test" | sed 's/test/test script/'

sed '2,4s/test/bigtest/' data1.txt

sed 's/is/www/' data1.txt
sed 's/is/tttt/2' data1.txt
sed 's/is/ee/g' data1.txt

#多命令组合,用换行符隔开，空格不行
echo
sed '2{
s/is/ee/2
s/test/bigtest/
}' data1.txt


#p标记额外输出修改过得行
echo "pp1-----"
sed '1,3s/is/www/p' data1.txt

#-n禁止sed输出，p只打印修改过得行
echo "pp2-----"
sed -n '1,3s/is/www/p' data1.txt

#使用!作分隔符
echo "!!!---"
#sed 's!this!these!' data1.txt
echo $HOME
echo $HOME | sed 's!/home/weng!/dir/wxl!'

#文本模式过滤器
#只修改匹配到over的行
sed '/over/s/this/qqqq/' data1.txt

#删除命令
echo "dddd--------"
sed '2,5d' data1.txt

#文件结尾字符$
sed '2,$d' data1.txt

#模式过滤
sed '/over/d' data1.txt

sed '/1/,/4/d' data1.txt

#插入命令i， 追加命令a， 修改命令c
echo "iiiiaaaccc----"
sed '2i\haha, new line' data1.txt
echo
sed '2a\haha, new line' data1.txt
echo
sed '2c\haha, new line' data1.txt

echo "pattern--- i a c------------"
sed '/over/i\haha, new line' data1.txt
sed '/over/a\haha, new line' data1.txt
sed '/over/c\haha, new line' data1.txt

#y 单个字符转换命令,这个命令是全局的，所有匹配到的字符都会被转换，无法控制在特定的地方使用
echo "yyy----------"
sed '/over/y/this/rtyu/' data1.txt

#打印命令p, =, l
echo "p, =, l------------"
sed '/over/p' data1.txt
echo

#只打印匹配到的行
sed -n '/over/p' data1.txt

# =命令 打印行号
echo "==------"
sed '=' data1.txt
echo 
#只打印行号
sed -n '/over/=' data1.txt
echo
#打印行号 + 匹配到的行
sed -n '/over/{
=
p
}' data1.txt

#打印ASCII字符 这里是制表符
echo
#echo "what	is	this" | sed 'l'
echo "what	is	this" | sed -n 'l'

echo "wwwrrrr-----------------"
#w命令写入到文件
#sed '2,$w tmp.txt' data1.txt

sed -n '5,$w tmp.txt' data1.txt
cat tmp.txt

#r命令从文件读取数据插入到数据流中
echo
sed '2r tmp.txt' data1.txt

#插入到匹配行后删掉此匹配行
echo
sed '/over/{
r tmp.txt
d
}' data1.txt








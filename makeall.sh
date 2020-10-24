#!/bin/bash

#创建函数，遍历目录，执行make 
function makeall {
    #这里必须声明为局部变量，否则下面的函数调用中改变了此值
    local var="$(pwd)"
    for file in $var/*
    do
        if [ -d "$file" ]
        then
            #echo "$file --d"
            cd $file
            makeall
            
        elif [ -f "$file" ]
        then
            #echo "$file --f"
            #echo $var
            
            if [ "$file" = "$var/Makefile" ] || [ "$file" = "$var/makefile" ]
            then
                cd $var #必须加上这行，否则可能在上面的函数调用中改变了当前所在目录
                #echo "$file"
                
                make
            
 #如果make (clean)不成功，则打印文件所在目录
                if [ $? -ne 0 ]
                then
                    echo "make error: $file"
                    pwd
                fi
            fi
        fi
    done
}

makeall







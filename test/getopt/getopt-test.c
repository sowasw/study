#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
    int ch;
    printf("%d %d %c %s\n", optind, opterr, optopt, optarg);
    //optind 下一个要处理的argv索引，初始值为1
    //optopt 未知选项
    //optarg 选项的参数
    
    //字符串开始的冒号表示忽略错误信息
    //选项后的冒号表示该选项后需要接参数,参数可以紧挨选项，也可以空格隔开
    //双冒号表示可以跟参数，也可以不跟，参数必须紧挨选项
    //当解析的选项不在optstring中，ch为？，optopt为该选项的字符
    while((ch = getopt(argc, argv, ":a:bcd::e")) != -1)
    {
        printf("%d %d %c %c %s\n", optind, opterr, optopt, ch, optarg);
    }
    
    return 0;
}

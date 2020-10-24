#!/bin/bash

#sed '{n; d}' data2.txt
#sed '{N; d}' data2.txt
#sed 'N; /\n/d' data2.txt

#sed 'N; D' data2.txt

#sed  'P; N; D' data2.txt
#sed -n 'N; P; D' data2.txt
#sed 'N; /\n/D' data2.txt

#sed 'D' data2.txt

#sed -n '3P; N ' data2.txt

#只显示最后二行
sed '{
:start
$q; N; 3,$D
b start
}' data2.txt

echo ------------
sed  '
s/my name/his family name/
$!N
s/my \nname/his \nfamily name/
P
D
' data3.txt

#sed -n '24,31p' sed_test2.sh | sh 可以执行上一条命令

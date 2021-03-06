#!/bin/bash2
# ============================================================== #
# Удаление "лишних средств IPC
#
# ============================================================== #
rm $HOME/tmp01 2>/dev/null
rm $HOME/tmp02 2>/dev/null
# Определение имени пользователя
m=`whoami`
#=========== УДАЛЕНИЕ ОЧЕРЕДЕЙ ===============
# получение списка очередей;
# выборка строк с именем пользователя;
# выборка столбца с ID очереди;
# запись в файл
ipcs -q | grep $m | cut -f2 -d' ' | grep [0-9] >$HOME/tmp01
# перебор файла с ID очередей
while test -s $HOME/tmp01
do
# выборка 1-й строки
    str=`head -n1 $HOME/tmp01`
# удаление очереди по ее ID
    ipcrm msg $str
# удаление 1-й строки из файла
    sed '1d' $HOME/tmp01 >$HOME/tmp02
    rm $HOME/tmp01
    mv $HOME/tmp02 $HOME/tmp01
done
# удаление временных файлов
rm $HOME/tmp01 2>/dev/null
rm $HOME/tmp02 2>/dev/null
#=========== то же - для семафоров ===============
ipcs -s | grep $m | cut -f2 -d' ' | grep [0-9] >$HOME/tmp01
while test -s $HOME/tmp01
do
    str=`head -n1 $HOME/tmp01`
    ipcrm sem $str
    sed '1d' $HOME/tmp01 >$HOME/tmp02
    rm $HOME/tmp01
    mv $HOME/tmp02 $HOME/tmp01
done
rm $HOME/tmp01 2>/dev/null
rm $HOME/tmp02 2>/dev/null
#=========== то же - для общих областей памяти ===============
ipcs -m | grep $m | cut -f2 -d' ' | grep [0-9] >$HOME/tmp01
while test -s $HOME/tmp01
do
    str=`head -n1 $HOME/tmp01`
    ipcrm shm $str
    sed '1d' $HOME/tmp01 >$HOME/tmp02
    rm $HOME/tmp01
    mv $HOME/tmp02 $HOME/tmp01
done
rm $HOME/tmp01 2>/dev/null
rm $HOME/tmp02 2>/dev/null


#!bash

target="manager.exe"
cflags="-Wall -g -c -D__DEBUG__"

for i in $(ls *.c)
do
	echo "[CC] ${i/.c/.o}"
	gcc $i ${cflags} -o ${i/.c/.o}
done

echo "[LD] ${target}"
gcc *.o -o $target

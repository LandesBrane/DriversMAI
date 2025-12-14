1. Программа 
admin@Drivers:~/DriversMAI/PZ1$ gcc -o read_dev read_dev.c
admin@Drivers:~/DriversMAI/PZ1$ ls
read_dev  read_dev.c
admin@Drivers:~/DriversMAI/PZ1$ ./read_dev /dev/zero
Прочитано 100 байт из /dev/zero
Первые байты (hex): 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
admin@Drivers:~/DriversMAI/PZ1$

2. Strace

trace.log
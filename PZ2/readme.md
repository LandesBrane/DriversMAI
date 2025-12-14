admin@Drivers:~/DriversMAI/PZ2$ sudo insmod hello.ko
[sudo] password for admin:
insmod: ERROR: could not load module hello.ko: No such file or directory
admin@Drivers:~/DriversMAI/PZ2$ sudo insmod mydriver.ko
admin@Drivers:~/DriversMAI/PZ2$ lsmod | grep hello
admin@Drivers:~/DriversMAI/PZ2$ lsmod | grep mydriver
mydriver               12288  0
admin@Drivers:~/DriversMAI/PZ2$ dmesg | tail -n 2
dmesg: read kernel buffer failed: Operation not permitted
admin@Drivers:~/DriversMAI/PZ2$ sudo dmesg | tail -n 2
[20891.829087] systemd[1]: Started systemd-journald.service - Journal Service.
[21494.111371] Hello, world! Module loaded.
admin@Drivers:~/DriversMAI/PZ2$

admin@Drivers:~/DriversMAI/PZ2$ sudo dmesg | tail -n 1
[21579.306745] Goodbye! Module unloaded.
admin@Drivers:~/DriversMAI/PZ2$
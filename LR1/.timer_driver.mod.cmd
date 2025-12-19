savedcmd_/home/admin/DriversMAI/LR1/timer_driver.mod := printf '%s\n'   timer_driver.o | awk '!x[$$0]++ { print("/home/admin/DriversMAI/LR1/"$$0) }' > /home/admin/DriversMAI/LR1/timer_driver.mod

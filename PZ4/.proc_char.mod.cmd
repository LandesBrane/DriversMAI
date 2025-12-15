savedcmd_/home/admin/DriversMAI/PZ4/proc_char.mod := printf '%s\n'   proc_char.o | awk '!x[$$0]++ { print("/home/admin/DriversMAI/PZ4/"$$0) }' > /home/admin/DriversMAI/PZ4/proc_char.mod

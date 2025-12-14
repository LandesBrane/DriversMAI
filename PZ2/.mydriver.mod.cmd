savedcmd_/home/admin/DriversMAI/PZ2/mydriver.mod := printf '%s\n'   mydriver.o | awk '!x[$$0]++ { print("/home/admin/DriversMAI/PZ2/"$$0) }' > /home/admin/DriversMAI/PZ2/mydriver.mod

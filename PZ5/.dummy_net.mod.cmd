savedcmd_/home/admin/DriversMAI/PZ5/dummy_net.mod := printf '%s\n'   dummy_net.o | awk '!x[$$0]++ { print("/home/admin/DriversMAI/PZ5/"$$0) }' > /home/admin/DriversMAI/PZ5/dummy_net.mod

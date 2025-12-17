savedcmd_/home/admin/DriversMAI/PZ6/pci_net.mod := printf '%s\n'   pci_net.o | awk '!x[$$0]++ { print("/home/admin/DriversMAI/PZ6/"$$0) }' > /home/admin/DriversMAI/PZ6/pci_net.mod

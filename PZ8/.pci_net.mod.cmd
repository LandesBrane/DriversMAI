savedcmd_/home/admin/DriversMAI/PZ8/pci_net.mod := printf '%s\n'   pci_net.o | awk '!x[$$0]++ { print("/home/admin/DriversMAI/PZ8/"$$0) }' > /home/admin/DriversMAI/PZ8/pci_net.mod

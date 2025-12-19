admin@Drivers:~/DriversMAI/LR1$ ./test_app 20
Performing 20 write-read cycles...
  Cycle 01: wrote 1000, read 1000
  Cycle 02: wrote 1001, read 1001
  Cycle 03: wrote 1002, read 1002
  Cycle 04: wrote 1003, read 1003
  Cycle 05: wrote 1004, read 1004
  Cycle 06: wrote 1005, read 1005
  Cycle 07: wrote 1006, read 1006
  Cycle 08: wrote 1007, read 1007
  Cycle 09: wrote 1008, read 1008
  Cycle 10: wrote 1009, read 1009
  Cycle 11: wrote 1010, read 1010
  Cycle 12: wrote 1011, read 1011
  Cycle 13: wrote 1012, read 1012
  Cycle 14: wrote 1013, read 1013
  Cycle 15: wrote 1014, read 1014
  Cycle 16: wrote 1015, read 1015
  Cycle 17: wrote 1016, read 1016
  Cycle 18: wrote 1017, read 1017
  Cycle 19: wrote 1018, read 1018
  Cycle 20: wrote 1019, read 1019

Requesting histogram via ioctl...
Histogram printed to kernel log (check dmesg)
admin@Drivers:~/DriversMAI/LR1$


admin@Drivers:~/DriversMAI/LR1$ dmesg | tail -30
dmesg: read kernel buffer failed: Operation not permitted
admin@Drivers:~/DriversMAI/LR1$ sudo dmesg | tail -30
[63292.834865] systemd[1]: systemd-networkd.service: Failed with result 'watchdog'.
[63292.840072] systemd[1]: systemd-networkd.service: Scheduled restart job, restart counter is at 2.
[63292.841727] systemd[1]: netplan-ovs-cleanup.service - OpenVSwitch configuration for cleanup was skipped because of an unmet condition check (ConditionFileIsExecutable=/usr/bin/ovs-vsctl).
[63293.183474] systemd[1]: Starting systemd-networkd.service - Network Configuration...
[63293.184125] systemd[1]: fwupd-refresh.service: Deactivated successfully.
[63293.184334] systemd[1]: Finished fwupd-refresh.service - Refresh fwupd metadata and update motd.
[63293.360718] systemd[1]: Started systemd-journald.service - Journal Service.
[63294.103392] e1000: enp0s3 NIC Link is Up 1000 Mbps Full Duplex, Flow Control: RX
[64145.572312] timer_driver: registered with major 240
[64317.884594] timer_driver: Histogram (bin = 50 us):
[64317.884607]   bin[00] (   0-  49 us): 0
[64317.884609]   bin[01] (  50-  99 us): 0
[64317.884610]   bin[02] ( 100- 149 us): 0
[64317.884611]   bin[03] ( 150- 199 us): 0
[64317.884612]   bin[04] ( 200- 249 us): 0
[64317.884612]   bin[05] ( 250- 299 us): 0
[64317.884613]   bin[06] ( 300- 349 us): 0
[64317.884613]   bin[07] ( 350- 399 us): 0
[64317.884614]   bin[08] ( 400- 449 us): 0
[64317.884615]   bin[09] ( 450- 499 us): 0
[64317.884615]   bin[10] ( 500- 549 us): 0
[64317.884654]   bin[11] ( 550- 599 us): 0
[64317.884655]   bin[12] ( 600- 649 us): 0
[64317.884655]   bin[13] ( 650- 699 us): 0
[64317.884656]   bin[14] ( 700- 749 us): 0
[64317.884668]   bin[15] ( 750- 799 us): 0
[64317.884669]   bin[16] ( 800- 849 us): 0
[64317.884669]   bin[17] ( 850- 899 us): 0
[64317.884670]   bin[18] ( 900- 949 us): 0
[64317.884671]   bin[19] ( 950- 999 us): 20
admin@Drivers:~/DriversMAI/LR1$
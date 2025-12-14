1.

admin@Drivers:~/DriversMAI/PZ2$ sudo dmesg | tail
[12383.046764] systemd[1]: systemd-journald.service: Failed with result 'watchdog'.
[12383.047455] systemd[1]: systemd-journald.service: Scheduled restart job, restart counter is at 3.
[12383.057334] systemd[1]: Starting systemd-journald.service - Journal Service...
[12383.076995] systemd-journald[1808]: Collecting audit messages is disabled.
[12383.078711] systemd-journald[1808]: File /var/log/journal/8b9d9ed9dff949efa4f736679e8a6302/system.journal corrupted or uncleanly shut down, renaming and replacing.
[12383.096431] systemd[1]: Started systemd-journald.service - Journal Service.
[14436.414431] mydriver: loading out-of-tree module taints kernel.
[14436.414553] mydriver: module verification failed: signature and/or required key missing - tainting kernel
[14436.416312] mydriver: registered with major number 240
[14436.416316] mydriver: buffer allocated



2. Приложение:

admin@Drivers:~/DriversMAI/PZ2$ ./test_app
Wrote 23 bytes
Read: Hello from user space!
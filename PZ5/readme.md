admin@Drivers:~/DriversMAI/PZ5$ sudo insmod dummy_net.ko
[sudo] password for admin:
admin@Drivers:~/DriversMAI/PZ5$ dmesg | tail
dmesg: read kernel buffer failed: Operation not permitted
admin@Drivers:~/DriversMAI/PZ5$ sudo dmesg | tail
[121832.048083]  arch_call_rest_init+0xe/0x30
[121832.048086]  start_kernel+0x354/0x450
[121832.048089]  x86_64_start_reservations+0x18/0x30
[121832.048092]  x86_64_start_kernel+0xbf/0x110
[121832.048094]  secondary_startup_64_no_verify+0x184/0x18b
[121832.048098]  </TASK>
[121834.264018] e1000: enp0s3 NIC Link is Down
[121838.359586] e1000: enp0s3 NIC Link is Up 1000 Mbps Full Duplex, Flow Control: RX
[121849.269656] workqueue: blk_mq_run_work_fn hogged CPU for >10000us 64 times, consider switching to WQ_UNBOUND
[123541.903386] Dummy network driver loaded, interface: dummy0, MAC: 12:34:56:78:9a:bc
admin@Drivers:~/DriversMAI/PZ5$ ip link show dummy0
3: dummy0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 12:34:56:78:9a:bc brd ff:ff:ff:ff:ff:ff
admin@Drivers:~/DriversMAI/PZ5$ sudo ip link set dummy0 up
admin@Drivers:~/DriversMAI/PZ5$ ping -I dummy0 8.8.8.8
ping: Warning: source address might be selected on device other than: dummy0
PING 8.8.8.8 (8.8.8.8) from 192.168.31.162 dummy0: 56(84) bytes of data.

^C
--- 8.8.8.8 ping statistics ---
88 packets transmitted, 0 received, 100% packet loss, time 89120ms

admin@Drivers:~/DriversMAI/PZ5$ sudo dmesg | tail
[123670.927472] dummy0: packet received for transmission, len=42, protocol=0x0806
[123671.952005] dummy0: packet received for transmission, len=42, protocol=0x0806
[123672.975453] dummy0: packet received for transmission, len=42, protocol=0x0806
[123673.999402] dummy0: packet received for transmission, len=42, protocol=0x0806
[123675.023679] dummy0: packet received for transmission, len=42, protocol=0x0806
[123676.048041] dummy0: packet received for transmission, len=42, protocol=0x0806
[123677.071393] dummy0: packet received for transmission, len=42, protocol=0x0806
[123678.095453] dummy0: packet received for transmission, len=42, protocol=0x0806
[123679.119459] dummy0: packet received for transmission, len=42, protocol=0x0806
[123680.143669] dummy0: packet received for transmission, len=42, protocol=0x0806
admin@Drivers:~/DriversMAI/PZ5$ sudo ip link set dummy0 down
admin@Drivers:~/DriversMAI/PZ5$ sudo rmmod dummy_net
admin@Drivers:~/DriversMAI/PZ5$ dmesg | tail
dmesg: read kernel buffer failed: Operation not permitted
admin@Drivers:~/DriversMAI/PZ5$ sudo rmmod dummy_net
rmmod: ERROR: Module dummy_net is not currently loaded
admin@Drivers:~/DriversMAI/PZ5$ sudo dmesg | tail
[123673.999402] dummy0: packet received for transmission, len=42, protocol=0x0806
[123675.023679] dummy0: packet received for transmission, len=42, protocol=0x0806
[123676.048041] dummy0: packet received for transmission, len=42, protocol=0x0806
[123677.071393] dummy0: packet received for transmission, len=42, protocol=0x0806
[123678.095453] dummy0: packet received for transmission, len=42, protocol=0x0806
[123679.119459] dummy0: packet received for transmission, len=42, protocol=0x0806
[123680.143669] dummy0: packet received for transmission, len=42, protocol=0x0806
[123702.159336] dummy0: packet received for transmission, len=70, protocol=0x86dd
[123714.279969] dummy0: device closed
[123719.269226] Dummy network driver unloaded
admin@Drivers:~/DriversMAI/PZ5$
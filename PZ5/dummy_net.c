#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/errno.h>

#define DRV_NAME "dummy_net"

// Функция передачи пакета (аналог hard_start_xmit)
static netdev_tx_t dummy_net_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    // Выводим информацию о пакете
    pr_info("%s: packet received for transmission, len=%d, protocol=0x%04x\n",
            dev->name, skb->len, ntohs(skb->protocol));

    // Имитируем отправку: освобождаем skb и "успешно" завершаем
    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;
    dev_kfree_skb(skb);  // освобождаем пакет

    return NETDEV_TX_OK;
}

// Функция открытия интерфейса (ifconfig dummy0 up)
static int dummy_net_open(struct net_device *dev)
{
    netif_start_queue(dev);
    pr_info("%s: device opened\n", dev->name);
    return 0;
}

// Функция закрытия интерфейса (ifconfig dummy0 down)
static int dummy_net_stop(struct net_device *dev)
{
    netif_stop_queue(dev);
    pr_info("%s: device closed\n", dev-> name);
    return 0;
}

// Операции сетевого устройства
static const struct net_device_ops dummy_net_ops = {
    .ndo_open = dummy_net_open,
    .ndo_stop = dummy_net_stop,
    .ndo_start_xmit = dummy_net_start_xmit,
};

// Функция инициализации устройства
static void dummy_setup(struct net_device *dev)
{
    // Устанавливаем тип устройства
    ether_setup(dev);

    // Отключаем некоторые функции (мы их не поддерживаем)
    dev->priv_flags &= ~IFF_TX_SKB_SHARING;
    dev->destructor = free_netdev;

    // Устанавливаем операции
    dev->netdev_ops = &dummy_net_ops;

    // Задаём MAC-адрес (можно и рандомный)
    // Пример: 12:34:56:78:9a:bc
    dev->dev_addr[0] = 0x12;
    dev->dev_addr[1] = 0x34;
    dev->dev_addr[2] = 0x56;
    dev->dev_addr[3] = 0x78;
    dev->dev_addr[4] = 0x9a;
    dev->dev_addr[5] = 0xbc;

    // Или можно вызвать eth_hw_addr_random(dev); для случайного MAC
}

// === Модуль ядра ===

static struct net_device *dummy_dev;

static int __init dummy_net_init(void)
{
    int ret;

    // Выделяем сетевое устройство
    dummy_dev = alloc_netdev(0, "dummy0", NET_NAME_UNKNOWN, dummy_setup);
    if (!dummy_dev)
        return -ENOMEM;

    // Регистрируем устройство
    ret = register_netdev(dummy_dev);
    if (ret) {
        pr_err("Failed to register net device\n");
        free_netdev(dummy_dev);
        return ret;
    }

    pr_info("Dummy network driver loaded, interface: dummy0, MAC: %pM\n",
            dummy_dev->dev_addr);
    return 0;
}

static void __exit dummy_net_exit(void)
{
    unregister_netdev(dummy_dev);
    pr_info("Dummy network driver unloaded\n");
}

module_init(dummy_net_init);
module_exit(dummy_net_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Dummy network driver for educational purposes");
MODULE_VERSION("1.0");
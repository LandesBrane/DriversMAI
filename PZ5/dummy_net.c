#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>

#define DRV_NAME "dummy_net"

static netdev_tx_t dummy_net_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    pr_info("%s: packet received for transmission, len=%d, protocol=0x%04x\n",
            dev->name, skb->len, ntohs(skb->protocol));

    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;
    dev_kfree_skb(skb);

    return NETDEV_TX_OK;
}

static int dummy_net_open(struct net_device *dev)
{
    netif_start_queue(dev);
    pr_info("%s: device opened\n", dev->name);
    return 0;
}

static int dummy_net_stop(struct net_device *dev)
{
    netif_stop_queue(dev);
    pr_info("%s: device closed\n", dev->name);
    return 0;
}

static const struct net_device_ops dummy_net_ops = {
    .ndo_open = dummy_net_open,
    .ndo_stop = dummy_net_stop,
    .ndo_start_xmit = dummy_net_start_xmit,
};

static void dummy_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->priv_flags &= ~IFF_TX_SKB_SHARING;
    dev->netdev_ops = &dummy_net_ops;

    // Установка MAC-адреса (совместимо с ядром 6.8)
    unsigned char mac[ETH_ALEN] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};
    eth_hw_addr_set(dev, mac);
}

// === Модуль ===

static struct net_device *dummy_dev;

static int __init dummy_net_init(void)
{
    int ret;
    dummy_dev = alloc_netdev(0, "dummy0", NET_NAME_UNKNOWN, dummy_setup);
    if (!dummy_dev)
        return -ENOMEM;

    ret = register_netdev(dummy_dev);
    if (ret) {
        pr_err("Failed to register net device\n");
        free_netdev(dummy_dev);
        return ret;
    }

    pr_info("Dummy network driver loaded, interface: %s, MAC: %pM\n",
            dummy_dev->name, dummy_dev->dev_addr);
    return 0;
}

static void __exit dummy_net_exit(void)
{
    unregister_netdev(dummy_dev);
    free_netdev(dummy_dev); // ← освобождаем здесь
    pr_info("Dummy network driver unloaded\n");
}

module_init(dummy_net_init);
module_exit(dummy_net_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Dummy network driver for educational purposes");
MODULE_VERSION("1.0");
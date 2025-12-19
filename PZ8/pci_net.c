#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/device.h>  // для sysfs

// === PCI IDs ===
#define PCI_VENDOR_ID_DUMMY     0x10EE
#define PCI_DEVICE_ID_DUMMY_NET 0x1234

// === Приватная структура драйвера ===
struct pci_net_priv {
    resource_size_t io_addr;  // I/O-адрес (имитация)
    struct net_device *netdev;
};

// === Сетевые операции ===
static netdev_tx_t pci_net_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    pr_info("%s: TX packet, len=%u, proto=0x%04x\n",
            dev->name, skb->len, ntohs(skb->protocol));

    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

static int pci_net_open(struct net_device *dev)
{
    netif_start_queue(dev);
    pr_info("%s: opened\n", dev->name);
    return 0;
}

static int pci_net_close(struct net_device *dev)
{
    netif_stop_queue(dev);
    pr_info("%s: closed\n", dev->name);
    return 0;
}

static const struct net_device_ops pci_net_netdev_ops = {
    .ndo_open = pci_net_open,
    .ndo_stop = pci_net_close,
    .ndo_start_xmit = pci_net_start_xmit,
};

// === Sysfs: атрибут io_addr ===
static ssize_t io_addr_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct pci_dev *pdev = to_pci_dev(dev);
    struct net_device *netdev = pci_get_drvdata(pdev);
    struct pci_net_priv *priv;

    if (!netdev)
        return -ENODEV;

    priv = netdev_priv(netdev);
    return sysfs_emit(buf, "0x%llx\n", (unsigned long long)priv->io_addr);
}

static DEVICE_ATTR_RO(io_addr);

static struct attribute *pci_net_attrs[] = {
    &dev_attr_io_addr.attr,
    NULL,
};

static const struct attribute_group pci_net_attr_group = {
    .attrs = pci_net_attrs,
};

// === PCI ID таблица ===
static const struct pci_device_id pci_net_ids[] = {
    { PCI_DEVICE(PCI_VENDOR_ID_DUMMY, PCI_DEVICE_ID_DUMMY_NET) },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_net_ids);

// === Probe ===
static int pci_net_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    struct net_device *netdev;
    struct pci_net_priv *priv;
    int err;
    u8 mac_addr[ETH_ALEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

    pr_info("pci_net: probing PCI device %04x:%04x\n", pdev->vendor, pdev->device);

    err = pcim_enable_device(pdev);
    if (err) {
        dev_err(&pdev->dev, "Cannot enable PCI device\n");
        return err;
    }

    err = pcim_iomap_regions(pdev, BIT(0), "pci_net");
    if (err) {
        dev_err(&pdev->dev, "Cannot request BAR\n");
        return err;
    }

    netdev = alloc_etherdev(sizeof(*priv));
    if (!netdev) {
        dev_err(&pdev->dev, "alloc_etherdev failed\n");
        return -ENOMEM;
    }

    priv = netdev_priv(netdev);
    priv->netdev = netdev;
    // Имитация I/O-адреса (в реальном драйвере: pci_resource_start(pdev, 0))
    priv->io_addr = 0xDEADBEEF;

    SET_NETDEV_DEV(netdev, &pdev->dev);
    pci_set_drvdata(pdev, netdev);

    eth_hw_addr_set(netdev, mac_addr);
    netdev->netdev_ops = &pci_net_netdev_ops;
    netdev->priv_flags &= ~IFF_TX_SKB_SHARING;

    err = register_netdev(netdev);
    if (err) {
        dev_err(&pdev->dev, "register_netdev failed\n");
        free_netdev(netdev);
        return err;
    }

    // Создаём sysfs-атрибут
    err = sysfs_create_group(&pdev->dev.kobj, &pci_net_attr_group);
    if (err) {
        dev_err(&pdev->dev, "Failed to create sysfs group\n");
        unregister_netdev(netdev);
        free_netdev(netdev);
        return err;
    }

    pr_info("pci_net: registered %s, MAC %pM, I/O addr 0x%llx\n",
            netdev->name, netdev->dev_addr, (unsigned long long)priv->io_addr);
    return 0;
}

// === Remove ===
static void pci_net_remove(struct pci_dev *pdev)
{
    struct net_device *netdev = pci_get_drvdata(pdev);

    if (netdev) {
        sysfs_remove_group(&pdev->dev.kobj, &pci_net_attr_group);
        unregister_netdev(netdev);
        free_netdev(netdev);
        pr_info("pci_net: device removed\n");
    }
}

// === PCI Driver ===
static struct pci_driver pci_net_driver = {
    .name = "pci_net",
    .id_table = pci_net_ids,
    .probe = pci_net_probe,
    .remove = pci_net_remove,
};

// === Init / Exit ===
static int __init pci_net_init(void)
{
    return pci_register_driver(&pci_net_driver);
}

static void __exit pci_net_exit(void)
{
    pci_unregister_driver(&pci_net_driver);
}

module_init(pci_net_init);
module_exit(pci_net_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("PCI Network Driver with sysfs I/O address attribute");
MODULE_VERSION("1.0");
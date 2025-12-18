
┌──────────────────────────────────────────── Ethernet driver support ────────────────────────────────────────────┐
  │  Arrow keys navigate the menu.  <Enter> selects submenus ---> (or empty submenus ----).  Highlighted letters    │
  │  are hotkeys.  Pressing <Y> includes, <N> excludes, <M> modularizes features.  Press <Esc><Esc> to exit, <?>    │
  │  for Help, </> for Search.  Legend: [*] built-in  [ ] excluded  <M> module  < > module capable                  │
  │                                                                                                                 │
  │ ┌───────────────────↑(-)──────────────────────────────────────────────────────────────────────────────────────┐ │
  │ │                   [*]   Cortina Gemini devices                                                              │ │
  │ │                   [*]   Davicom devices                                                                     │ │
  │ │                   < >   Dave ethernet support (DNET)                                                        │ │
  │ │                   [*]   Digital Equipment devices                                                           │ │
  │ │                   [*]     DEC - Tulip devices                                                               │ │
  │ │                   < >       Early DECchip Tulip (dc2104x) PCI support                                       │ │
  │ │                   < >       DECchip Tulip (dc2114x) PCI support                                             │ │
  │ │                   < >       Winbond W89c840 Ethernet support                                                │ │
  │ │                   < >       Davicom DM910x/DM980x support                                                   │ │
  │ │                   < >       ULi M526x controller support                                                    │ │
  │ │                   < >       Xircom CardBus support                                                          │ │
  │ │                   [*]   D-Link devices                                                                      │ │
  │ │                   < >     DL2000/TC902x/IP1000A-based Gigabit Ethernet support                              │ │
  │ │                   < >     Sundance Alta support                                                             │ │
  │ │                   <M>   Dummy PCI Network Driver                                                            │ │
  │ │                   [*]   Emulex devices                                                                      │ │
  │ └───────────────────↓(+)──────────────────────────────────────────────────────────────────────────────────────┘ │
  ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
  │                            <Select>    < Exit >    < Help >    < Save >    < Load >   



admin@Drivers:~/DriversMAI/PZ7/linux$ grep DUMMY_PCI_NET .config
CONFIG_DUMMY_PCI_NET=m
admin@Drivers:~/DriversMAI/PZ7/linux$


admin@Drivers:~/DriversMAI/PZ7/linux$ ls -l drivers/net/ethernet/dummy_pci/pci_net.ko
-rw-rw-r-- 1 admin admin 13552 Dec 18 15:27 drivers/net/ethernet/dummy_pci/pci_net.ko
admin@Drivers:~/DriversMAI/PZ7/linux$
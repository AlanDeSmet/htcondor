#!/bin/sh
sudo /etc/rc.d/init.d/dnsmasq stop
echo "Clearing DHCP State"
sudo rm /var/lib/dnsmasq/dnsmasq.leases
sudo /etc/rc.d/init.d/dnsmasq start

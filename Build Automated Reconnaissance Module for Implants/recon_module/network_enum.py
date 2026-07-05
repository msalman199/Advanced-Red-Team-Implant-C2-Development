#!/usr/bin/env python3
import netifaces
import subprocess
import json
import socket

class NetworkEnumerator:
    def __init__(self):
        self.data = {}
    
    def enumerate_interfaces(self):
        interfaces = []
        for interface in netifaces.interfaces():
            iface_data = {'name': interface}
            
            # Get IP addresses
            addrs = netifaces.ifaddresses(interface)
            if netifaces.AF_INET in addrs:
                iface_data['ipv4'] = addrs[netifaces.AF_INET]
            if netifaces.AF_INET6 in addrs:
                iface_data['ipv6'] = addrs[netifaces.AF_INET6]
            
            interfaces.append(iface_data)
        
        self.data['interfaces'] = interfaces
        return interfaces
    
    def enumerate_routing_table(self):
        try:
            result = subprocess.run(['route', '-n'], capture_output=True, text=True)
            routes = []
            
            for line in result.stdout.split('\n')[2:]:  # Skip headers
                if line.strip():
                    parts = line.split()
                    if len(parts) >= 8:
                        route = {
                            'destination': parts[0],
                            'gateway': parts[1],
                            'genmask': parts[2],
                            'flags': parts[3],
                            'interface': parts[7]
                        }
                        routes.append(route)
            
            self.data['routing_table'] = routes
            return routes
        except Exception as e:
            print(f"Error enumerating routing table: {e}")
            return []
    
    def enumerate_arp_table(self):
        try:
            result = subprocess.run(['arp', '-a'], capture_output=True, text=True)
            arp_entries = []
            
            for line in result.stdout.split('\n'):
                if line.strip() and '(' in line:
                    # Parse ARP entries
                    parts = line.split()
                    if len(parts) >= 4:
                        entry = {
                            'hostname': parts[0],
                            'ip': parts[1].strip('()'),
                            'mac': parts[3] if len(parts) > 3 else 'unknown'
                        }
                        arp_entries.append(entry)
            
            self.data['arp_table'] = arp_entries
            return arp_entries
        except Exception as e:
            print(f"Error enumerating ARP table: {e}")
            return []
    
    def save_results(self, filename='network_enum.json'):
        with open(filename, 'w') as f:
            json.dump(self.data, f, indent=2)
        print(f"Network enumeration saved to {filename}")

if __name__ == "__main__":
    enumerator = NetworkEnumerator()
    interfaces = enumerator.enumerate_interfaces()
    routes = enumerator.enumerate_routing_table()
    arp = enumerator.enumerate_arp_table()
    enumerator.save_results()
    print(f"Enumerated {len(interfaces)} interfaces, {len(routes)} routes, {len(arp)} ARP entries")

#!/usr/bin/env python3
import subprocess
import json
import re

class ServiceEnumerator:
    def __init__(self):
        self.data = {}
    
    def enumerate_systemd_services(self):
        try:
            result = subprocess.run(['systemctl', 'list-units', '--type=service', '--all', '--no-pager'], 
                                  capture_output=True, text=True)
            services = []
            
            for line in result.stdout.split('\n')[1:]:
                if line.strip() and not line.startswith('UNIT'):
                    parts = line.split()
                    if len(parts) >= 4:
                        service = {
                            'name': parts[0],
                            'load': parts[1],
                            'active': parts[2],
                            'sub': parts[3],
                            'description': ' '.join(parts[4:]) if len(parts) > 4 else ''
                        }
                        services.append(service)
            
            self.data['systemd_services'] = services
            return services
        except Exception as e:
            print(f"Error enumerating systemd services: {e}")
            return []
    
    def enumerate_listening_services(self):
        try:
            result = subprocess.run(['netstat', '-tlnp'], capture_output=True, text=True)
            listening = []
            
            for line in result.stdout.split('\n'):
                if 'LISTEN' in line:
                    parts = line.split()
                    if len(parts) >= 4:
                        service = {
                            'protocol': parts[0],
                            'address': parts[3],
                            'process': parts[6] if len(parts) > 6 else 'unknown'
                        }
                        listening.append(service)
            
            self.data['listening_services'] = listening
            return listening
        except Exception as e:
            print(f"Error enumerating listening services: {e}")
            return []
    
    def save_results(self, filename='service_enum.json'):
        with open(filename, 'w') as f:
            json.dump(self.data, f, indent=2)
        print(f"Service enumeration saved to {filename}")

if __name__ == "__main__":
    enumerator = ServiceEnumerator()
    systemd_services = enumerator.enumerate_systemd_services()
    listening_services = enumerator.enumerate_listening_services()
    enumerator.save_results()
    print(f"Enumerated {len(systemd_services)} systemd services and {len(listening_services)} listening services")

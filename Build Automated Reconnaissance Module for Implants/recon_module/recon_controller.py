#!/usr/bin/env python3
import json
import datetime
import os
import sys
from process_enum import ProcessEnumerator
from service_enum import ServiceEnumerator
from network_enum import NetworkEnumerator

class ReconController:
    def __init__(self, output_dir='recon_output'):
        self.output_dir = output_dir
        self.timestamp = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
        self.create_output_directory()
    
    def create_output_directory(self):
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        
        # Create timestamped subdirectory
        self.session_dir = os.path.join(self.output_dir, f'recon_{self.timestamp}')
        os.makedirs(self.session_dir, exist_ok=True)
    
    def run_process_enumeration(self):
        print("[+] Running process enumeration...")
        proc_enum = ProcessEnumerator()
        results = proc_enum.enumerate_processes()
        
        output_file = os.path.join(self.session_dir, 'processes.json')
        proc_enum.save_results(output_file)
        return results
    
    def run_service_enumeration(self):
        print("[+] Running service enumeration...")
        svc_enum = ServiceEnumerator()
        svc_enum.enumerate_systemd_services()
        svc_enum.enumerate_listening_services()
        
        output_file = os.path.join(self.session_dir, 'services.json')
        svc_enum.save_results(output_file)
        return svc_enum.data
    
    def run_network_enumeration(self):
        print("[+] Running network enumeration...")
        net_enum = NetworkEnumerator()
        net_enum.enumerate_interfaces()
        net_enum.enumerate_routing_table()
        net_enum.enumerate_arp_table()
        
        output_file = os.path.join(self.session_dir, 'network.json')
        net_enum.save_results(output_file)
        return net_enum.data
    
    def generate_summary_report(self, proc_data, svc_data, net_data):
        summary = {
            'timestamp': self.timestamp,
            'summary': {
                'total_processes': proc_data.get('total_processes', 0),
                'systemd_services': len(svc_data.get('systemd_services', [])),
                'listening_services': len(svc_data.get('listening_services', [])),
                'network_interfaces': len(net_data.get('interfaces', [])),
                'routing_entries': len(net_data.get('routing_table', [])),
                'arp_entries': len(net_data.get('arp_table', []))
            }
        }
        
        summary_file = os.path.join(self.session_dir, 'summary.json')
        with open(summary_file, 'w') as f:
            json.dump(summary, f, indent=2)
        
        print(f"\n[+] Reconnaissance Summary:")
        print(f"    Processes: {summary['summary']['total_processes']}")
        print(f"    Systemd Services: {summary['summary']['systemd_services']}")
        print(f"    Listening Services: {summary['summary']['listening_services']}")
        print(f"    Network Interfaces: {summary['summary']['network_interfaces']}")
        print(f"    Routing Entries: {summary['summary']['routing_entries']}")
        print(f"    ARP Entries: {summary['summary']['arp_entries']}")
        print(f"\n[+] Results saved to: {self.session_dir}")
    
    def run_full_reconnaissance(self):
        print(f"[+] Starting automated reconnaissance - {self.timestamp}")
        
        try:
            proc_data = self.run_process_enumeration()
            svc_data = self.run_service_enumeration()
            net_data = self.run_network_enumeration()
            
            self.generate_summary_report(proc_data, svc_data, net_data)
            print("\n[+] Reconnaissance completed successfully!")
            
        except Exception as e:
            print(f"[-] Error during reconnaissance: {e}")
            sys.exit(1)

if __name__ == "__main__":
    controller = ReconController()
    controller.run_full_reconnaissance()

#!/usr/bin/env python3
import psutil
import json
import datetime

class ProcessEnumerator:
    def __init__(self):
        self.data = {}
    
    def enumerate_processes(self):
        processes = []
        for proc in psutil.process_iter(['pid', 'name', 'username', 'cmdline', 'cpu_percent', 'memory_percent']):
            try:
                proc_info = proc.info
                proc_info['create_time'] = datetime.datetime.fromtimestamp(proc.create_time()).isoformat()
                processes.append(proc_info)
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                continue
        
        self.data['processes'] = processes
        self.data['total_processes'] = len(processes)
        return self.data
    
    def save_results(self, filename='process_enum.json'):
        with open(filename, 'w') as f:
            json.dump(self.data, f, indent=2)
        print(f"Process enumeration saved to {filename}")

if __name__ == "__main__":
    enumerator = ProcessEnumerator()
    results = enumerator.enumerate_processes()
    enumerator.save_results()
    print(f"Enumerated {results['total_processes']} processes")

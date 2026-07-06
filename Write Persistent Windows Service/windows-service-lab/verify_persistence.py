#!/usr/bin/env python3
import subprocess
import json
import os
import time

def check_persistence_mechanisms():
    """Comprehensive persistence check"""
    results = {}
    
    # Check systemd service
    try:
        result = subprocess.run(['systemctl', 'is-enabled', 'mypersistentservice.service'], 
                              capture_output=True, text=True)
        results['systemd_enabled'] = result.returncode == 0
        results['systemd_status'] = result.stdout.strip()
    except:
        results['systemd_enabled'] = False
        results['systemd_status'] = 'error'
    
    # Check service is active
    try:
        result = subprocess.run(['systemctl', 'is-active', 'mypersistentservice.service'], 
                              capture_output=True, text=True)
        results['service_active'] = result.stdout.strip() == 'active'
    except:
        results['service_active'] = False
    
    # Check configuration file
    config_file = '/tmp/mypersistentservice_config.json'
    results['config_exists'] = os.path.exists(config_file)
    
    if results['config_exists']:
        try:
            with open(config_file, 'r') as f:
                config = json.load(f)
            results['config_valid'] = 'service_name' in config
        except:
            results['config_valid'] = False
    
    # Check startup script
    startup_script = '/etc/init.d/mypersistentservice'
    results['startup_script_exists'] = os.path.exists(startup_script)
    
    # Check log file
    log_file = '/tmp/mypersistentservice.log'
    results['log_file_exists'] = os.path.exists(log_file)
    
    return results

def print_persistence_report(results):
    """Print formatted persistence report"""
    print("=== Service Persistence Report ===")
    print(f"Systemd Service Enabled: {'✓' if results['systemd_enabled'] else '✗'}")
    print(f"Service Currently Active: {'✓' if results['service_active'] else '✗'}")
    print(f"Configuration File: {'✓' if results['config_exists'] else '✗'}")
    print(f"Startup Script: {'✓' if results['startup_script_exists'] else '✗'}")
    print(f"Log File: {'✓' if results['log_file_exists'] else '✗'}")
    
    # Overall assessment
    critical_checks = [results['systemd_enabled'], results['service_active']]
    if all(critical_checks):
        print("\n🟢 Service persistence: EXCELLENT")
    elif any(critical_checks):
        print("\n🟡 Service persistence: PARTIAL")
    else:
        print("\n🔴 Service persistence: FAILED")

if __name__ == "__main__":
    results = check_persistence_mechanisms()
    print_persistence_report(results)

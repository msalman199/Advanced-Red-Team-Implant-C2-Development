#!/usr/bin/env python3
import subprocess
import sys
import time
import os

class ServiceManager:
    def __init__(self, service_name="mypersistentservice"):
        self.service_name = service_name
    
    def check_service_status(self):
        """Check if service is running"""
        try:
            result = subprocess.run(['systemctl', 'is-active', f'{self.service_name}.service'], 
                                  capture_output=True, text=True)
            return result.stdout.strip() == 'active'
        except:
            return False
    
    def start_service(self):
        """Start the service"""
        try:
            subprocess.run(['systemctl', 'start', f'{self.service_name}.service'], check=True)
            return True
        except:
            return False
    
    def stop_service(self):
        """Stop the service"""
        try:
            subprocess.run(['systemctl', 'stop', f'{self.service_name}.service'], check=True)
            return True
        except:
            return False
    
    def restart_service(self):
        """Restart the service"""
        try:
            subprocess.run(['systemctl', 'restart', f'{self.service_name}.service'], check=True)
            return True
        except:
            return False
    
    def get_service_logs(self, lines=20):
        """Get service logs"""
        try:
            result = subprocess.run(['journalctl', '-u', f'{self.service_name}.service', 
                                   '-n', str(lines), '--no-pager'], 
                                  capture_output=True, text=True)
            return result.stdout
        except:
            return "Failed to retrieve logs"
    
    def simulate_reboot_test(self):
        """Simulate reboot persistence test"""
        print("=== Reboot Persistence Test ===")
        
        # Check initial status
        print(f"1. Initial service status: {'Running' if self.check_service_status() else 'Stopped'}")
        
        # Stop service
        print("2. Stopping service...")
        self.stop_service()
        time.sleep(2)
        print(f"   Service status: {'Running' if self.check_service_status() else 'Stopped'}")
        
        # Start service (simulating boot)
        print("3. Starting service (simulating boot)...")
        self.start_service()
        time.sleep(3)
        print(f"   Service status: {'Running' if self.check_service_status() else 'Stopped'}")
        
        # Check logs
        print("4. Recent service logs:")
        logs = self.get_service_logs(10)
        print(logs[-500:] if len(logs) > 500 else logs)

def main():
    manager = ServiceManager()
    
    if len(sys.argv) > 1:
        action = sys.argv[1]
        
        if action == 'status':
            status = "Running" if manager.check_service_status() else "Stopped"
            print(f"Service status: {status}")
        
        elif action == 'start':
            if manager.start_service():
                print("Service started successfully")
            else:
                print("Failed to start service")
        
        elif action == 'stop':
            if manager.stop_service():
                print("Service stopped successfully")
            else:
                print("Failed to stop service")
        
        elif action == 'restart':
            if manager.restart_service():
                print("Service restarted successfully")
            else:
                print("Failed to restart service")
        
        elif action == 'logs':
            print(manager.get_service_logs())
        
        elif action == 'test-reboot':
            manager.simulate_reboot_test()
        
        else:
            print("Usage: python3 service_manager.py [status|start|stop|restart|logs|test-reboot]")
    else:
        print("Service Manager - Available commands:")
        print("  status      - Check service status")
        print("  start       - Start service")
        print("  stop        - Stop service")
        print("  restart     - Restart service")
        print("  logs        - Show service logs")
        print("  test-reboot - Test reboot persistence")

if __name__ == "__main__":
    main()

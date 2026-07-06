#!/usr/bin/env python3
import os
import sys
import time
import json
import signal
import logging
from datetime import datetime
import subprocess

class PersistentService:
    def __init__(self, name="PersistentService"):
        self.name = name
        self.running = True
        self.config_file = f"/tmp/{name.lower()}_config.json"
        self.log_file = f"/tmp/{name.lower()}.log"
        self.pid_file = f"/tmp/{name.lower()}.pid"
        
        # Setup logging
        logging.basicConfig(
            filename=self.log_file,
            level=logging.INFO,
            format='%(asctime)s - %(levelname)s - %(message)s'
        )
        self.logger = logging.getLogger(name)
    
    def create_persistence_config(self):
        """Create configuration for service persistence"""
        config = {
            "service_name": self.name,
            "executable_path": os.path.abspath(__file__),
            "working_directory": os.getcwd(),
            "auto_start": True,
            "restart_on_failure": True,
            "created_timestamp": datetime.now().isoformat()
        }
        
        with open(self.config_file, 'w') as f:
            json.dump(config, f, indent=2)
        
        self.logger.info(f"Persistence config created: {self.config_file}")
        return config
    
    def install_systemd_service(self):
        """Install as systemd service for persistence"""
        service_content = f"""[Unit]
Description={self.name} - Persistent Service
After=network.target

[Service]
Type=simple
User=root
WorkingDirectory={os.getcwd()}
ExecStart=/usr/bin/python3 {os.path.abspath(__file__)} --daemon
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
"""
        
        service_file = f"/etc/systemd/system/{self.name.lower()}.service"
        
        try:
            with open(service_file, 'w') as f:
                f.write(service_content)
            
            # Enable and start service
            subprocess.run(['systemctl', 'daemon-reload'], check=True)
            subprocess.run(['systemctl', 'enable', f'{self.name.lower()}.service'], check=True)
            
            self.logger.info(f"Systemd service installed: {service_file}")
            return True
        except Exception as e:
            self.logger.error(f"Failed to install systemd service: {e}")
            return False
    
    def create_startup_script(self):
        """Create startup script for additional persistence"""
        startup_script = f"""#!/bin/bash
# {self.name} Startup Script
cd {os.getcwd()}
python3 {os.path.abspath(__file__)} --daemon &
echo "Started {self.name} service"
"""
        
        script_path = f"/etc/init.d/{self.name.lower()}"
        
        try:
            with open(script_path, 'w') as f:
                f.write(startup_script)
            
            os.chmod(script_path, 0o755)
            
            # Add to startup (SysV style)
            subprocess.run(['update-rc.d', f'{self.name.lower()}', 'defaults'], 
                         capture_output=True)
            
            self.logger.info(f"Startup script created: {script_path}")
            return True
        except Exception as e:
            self.logger.error(f"Failed to create startup script: {e}")
            return False
    
    def write_pid_file(self):
        """Write process ID to file"""
        with open(self.pid_file, 'w') as f:
            f.write(str(os.getpid()))
    
    def signal_handler(self, signum, frame):
        """Handle shutdown signals"""
        self.logger.info(f"Received signal {signum}, shutting down...")
        self.running = False
    
    def service_main_loop(self):
        """Main service logic"""
        self.logger.info(f"{self.name} service started")
        self.write_pid_file()
        
        # Register signal handlers
        signal.signal(signal.SIGTERM, self.signal_handler)
        signal.signal(signal.SIGINT, self.signal_handler)
        
        counter = 0
        while self.running:
            # Service work simulation
            counter += 1
            self.logger.info(f"Service heartbeat #{counter} - {datetime.now()}")
            
            # Check if we should restart (persistence check)
            if counter % 60 == 0:  # Every 60 iterations
                self.logger.info("Performing persistence check...")
                self.verify_persistence()
            
            time.sleep(10)  # Wait 10 seconds
        
        self.logger.info(f"{self.name} service stopped")
        
        # Clean up PID file
        if os.path.exists(self.pid_file):
            os.remove(self.pid_file)
    
    def verify_persistence(self):
        """Verify persistence mechanisms are in place"""
        checks = []
        
        # Check systemd service
        try:
            result = subprocess.run(['systemctl', 'is-enabled', f'{self.name.lower()}.service'], 
                                  capture_output=True, text=True)
            if result.returncode == 0:
                checks.append("systemd: enabled")
            else:
                checks.append("systemd: disabled")
        except:
            checks.append("systemd: error")
        
        # Check startup script
        script_path = f"/etc/init.d/{self.name.lower()}"
        if os.path.exists(script_path):
            checks.append("startup_script: exists")
        else:
            checks.append("startup_script: missing")
        
        # Check config file
        if os.path.exists(self.config_file):
            checks.append("config: exists")
        else:
            checks.append("config: missing")
        
        self.logger.info(f"Persistence check: {', '.join(checks)}")
    
    def install(self):
        """Install the service with persistence"""
        print(f"Installing {self.name} service...")
        
        # Create persistence configuration
        self.create_persistence_config()
        
        # Install systemd service
        if self.install_systemd_service():
            print("✓ Systemd service installed")
        else:
            print("✗ Systemd service installation failed")
        
        # Create startup script
        if self.create_startup_script():
            print("✓ Startup script created")
        else:
            print("✗ Startup script creation failed")
        
        print(f"{self.name} installation completed!")
    
    def uninstall(self):
        """Uninstall the service"""
        print(f"Uninstalling {self.name} service...")
        
        # Stop and disable systemd service
        try:
            subprocess.run(['systemctl', 'stop', f'{self.name.lower()}.service'], 
                         capture_output=True)
            subprocess.run(['systemctl', 'disable', f'{self.name.lower()}.service'], 
                         capture_output=True)
            
            service_file = f"/etc/systemd/system/{self.name.lower()}.service"
            if os.path.exists(service_file):
                os.remove(service_file)
            
            subprocess.run(['systemctl', 'daemon-reload'], capture_output=True)
            print("✓ Systemd service removed")
        except Exception as e:
            print(f"✗ Systemd service removal failed: {e}")
        
        # Remove startup script
        try:
            script_path = f"/etc/init.d/{self.name.lower()}"
            if os.path.exists(script_path):
                subprocess.run(['update-rc.d', f'{self.name.lower()}', 'remove'], 
                             capture_output=True)
                os.remove(script_path)
            print("✓ Startup script removed")
        except Exception as e:
            print(f"✗ Startup script removal failed: {e}")
        
        # Clean up files
        for file_path in [self.config_file, self.pid_file]:
            if os.path.exists(file_path):
                os.remove(file_path)
        
        print(f"{self.name} uninstallation completed!")

def main():
    service = PersistentService("MyPersistentService")
    
    if len(sys.argv) > 1:
        if sys.argv[1] == '--install':
            service.install()
        elif sys.argv[1] == '--uninstall':
            service.uninstall()
        elif sys.argv[1] == '--daemon':
            service.service_main_loop()
        else:
            print("Usage: python3 persistent_service.py [--install|--uninstall|--daemon]")
    else:
        print("Interactive mode - starting service...")
        service.service_main_loop()

if __name__ == "__main__":
    main()

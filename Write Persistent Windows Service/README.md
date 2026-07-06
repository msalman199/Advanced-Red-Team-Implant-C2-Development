<div align="center">

# 🔄 Persistent Windows Service
## Cross-Platform Service Development & Persistence

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Python3](https://img.shields.io/badge/Python3-3776AB?style=for-the-badge&logo=python&logoColor=white)
![systemd](https://img.shields.io/badge/systemd-Daemon-red?style=for-the-badge)
![Service Persistence](https://img.shields.io/badge/Service%20Persistence-Advanced-orange?style=for-the-badge)
![Automation](https://img.shields.io/badge/Automation-DevOps-brightgreen?style=for-the-badge)
![Windows Concepts](https://img.shields.io/badge/Windows%20Concepts-Cross%20Platform-blue?style=for-the-badge)
![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Training-blue?style=for-the-badge)

**Create persistent services that survive system reboots using modern systemd and legacy startup mechanisms**

[Objectives](#-objectives) • [Prerequisites](#-prerequisites) • [Tasks](#-tasks) • [Testing](#-testing) • [Key Takeaways](#-key-takeaways)

</div>

---

## 📋 Table of Contents

1. [Objectives](#-objectives)
2. [Prerequisites](#-prerequisites)
3. [Lab Environment](#-lab-environment)
4. [Task 1: Development Environment Setup](#-task-1-development-environment-setup)
5. [Task 2: Persistent Service Code](#-task-2-persistent-service-code)
6. [Task 3: Service Installation & Verification](#-task-3-service-installation--verification)
7. [Task 4: Persistence Testing](#-task-4-persistence-testing)
8. [Task 5: Advanced Verification](#-task-5-advanced-verification)
9. [Troubleshooting](#-troubleshooting)
10. [Key Takeaways](#-key-takeaways)
11. [MITRE ATT&CK Mapping](#-mitre-attck-mapping)

---

## 🎯 Objectives

| # | Objective | Status |
|---|-----------|--------|
| 1️⃣ | Create a Windows service using open-source tools on Linux | ☐ |
| 2️⃣ | Implement service persistence mechanisms for system reboots | ☐ |
| 3️⃣ | Test and verify service persistence functionality | ☐ |
| 4️⃣ | Understand cross-platform service development concepts | ☐ |
| 5️⃣ | Master service lifecycle management and automation | ☐ |

---

## ✅ Prerequisites

| Prerequisite | Level | Details |
|--------------|-------|---------|
| 🔧 System Services & Daemons | Intermediate | Understanding of systemd, init processes, background services |
| 🐍 Python Programming | Intermediate | Classes, file I/O, subprocess, logging modules |
| 🐧 Linux Command-Line | Intermediate | Shell commands, file permissions, process management |
| ⚙️ Process Management | Beginner | PIDs, signals, process lifecycle, auto-restart concepts |
| 📚 Configuration Files | Beginner | JSON, INI format, systemd unit files |

---

## 🏗️ Lab Environment

<div align="center">

**Al Nafi provides Linux-based cloud machines for this lab**

</div>

- ✨ Dedicated bare-metal Linux environment (Ubuntu/Debian)
- 🔑 Root access for service installation
- 🚀 Click **Start Lab** to begin your session
- ⚙️ Python3 and systemd pre-configured

---

## 🚀 Task 1: Development Environment Setup

### 🔹 Subtask 1.1: Install Required Tools

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**ESSENTIAL**</span>

Install Python, development tools, and service libraries:

```bash
# 📦 Update system packages
sudo apt update
sudo apt install -y python3 python3-pip python3-venv git build-essential

# 🔍 Verify installations
python3 --version
pip3 --version
```

**Expected Output:**
```
Python 3.10.x
pip 22.x
```

---

### 🔹 Subtask 1.2: Create Project Directory

<span style="background-color: #2196F3; color: white; padding: 3px 8px; border-radius: 3px;">**PROJECT SETUP**</span>

Initialize your workspace with virtual environment:

```bash
# 📁 Create lab directory
mkdir ~/windows-service-lab
cd ~/windows-service-lab

# 🐍 Create Python virtual environment
python3 -m venv venv

# ✅ Activate virtual environment
source venv/bin/activate

# 📍 Verify activation (prompt should show 'venv')
which python3
```

**Directory Structure:**
```
windows-service-lab/
├── venv/                      # Virtual environment
├── persistent_service.py       # Main service code
├── service_manager.py          # Management CLI
└── verify_persistence.py       # Testing utility
```

---

### 🔹 Subtask 1.3: Install Cross-Platform Libraries

<span style="background-color: #FF9800; color: white; padding: 3px 8px; border-radius: 3px;">**DEPENDENCIES**</span>

Install Python packages for service management:

```bash
# 📦 Install required libraries
pip install pywin32 python-daemon psutil

# ✅ Verify installations
pip list | grep -E "(pywin32|daemon|psutil)"

# 🔍 Check library versions
python3 -c "import psutil; print(psutil.__version__)"
```

**Library Purposes:**
- 🪟 **pywin32** - Windows API compatibility layer
- 🎯 **python-daemon** - Unix daemon creation utilities
- 📊 **psutil** - Process and system utilities

---

## 📝 Task 2: Persistent Service Code

### 🔹 Subtask 2.1: Create Service Base Class

<span style="background-color: #9C27B0; color: white; padding: 3px 8px; border-radius: 3px;">**SERVICE CORE**</span>

Implement the main persistent service module:

```bash
# 📄 Create service file
nano persistent_service.py
```

```python
#!/usr/bin/env python3
"""
🔄 Persistent Windows Service - Cross-Platform Implementation
Simulates Windows service persistence on Linux using systemd
"""
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
        
        # 📋 Setup logging
        logging.basicConfig(
            filename=self.log_file,
            level=logging.INFO,
            format='%(asctime)s - %(levelname)s - %(message)s'
        )
        self.logger = logging.getLogger(name)
    
    def create_persistence_config(self):
        """💾 Create configuration for service persistence"""
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
        """🔄 Install as systemd service for auto-restart"""
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
            
            # 🔧 Enable and start service
            subprocess.run(['systemctl', 'daemon-reload'], check=True)
            subprocess.run(['systemctl', 'enable', f'{self.name.lower()}.service'], check=True)
            
            self.logger.info(f"Systemd service installed: {service_file}")
            return True
        except Exception as e:
            self.logger.error(f"Failed to install systemd service: {e}")
            return False
    
    def create_startup_script(self):
        """🚀 Create startup script for legacy persistence"""
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
            
            # 📌 Add to startup (SysV style)
            subprocess.run(['update-rc.d', f'{self.name.lower()}', 'defaults'], 
                         capture_output=True)
            
            self.logger.info(f"Startup script created: {script_path}")
            return True
        except Exception as e:
            self.logger.error(f"Failed to create startup script: {e}")
            return False
    
    def write_pid_file(self):
        """📍 Write process ID to file"""
        with open(self.pid_file, 'w') as f:
            f.write(str(os.getpid()))
    
    def signal_handler(self, signum, frame):
        """🛑 Handle shutdown signals gracefully"""
        self.logger.info(f"Received signal {signum}, shutting down...")
        self.running = False
    
    def service_main_loop(self):
        """⚙️ Main service execution loop"""
        self.logger.info(f"{self.name} service started")
        self.write_pid_file()
        
        # 📌 Register signal handlers
        signal.signal(signal.SIGTERM, self.signal_handler)
        signal.signal(signal.SIGINT, self.signal_handler)
        
        counter = 0
        while self.running:
            # 💼 Service work simulation
            counter += 1
            self.logger.info(f"Service heartbeat #{counter} - {datetime.now()}")
            
            # 🔍 Check persistence periodically
            if counter % 60 == 0:  # Every 60 iterations
                self.logger.info("Performing persistence check...")
                self.verify_persistence()
            
            time.sleep(10)  # ⏱️ Wait 10 seconds
        
        self.logger.info(f"{self.name} service stopped")
        
        # 🧹 Clean up PID file
        if os.path.exists(self.pid_file):
            os.remove(self.pid_file)
    
    def verify_persistence(self):
        """🔍 Verify persistence mechanisms are in place"""
        checks = []
        
        # ✅ Check systemd service
        try:
            result = subprocess.run(['systemctl', 'is-enabled', f'{self.name.lower()}.service'], 
                                  capture_output=True, text=True)
            if result.returncode == 0:
                checks.append("systemd: enabled")
            else:
                checks.append("systemd: disabled")
        except:
            checks.append("systemd: error")
        
        # ✅ Check startup script
        script_path = f"/etc/init.d/{self.name.lower()}"
        if os.path.exists(script_path):
            checks.append("startup_script: exists")
        else:
            checks.append("startup_script: missing")
        
        # ✅ Check config file
        if os.path.exists(self.config_file):
            checks.append("config: exists")
        else:
            checks.append("config: missing")
        
        self.logger.info(f"Persistence check: {', '.join(checks)}")
    
    def install(self):
        """📦 Install the service with all persistence mechanisms"""
        print(f"📦 Installing {self.name} service...")
        
        # 💾 Create persistence configuration
        self.create_persistence_config()
        
        # 🔧 Install systemd service
        if self.install_systemd_service():
            print("✓ Systemd service installed")
        else:
            print("✗ Systemd service installation failed")
        
        # 🚀 Create startup script
        if self.create_startup_script():
            print("✓ Startup script created")
        else:
            print("✗ Startup script creation failed")
        
        print(f"✅ {self.name} installation completed!")
    
    def uninstall(self):
        """🗑️ Uninstall the service completely"""
        print(f"🗑️ Uninstalling {self.name} service...")
        
        # 🛑 Stop and disable systemd service
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
        
        # 🗑️ Remove startup script
        try:
            script_path = f"/etc/init.d/{self.name.lower()}"
            if os.path.exists(script_path):
                subprocess.run(['update-rc.d', f'{self.name.lower()}', 'remove'], 
                             capture_output=True)
                os.remove(script_path)
            print("✓ Startup script removed")
        except Exception as e:
            print(f"✗ Startup script removal failed: {e}")
        
        # 🧹 Clean up files
        for file_path in [self.config_file, self.pid_file]:
            if os.path.exists(file_path):
                os.remove(file_path)
        
        print(f"✅ {self.name} uninstallation completed!")

def main():
    """🚀 Main entry point"""
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
        print("📍 Interactive mode - starting service...")
        service.service_main_loop()

if __name__ == "__main__":
    main()
```

**🔑 Key Components:**
- 💾 **JSON Configuration** - Persistence state tracking
- 🔄 **systemd Integration** - Modern auto-restart
- 🚀 **Init.d Script** - Legacy startup compatibility
- 📋 **Logging** - Audit trail of service activity
- 🛑 **Signal Handlers** - Graceful shutdown
- ✅ **Health Checks** - Persistence verification

---

### 🔹 Subtask 2.2: Create Service Management Script

<span style="background-color: #673AB7; color: white; padding: 3px 8px; border-radius: 3px;">**MANAGEMENT CLI**</span>

Build the service control and monitoring utility:

```bash
# 📋 Create management script
nano service_manager.py
```

```python
#!/usr/bin/env python3
"""
🎛️ Service Manager - Control and Monitor Persistent Service
Provides CLI interface for service lifecycle management
"""
import subprocess
import sys
import time
import os

class ServiceManager:
    def __init__(self, service_name="mypersistentservice"):
        self.service_name = service_name
    
    def check_service_status(self):
        """📊 Check if service is running"""
        try:
            result = subprocess.run(['systemctl', 'is-active', f'{self.service_name}.service'], 
                                  capture_output=True, text=True)
            return result.stdout.strip() == 'active'
        except:
            return False
    
    def start_service(self):
        """▶️ Start the service"""
        try:
            subprocess.run(['systemctl', 'start', f'{self.service_name}.service'], check=True)
            return True
        except:
            return False
    
    def stop_service(self):
        """⏹️ Stop the service"""
        try:
            subprocess.run(['systemctl', 'stop', f'{self.service_name}.service'], check=True)
            return True
        except:
            return False
    
    def restart_service(self):
        """🔄 Restart the service"""
        try:
            subprocess.run(['systemctl', 'restart', f'{self.service_name}.service'], check=True)
            return True
        except:
            return False
    
    def get_service_logs(self, lines=20):
        """📖 Get service logs from journalctl"""
        try:
            result = subprocess.run(['journalctl', '-u', f'{self.service_name}.service', 
                                   '-n', str(lines), '--no-pager'], 
                                  capture_output=True, text=True)
            return result.stdout
        except:
            return "Failed to retrieve logs"
    
    def simulate_reboot_test(self):
        """🔄 Simulate reboot persistence test"""
        print("╔════════════════════════════════════════╗")
        print("║   Reboot Persistence Test              ║")
        print("╚════════════════════════════════════════╝")
        print()
        
        # 1️⃣ Check initial status
        print(f"1️⃣  Initial service status: {'✅ Running' if self.check_service_status() else '⛔ Stopped'}")
        
        # 2️⃣ Stop service
        print("2️⃣  Stopping service...")
        self.stop_service()
        time.sleep(2)
        print(f"    Status: {'✅ Running' if self.check_service_status() else '⛔ Stopped'}")
        
        # 3️⃣ Start service (simulating boot)
        print("3️⃣  Starting service (simulating boot)...")
        self.start_service()
        time.sleep(3)
        print(f"    Status: {'✅ Running' if self.check_service_status() else '⛔ Stopped'}")
        
        # 4️⃣ Check logs
        print("4️⃣  Recent service logs:")
        logs = self.get_service_logs(10)
        print("    " + "\n    ".join(logs.strip().split('\n')[-5:]))

def main():
    """🎛️ Main CLI handler"""
    manager = ServiceManager()
    
    if len(sys.argv) > 1:
        action = sys.argv[1]
        
        if action == 'status':
            status = "✅ Running" if manager.check_service_status() else "⛔ Stopped"
            print(f"Service status: {status}")
        
        elif action == 'start':
            if manager.start_service():
                print("✅ Service started successfully")
            else:
                print("❌ Failed to start service")
        
        elif action == 'stop':
            if manager.stop_service():
                print("✅ Service stopped successfully")
            else:
                print("❌ Failed to stop service")
        
        elif action == 'restart':
            if manager.restart_service():
                print("✅ Service restarted successfully")
            else:
                print("❌ Failed to restart service")
        
        elif action == 'logs':
            print(manager.get_service_logs())
        
        elif action == 'test-reboot':
            manager.simulate_reboot_test()
        
        else:
            print("❌ Unknown command")
    else:
        print("🎛️  Service Manager - Available commands:")
        print("    status      - Check service status")
        print("    start       - Start service")
        print("    stop        - Stop service")
        print("    restart     - Restart service")
        print("    logs        - Show service logs")
        print("    test-reboot - Test reboot persistence")

if __name__ == "__main__":
    main()
```

**🔧 Commands Available:**
- 📊 `status` - Check if service is running
- ▶️ `start` - Launch the service
- ⏹️ `stop` - Halt the service
- 🔄 `restart` - Restart the service
- 📖 `logs` - View service logs
- 🔄 `test-reboot` - Simulate boot cycle

---

### 🔹 Subtask 2.3: Compile and Test Code Structure

<span style="background-color: #F44336; color: white; padding: 3px 8px; border-radius: 3px;">**VERIFY**</span>

Validate Python syntax and structure:

```bash
# 🔍 Check Python syntax
python3 -m py_compile persistent_service.py
python3 -m py_compile service_manager.py

# 📋 List created files
ls -lah *.py

# ✅ Run syntax check
echo "✅ Syntax validation complete"

# ✍️ TODO: Review code for security considerations
# ✍️ TODO: Document all imported modules
```

**Verification Points:**
- ✅ No syntax errors
- ✅ All imports available
- ✅ Files are executable
- ✅ Proper permissions set

---

## 🔧 Task 3: Service Installation & Verification

### 🔹 Subtask 3.1: Install the Service

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**INSTALLATION**</span>

Make scripts executable and install service:

```bash
# 🔧 Make scripts executable
chmod +x persistent_service.py
chmod +x service_manager.py

# 📦 Install the service
sudo python3 persistent_service.py --install

# 📍 Watch for confirmation messages
# Expected: "✓ Systemd service installed"
# Expected: "✓ Startup script created"
```

**Installation Steps:**
1. 💾 Creates JSON configuration file
2. 🔧 Installs systemd unit file
3. 🚀 Creates startup script
4. 📋 Enables auto-start
5. ✅ Completes installation

---

### 🔹 Subtask 3.2: Verify Installation

<span style="background-color: #2196F3; color: white; padding: 3px 8px; border-radius: 3px;">**VALIDATION**</span>

Check service was installed correctly:

```bash
# 📊 Check service status
python3 service_manager.py status

# 🔍 View systemd service definition
sudo systemctl status mypersistentservice.service

# 📋 Verify service is enabled
sudo systemctl is-enabled mypersistentservice.service
# Output should be: enabled

# 🚀 Start the service
python3 service_manager.py start

# ⏱️ Wait for startup
sleep 2

# ✅ Verify running
python3 service_manager.py status
# Output should show: ✅ Running
```

**Expected Results:**
- ✅ Service status shows "enabled"
- ✅ Service starts without errors
- ✅ Service appears in `ps aux` output
- ✅ Systemd reports "active (running)"

---

### 🔹 Subtask 3.3: Monitor Service Activity

<span style="background-color: #FF9800; color: white; padding: 3px 8px; border-radius: 3px;">**MONITORING**</span>

Observe service behavior and logging:

```bash
# 📖 Check service logs (last 20 lines)
python3 service_manager.py logs

# 🔴 Real-time log monitoring
sudo journalctl -u mypersistentservice.service -f

# 💾 View persistent log file
cat /tmp/mypersistentservice.log | tail -20

# 📊 Monitor with watch command
watch -n 1 'systemctl status mypersistentservice.service'

# 🔍 Check process info
ps aux | grep persistent_service | grep -v grep
```

**Log Output Example:**
```
2024-07-05 12:30:45 - INFO - MyPersistentService service started
2024-07-05 12:30:55 - INFO - Service heartbeat #1 - 2024-07-05 12:30:55
2024-07-05 12:31:05 - INFO - Service heartbeat #2 - 2024-07-05 12:31:05
```

---

## 🧪 Task 4: Persistence Testing

### 🔹 Subtask 4.1: Simulate Reboot Test

<span style="background-color: #9C27B0; color: white; padding: 3px 8px; border-radius: 3px;">**REBOOT SIMULATION**</span>

Test service recovery on boot:

```bash
# 🔄 Run automated reboot simulation test
python3 service_manager.py test-reboot

# 📊 Expected output shows:
# 1️⃣  Initial service status: ✅ Running
# 2️⃣  Stopping service...
#     Status: ⛔ Stopped
# 3️⃣  Starting service (simulating boot)...
#     Status: ✅ Running
# 4️⃣  Recent service logs: [shows restart entry]
```

**Test Validation:**
- ✅ Service stops on command
- ✅ Service restarts successfully
- ✅ No errors during restart
- ✅ Service resumes immediately

---

### 🔹 Subtask 4.2: Manual Persistence Verification

<span style="background-color: #00BCD4; color: white; padding: 3px 8px; border-radius: 3px;">**MANUAL CHECKS**</span>

Verify persistence mechanisms manually:

```bash
# ✅ Check if service is enabled for auto-start
sudo systemctl is-enabled mypersistentservice.service
# Expected: enabled

# ✅ Verify startup script exists and is executable
ls -la /etc/init.d/mypersistentservice
# Expected: -rwxr-xr-x (755 permissions)

# 📄 View systemd configuration
sudo cat /etc/systemd/system/mypersistentservice.service

# 🔍 Check if service would start on boot
systemctl list-unit-files | grep mypersistentservice
```

**Verification Checklist:**
- ✅ `systemctl is-enabled` returns "enabled"
- ✅ Init.d script exists and is executable
- ✅ systemd service file is valid
- ✅ Service appears in unit-files list

---

### 🔹 Subtask 4.3: Test Service Recovery

<span style="background-color: #E91E63; color: white; padding: 3px 8px; border-radius: 3px;">**RECOVERY TEST**</span>

Verify automatic restart on failure:

```bash
# 1️⃣ Stop service and monitor auto-restart
echo "=== Manual Stop Test ==="
python3 service_manager.py stop
sleep 2

# 📊 Check status after stop
python3 service_manager.py status

# 🔄 Restart and verify
python3 service_manager.py start
sleep 3

# 2️⃣ Force kill process and check auto-restart
echo "=== Force Kill Test ==="
sudo pkill -f persistent_service.py

# ⏱️ Wait for systemd to auto-restart
echo "Waiting for auto-restart (systemd RestartSec=10)..."
sleep 15

# 📊 Check if service restarted automatically
python3 service_manager.py status

# 📖 View logs showing restart
python3 service_manager.py logs | grep -i "restart\|started"
```

**Expected Behavior:**
- ✅ Service stops on `stop` command
- ✅ Systemd auto-restarts within 10 seconds
- ✅ Process kill triggers auto-restart
- ✅ Logs show restart timestamp
- ✅ Service status returns to "Running"

---

### 🔹 Subtask 4.4: Verify Configuration Persistence

<span style="background-color: #1976D2; color: white; padding: 3px 8px; border-radius: 3px;">**CONFIG AUDIT**</span>

Confirm persistence files and settings:

```bash
# 📋 View configuration file
echo "=== Service Configuration ==="
cat /tmp/mypersistentservice_config.json

# 📊 Expected output:
# {
#   "service_name": "MyPersistentService",
#   "auto_start": true,
#   "restart_on_failure": true,
#   ...
# }

# 🔍 View service definition
echo -e "\n=== Systemd Service Unit ==="
sudo cat /etc/systemd/system/mypersistentservice.service

# 📄 Check startup script
echo -e "\n=== Init.d Startup Script ==="
cat /etc/init.d/mypersistentservice

# 📊 View log file
echo -e "\n=== Service Log File ==="
tail -20 /tmp/mypersistentservice.log
```

**Config Validation:**
- ✅ `auto_start` is true
- ✅ `restart_on_failure` is true
- ✅ Service paths are correct
- ✅ Timestamp shows creation date
- ✅ All config values are valid

---

## 🔍 Task 5: Advanced Verification

### 🔹 Subtask 5.1: Create Persistence Verification Script

<span style="background-color: #673AB7; color: white; padding: 3px 8px; border-radius: 3px;">**DIAGNOSTIC TOOL**</span>

Build comprehensive persistence verification utility:

```bash
# 📋 Create verification script
nano verify_persistence.py
```

```python
#!/usr/bin/env python3
"""
🔍 Persistence Verification Tool
Comprehensive check of all persistence mechanisms
"""
import subprocess
import json
import os
import time

def check_persistence_mechanisms():
    """✅ Comprehensive persistence check"""
    results = {}
    
    # 🔄 Check systemd service
    try:
        result = subprocess.run(['systemctl', 'is-enabled', 'mypersistentservice.service'], 
                              capture_output=True, text=True)
        results['systemd_enabled'] = result.returncode == 0
        results['systemd_status'] = result.stdout.strip()
    except:
        results['systemd_enabled'] = False
        results['systemd_status'] = 'error'
    
    # ✅ Check service is active
    try:
        result = subprocess.run(['systemctl', 'is-active', 'mypersistentservice.service'], 
                              capture_output=True, text=True)
        results['service_active'] = result.stdout.strip() == 'active'
    except:
        results['service_active'] = False
    
    # 💾 Check configuration file
    config_file = '/tmp/mypersistentservice_config.json'
    results['config_exists'] = os.path.exists(config_file)
    
    if results['config_exists']:
        try:
            with open(config_file, 'r') as f:
                config = json.load(f)
            results['config_valid'] = 'service_name' in config
        except:
            results['config_valid'] = False
    
    # 🚀 Check startup script
    startup_script = '/etc/init.d/mypersistentservice'
    results['startup_script_exists'] = os.path.exists(startup_script)
    
    # 📖 Check log file
    log_file = '/tmp/mypersistentservice.log'
    results['log_file_exists'] = os.path.exists(log_file)
    
    return results

def print_persistence_report(results):
    """📊 Print formatted persistence report"""
    print("╔════════════════════════════════════════════════════════════╗")
    print("║          Service Persistence Verification Report            ║")
    print("╚════════════════════════════════════════════════════════════╝")
    print()
    
    print(f"🔄 Systemd Service Enabled: {'✅ YES' if results['systemd_enabled'] else '❌ NO'}")
    print(f"🔌 Service Currently Active: {'✅ YES' if results['service_active'] else '❌ NO'}")
    print(f"💾 Configuration File: {'✅ EXISTS' if results['config_exists'] else '❌ MISSING'}")
    print(f"🚀 Startup Script: {'✅ EXISTS' if results['startup_script_exists'] else '❌ MISSING'}")
    print(f"📖 Log File: {'✅ EXISTS' if results['log_file_exists'] else '❌ MISSING'}")
    print()
    
    # 📈 Overall assessment
    critical_checks = [results['systemd_enabled'], results['service_active']]
    if all(critical_checks):
        print("🟢 Service persistence: EXCELLENT")
    elif any(critical_checks):
        print("🟡 Service persistence: PARTIAL")
    else:
        print("🔴 Service persistence: FAILED")

if __name__ == "__main__":
    results = check_persistence_mechanisms()
    print_persistence_report(results)
```

**Verification Checks:**
- ✅ Systemd enabled status
- ✅ Service running status
- ✅ Configuration file presence
- ✅ Startup script existence
- ✅ Log file presence
- 📊 Overall health assessment

---

### 🔹 Subtask 5.2: Run Comprehensive Verification

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**FULL AUDIT**</span>

Execute complete persistence verification:

```bash
# 🔧 Make verification script executable
chmod +x verify_persistence.py

# 🔍 Run comprehensive check
python3 verify_persistence.py

# 📊 Expected output shows all checks passing
# 🟢 Service persistence: EXCELLENT

# 📈 Detailed status check
echo "=== Detailed Service Status ==="
systemctl status mypersistentservice.service

# 🔍 Show all persistence artifacts
echo "=== Persistence Artifacts ==="
echo "Config file:"
ls -la /tmp/mypersistentservice_config.json
echo "Systemd unit:"
ls -la /etc/systemd/system/mypersistentservice.service
echo "Startup script:"
ls -la /etc/init.d/mypersistentservice
```

**Expected Results:**
- 🟢 All verification checks pass
- ✅ Systemd service shows "enabled"
- ✅ Service status shows "active (running)"
- ✅ All artifact files present
- 📊 Health assessment: EXCELLENT

---

### 🔹 Subtask 5.3: Cleanup (Optional)

<span style="background-color: #FF6B6B; color: white; padding: 3px 8px; border-radius: 3px;">**REMOVAL**</span>

Uninstall service when lab is complete:

```bash
# 🗑️ Uninstall the service
sudo python3 persistent_service.py --uninstall

# ✅ Run verification to confirm removal
python3 verify_persistence.py

# 📊 Expected to show all checks failing
# 🔴 Service persistence: FAILED

# 🧹 Verify service is stopped
systemctl status mypersistentservice.service 2>&1 | grep -i "inactive\|not-found"
```

**Uninstall Steps:**
1. 🛑 Stops systemd service
2. 📋 Disables auto-start
3. 🗑️ Removes service files
4. 🗑️ Removes startup scripts
5. 🧹 Cleans temporary files

---

## 🛠️ Troubleshooting

<details>
<summary><b>❌ "Permission denied" installing service</b></summary>

**Problem:** Cannot write to `/etc/systemd/system/` or `/etc/init.d/`

**Solutions:**

```bash
# 🔐 Run with sudo
sudo python3 persistent_service.py --install

# 🔍 Check permissions on directories
ls -ld /etc/systemd/system/
ls -ld /etc/init.d/

# ✅ Verify user has sudo rights
sudo -l

# 💡 If not in sudoers group:
sudo usermod -aG sudo $USER
# Log out and back in for changes to take effect
```

</details>

<details>
<summary><b>❌ Service fails to start with "python3: not found"</b></summary>

**Problem:** systemd cannot find python3 interpreter

**Solutions:**

```bash
# 🔍 Find python3 location
which python3
/usr/bin/python3 --version

# 🔧 Update service file with full path
# Edit /etc/systemd/system/mypersistentservice.service
# Change: ExecStart=/usr/bin/python3 ...

sudo systemctl daemon-reload
sudo systemctl restart mypersistentservice.service

# ✅ Verify with
systemctl status mypersistentservice.service
```

</details>

<details>
<summary><b>⚠️ Service won't restart automatically</b></summary>

**Problem:** Systemd RestartSec and Restart policies not working

**Solutions:**

```bash
# 🔍 Check systemd configuration
systemctl cat mypersistentservice.service | grep -i restart

# Expected to see:
# Restart=always
# RestartSec=10

# 🔧 If missing, edit the service file
sudo nano /etc/systemd/system/mypersistentservice.service

# Add these lines in [Service] section:
# Restart=always
# RestartSec=10

# 🔄 Reload and restart
sudo systemctl daemon-reload
sudo systemctl restart mypersistentservice.service

# ✅ Test restart
sudo systemctl stop mypersistentservice.service
sleep 12
systemctl status mypersistentservice.service
```

</details>

<details>
<summary><b>❌ Logs show "[FAILED] to restart service"</b></summary>

**Problem:** Service keeps failing and restarting indefinitely

**Solutions:**

```bash
# 📖 Check detailed error logs
journalctl -u mypersistentservice.service -n 50 -e

# 🔍 Look for specific error messages
journalctl -u mypersistentservice.service | grep -i error

# 🧪 Test service manually
python3 persistent_service.py --daemon

# 🔧 Check Python module imports
python3 -c "import psutil; import json; print('✅ Imports OK')"

# 💾 Verify config file is readable
cat /tmp/mypersistentservice_config.json

# 📝 Check file permissions
ls -la /tmp/ | grep mypersistentservice
```

</details>

<details>
<summary><b>⚠️ Service runs but doesn't appear in systemctl</b></summary>

**Problem:** Service running but systemctl shows "not found"

**Solutions:**

```bash
# 🔍 Check if systemd recognized the service
systemctl list-unit-files | grep mypersistentservice

# 🔄 Reload systemd daemon
sudo systemctl daemon-reload

# ✅ Verify service file exists
ls -la /etc/systemd/system/mypersistentservice.service

# 🔍 Check for syntax errors in service file
systemd-analyze verify /etc/systemd/system/mypersistentservice.service

# 🧪 Test service manually
sudo systemctl start mypersistentservice.service
systemctl status mypersistentservice.service
```

</details>

<details>
<summary><b>❌ Virtual environment not activated when service runs</b></summary>

**Problem:** Service can't find dependencies installed in venv

**Solutions:**

```bash
# 🔧 Option 1: Install dependencies globally
pip3 install pywin32 python-daemon psutil

# 🔧 Option 2: Update service to use venv python
# Edit /etc/systemd/system/mypersistentservice.service
# Change ExecStart to:
# ExecStart=/home/user/windows-service-lab/venv/bin/python3 ...

# 💾 Add full path in service file:
sudo nano /etc/systemd/system/mypersistentservice.service

# Find absolute path to venv python
echo $VIRTUAL_ENV/bin/python3

# 🔄 Reload and restart
sudo systemctl daemon-reload
sudo systemctl restart mypersistentservice.service
```

</details>

---

## 🎓 Key Takeaways

| Concept | Learning Point | Application | Linux Command |
|---------|-----------------|-------------|----------------|
| 🔄 **Service Persistence** | Service survives reboots | Auto-healing infrastructure | `systemctl enable` |
| 🚀 **Systemd Integration** | Modern service management | Device startup configuration | `systemctl daemon-reload` |
| 🔄 **Auto-Restart** | Process recovery on failure | Fault-tolerant systems | `Restart=always` |
| 💾 **Configuration Files** | Persistent state tracking | Application recovery | `cat /tmp/*.json` |
| 📋 **Logging** | Audit trail of service activity | Forensics and debugging | `journalctl -u` |
| 🛑 **Signal Handling** | Graceful shutdown | Clean process termination | `signal.signal()` |
| 📊 **Health Checks** | Verify persistence mechanisms | Monitoring and alerts | `systemctl status` |
| 🔧 **Init.d Legacy** | SysV-style startup scripts | Compatibility with older systems | `/etc/init.d/` |

---

## 🎯 MITRE ATT&CK Mapping

| Tactic | Technique | Procedure | Detection |
|--------|-----------|-----------|-----------|
| **Persistence** | [T1547.014](https://attack.mitre.org/techniques/T1547/014/) | Systemd/Upstart Services | Monitor new systemd units |
| **Persistence** | [T1547.004](https://attack.mitre.org/techniques/T1547/004/) | RC Scripts (Init.d) | Alert on /etc/init.d changes |
| **Execution** | [T1053.006](https://attack.mitre.org/techniques/T1053/006/) | Systemd Timers | Monitor timer unit creation |
| **Privilege Escalation** | [T1548](https://attack.mitre.org/techniques/T1548/) | Abuse Elevation Control | Track systemd service privileges |
| **Defense Evasion** | [T1562.008](https://attack.mitre.org/techniques/T1562/008/) | Disable or Modify Logs | Monitor journalctl access |

**Detection Methods:**

```bash
# 🔍 Monitor systemd service creation
sudo auditctl -w /etc/systemd/system/ -p wa

# 📊 Monitor init.d changes
sudo auditctl -w /etc/init.d/ -p wa

# 📖 Real-time journal monitoring
sudo journalctl -f | grep -i "started\|failed\|error"

# 🚨 Alert on service enable/disable
sudo auditctl -a always,exit -F dir=/etc/systemd/system/ -F perm=w
```

---

## 📚 Cross-Platform Comparison

| Feature | Linux systemd | Windows Service | Similarity |
|---------|---------------|-----------------|-----------|
| 🚀 Auto-Start | `WantedBy=multi-user.target` | `StartType=auto` | Both auto-launch on boot |
| 🔄 Auto-Restart | `Restart=always` | `ServiceErrorControl` | Both recover from crashes |
| 📋 Configuration | `.service` unit files | Registry/SCM | Both store settings |
| 📖 Logging | `journalctl` | Event Viewer | Both maintain audit logs |
| 🛑 Graceful Stop | `ExecStop=` | Service Control Handler | Both support clean shutdown |
| 🔐 Privileges | `User=root` | `LocalSystem` | Both support privilege specification |

---

## 🔗 Advanced Topics

### Makefile for Easy Management

```bash
cat > Makefile << 'MAKE'
.PHONY: install uninstall start stop restart status logs clean

install:
	chmod +x *.py
	sudo python3 persistent_service.py --install

uninstall:
	sudo python3 persistent_service.py --uninstall

start:
	python3 service_manager.py start

stop:
	python3 service_manager.py stop

restart:
	python3 service_manager.py restart

status:
	python3 service_manager.py status

logs:
	python3 service_manager.py logs

test-reboot:
	python3 service_manager.py test-reboot

verify:
	python3 verify_persistence.py

clean:
	rm -f /tmp/mypersistentservice*
	sudo systemctl daemon-reload

MAKE

# 📦 Use with:
make install
make status
make logs
make uninstall
```

---

<div align="center">

## 📚 Al Nafi Cybersecurity Training Platform

**Master service persistence and cross-platform automation**

![Progress](https://img.shields.io/badge/Progress-Lab%20Complete-brightgreen?style=flat-square)
![Difficulty](https://img.shields.io/badge/Difficulty-Advanced-red?style=flat-square)
![Time](https://img.shields.io/badge/Estimated%20Time-150%20minutes-blue?style=flat-square)

**Next Steps:** Explore Kubernetes service deployment, container persistence, and cloud-native service management

---

*Last Updated: July 2024 | Version: 1.0 | Certification: Al Nafi DevOps & Automation*

</div>

#!/bin/bash

echo "[+] Deploying Automated Reconnaissance Module"

# Create deployment directory
DEPLOY_DIR="/opt/recon_module"
sudo mkdir -p $DEPLOY_DIR

# Copy files
sudo cp *.py $DEPLOY_DIR/
sudo chmod +x $DEPLOY_DIR/*.py

# Create systemd service for persistence (optional)
cat > recon.service << 'SERVICE_EOF'
[Unit]
Description=Automated Reconnaissance Module
After=network.target

[Service]
Type=simple
User=nobody
WorkingDirectory=/opt/recon_module
ExecStart=/usr/bin/python3 /opt/recon_module/stealth_module.py
Restart=always
RestartSec=300

[Install]
WantedBy=multi-user.target
SERVICE_EOF

echo "[+] Deployment completed to $DEPLOY_DIR"
echo "[+] To install as service: sudo cp recon.service /etc/systemd/system/"
echo "[+] To start service: sudo systemctl enable recon && sudo systemctl start recon"

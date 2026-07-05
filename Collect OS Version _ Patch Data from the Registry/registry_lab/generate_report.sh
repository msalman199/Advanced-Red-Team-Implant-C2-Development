#!/bin/bash

echo "System Information Report" > system_report.txt
echo "=========================" >> system_report.txt
echo "Generated on: $(date)" >> system_report.txt
echo "" >> system_report.txt

echo "--- Linux System Information ---" >> system_report.txt
./linux_system_info >> system_report.txt

echo "" >> system_report.txt
echo "--- Windows Registry Simulation ---" >> system_report.txt
wine registry_reader.exe >> system_report.txt 2>/dev/null

echo "" >> system_report.txt
echo "--- Advanced Registry Scan ---" >> system_report.txt
wine advanced_registry.exe >> system_report.txt 2>/dev/null

echo "Report generated: system_report.txt"

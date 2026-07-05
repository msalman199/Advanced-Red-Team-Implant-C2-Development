#!/bin/bash

echo "PE Parser Lab Summary Report" > lab_report.txt
echo "===========================" >> lab_report.txt
echo "Date: $(date)" >> lab_report.txt
echo >> lab_report.txt

echo "Files Created:" >> lab_report.txt
find . -name "*.c" -o -name "*.h" | sort >> lab_report.txt
echo >> lab_report.txt

echo "Executables Built:" >> lab_report.txt
ls -la output/ >> lab_report.txt
echo >> lab_report.txt

echo "Test Results:" >> lab_report.txt
./output/pe_parser_enhanced samples/hello.exe >> lab_report.txt 2>&1

echo "Report saved to lab_report.txt"
cat lab_report.txt

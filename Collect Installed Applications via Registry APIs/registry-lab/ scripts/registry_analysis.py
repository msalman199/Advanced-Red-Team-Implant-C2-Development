#!/usr/bin/env python3
import json
import re
from collections import Counter
from datetime import datetime

class RegistryAnalyzer:
    def __init__(self, json_file):
        with open(json_file, 'r') as f:
            self.apps = json.load(f)
    
    def analyze_installation_patterns(self):
        """Analyze installation date patterns"""
        print("\nINSTALLATION PATTERN ANALYSIS")
        print("-" * 40)
        
        dates = [app['install_date'] for app in self.apps if app['install_date'] != 'Unknown']
        
        # Extract years and months
        years = []
        months = []
        
        for date_str in dates:
            if len(date_str) == 8:  # YYYYMMDD format
                years.append(date_str[:4])
                months.append(date_str[4:6])
        
        if years:
            year_counts = Counter(years)
            month_counts = Counter(months)
            
            print("Installation by Year:")
            for year, count in sorted(year_counts.items()):
                print(f"  {year}: {count} installations")
            
            print("\nInstallation by Month:")
            month_names = {
                '01': 'Jan', '02': 'Feb', '03': 'Mar', '04': 'Apr',
                '05': 'May', '06': 'Jun', '07': 'Jul', '08': 'Aug',
                '09': 'Sep', '10': 'Oct', '11': 'Nov', '12': 'Dec'
            }
            
            for month, count in sorted(month_counts.items()):
                month_name = month_names.get(month, month)
                print(f"  {month_name}: {count} installations")
    
    def analyze_publishers(self):
        """Analyze software publishers"""
        print("\nPUBLISHER ANALYSIS")
        print("-" * 40)
        
        publishers = [app['publisher'] for app in self.apps if app['publisher'] != 'Unknown']
        publisher_counts = Counter(publishers)
        
        print("Top Software Publishers:")
        for publisher, count in publisher_counts.most_common(10):
            print(f"  {publisher}: {count} applications")
    
    def analyze_install_locations(self):
        """Analyze installation locations"""
        print("\nINSTALLATION LOCATION ANALYSIS")
        print("-" * 40)
        
        locations = [app['install_location'] for app in self.apps if app['install_location'] != 'Unknown']
        
        # Extract drive letters
        drives = []
        program_files = 0
        
        for location in locations:
            if location:
                # Extract drive letter
                if len(location) > 0 and location[1] == ':':
                    drives.append(location[0].upper())
                
                # Check for Program Files
                if 'Program Files' in location:
                    program_files += 1
        
        if drives:
            drive_counts = Counter(drives)
            print("Installation by Drive:")
            for drive, count in sorted(drive_counts.items()):
                print(f"  {drive}:\\ - {count} applications")
        
        print(f"\nProgram Files installations: {program_files}")
    
    def detect_suspicious_patterns(self):
        """Detect potentially suspicious installation patterns"""
        print("\nSUSPICIOUS PATTERN DETECTION")
        print("-" * 40)
        
        suspicious = []
        
        for app in self.apps:
            # Check for unusual installation locations
            location = app['install_location'].lower()
            if any(path in location for path in ['temp', 'appdata', 'users']):
                suspicious.append(f"Unusual location: {app['name']} in {app['install_location']}")
            
            # Check for missing publisher
            if app['publisher'] == 'Unknown' and app['name'] != 'Unknown':
                suspicious.append(f"No publisher: {app['name']}")
            
            # Check for very recent installations (demo data check)
            if app['install_date'].startswith('2023'):
                suspicious.append(f"Recent install: {app['name']} on {app['install_date']}")
        
        if suspicious:
            print("Potential items of interest:")
            for item in suspicious[:10]:  # Show first 10
                print(f"  • {item}")
        else:
            print("No suspicious patterns detected.")

def main():
    import sys
    
    if len(sys.argv) < 2:
        print("Usage: python3 registry_analysis.py <json_file>")
        sys.exit(1)
    
    analyzer = RegistryAnalyzer(sys.argv[1])
    
    print("REGISTRY ANALYSIS REPORT")
    print("=" * 50)
    print(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    
    analyzer.analyze_installation_patterns()
    analyzer.analyze_publishers()
    analyzer.analyze_install_locations()
    analyzer.detect_suspicious_patterns()

if __name__ == "__main__":
    main()

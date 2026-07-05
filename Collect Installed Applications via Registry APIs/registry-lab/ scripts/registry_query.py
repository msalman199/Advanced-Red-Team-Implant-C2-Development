#!/usr/bin/env python3
import os
import json
from datetime import datetime

class RegistryParser:
    def __init__(self):
        self.software_keys = [
            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
            "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
        ]
        self.installed_apps = []
    
    def parse_registry_file(self, registry_file):
        """Parse Windows registry file for installed applications"""
        try:
            from Registry import Registry
            
            reg = Registry.Registry(registry_file)
            
            for key_path in self.software_keys:
                try:
                    uninstall_key = reg.open(key_path)
                    self.extract_software_info(uninstall_key)
                except Registry.RegistryKeyNotFoundException:
                    print(f"Key not found: {key_path}")
                    continue
                    
        except Exception as e:
            print(f"Error parsing registry: {e}")
    
    def extract_software_info(self, uninstall_key):
        """Extract software information from uninstall keys"""
        for subkey in uninstall_key.subkeys():
            app_info = {
                'name': 'Unknown',
                'version': 'Unknown',
                'publisher': 'Unknown',
                'install_date': 'Unknown',
                'install_location': 'Unknown',
                'registry_key': subkey.name()
            }
            
            try:
                # Extract common software properties
                for value in subkey.values():
                    if value.name() == "DisplayName":
                        app_info['name'] = value.value()
                    elif value.name() == "DisplayVersion":
                        app_info['version'] = value.value()
                    elif value.name() == "Publisher":
                        app_info['publisher'] = value.value()
                    elif value.name() == "InstallDate":
                        app_info['install_date'] = value.value()
                    elif value.name() == "InstallLocation":
                        app_info['install_location'] = value.value()
                
                # Only add if we have a display name
                if app_info['name'] != 'Unknown':
                    self.installed_apps.append(app_info)
                    
            except Exception as e:
                print(f"Error processing subkey {subkey.name()}: {e}")
    
    def save_results(self, output_file):
        """Save extracted software information to JSON file"""
        with open(output_file, 'w') as f:
            json.dump(self.installed_apps, f, indent=2)
        
        print(f"Found {len(self.installed_apps)} installed applications")
        print(f"Results saved to: {output_file}")

def main():
    parser = RegistryParser()
    
    # For demonstration, we'll create a sample registry structure
    print("Registry Query Tool for Installed Applications")
    print("=" * 50)
    
    registry_file = input("Enter path to Windows registry file (or press Enter for demo): ").strip()
    
    if not registry_file:
        # Create demo data
        demo_apps = [
            {
                'name': 'Microsoft Office 365',
                'version': '16.0.14326.20404',
                'publisher': 'Microsoft Corporation',
                'install_date': '20231015',
                'install_location': 'C:\\Program Files\\Microsoft Office',
                'registry_key': '{90160000-0011-0000-1000-0000000FF1CE}'
            },
            {
                'name': 'Google Chrome',
                'version': '118.0.5993.88',
                'publisher': 'Google LLC',
                'install_date': '20231020',
                'install_location': 'C:\\Program Files\\Google\\Chrome',
                'registry_key': '{8A69D345-D564-463C-AFF1-A69D9E530F96}'
            },
            {
                'name': 'Adobe Acrobat Reader DC',
                'version': '23.006.20380',
                'publisher': 'Adobe Inc.',
                'install_date': '20231018',
                'install_location': 'C:\\Program Files\\Adobe\\Acrobat DC',
                'registry_key': '{AC76BA86-7AD7-1033-7B44-AC0F074E4100}'
            }
        ]
        
        parser.installed_apps = demo_apps
        print("Using demo data for illustration...")
    else:
        if os.path.exists(registry_file):
            parser.parse_registry_file(registry_file)
        else:
            print(f"Registry file not found: {registry_file}")
            return
    
    # Save results
    output_file = f"output/installed_apps_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
    parser.save_results(output_file)

if __name__ == "__main__":
    main()

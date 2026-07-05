#!/usr/bin/env python3
import json
import sys
from datetime import datetime

class SoftwareReportGenerator:
    def __init__(self, json_file):
        self.json_file = json_file
        self.software_data = []
        self.load_data()
    
    def load_data(self):
        """Load software data from JSON file"""
        try:
            with open(self.json_file, 'r') as f:
                self.software_data = json.load(f)
        except Exception as e:
            print(f"Error loading data: {e}")
            sys.exit(1)
    
    def generate_summary_report(self):
        """Generate summary statistics"""
        print("\n" + "="*60)
        print("INSTALLED SOFTWARE SUMMARY REPORT")
        print("="*60)
        print(f"Total Applications Found: {len(self.software_data)}")
        print(f"Report Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print("="*60)
        
        # Publisher statistics
        publishers = {}
        for app in self.software_data:
            pub = app.get('publisher', 'Unknown')
            publishers[pub] = publishers.get(pub, 0) + 1
        
        print(f"\nTop Publishers:")
        for pub, count in sorted(publishers.items(), key=lambda x: x[1], reverse=True)[:5]:
            print(f"  {pub}: {count} applications")
    
    def generate_detailed_report(self):
        """Generate detailed application listing"""
        print(f"\n{'='*60}")
        print("DETAILED APPLICATION LISTING")
        print("="*60)
        
        for i, app in enumerate(self.software_data, 1):
            print(f"\n[{i}] {app['name']}")
            print(f"    Version: {app['version']}")
            print(f"    Publisher: {app['publisher']}")
            print(f"    Install Date: {app['install_date']}")
            print(f"    Install Location: {app['install_location']}")
            print(f"    Registry Key: {app['registry_key']}")
    
    def export_csv(self, csv_file):
        """Export data to CSV format"""
        import csv
        
        fieldnames = ['name', 'version', 'publisher', 'install_date', 'install_location', 'registry_key']
        
        with open(csv_file, 'w', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(self.software_data)
        
        print(f"\nData exported to CSV: {csv_file}")
    
    def search_software(self, search_term):
        """Search for specific software"""
        results = []
        search_term = search_term.lower()
        
        for app in self.software_data:
            if (search_term in app['name'].lower() or 
                search_term in app['publisher'].lower()):
                results.append(app)
        
        if results:
            print(f"\nSearch Results for '{search_term}':")
            print("-" * 40)
            for app in results:
                print(f"• {app['name']} v{app['version']} by {app['publisher']}")
        else:
            print(f"No applications found matching '{search_term}'")

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 format_output.py <json_file>")
        sys.exit(1)
    
    json_file = sys.argv[1]
    generator = SoftwareReportGenerator(json_file)
    
    while True:
        print(f"\n{'='*40}")
        print("SOFTWARE ANALYSIS MENU")
        print("="*40)
        print("1. Summary Report")
        print("2. Detailed Report")
        print("3. Export to CSV")
        print("4. Search Software")
        print("5. Exit")
        
        choice = input("\nSelect option (1-5): ").strip()
        
        if choice == '1':
            generator.generate_summary_report()
        elif choice == '2':
            generator.generate_detailed_report()
        elif choice == '3':
            csv_file = f"output/software_export_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
            generator.export_csv(csv_file)
        elif choice == '4':
            search_term = input("Enter search term: ").strip()
            generator.search_software(search_term)
        elif choice == '5':
            print("Exiting...")
            break
        else:
            print("Invalid option. Please try again.")

if __name__ == "__main__":
    main()

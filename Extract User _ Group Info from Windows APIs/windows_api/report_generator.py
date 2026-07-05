#!/usr/bin/env python3
import json
import os
from datetime import datetime

class ReportGenerator:
    def __init__(self, json_file="windows_enumeration.json"):
        self.json_file = json_file
        self.data = self.load_data()
    
    def load_data(self):
        """Load enumeration data from JSON file"""
        try:
            with open(self.json_file, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            print(f"Error: {self.json_file} not found. Run the main extractor first.")
            return None
    
    def generate_html_report(self):
        """Generate HTML report"""
        if not self.data:
            return
        
        html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <title>Windows API Enumeration Report</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; }}
        .header {{ background-color: #2c3e50; color: white; padding: 20px; }}
        .section {{ margin: 20px 0; }}
        table {{ border-collapse: collapse; width: 100%; }}
        th, td {{ border: 1px solid #ddd; padding: 8px; text-align: left; }}
        th {{ background-color: #f2f2f2; }}
        .risk-high {{ color: #e74c3c; }}
        .risk-medium {{ color: #f39c12; }}
        .risk-low {{ color: #27ae60; }}
    </style>
</head>
<body>
    <div class="header">
        <h1>Windows API Enumeration Report</h1>
        <p>Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
        <p>Target: {self.data.get('target', 'N/A')}</p>
    </div>
    
    <div class="section">
        <h2>Summary</h2>
        <p>Total Users: {self.data['summary']['total_users']}</p>
        <p>Total Groups: {self.data['summary']['total_groups']}</p>
    </div>
    
    <div class="section">
        <h2>Users</h2>
        <table>
            <tr><th>Username</th><th>RID</th><th>SID</th><th>Status</th></tr>
"""
        
        for user in self.data['users']:
            html_content += f"""
            <tr>
                <td>{user['username']}</td>
                <td>{user['rid']}</td>
                <td>{user['sid']}</td>
                <td>{user.get('status', 'Unknown')}</td>
            </tr>
"""
        
        html_content += """
        </table>
    </div>
    
    <div class="section">
        <h2>Groups</h2>
        <table>
            <tr><th>Group Name</th><th>RID</th><th>SID</th><th>Type</th></tr>
"""
        
        for group in self.data['groups']:
            html_content += f"""
            <tr>
                <td>{group['groupname']}</td>
                <td>{group['rid']}</td>
                <td>{group['sid']}</td>
                <td>{group.get('type', 'Unknown')}</td>
            </tr>
"""
        
        html_content += """
        </table>
    </div>
</body>
</html>
"""
        
        with open('enumeration_report.html', 'w') as f:
            f.write(html_content)
        
        print("HTML report generated: enumeration_report.html")
    
    def generate_csv_export(self):
        """Generate CSV exports"""
        if not self.data:
            return
        
        # Users CSV
        with open('users_export.csv', 'w') as f:
            f.write("Username,RID,SID,Status\n")
            for user in self.data['users']:
                f.write(f"{user['username']},{user['rid']},{user['sid']},{user.get('status', 'Unknown')}\n")
        
        # Groups CSV
        with open('groups_export.csv', 'w') as f:
            f.write("Group Name,RID,SID,Type\n")
            for group in self.data['groups']:
                f.write(f"{group['groupname']},{group['rid']},{group['sid']},{group.get('type', 'Unknown')}\n")
        
        print("CSV exports generated: users_export.csv, groups_export.csv")

if __name__ == "__main__":
    generator = ReportGenerator()
    generator.generate_html_report()
    generator.generate_csv_export()

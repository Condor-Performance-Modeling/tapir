import pandas as pd
import json

# Replace 'data.csv' with your actual file path
csv_file = 'bs_data.csv'
json_file = 'bs_data.json'

# Read the CSV into a pandas DataFrame
df = pd.read_csv(csv_file)

# Optional: ensure numeric fields are properly converted
df['Return'] = pd.to_numeric(df['Return'], errors='coerce')
df['Market Cap'] = pd.to_numeric(df['Market Cap'], errors='coerce')

# Convert DataFrame to list of dictionaries (JSON array)
json_data = df.to_dict(orient='records')

# Write JSON to file (compact format)
with open(json_file, 'w') as f:
    json.dump(json_data, f, separators=(',', ':'))

print(f"Converted {len(json_data)} records from CSV to JSON.")


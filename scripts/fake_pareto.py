import json
import random
def generate_data(n=5000):
    data = []
    for _ in range(n):
        ipc = round(random.uniform(0.1, 10.0), 3)

        # Area and power scale with IPC with some randomness
        area  = round(ipc * random.uniform(0.8, 1.2), 2)
        power = round(ipc * random.uniform(0.7, 1.5), 2)

        data.append({"ipc": ipc, "area": area, "power": power})
    return data

with open("fake_pareto_data.json", "w") as f:
    json.dump(generate_data(), f, indent=2)

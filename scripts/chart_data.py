import json
import random

def normalize_for_ternary(ipc_vals, area_vals, power_vals, spread=1.0):
    min_ipc, max_ipc = min(ipc_vals), max(ipc_vals)
    min_area, max_area = min(area_vals), max(area_vals)
    min_power, max_power = min(power_vals), max(power_vals)

    def scale(val, vmin, vmax):
        return (val - vmin) / (vmax - vmin + 1e-9)

    ternary_list = []
    for ipc, area, power in zip(ipc_vals, area_vals, power_vals):
        ipc_scaled   = scale(ipc, min_ipc, max_ipc)
        area_scaled  = scale(area, min_area, max_area)
        power_scaled = scale(power, min_power, max_power)

        # Apply spreading function: push values toward extremes
        ipc_spread   = ipc_scaled ** spread
        area_spread  = area_scaled ** spread
        power_spread = power_scaled ** spread

        total = ipc_spread + area_spread + power_spread + 1e-9
        ternary = {
            "ipc": round(ipc_spread / total, 4),
            "area": round(area_spread / total, 4),
            "power": round(power_spread / total, 4),
        }
        ternary_list.append(ternary)
    return ternary_list

def generate_graph(n=100, loop_ipc=False, loop_power=False,
                   loop_area=False, scale_factor=1.0):
    raw_nodes = []

    # Step 1: Generate raw ipc/area/power
    for i in range(n):
        ipc = round(random.uniform(0.1, 10.0), 3)
        area = round(ipc * random.uniform(0.8, 1.2), 2)
        power = round(ipc * random.uniform(0.7, 1.5), 2)
        raw_nodes.append({
            "id": f"N{i}",
            "ipc": ipc,
            "area": area,
            "power": power
        })

    # Step 2: Calculate min/max for scaling
    ipc_vals   = [n["ipc"] for n in raw_nodes]
    area_vals  = [n["area"] for n in raw_nodes]
    power_vals = [n["power"] for n in raw_nodes]

    ternary_coords = normalize_for_ternary(ipc_vals,   area_vals,
                                           power_vals, spread=1.7)

    min_area,  max_area  = min(area_vals),  max(area_vals)
    min_power, max_power = min(power_vals), max(power_vals)

    # Step 3: Normalize and compute score
    nodes = []
    for n in raw_nodes:
        area_scaled = (n["area"] - min_area) / (max_area - min_area) \
                       if max_area > min_area else 0

        power_scaled = (n["power"] - min_power) / (max_power - min_power) \
                       if max_power > min_power else 0

        derating = scale_factor * (area_scaled * power_scaled)
        raw_score = n["ipc"] - derating
        score = round(raw_score if raw_score > 0 else 0.0001, 4)

        n["score"] = score
        nodes.append(n)

    # Step 4: Create sorted links
    def create_ordered_links(key, reverse=False, loop=False, label=None):
        sorted_nodes = sorted(nodes, key=lambda x: x[key], reverse=reverse)
        for i in range(len(sorted_nodes) - 1):
            links.append({
                "source": sorted_nodes[i]["id"],
                "target": sorted_nodes[i + 1]["id"],
                "label": label
            })
        if loop:
            links.append({
                "source": sorted_nodes[-1]["id"],
                "target": sorted_nodes[0]["id"],
                "label": label
            })

    links = []
    create_ordered_links("ipc", reverse=True, loop=loop_ipc, label="ipc")
    create_ordered_links("power", reverse=False, loop=loop_power, label="power")
    create_ordered_links("area", reverse=False, loop=loop_area, label="area")

    min_ipc, max_ipc = min(ipc_vals), max(ipc_vals)
    for node in nodes:

        ipc_scaled = (node["ipc"] - min_ipc) / (max_ipc - min_ipc) \
                     if max_ipc > min_ipc else 0

        area_scaled = (node["area"] - min_area) / (max_area - min_area) \
                     if max_area > min_area else 0

        power_scaled = (node["power"] - min_power) / (max_power - min_power) \
                     if max_power > min_power else 0
    
        total = ipc_scaled + area_scaled + power_scaled + 1e-9

        node["ternary"] = {
            "ipc": round(ipc_scaled / total, 4),
            "area": round(area_scaled / total, 4),
            "power": round(power_scaled / total, 4)
        }

    return {"nodes": nodes, "links": links}

with open("chart_data.json", "w") as f:
    json.dump(generate_graph(100, loop_ipc=True,
                                  loop_power=True,
                                  loop_area=True,
                                  scale_factor=1.5), f, indent=2)


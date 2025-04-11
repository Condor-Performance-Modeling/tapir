import json
import random

def generate_graph(n=5000):
    nodes = []
    links = []

    for i in range(n):
        node_id = f"N{i}"
        ipc = round(random.uniform(0.1, 10.0), 3)
        area = round(ipc * random.uniform(0.8, 1.2), 2)
        power = round(ipc * random.uniform(0.7, 1.5), 2)
        score = round(ipc / (area * power + 1e-5), 4)

        nodes.append({
            "id": node_id,
            "ipc": ipc,
            "area": area,
            "power": power,
            "score": score
        })

    for _ in range(n * 2):
        src, tgt = random.sample(nodes, 2)
        links.append({"source": src["id"], "target": tgt["id"]})

    return {"nodes": nodes, "links": links}

with open("force_data.json", "w") as f:
    json.dump(generate_graph(5000), f, indent=2)


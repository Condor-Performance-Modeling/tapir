#! /usr/bin/env bash
python3 chart_data.py
cp chart_data.json ../dynamic/force/data.json
cp chart_data.json ../dynamic/scatter3d/data.json
cp chart_data.json ../dynamic/ternary/data.json
cp chart_data.json ../dynamic/bubble/data.json

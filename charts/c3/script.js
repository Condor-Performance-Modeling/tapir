const width = 960;
const height = 500;

const svg = d3.select("#map")
    .attr("width", width)
    .attr("height", height);

const orthoProjection = d3.geoOrthographic()
    .scale(250)
    .translate([width / 2, height / 2])
    .clipAngle(90);

const equirectProjection = d3.geoEquirectangular()
    .scale(150)
    .translate([width / 2, height / 2]);

const path = d3.geoPath().projection(orthoProjection);

const graticule = d3.geoGraticule();

svg.append("path")
    .datum(graticule)
    .attr("class", "graticule")
    .attr("d", path);

// ✅ Corrected block — use `const world = window.geojsonData;` and not `{ ... }`
const world = window.geojsonData; // <- assumes you've injected this via Qt

const countries = topojson.feature(world, world.objects.countries);

svg.selectAll(".land")
    .data(countries.features)
  .enter().append("path")
    .attr("class", "land")
    .attr("d", path);

svg.append("path")
    .datum(topojson.mesh(world, world.objects.countries, (a, b) => a !== b))
    .attr("class", "boundary")
    .attr("d", path);

// Toggle projection on click
let currentProjection = orthoProjection;

svg.on("click", () => {
  currentProjection = (currentProjection === orthoProjection)
    ? equirectProjection
    : orthoProjection;

  path.projection(currentProjection);

  svg.selectAll("path")
      .transition()
      .duration(1000)
      .attr("d", path);
});


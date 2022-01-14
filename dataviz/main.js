let gcc = [
	{ time: 1.389, cores: 2 },
	{ time: 1.15, cores: 3 },
	{ time: 1.162, cores: 4 },
	{ time: 0.973, cores: 5 },
	{ time: 0.832, cores: 6 },
	{ time: 0.783, cores: 7 },
	{ time: 0.757, cores: 8 },
	{ time: 0.709, cores: 9 },
	{ time: 0.647, cores: 10 },
	{ time: 0.665, cores: 11 },
	{ time: 0.606, cores: 12 },
	{ time: 0.583, cores: 13 },
	{ time: 0.576, cores: 14 },
	{ time: 0.563, cores: 15 },
	{ time: 0.554, cores: 16 },
];

let llvm = [
	{ time: 1.181, cores: 2 },
	{ time: 0.888, cores: 3 },
	{ time: 0.843, cores: 4 },
	{ time: 0.86, cores: 5 },
	{ time: 0.747, cores: 6 },
	{ time: 0.643, cores: 7 },
	{ time: 0.681, cores: 8 },
	{ time: 0.671, cores: 9 },
	{ time: 0.617, cores: 10 },
	{ time: 0.547, cores: 11 },
	{ time: 0.58, cores: 12 },
	{ time: 0.577, cores: 13 },
	{ time: 0.559, cores: 14 },
	{ time: 0.547, cores: 15 },
	{ time: 0.542, cores: 16 },
];

const primaryColor = 'blueviolet';
const secondaryColor = 'steelblue';
const data = llvm;
const data2 = gcc;

// set the dimensions and margins of the graph
const margin = { top: 30, right: 50, bottom: 30, left: 60 },
	width = 1000 - margin.left - margin.right,
	height = 700 - margin.top - margin.bottom;

// append the svg object to the body of the page
const svg = d3
	.select('#chart')
	.append('svg')
	.attr('width', width + margin.left + margin.right)
	.attr('height', height + margin.top + margin.bottom)
	.append('g')
	.attr('transform', `translate(${margin.left},${margin.top})`);

const combinedData = [...data, ...data2];

// X axis
const x = d3
	.scalePow()
	.exponent(-2.5)
	.domain(d3.extent(combinedData.map((a) => a.time)))
	.range([0, width]);
svg.append('g')
	.call(
		d3
			.axisBottom(x)
			.tickValues(combinedData.map((a) => a.time))
			.tickFormat((d) => d.toFixed(3))
	)
	.attr('transform', `translate(0, ${height})`);

// Y axis
const y = d3
	.scaleLinear()
	.domain(d3.extent(data.map((d) => d.cores)))
	.range([0, height]);

svg.append('g').call(d3.axisLeft(y));

function drawLineWithLabels(data, color) {
	// line
	svg.append('path')
		.datum(data)
		.attr('fill', 'none')
		.attr('stroke', color)
		.attr('stroke-width', 1.5)
		.attr(
			'd',
			d3
				.line()
				.curve(d3['curveCardinal'])
				.x((d) => x(d.time))
				.y((d) => y(d.cores))
		);

	// dots
	svg.selectAll('myCircles')
		.data(data)
		.enter()
		.append('circle')
		.attr('fill', color)
		.attr('stroke', 'none')
		.attr('cx', function (d) {
			return x(d.time);
		})
		.attr('cy', function (d) {
			return y(d.cores);
		})
		.attr('r', 3);

	// dot labels

	// group for positioning
	let label = svg
		.selectAll('myLabel')
		.data(data)
		.enter()
		.append('g')
		.attr(
			'transform',
			(d) => `translate(${x(d.time) - 15}, ${y(d.cores) - 8})`
		);
	// label text
	label
		.append('text')
		.text((a) => a.time.toFixed(3))
		.attr('fill', 'white')
		.attr('stroke', 'none')
		.classed('myText', true);

	// label bg
	label.selectAll('.myText').each(function () {
		d3.select(this.parentNode)
			.insert('rect', ':first-child')
			.attr('fill', color)
			.attr('width', this.getComputedTextLength())
			.attr('height', 15)
			.attr('x', 0)
			.attr('y', 0)
			.attr('transform', 'translate(-3, -12)')
			.attr('ry', 2);
	});
}

drawLineWithLabels(data, primaryColor);
drawLineWithLabels(data2, secondaryColor);

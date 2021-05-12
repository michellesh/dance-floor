import * as d3 from 'd3';

import { distance, getCanvasDimensions, random } from 'utils';
import { showLEDs, showStrands } from 'animations';

export const rippleEffect = (context, strands) => {
  const { width, height } = getCanvasDimensions(context);
  const RIPPLE_WIDTH = 50;
  const DAMPENING = 0.99;

  let current = d3
    .range(0, strands.length)
    .map(() => d3.range(0, strands[0].length).map(() => 0));
  let previous = d3
    .range(0, strands.length)
    .map(() => d3.range(0, strands[0].length).map(() => 0));

  const x = random(1, strands.length - 1);
  const y = random(1, strands[0].length - 1);
  current[x][y] = 5;

  const _rippleEffect = () => {
    for (let i = 1; i < strands.length - 1; i++) {
      for (let j = 1; j < strands[0].length - 1; j++) {
        current[i][j] =
          (previous[i - 1][j] +
            previous[i + 1][j] +
            previous[i][j - 1] +
            previous[i][j + 1]) /
            2 -
          current[i][j];
        current[i][j] = current[i][j] * DAMPENING;
        const radius = current[i][j] < 0 ? 0 : current[i][j];
        strands[i][j] = strands[i][j].radius(1 + radius * 10);
      }
    }
    showStrands(context, strands);
    const temp = previous;
    previous = current;
    current = temp;
    window.requestAnimationFrame(_rippleEffect);
  };

  window.requestAnimationFrame(_rippleEffect);
};

export const ripple = (context, strands) => {
  const { width, height } = getCanvasDimensions(context);
  const X = width / 2;
  const Y = height / 2;
  const RIPPLE_WIDTH = 50;
  const LED_COLOR = 'white';
  const RIPPLE_COLOR = 'gold';

  const increaseRadius = d3
    .scaleLinear()
    .domain([0, RIPPLE_WIDTH])
    .range([1, 5]);
  const decreaseRadius = d3
    .scaleLinear()
    .domain([0, RIPPLE_WIDTH])
    .range([5, 1]);
  const increaseColor = d3
    .scaleLinear()
    .domain([0, RIPPLE_WIDTH])
    .range([LED_COLOR, RIPPLE_COLOR]);
  const decreaseColor = d3
    .scaleLinear()
    .domain([0, RIPPLE_WIDTH])
    .range([RIPPLE_COLOR, LED_COLOR]);

  const inRippleRange = (led, radius) => {
    const d = distance(led, { x: X, y: Y });
    return d < radius && d > radius - RIPPLE_WIDTH;
  };

  const prominence = (led, radius) => radius - distance(led, { x: X, y: Y });

  const getRadius = prominence =>
    prominence - RIPPLE_WIDTH / 2 < 0
      ? increaseRadius(prominence)
      : decreaseRadius(prominence);

  const getColor = prominence =>
    prominence - RIPPLE_WIDTH / 2 < 0
      ? increaseColor(prominence)
      : decreaseColor(prominence);

  let leds = strands.flatMap(strand => strand);
  let radius = 0;
  const _ripple = () => {
    leds = leds.map(led =>
      inRippleRange(led, radius)
        ? led
            .color(getColor(prominence(led, radius)))
            .radius(getRadius(prominence(led, radius)))
        : led.color(LED_COLOR).radius(1)
    );
    showLEDs(context, leds);
    radius = radius >= width * 0.75 ? 0 : radius + 5;
    window.requestAnimationFrame(_ripple);
  };

  window.requestAnimationFrame(_ripple);
};

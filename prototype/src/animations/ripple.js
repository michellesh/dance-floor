import * as d3 from 'd3';

import { distance, getCanvasDimensions } from 'utils';
import { showLEDs } from 'animations';

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

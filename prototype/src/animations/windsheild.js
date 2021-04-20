import * as d3 from 'd3';

import * as u from 'utils';
import { getCenterCoords, showStrands } from 'animations';

export const windshield = (context, strands) => {
  const centerCoords = getCenterCoords(context, strands);
  const { width, height } = u.getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);

  const START_ANGLE = 225;
  const END_ANGLE = 135;
  const ANGLE_THRESHOLD = 10;
  const incAngle = angle =>
    angle === 360 ? 0 : angle === END_ANGLE ? START_ANGLE : angle + 1;
  const angleOf = p => {
    const a = u.degrees(u.angleBetween(centerCoords, p)) + 90;
    return a < 0 ? a + 360 : a > 360 ? a - 360 : a;
  };
  const angleToRadius = d3
    .scaleLinear()
    .domain([-ANGLE_THRESHOLD, 0, ANGLE_THRESHOLD])
    .range([1, 5, 1])
    .clamp(true);
  const strandAngles = strands.reduce((acc, strand, strandIndex) => {
    const angles = strand.map(led => angleOf(led));
    const max = Math.max(...angles);
    const min = Math.min(...angles);
    acc[strandIndex] = min + (max - min) / 2;
    return acc;
  }, {});
  const isBetween = (v, min, max) => v >= min && v <= max;
  const getRelevantStrands = angle =>
    Object.keys(strandAngles).filter(strandIndex =>
      isBetween(
        strandAngles[strandIndex],
        angle - ANGLE_THRESHOLD * 2,
        angle + ANGLE_THRESHOLD * 2
      )
    );

  let angleDegrees = START_ANGLE;

  const _windshield = () => {
    getRelevantStrands(angleDegrees).forEach(strandIndex => {
      strands[strandIndex] = strands[strandIndex].map(led =>
        led.radius(angleToRadius(Math.abs(angleDegrees - angleOf(led))))
      );
    });

    showStrands(context, strands);

    /*
    // For debugging. Draws the current angle.
    u.drawLine(
      context,
      centerCoords,
      u.pointOnCircumference(width * 0.75, centerCoords, angleDegrees)
    );
    */

    angleDegrees = incAngle(angleDegrees);
    window.requestAnimationFrame(_windshield);
  };

  window.requestAnimationFrame(_windshield);
};

// An alternative way to get the "windshield wiper" effect.
// Not nearly as cool. Might be useful for testing strands
export const windshield2 = (context, strands) => {
  const { width, height } = u.getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);

  const DIST_THRESHOLD = 20;
  const distToRadius = d3
    .scaleLinear()
    .domain([-DIST_THRESHOLD, 0, DIST_THRESHOLD])
    .range([1, 5, 1]);
  const distRange = d3.range(-DIST_THRESHOLD, DIST_THRESHOLD);
  const incIndex = (i, arr) => (i === arr.length - 1 ? 0 : i + 1);

  let distIndex = 0;
  let strandIndex = 0;

  const _windshield = () => {
    strands[strandIndex] = strands[strandIndex].map(led =>
      led.radius(distToRadius(distRange[distIndex]))
    );

    showStrands(context, strands);

    strandIndex =
      distIndex === distRange.length - 1
        ? incIndex(strandIndex, strands)
        : strandIndex;
    distIndex = incIndex(distIndex, distRange);
    window.requestAnimationFrame(_windshield);
  };

  window.requestAnimationFrame(_windshield);
};

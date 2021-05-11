import React, { useEffect } from 'react';
import styled from 'styled-components';
import * as d3 from 'd3';
import { isEqual, reverse } from 'lodash';

import * as animations from 'animations';
import { useCanvas } from 'hooks';
import {
  angleBetween,
  distance,
  drawDot,
  drawLine,
  pointOnEllipse,
  pointOnLine,
  radians
} from 'utils';

window.d3 = d3;

const WIDTH = 1100;
const HEIGHT = 900;
const RADIUS_X = 100;
const SAIL_COLOR = '#455B68';
const BACKGROUND_COLOR = 'black';
const LED_COLOR = 'white';
const LED_DENSITY = 165;
const NUM_STRANDS = 7;
const NUM_LEDS = 150;

const Canvas = styled.canvas`
  background-color: ${BACKGROUND_COLOR};
  //background-image: url(${process.env.PUBLIC_URL}/img/sail_layout.png);
  //background-size: ${WIDTH}px ${HEIGHT}px;
`;
const Container = styled.div`
  background-color: ${BACKGROUND_COLOR};
  height: 100%;
  text-align: center;
`;

const Ellipse = (
  p1,
  p2,
  rx = RADIUS_X,
  startAngle = radians(0),
  endAngle = radians(360)
) => {
  const _ellipse = {
    x: pointOnLine(p1, p2, 0.5).x,
    y: pointOnLine(p1, p2, 0.5).y,
    rx,
    ry: distance(p1, p2) / 2,
    rotation: angleBetween(p1, p2),
    startAngle,
    endAngle
  };
  return Object.freeze({
    ..._ellipse,
    draw: (context, strokeColor = 'black', fillColor) => {
      const { x, y, rx, ry, rotation, startAngle, endAngle } = _ellipse;
      context.beginPath();
      context.ellipse(x, y, rx, ry, rotation, startAngle, endAngle);
      context.strokeStyle = strokeColor;
      context.stroke();
      if (fillColor) {
        context.fillStyle = fillColor;
        context.fill();
      }
    },
    pad: () => Ellipse(p1, p2, rx - 1, startAngle, endAngle)
  });
};

const LED = config => {
  const defaults = {
    color: LED_COLOR,
    endAngle: 2 * Math.PI,
    radius: 1,
    startAngle: 0
  };
  const _led = { ...defaults, ...config };
  return Object.freeze({
    ..._led,
    color: color => LED({ ..._led, color }),
    r: _led.radius,
    radius: radius => LED({ ..._led, radius }),
    resetColor: () => LED({ ..._led, color: defaults.color }),
    resetRadius: () => LED({ ..._led, radius: defaults.radius }),
    draw: context => {
      const { x, y, color, radius, startAngle, endAngle } = _led;
      context.beginPath();
      context.arc(x, y, radius, startAngle, endAngle);
      context.fillStyle = color;
      context.fill();
    }
  });
};

const Sail = config => {
  const {
    p1,
    p2,
    p3,
    rx,
    numStrands,
    rightHemisphere,
    leftHemisphere
  } = config;
  const ellipses = d3.range(0, numStrands).map(i => {
    const incAmount = rx / (numStrands - 1);
    const pInc = pointOnLine(p3, p1, i / (numStrands - 1));
    const newRx = rx - i * incAmount * 2;
    const [startAngle, endAngle] =
      newRx >= 0 ? rightHemisphere : leftHemisphere;
    return Ellipse(pInc, p2, Math.abs(newRx), startAngle, endAngle);
  });
  const _sail = {
    ...config,
    e1: Ellipse(p1, p2, rx, radians(240), radians(360)),
    e2: Ellipse(p2, p3, rx, radians(180), radians(300)),
    e3: Ellipse(p3, p1, rx, radians(180), radians(360)),
    ellipses
  };
  return Object.freeze({
    ..._sail,
    setNumStrands: numStrands => Sail({ ..._sail, numStrands }),
    getLEDs: () =>
      ellipses
        .map(e => {
          const angleStep = (e.endAngle - e.startAngle) / LED_DENSITY;
          const leds = d3
            .range(e.startAngle, e.endAngle, angleStep)
            .map(angle => LED(pointOnEllipse(e, angle)));
          return isEqual([e.startAngle, e.endAngle], rightHemisphere)
            ? reverse(leds)
            : leds;
        })
        .filter(ledStrand => ledStrand.length > 0),
    draw: context => {
      context.beginPath();
      context.moveTo(p1.x, p1.y);
      context.lineTo(WIDTH / 2, WIDTH / 2);
      context.lineTo(p3.x, p3.y);
      context.fillStyle = SAIL_COLOR;
      context.fill();
      [_sail.e1, _sail.e2, _sail.e3].forEach(e => {
        e.draw(context, BACKGROUND_COLOR, BACKGROUND_COLOR);
      });
    }
  });
};

const SAILS = [
  {
    p1: { x: -60, y: 300 },
    p2: { x: 880, y: 550 },
    p3: { x: -60, y: 800 },
    rx: RADIUS_X,
    leftHemisphere: [radians(260), radians(320)],
    rightHemisphere: [radians(40), radians(100)]
  },
  {
    p1: { x: 300, y: -100 },
    p2: { x: 750, y: 750 },
    p3: { x: -100, y: 300 },
    rx: RADIUS_X,
    leftHemisphere: [radians(255), radians(320)],
    rightHemisphere: [radians(40), radians(105)]
  },
  {
    p1: { x: WIDTH + 100, y: 300 },
    p2: { x: WIDTH - 750, y: 750 },
    p3: { x: WIDTH - 300, y: -100 },
    rx: RADIUS_X,
    leftHemisphere: [radians(255), radians(320)],
    rightHemisphere: [radians(40), radians(105)]
  },
  {
    p1: { x: WIDTH + 60, y: 800 },
    p2: { x: WIDTH - 880, y: 550 },
    p3: { x: WIDTH + 60, y: 300 },
    rx: RADIUS_X,
    leftHemisphere: [radians(260), radians(320)],
    rightHemisphere: [radians(40), radians(100)]
  }
].map(s => Sail(s).setNumStrands(NUM_STRANDS));

const Triangle = config => {
  const { p1, p2, p3, color } = config;
  const lines = d3
    .range(0, NUM_STRANDS)
    .map(i => ({ p1: p2, p2: pointOnLine(p3, p1, i / (NUM_STRANDS - 1)) }));
  const _triangle = { ...config, lines };
  return Object.freeze({
    ..._triangle,
    getLEDs: () =>
      d3.range(0, NUM_STRANDS).map(i => {
        const pInc = pointOnLine(p3, p1, i / (NUM_STRANDS - 1));
        return d3
          .range(0, NUM_LEDS)
          .map(j => LED(pointOnLine(p2, pInc, j / (NUM_LEDS - 1))));
      }),
    draw: context => {
      drawDot(context, p1, 2, 'red');
      drawDot(context, p2, 2, 'green');
      drawDot(context, p3, 2, 'yellow');
      lines.forEach(line => drawLine(context, line.p1, line.p2, color));
    }
  });
};

const TRIANGLES = [
  {
    p1: { x: 80, y: 500 },
    p2: { x: 400, y: 795 },
    p3: { x: 50, y: 850 },
    color: 'red'
  },
  {
    p1: { x: 410, y: 100 },
    p2: { x: 470, y: 650 },
    p3: { x: 90, y: 325 },
    color: 'orange'
  },
  {
    p1: { x: 1000, y: 325 },
    p2: { x: 630, y: 650 },
    p3: { x: 690, y: 100 },
    color: 'green'
  },
  {
    p1: { x: 1050, y: 850 },
    p2: { x: 700, y: 795 },
    p3: { x: 1020, y: 500 },
    color: 'blue'
  }
].map(t => Triangle(t));

//const SAIL_STRANDS = SAILS.map(sail => sail.getLEDs().map(leds => leds));
const SAIL_STRANDS = TRIANGLES.map(tri => tri.getLEDs().map(leds => leds));
const STRANDS = SAIL_STRANDS.flatMap(sailStrand => sailStrand);

const Prototype = () => {
  const [canvasRef, context] = useCanvas();

  useEffect(() => {
    if (context) {
      // This colors in the background of the sail. Ignore for now
      //SAILS.forEach(sail => sail.draw(context));
      TRIANGLES.forEach(tri => tri.draw(context));

      // Draw LEDs only
      STRANDS.forEach(strand => strand.forEach(led => led.draw(context)));

      // Animation
      //animations.windshield(context, STRANDS);

      // No animation
      //animations.showStrands(context, STRANDS);
    }
  }, [context]);

  return (
    <Container>
      <Canvas ref={canvasRef} width={WIDTH} height={HEIGHT} />
    </Container>
  );
};

export default Prototype;

import { getCanvasDimensions } from 'utils';

export const getCenterCoords = (context, strands) => {
  const { width } = getCanvasDimensions(context);
  return {
    x: width / 2,
    y: strands[0][0].y
  };
};

export const showLEDs = (context, leds) => {
  const { width, height } = getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);
  leds.forEach(led => led.draw(context));
};

export const showStrands = (context, strands) => {
  const { width, height } = getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);
  showLEDs(
    context,
    strands.flatMap(strand => strand)
  );
};

export * from './fan';
export * from './ripple';
export * from './windsheild';

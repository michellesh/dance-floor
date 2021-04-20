import { getCanvasDimensions } from 'utils';
import { showStrands } from 'animations';

export const fan = (context, strands) => {
  const { width, height } = getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);

  const maxStrandLength = Math.max(...strands.map(strand => strand.length));
  let ledIndex = 0;

  const _fan = () => {
    strands.forEach(strand => {
      if (ledIndex < strand.length) {
        strand[ledIndex] = strand[ledIndex].radius(3);
      }
      if (ledIndex > 0 && ledIndex - 1 < strand.length) {
        strand[ledIndex - 1] = strand[ledIndex - 1].radius(1);
      }
    });
    showStrands(context, strands);
    ledIndex = ledIndex === maxStrandLength ? 0 : ledIndex + 1;
    window.requestAnimationFrame(_fan);
  };

  window.requestAnimationFrame(_fan);
};

export const fanReverse = (context, strands) => {
  const { width, height } = getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);

  const maxStrandLength = Math.max(...strands.map(strand => strand.length));
  let ledIndex = maxStrandLength;

  const _fanReverse = () => {
    strands.forEach(strand => {
      if (ledIndex >= 0 && ledIndex < strand.length) {
        strand[ledIndex] = strand[ledIndex].radius(3);
      }
      if (ledIndex + 1 < strand.length) {
        strand[ledIndex + 1] = strand[ledIndex + 1].radius(1);
      }
    });
    showStrands(context, strands);
    ledIndex = ledIndex === -1 ? maxStrandLength : ledIndex - 1;
    window.requestAnimationFrame(_fanReverse);
  };

  window.requestAnimationFrame(_fanReverse);
};

export const fanAlternate = (context, strands) => {
  const { width, height } = getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);

  const maxStrandLength = Math.max(...strands.map(strand => strand.length));
  let ledIndexForward = 0;
  let ledIndexBackward = maxStrandLength;

  const _fanAlternate = () => {
    strands.forEach((strand, strandIndex) => {
      if (strandIndex % 2 === 0) {
        if (ledIndexForward < strand.length) {
          strand[ledIndexForward] = strand[ledIndexForward].radius(3);
        }
        if (ledIndexForward > 0 && ledIndexForward - 1 < strand.length) {
          strand[ledIndexForward - 1] = strand[ledIndexForward - 1].radius(1);
        }
      } else {
        if (ledIndexBackward >= 0 && ledIndexBackward < strand.length) {
          strand[ledIndexBackward] = strand[ledIndexBackward].radius(3);
        }
        if (ledIndexBackward + 1 < strand.length) {
          strand[ledIndexBackward + 1] = strand[ledIndexBackward + 1].radius(1);
        }
      }
    });
    showStrands(context, strands);
    ledIndexForward =
      ledIndexForward === maxStrandLength ? 0 : ledIndexForward + 1;
    ledIndexBackward =
      ledIndexBackward === -1 ? maxStrandLength : ledIndexBackward - 1;
    window.requestAnimationFrame(_fanAlternate);
  };

  window.requestAnimationFrame(_fanAlternate);
};

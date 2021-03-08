import { getCanvasDimensions } from 'utils';
import { showStrands } from 'animations';

export const fan = (context, strands) => {
  const { width, height } = getCanvasDimensions(context);
  context.clearRect(0, 0, width, height);

  const maxStrandLength = Math.max(...strands.map(strand => strand.length));
  let currentStrand = 0;

  const _fan = () => {
    strands.forEach(strand => {
      if (currentStrand < strand.length) {
        strand[currentStrand] = strand[currentStrand].radius(3);
      }
      if (currentStrand > 0 && currentStrand - 1 < strand.length) {
        strand[currentStrand - 1] = strand[currentStrand - 1].radius(1);
      }
    });
    showStrands(context, strands);
    currentStrand = currentStrand === maxStrandLength ? 0 : currentStrand + 1;
    window.requestAnimationFrame(_fan);
  };

  window.requestAnimationFrame(_fan);
};

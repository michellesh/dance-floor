export const drawDot = (context, p, radius = 2) => {
  context.beginPath();
  context.arc(p.x, p.y, radius, 0, 2 * Math.PI);
  context.fillStyle = 'white';
  context.fill();
};

export const drawLine = (context, p1, p2) => {
  context.beginPath();
  context.moveTo(p1.x, p1.y);
  context.lineTo(p2.x, p2.y);
  context.strokeStyle = 'white';
  context.stroke();
};

export const getCanvasDimensions = context => {
  if (context) {
    const { width, height } = context.canvas.getBoundingClientRect();
    return { width, height };
  }
};

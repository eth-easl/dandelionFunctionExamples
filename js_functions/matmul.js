async function matmul(inMat, rows, cols) {
  const outMat = new Array(rows * rows).fill(0); // Initialize the output matrix

  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < rows; j++) {
      for (let k = 0; k < cols; k++) {
        outMat[i * rows + j] += inMat[i * cols + k] * inMat[j * cols + k];
      }
    }
  }
  return outMat;
}

export default {
  async fetch(request) {
    try {
      const { inMat, rows, cols } = await request.json();

      const result = await matmul(inMat, rows, cols);

      return new Response(JSON.stringify({ outMat: result }), {
        headers: { 'Content-Type': 'application/json' },
      });
    } catch (error) {
      return new Response(JSON.stringify({ error: error.message }), {
        status: 400,
        headers: { 'Content-Type': 'application/json' },
      });
    }
  },
};


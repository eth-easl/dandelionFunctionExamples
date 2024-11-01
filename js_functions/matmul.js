export default {
  async fetch(request) {
    return await handleRequest(request);
  }
};

async function handleRequest(request) {
  if (request.method !== "GET") {
    return new Response("Method not allowed", { status: 405 });
  }

  const body = await request.json();
  const matrixData = body.sets[0].items[0].data;

  const resultData = matmulService(matrixData);

  const response = {
    sets: [
      {
        identifier: "outmatrix",
        items: [
          {
            identifier: "",
            key: 0,
            data: resultData
          }
        ]
      }
    ]
  };

  return new Response(JSON.stringify(response), {
    headers: { "Content-Type": "application/json" },
  });
}

function matmulService(data) {
  const rows = data[0];
  const cols = (data.length - 1) / rows;
  const inMat = data.slice(1);

  const outMat = new Array(rows * rows).fill(0);

  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < rows; j++) {
      for (let k = 0; k < cols; k++) {
        outMat[i * rows + j] += inMat[i * cols + k] * inMat[j * cols + k];
      }
    }
  }

  const byteArray = [];
  for (const value of outMat) {
    const buffer = new ArrayBuffer(8);
    const view = new DataView(buffer);
    view.setBigInt64(0, BigInt(value), true);
    byteArray.push(...new Uint8Array(buffer));
  }

  const buffer = new ArrayBuffer(8);
  const view = new DataView(buffer);
  view.setBigInt64(0, BigInt(rows), true);
  byteArray.unshift(...new Uint8Array(buffer));

  return byteArray;
}
import { BSON } from "bson";

export default {
  async fetch(request) {
    const url = new URL(request.url);

    if (request.method === "POST" && url.pathname.includes("matmul")) {
      return await handleRequest(request);
    } else if (request.method === "POST" && url.pathname === "/") {
      return new Response("ok", { status: 200 });
    } else {
      return new Response("Not Found", { status: 404 });
    }
  }
};

async function handleRequest(request) {
  const requestArrayBuffer = await request.arrayBuffer();

  if (requestArrayBuffer.byteLength === 0) {
    return new Response("Empty request body", { status: 400 });
  }

  const requestBuffer = new Uint8Array(requestArrayBuffer);

  try {
    const body = BSON.deserialize(requestBuffer);

    const matrixDataBinary = body.sets[0].items[0].data;
    const matrixData = new Uint8Array(matrixDataBinary.buffer);
    // console.debug("Converted matrix data to Uint8Array:", matrixData);

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

    const bsonResponse = BSON.serialize(response);

    return new Response(bsonResponse, {
      headers: { "Content-Type": "application/bson" }
    });
  } catch (error) {
    console.error("Failed to deserialize BSON:", error);
    return new Response("Invalid BSON format", { status: 400 });
  }
}

function matmulService(data) {
  const rows = new DataView(data.buffer).getBigInt64(0, true);
  const cols = BigInt((data.length - 8) / 8) / rows;

  const inMat = new BigInt64Array(data.buffer, 8);

  const outMat = new BigInt64Array(Number(rows * rows) + 1);
  outMat[0] = rows;

  for (let i = 0n; i < rows; i++) {
    for (let j = 0n; j < rows; j++) {
      let sum = 0n;
      for (let k = 0n; k < cols; k++) {
        sum += inMat[Number(i * cols + k)] * inMat[Number(j * cols + k)];
      }
      outMat[Number(i * rows + j + 1n)] = sum;
    }
  }

  const byteArray = new Uint8Array(outMat.buffer);
  return byteArray;
}

import { BSON } from "bson";

export default {
  async fetch(request) {
    const url = new URL(request.url);

    if (request.method === "POST" && url.pathname.includes("io")) {
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

    const getRequestBinary = body.sets.find(set => set.identifier === "get_request")
                                 .items.find(item => item.identifier === "get_request").data;
    const postRequestBinary = body.sets.find(set => set.identifier === "post_request")
                                  .items.find(item => item.identifier === "post_request").data;

    const getRequestData = new Uint8Array(getRequestBinary.buffer);
    const postRequestData = new Uint8Array(postRequestBinary.buffer);

    const getRequestString = new TextDecoder("utf-8").decode(getRequestData);
    const postRequestString = new TextDecoder("utf-8").decode(postRequestData);

    const getUri = getRequestString.replace(/^GET /, "").replace(/ HTTP\/1.1$/, "");
    const postUri = postRequestString.replace(/^POST /, "").replace(/ HTTP\/1.1\n\n$/, "");

    // Call ioService to perform GET, simulate busy function, and POST
    const responseData = await ioService(getUri, postUri);

    // Return response as BSON
    return new Response(responseData, {
      headers: { "Content-Type": "application/bson" }
    });

  } catch (error) {
    console.error("Failed to deserialize BSON:", error);
    return new Response("Invalid BSON format", { status: 400 });
  }
}

async function ioService(getUri, postUri) {
  const getResponse = await fetch(getUri);
  const getRequestBuf = new Uint8Array(await getResponse.arrayBuffer());

  const dataView = new DataView(getRequestBuf.buffer);
  const iterations = dataView.getBigUint64(0, true);


  const checksum = busy(Number(iterations));
  dataView.setBigUint64(0, BigInt(checksum), true);

  const postResponse = await fetch(postUri, {
    method: "POST",
    body: getRequestBuf,
    headers: { "Content-Type": "application/octet-stream" }
  });
  const postResponseBuf = new Uint8Array(await postResponse.arrayBuffer());


  const responseVec = new Uint8Array(2 + postResponseBuf.length);
  responseVec.set([0x0A, 0x0A], 0); // '\n\n' at the beginning
  responseVec.set(postResponseBuf, 2);

  const response = {
    sets: [
      {
        identifier: "store_body",
        items: [
          {
            identifier: "body",
            key: 0,
            data: responseVec
          }
        ]
      }
    ]
  };

  return BSON.serialize(response);
}

function busy(iterations) {
  let counter = 0;
  for (let i = 0; i < iterations; i++) {
    counter++;
  }
  return counter;
}

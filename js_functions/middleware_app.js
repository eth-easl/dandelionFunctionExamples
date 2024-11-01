import { BSON } from "bson";

const LOG_SERVER_COUNT = 10;

export default {
  async fetch(request) {
    const url = new URL(request.url);

    if (request.method === "POST" && url.pathname.includes("middleware_app")) {
      return await handleMiddleware(request);
    } else if (request.method === "POST" && url.pathname === "/") {
      return new Response("ok", { status: 200 });
    } else {
      return new Response("Not Found", { status: 404 });
    }
  }
};

async function handleMiddleware(request) {
  const hostHeader = request.headers.get("host");
  const ip = hostHeader ? hostHeader.split(":")[0] : "127.0.0.1";
  const AUTH_SERVER = `${ip}:8000`;

  const requestArrayBuffer = await request.arrayBuffer();

  if (requestArrayBuffer.byteLength === 0) {
    return new Response("Empty request body", { status: 400 });
  }

  const requestBuffer = new Uint8Array(requestArrayBuffer);

  try {
    const body = BSON.deserialize(requestBuffer);

    const authorizationItem = body.sets.find(set => set.identifier === "inputs")
                                  .items.find(item => item.identifier === "Authorization");

    const authDataBinary = authorizationItem.data;
    const authDataBase64 = authDataBinary.toString('base64');
    const authDataBuffer = Uint8Array.from(atob(authDataBase64), c => c.charCodeAt(0));
    const authHeader = new TextDecoder("utf-8").decode(authDataBuffer);
    const authToken = authHeader.replace("Bearer ", "").trim();

    // Authorization request to AUTH_SERVER
    const authUrl = `http://${AUTH_SERVER}/authorize`;
    const authRequestBody = JSON.stringify({ token: authToken });

    const authResponse = await fetch(authUrl, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: authRequestBody,
    });

    if (!authResponse.ok) {
      return new Response("Authorization failed", { status: 403 });
    }

    const { authorized, token } = await authResponse.json();
    
    const logUrls = Array.from({ length: LOG_SERVER_COUNT }, (_, i) => `http://${AUTH_SERVER}/logs/${i}`);
    const logPromises = logUrls.map(url =>
      fetch(url, {
        headers: { "Authorization": `Bearer ${token}` }
      })
    );

    const logResponses = await Promise.all(logPromises);
    const logEvents = [];

    for (const response of logResponses) {
      if (response.ok) {
        const logJson = await response.json();
        const events = logJson.events || [];
        logEvents.push(...events);
      }
    }

    logEvents.sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp));

    const htmlLogs = renderPage(logEvents);

    const response = {
      sets: [
        {
          identifier: "middleware_body",
          items: [
            {
              identifier: "body",
              key: 0,
              data: new TextEncoder().encode(htmlLogs),
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
    console.error("Failed to deserialize BSON or process request:", error);
    return new Response("Invalid BSON format", { status: 400 });
  }
}

function renderPage(events) {
  const LOGS_TEMPLATE_HEAD = `
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Logs</title>
    </head>
    <body>
        <table>
            <tr>
                <th>Timestamp</th>
                <th>Server ID</th>
                <th>Event Type</th>
                <th>Details</th>
            </tr>
  `;

  const LOGS_TEMPLATE_TAIL = `
        </table>
    </body>
    </html>
  `;

  const rows = events.map(event => `
    <tr>
      <td>${event.timestamp}</td>
      <td>${event.server_id}</td>
      <td>${event.event_type}</td>
      <td>${event.details}</td>
    </tr>
  `).join("");

  return LOGS_TEMPLATE_HEAD + rows + LOGS_TEMPLATE_TAIL;
}
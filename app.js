const API_BASE = "http://localhost:8000/api";

async function callCompute() {
  const num = document.getElementById("numInput").value;
  if (!num) return alert("Enter a number");
  try {
    const res = await fetch(`${API_BASE}/compute?n=${num}`);
    const data = await res.json();
    document.getElementById("computeResult").textContent = JSON.stringify(data, null, 2);
  } catch (err) {
    document.getElementById("computeResult").textContent = `Error: ${err.message}`;
  }
}

async function sendNotification() {
  const msg = document.getElementById("msgInput").value;
  if (!msg) return alert("Enter a message");
  try {
    const res = await fetch(`${API_BASE}/notify`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ message: msg })
    });
    const data = await res.json();
    document.getElementById("notifyResult").textContent = JSON.stringify(data, null, 2);
  } catch (err) {
    document.getElementById("notifyResult").textContent = `Error: ${err.message}`;
  }
}

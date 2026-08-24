import os
import logging
import httpx
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

app = FastAPI(title="Polyglot API Gateway")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

CPP_SERVICE = os.getenv("CPP_SERVICE_URL", "http://cpp-service:8080")
JAVA_SERVICE = os.getenv("JAVA_SERVICE_URL", "http://java-service:8081")

class NotificationRequest(BaseModel):
    message: str

@app.get("/api/compute")
async def proxy_compute(n: int):
    if n < 1 or n > 1000000:
        raise HTTPException(400, "n must be between 1 and 1,000,000")
    try:
        async with httpx.AsyncClient(timeout=10.0) as client:
            res = await client.get(f"{CPP_SERVICE}/compute", params={"n": n})
            res.raise_for_status()
            return res.json()
    except Exception as e:
        logger.error(f"C++ service error: {e}")
        raise HTTPException(502, "Compute service unavailable")

@app.post("/api/notify")
async def proxy_notify(req: NotificationRequest):
    try:
        async with httpx.AsyncClient(timeout=5.0) as client:
            res = await client.post(f"{JAVA_SERVICE}/api/notifications", json={"message": req.message})
            res.raise_for_status()
            return res.json()
    except Exception as e:
        logger.error(f"Java service error: {e}")
        raise HTTPException(502, "Notification service unavailable")

@app.get("/health")
def health():
    return {"status": "ok", "gateway": "python-fastapi"}

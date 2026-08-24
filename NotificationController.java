package com.example;

import org.springframework.web.bind.annotation.*;
import java.time.Instant;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.UUID;

@RestController
@RequestMapping("/api/notifications")
public class NotificationController {
    private final Map<String, String> store = new ConcurrentHashMap<>();

    @PostMapping
    public Map<String, Object> create(@RequestBody Map<String, String> payload) {
        String id = UUID.randomUUID().toString();
        String msg = payload.getOrDefault("message", "empty");
        store.put(id, msg);
        return Map.of(
            "id", id,
            "message", msg,
            "status", "queued",
            "timestamp", Instant.now().toString(),
            "service", "java-springboot"
        );
    }

    @GetMapping("/count")
    public Map<String, Integer> count() {
        return Map.of("total", store.size());
    }
}

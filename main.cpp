#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include "httplib.h" // Header-only: https://github.com/yhirose/cpp-httplib

long long heavy_compute(int n) {
    // Simulate CPU-intensive work: sum of primes up to n
    long long sum = 0;
    for (int i = 2; i <= n; ++i) {
        bool prime = true;
        for (int j = 2; j <= std::sqrt(i); ++j) {
            if (i % j == 0) { prime = false; break; }
        }
        if (prime) sum += i;
    }
    return sum;
}

int main() {
    httplib::Server svr;

    svr.Get("/compute", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("n")) {
            res.status = 400;
            res.set_content("{\"error\":\"missing ?n= parameter\"}", "application/json");
            return;
        }
        int n = std::stoi(req.get_param_value("n"));
        if (n < 1 || n > 1000000) {
            res.status = 400;
            res.set_content("{\"error\":\"n out of range\"}", "application/json");
            return;
        }

        auto start = std::chrono::high_resolution_clock::now();
        long long result = heavy_compute(n);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();

        res.set_content(
            "{\"result\":" + std::to_string(result) +
            ",\"time_ms\":" + std::to_string(ms) +
            ",\"service\":\"cpp-httplib\"}",
            "application/json"
        );
    });

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"status\":\"ok\",\"service\":\"cpp\"}", "application/json");
    });

    std::cout << "C++ service running on :8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}

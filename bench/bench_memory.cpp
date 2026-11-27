#include "ultralob/order_pool.hpp"
#include "ultralob/price_level.hpp"
#include "ultralob/order_book.hpp"

#include <cstdint>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#elif defined(__linux__)
    #include <fstream>
    #include <string>
    #include <unistd.h>
    #include <sys/resource.h>
#elif defined(__APPLE__)
    #include <sys/resource.h>
#endif

int main() {
    using namespace ultralob;

    std::cout << "--- UltraLOB Memory Profile ---\n";
    std::cout << "sizeof(Order): " << sizeof(Order) << " bytes\n";
    std::cout << "sizeof(PriceLevel): " << sizeof(PriceLevel) << " bytes\n";

    const std::uint64_t pool_bytes =
        sizeof(OrderPool) +
        (sizeof(Order) * static_cast<std::uint64_t>(OrderPool::DEFAULT_CAPACITY));

    std::cout << "OrderPool capacity: " << OrderPool::DEFAULT_CAPACITY << " orders\n";
    std::cout << "Estimated OrderPool footprint: "
              << (pool_bytes / 1024.0 / 1024.0) << " MiB\n";
    std::cout << "Default journal mapping capacity: 1024 MiB (virtual mapping)\n";

    std::uint64_t current_rss_bytes = 0;
    std::uint64_t peak_rss_bytes = 0;

#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        current_rss_bytes = static_cast<std::uint64_t>(pmc.WorkingSetSize);
        peak_rss_bytes = static_cast<std::uint64_t>(pmc.PeakWorkingSetSize);
    }
#elif defined(__linux__)
    // /proc/self/statm reports resident pages; convert using the OS page size.
    std::ifstream statm("/proc/self/statm");
    std::uint64_t total_pages = 0;
    std::uint64_t resident_pages = 0;
    if (statm >> total_pages >> resident_pages) {
        const long page_size = sysconf(_SC_PAGESIZE);
        if (page_size > 0) {
            current_rss_bytes = resident_pages * static_cast<std::uint64_t>(page_size);
        }
    }

    struct rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        // Linux reports ru_maxrss in KiB.
        peak_rss_bytes = static_cast<std::uint64_t>(usage.ru_maxrss) * 1024ULL;
    }
#elif defined(__APPLE__)
    struct rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        // macOS reports ru_maxrss in bytes.
        peak_rss_bytes = static_cast<std::uint64_t>(usage.ru_maxrss);
    }
#endif

    if (current_rss_bytes != 0) {
        std::cout << "Current resident set: "
                  << (current_rss_bytes / 1024.0 / 1024.0) << " MiB\n";
    }
    if (peak_rss_bytes != 0) {
        std::cout << "Peak resident set: "
                  << (peak_rss_bytes / 1024.0 / 1024.0) << " MiB\n";
    }

    return 0;
}

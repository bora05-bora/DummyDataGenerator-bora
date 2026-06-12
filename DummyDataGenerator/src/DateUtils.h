#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Convert "YYYY-MM-DD" to Julian Day Number
inline long dateToDays(const std::string& dateStr) {
    int y, m, d;
    char sep;
    std::istringstream ss(dateStr);
    if (!(ss >> y >> sep >> m >> sep >> d))
        throw std::invalid_argument("Bad date format: " + dateStr);

    if (m <= 2) { y--; m += 12; }
    long A = y / 100;
    long B = 2 - A + A / 4;
    return static_cast<long>(365.25  * (y + 4716))
         + static_cast<long>(30.6001 * (m + 1))
         + d + B - 1524;
}

// Convert Julian Day Number to "YYYY-MM-DD"
inline std::string daysToDate(long jd) {
    long l = jd + 68569;
    long n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    long i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    long j = (80 * l) / 2447;
    int  d = static_cast<int>(l - (2447 * j) / 80);
    l = j / 11;
    int  m = static_cast<int>(j + 2 - 12 * l);
    int  y = static_cast<int>(100 * (n - 49) + i + l);

    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << y  << '-'
        << std::setw(2) << std::setfill('0') << m  << '-'
        << std::setw(2) << std::setfill('0') << d;
    return oss.str();
}

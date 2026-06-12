#pragma once
#include <string>
#include <nlohmann/json.hpp>

// mirrors Models.h from DataMonitor-bora
enum class OrderStatus {
    Pending,
    Processing,
    Completed,
    Cancelled
};

inline std::string orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::Pending:    return "Pending";
        case OrderStatus::Processing: return "Processing";
        case OrderStatus::Completed:  return "Completed";
        case OrderStatus::Cancelled:  return "Cancelled";
        default:                      return "Pending";
    }
}

inline OrderStatus orderStatusFromString(const std::string& s) {
    if (s == "Processing") return OrderStatus::Processing;
    if (s == "Completed")  return OrderStatus::Completed;
    if (s == "Cancelled")  return OrderStatus::Cancelled;
    return OrderStatus::Pending;
}

struct OrderData {
    int         id;
    std::string sample;
    int         quantity;
    OrderStatus status;
    std::string date; // YYYY-MM-DD

    nlohmann::json toJson() const {
        return {
            {"id",       id},
            {"sample",   sample},
            {"quantity", quantity},
            {"status",   orderStatusToString(status)},
            {"date",     date}
        };
    }
};

struct SampleData {
    int         id;
    std::string name;
    int         stock;
    double      productionTime; // hours
    double      yield;          // percent

    nlohmann::json toJson() const {
        return {
            {"id",             id},
            {"name",           name},
            {"stock",          stock},
            {"productionTime", productionTime},
            {"yield",          yield}
        };
    }
};

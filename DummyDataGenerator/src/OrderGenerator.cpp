#include "OrderGenerator.h"
#include "DateUtils.h"
#include <stdexcept>

OrderGenerator::OrderGenerator(const GeneratorConfig&          cfg,
                               std::mt19937&                   rng,
                               const std::vector<std::string>& sampleNames)
    : cfg_(cfg), rng_(rng), sampleNames_(sampleNames)
{
    startDay_ = dateToDays(cfg_.startDate);
    endDay_   = dateToDays(cfg_.endDate);
    if (startDay_ > endDay_)
        throw std::invalid_argument("startDate must be before endDate");
}

OrderStatus OrderGenerator::pickStatus() {
    std::uniform_int_distribution<int> d(1, 100);
    int roll = d(rng_);

    int cumPending    = cfg_.weightPending;
    int cumProcessing = cumPending    + cfg_.weightProcessing;
    int cumCompleted  = cumProcessing + cfg_.weightCompleted;

    if (roll <= cumPending)    return OrderStatus::Pending;
    if (roll <= cumProcessing) return OrderStatus::Processing;
    if (roll <= cumCompleted)  return OrderStatus::Completed;
    return OrderStatus::Cancelled;
}

std::string OrderGenerator::pickDate() {
    std::uniform_int_distribution<long> d(startDay_, endDay_);
    return daysToDate(d(rng_));
}

std::vector<OrderData> OrderGenerator::generate() {
    if (sampleNames_.empty())
        throw std::runtime_error("Sample name pool is empty");

    std::uniform_int_distribution<int> qtyDist(cfg_.minQuantity, cfg_.maxQuantity);
    std::uniform_int_distribution<int> sampleDist(
        0, static_cast<int>(sampleNames_.size()) - 1);

    std::vector<OrderData> orders;
    orders.reserve(cfg_.orderCount);

    for (int i = 0; i < cfg_.orderCount; ++i) {
        OrderData o;
        o.id       = i + 1;
        o.sample   = sampleNames_[sampleDist(rng_)];
        o.quantity = qtyDist(rng_);
        o.status   = pickStatus();
        o.date     = pickDate();
        orders.push_back(std::move(o));
    }

    return orders;
}

#pragma once
#include <vector>
#include <random>
#include "DataModels.h"
#include "Config.h"

class OrderGenerator {
public:
    OrderGenerator(const GeneratorConfig&          cfg,
                   std::mt19937&                   rng,
                   const std::vector<std::string>& sampleNames);

    std::vector<OrderData> generate();

private:
    const GeneratorConfig&          cfg_;
    std::mt19937&                   rng_;
    const std::vector<std::string>& sampleNames_;

    long startDay_;
    long endDay_;

    OrderStatus pickStatus();
    std::string pickDate();
};

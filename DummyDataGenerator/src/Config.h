#pragma once
#include <string>

struct GeneratorConfig {
    int orderCount  = 100;
    int sampleCount = 10;

    std::string startDate = "2026-01-01"; // YYYY-MM-DD
    std::string endDate   = "2026-12-31";

    std::string outputDir         = "output";
    std::string ordersOutputFile  = "orders_dummy.json";
    std::string samplesOutputFile = "samples_dummy.json";

    unsigned int seed = 0; // 0 = time-based seed

    int minQuantity = 1;
    int maxQuantity = 200;

    int minStock = 0;
    int maxStock = 500;

    double minProductionTime = 1.0;  // hours
    double maxProductionTime = 48.0;

    double minYield = 50.0; // percent
    double maxYield = 99.9;

    // Status weight (must sum to 100)
    int weightPending    = 40;
    int weightProcessing = 25;
    int weightCompleted  = 25;
    int weightCancelled  = 10;
};

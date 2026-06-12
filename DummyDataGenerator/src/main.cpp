#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <chrono>
#include <stdexcept>
#include <iomanip>

#include <nlohmann/json.hpp>

#include "Config.h"
#include "DataModels.h"
#include "SampleGenerator.h"
#include "OrderGenerator.h"

namespace fs = std::filesystem;

static GeneratorConfig parseArgs(int argc, char* argv[]) {
    GeneratorConfig cfg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto eq = arg.find('=');
        if (eq == std::string::npos || arg.size() < 3) continue;
        std::string key = arg.substr(2, eq - 2); // strip "--"
        std::string val = arg.substr(eq + 1);

        if      (key == "orders")   cfg.orderCount  = std::stoi(val);
        else if (key == "samples")  cfg.sampleCount = std::stoi(val);
        else if (key == "start")    cfg.startDate   = val;
        else if (key == "end")      cfg.endDate     = val;
        else if (key == "seed")     cfg.seed        = static_cast<unsigned>(std::stoul(val));
        else if (key == "out")      cfg.outputDir   = val;
        else if (key == "min-qty")  cfg.minQuantity = std::stoi(val);
        else if (key == "max-qty")  cfg.maxQuantity = std::stoi(val);
    }
    return cfg;
}

static void saveJson(const fs::path& path, const nlohmann::json& j) {
    std::ofstream ofs(path);
    if (!ofs) throw std::runtime_error("Cannot open file: " + path.string());
    ofs << j.dump(2) << '\n';
}

int main(int argc, char* argv[]) {
    try {
        GeneratorConfig cfg = parseArgs(argc, argv);

        unsigned int seed = cfg.seed;
        if (seed == 0)
            seed = static_cast<unsigned int>(
                std::chrono::steady_clock::now().time_since_epoch().count());
        std::mt19937 rng(seed);

        std::cout << "=== DummyDataGenerator ===\n"
                  << "seed    : " << seed           << '\n'
                  << "samples : " << cfg.sampleCount << '\n'
                  << "orders  : " << cfg.orderCount  << '\n'
                  << "range   : " << cfg.startDate   << " ~ " << cfg.endDate << "\n\n";

        SampleGenerator sampleGen(cfg, rng);
        auto samples = sampleGen.generate();

        OrderGenerator orderGen(cfg, rng, sampleGen.getSampleNames());
        auto orders = orderGen.generate();

        fs::path outDir(cfg.outputDir);
        fs::create_directories(outDir);

        nlohmann::json samplesJson;
        for (const auto& s : samples)
            samplesJson["samples"].push_back(s.toJson());
        fs::path samplesPath = outDir / cfg.samplesOutputFile;
        saveJson(samplesPath, samplesJson);
        std::cout << "[OK] samples -> " << samplesPath << '\n';

        nlohmann::json ordersJson;
        for (const auto& o : orders)
            ordersJson["orders"].push_back(o.toJson());
        fs::path ordersPath = outDir / cfg.ordersOutputFile;
        saveJson(ordersPath, ordersJson);
        std::cout << "[OK] orders  -> " << ordersPath << '\n';

        int cntPending = 0, cntProcessing = 0, cntCompleted = 0, cntCancelled = 0;
        for (const auto& o : orders) {
            switch (o.status) {
                case OrderStatus::Pending:    ++cntPending;    break;
                case OrderStatus::Processing: ++cntProcessing; break;
                case OrderStatus::Completed:  ++cntCompleted;  break;
                case OrderStatus::Cancelled:  ++cntCancelled;  break;
            }
        }

        auto pct = [n = cfg.orderCount](int k) {
            return static_cast<double>(k) * 100.0 / n;
        };

        std::cout << "\n--- Status distribution ---\n"
                  << std::fixed << std::setprecision(1)
                  << "  Pending    : " << cntPending    << " (" << pct(cntPending)    << "%)\n"
                  << "  Processing : " << cntProcessing << " (" << pct(cntProcessing) << "%)\n"
                  << "  Completed  : " << cntCompleted  << " (" << pct(cntCompleted)  << "%)\n"
                  << "  Cancelled  : " << cntCancelled  << " (" << pct(cntCancelled)  << "%)\n"
                  << "\nDone.\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERROR] " << ex.what() << '\n';
        return 1;
    }
    return 0;
}

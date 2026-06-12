#include "SampleGenerator.h"
#include <sstream>
#include <iomanip>
#include <cmath>

// Follows the Alpha-001, Beta-002, ... naming convention from DataMonitor-bora
const std::vector<std::string> SampleGenerator::kPrefixes = {
    "Alpha",   "Beta",    "Gamma",   "Delta",   "Epsilon",
    "Zeta",    "Eta",     "Theta",   "Iota",    "Kappa",
    "Lambda",  "Mu",      "Nu",      "Xi",      "Omicron",
    "Pi",      "Rho",     "Sigma",   "Tau",     "Upsilon"
};

SampleGenerator::SampleGenerator(const GeneratorConfig& cfg, std::mt19937& rng)
    : cfg_(cfg), rng_(rng)
{}

std::string SampleGenerator::buildSampleName(int index) const {
    const std::string& prefix = kPrefixes[index % kPrefixes.size()];
    std::ostringstream oss;
    oss << prefix << '-' << std::setw(3) << std::setfill('0') << (index + 1);
    return oss.str();
}

std::vector<SampleData> SampleGenerator::generate() {
    std::uniform_int_distribution<int>     stockDist(cfg_.minStock, cfg_.maxStock);
    std::uniform_real_distribution<double> timeDist(cfg_.minProductionTime, cfg_.maxProductionTime);
    std::uniform_real_distribution<double> yieldDist(cfg_.minYield, cfg_.maxYield);

    std::vector<SampleData> samples;
    samples.reserve(cfg_.sampleCount);
    sampleNames_.clear();

    for (int i = 0; i < cfg_.sampleCount; ++i) {
        SampleData s;
        s.id             = i + 1;
        s.name           = buildSampleName(i);
        s.stock          = stockDist(rng_);
        s.productionTime = std::round(timeDist(rng_)  * 10.0) / 10.0;
        s.yield          = std::round(yieldDist(rng_) * 10.0) / 10.0;

        sampleNames_.push_back(s.name);
        samples.push_back(std::move(s));
    }

    return samples;
}

#pragma once
#include <vector>
#include <random>
#include "DataModels.h"
#include "Config.h"

class SampleGenerator {
public:
    explicit SampleGenerator(const GeneratorConfig& cfg, std::mt19937& rng);

    std::vector<SampleData> generate();

    // Used by OrderGenerator to pick sample names
    const std::vector<std::string>& getSampleNames() const { return sampleNames_; }

private:
    const GeneratorConfig&   cfg_;
    std::mt19937&            rng_;
    std::vector<std::string> sampleNames_;

    static const std::vector<std::string> kPrefixes;

    std::string buildSampleName(int index) const;
};

#include "DataProcessor.h"
#include <numeric> // For validation (sum function)

DataProcessor::DataProcessor() : rawData({}) {}

void DataProcessor::setRawData(const std::vector<float>& data)
{
    rawData = data;
}

std::map<std::string, float> DataProcessor::processData()
{
    // Example: Map raw data to organs
    std::map<std::string, float> processedData;

    const std::vector<std::string> organs = {"Heart", "Liver", "Lungs", "Kidneys", "Stomach"};

    for (size_t i = 0; i < organs.size() && i < rawData.size(); ++i) {
        processedData[organs[i]] = rawData[i];
    }

    return processedData;
}

bool DataProcessor::validateData() const
{
    // Example: Validation checks if data is non-zero and has expected size
    return !rawData.empty() && std::accumulate(rawData.begin(), rawData.end(), 0.0f) > 0;
}

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <vector>
#include <map>
#include <string>

class DataProcessor
{
private:
    std::vector<float> rawData;

public:
    DataProcessor();

    // Functions
    void setRawData(const std::vector<float>& data);
    std::map<std::string, float> processData();
    bool validateData() const;
};

#endif // DATAPROCESSOR_H

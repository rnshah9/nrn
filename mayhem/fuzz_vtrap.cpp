#include <stdint.h>
#include <stdio.h>
#include <climits>

#include <fuzzer/FuzzedDataProvider.h>

extern "C" double vtrap(const double x, const double y);

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    FuzzedDataProvider provider(data, size);
    double x = provider.ConsumeFloatingPoint<double>();
    double y = provider.ConsumeFloatingPoint<double>();
    
    vtrap(x, y);

    return 0;
}
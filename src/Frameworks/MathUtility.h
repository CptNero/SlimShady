#ifndef SLIMSHADY_MATHUTILITY_H
#define SLIMSHADY_MATHUTILITY_H

#endif //SLIMSHADY_MATHUTILITY_H

#include <vector>
#include <numeric>

namespace MathUtility {

    template<typename Type, typename ReturnType>
    ReturnType Mean(std::vector<Type> x) {
      uint32_t sum = 0;

      for (uint32_t i = 0; i < x.size(); i++) {
        sum += x[i];
      }

      return (ReturnType)sum / (ReturnType)x.size();
    }

    template<typename Type, typename ReturnType>
    ReturnType Variance(std::vector<Type> x, Type mean) {
      uint32_t sum = 0;
      uint32_t size = x.size();

      for(int i = 0; i < x.size(); i++) {
        sum += (x[i] - mean) * (x[i] - mean);
      }

      return (ReturnType)sum / (ReturnType)size;
    }

    template<typename Type, typename MeanType, typename ReturnType>
    ReturnType Covariance(std::vector<Type> x, std::vector<Type> y, MeanType xMean, MeanType yMean) {
      uint32_t size = x.size();
      double sumX = 0;
      double sumY = 0;
      double sumXY = 0;

      for (int i = 0; i < size; i++) {
        sumX += x[i] - xMean;
        sumY += y[i] - yMean;
        sumXY += (x[i] - xMean) * (y[i] - yMean);
      }

      ReturnType covariance = (sumXY - sumX * sumY / size) / (ReturnType)size;

      return covariance;
    }
}



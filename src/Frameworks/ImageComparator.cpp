#include <iostream>
#include <cmath>
#include "ImageComparator.h"
#include "MathUtility.h"
#include "Configurations.h"
#include "../Widgets/ConsoleWidget.h"

ImageComparator::ImageComparator() {
}

ImageComparator::~ImageComparator() {
}

ImageComparator::TextureHistogram ImageComparator::CreateTextureHistogram(std::vector<uint8_t>& textureData) {
  TextureHistogram textureHistogram;

  for (uint64_t i = 0; i < textureData.size(); i+=4) {
        textureHistogram.RedChannel[textureData[i]]++;
        textureHistogram.GreenChannel[textureData[i+1]]++;
        textureHistogram.BlueChannel[textureData[i+2]]++;
        textureHistogram.AlphaChannel[textureData[i+3]]++;
  }

  return  textureHistogram;
}

double ImageComparator::CalculateChiSquareDistance(std::vector<uint8_t>& taskTextureData, std::vector<uint8_t>& renderedTextureData) {
  if (Configurations::IsDebugEnabled) {
    if (taskTextureData.size() != renderedTextureData.size()) {
      ConsoleWidget::LogMessage("Warning, images aren't of the same resolution");
    }
  }

  TextureHistogram taskHistogram = CreateTextureHistogram(taskTextureData);
  TextureHistogram renderedHistogram = CreateTextureHistogram(renderedTextureData);

  double sum = 0.0f;

  for(int i = 0; i < 255; i++) {
      sum += CalculateChiSquareDistanceForColorValue(taskHistogram.RedChannel[i], renderedHistogram.RedChannel[i]);
      sum += CalculateChiSquareDistanceForColorValue(taskHistogram.GreenChannel[i], renderedHistogram.GreenChannel[i]);
      sum += CalculateChiSquareDistanceForColorValue(taskHistogram.BlueChannel[i], renderedHistogram.BlueChannel[i]);
      sum += CalculateChiSquareDistanceForColorValue(taskHistogram.AlphaChannel[i], renderedHistogram.AlphaChannel[i]);
  }

  return sum;
}

double ImageComparator::CalculateChiSquareDistanceForColorValue(double taskColorValue, double renderedColorValue) {
    if (taskColorValue != 0) {
        return ((taskColorValue - renderedColorValue) * (taskColorValue - renderedColorValue)) / taskColorValue;
    }

    return 0.0;
}

double ImageComparator::CalculateSSIMForColorChannel(
        const std::vector<uint8_t> &taskTextureColor,
        const std::vector<uint8_t> &renderedTextureColor) {
  double taskMean = MathUtility::Mean<uint8_t, double>(taskTextureColor);
  double renderedMean = MathUtility::Mean<uint8_t, double>(renderedTextureColor);

  double taskVariance = MathUtility::Variance<uint8_t, double>(taskTextureColor, taskMean);
  double renderedVariance = MathUtility::Variance<uint8_t, double>(renderedTextureColor, renderedMean);

  double taskRenderedCovariance =
          MathUtility::Covariance<uint8_t, double, double>(taskTextureColor, renderedTextureColor, taskMean, renderedMean);

  double pixelDynamicRange = (double)std::pow(2, 8) - 1;
  double k1 = 0.01;
  double k2 = 0.03;

  double c1 = std::pow(k1 * pixelDynamicRange, 2);
  double c2 = std::pow(k2 * pixelDynamicRange, 2);
  double c3 = c2 / 2.0;

  double alpha = 1;
  double beta = 1;
  double delta = 1;

  double luminance = (2 * taskMean * renderedMean + c1) / ((taskMean * taskMean) + (renderedMean * renderedMean) + c1);
  double contrast = (2 * taskVariance * renderedVariance + c2) / ((taskVariance * taskVariance) + (renderedVariance * renderedVariance) + c2);
  double structure = (taskRenderedCovariance + c3) / (std::sqrt(taskVariance) * std::sqrt(renderedVariance) + c3);

  return std::pow(luminance, alpha) * std::pow(contrast, beta) * std::pow(structure, delta);
}

std::vector<uint8_t> ImageComparator::ExtractColorChannel(const std::vector<uint8_t>& textureData, ImageComparator::RGBAComponent component) {
  std::vector<uint8_t> colorChannel;
  const int componentOffset = 4;
  int i = 0;

  switch (component) {
    case RGBAComponent::RED: {
      i = 0;
      break;
    }
    case RGBAComponent::GREEN: {
      i = 1;
      break;
    }
    case RGBAComponent::BLUE: {
      i = 2;
      break;
    }
    case RGBAComponent::ALPHA: {
      i = 3;
      break;
    }
  }

  for(; i < textureData.size(); i += componentOffset) {
    colorChannel.emplace_back(textureData[i]);
  }

  return colorChannel;
}

double ImageComparator::CalculateSSIM(std::vector<uint8_t>& taskTextureData, std::vector<uint8_t>& renderedTextureData) {
  if (Configurations::IsDebugEnabled) {
    if (taskTextureData.size() != renderedTextureData.size()) {
      ConsoleWidget::LogMessage("Warning, images aren't of the same resolution");
    }
  }

  std::vector<uint8_t> taskRedColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::RED);
  std::vector<uint8_t> taskGreenColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::GREEN);
  std::vector<uint8_t> taskBlueColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::BLUE);
  std::vector<uint8_t> taskAlphaColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::ALPHA);

  std::vector<uint8_t> renderedRedColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::RED);
  std::vector<uint8_t> renderedGreenColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::GREEN);
  std::vector<uint8_t> renderedBlueColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::BLUE);
  std::vector<uint8_t> renderedAlphaColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::ALPHA);

  double SSIM = 0.0;
  SSIM += CalculateSSIMForColorChannel(taskRedColorChannel, renderedRedColorChannel);
  SSIM += CalculateSSIMForColorChannel(taskGreenColorChannel, renderedGreenColorChannel);
  SSIM += CalculateSSIMForColorChannel(taskBlueColorChannel, renderedBlueColorChannel);
  SSIM += CalculateSSIMForColorChannel(taskAlphaColorChannel, renderedAlphaColorChannel);

  return SSIM;
}



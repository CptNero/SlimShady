#include <utility>
#include <iostream>
#include "ImageComparator.h"
#include "MathUtility.h"

ImageComparator::ImageComparator(std::vector<uint8_t> taskTextureData, std::vector<uint8_t> renderedTextureData) :
  m_TaskTextureData(std::move(taskTextureData)),
  m_RenderedTextureData(std::move(renderedTextureData))
{
}

ImageComparator::~ImageComparator() {
}


double ImageComparator::CompareImages() {
  return CalculateSSIM(m_TaskTextureData, m_RenderedTextureData);
}

ImageComparator::TextureHistogram ImageComparator::CreateTextureHistogram(std::vector<uint8_t> textureData) {
  TextureHistogram textureHistogram;
  const uint16_t rgbaComponentLength = 4;
  uint16_t componentCounter = 0;

  for (uint64_t i = 0; i < textureData.size(); i++) {
    switch (componentCounter) {
      case RGBAComponent::RED: {
        textureHistogram.RedChannel[textureData[i]]++;
        break;
      }
      case RGBAComponent::GREEN: {
        textureHistogram.GreenChannel[textureData[i]]++;
        break;
      }
      case RGBAComponent::BLUE: {
        textureHistogram.BlueChannel[textureData[i]]++;
        break;
      }
      case RGBAComponent::ALPHA: {
        textureHistogram.AlphaChannel[textureData[i]]++;
        break;
      }
    }

    (componentCounter == rgbaComponentLength) ? componentCounter = 0 : componentCounter++;
  }

  return  textureHistogram;
}

float ImageComparator::CalculateChiSquareDistance(const std::vector<uint8_t>& taskTextureData, const std::vector<uint8_t>& renderedTextureData) {
  TextureHistogram taskHistogram = CreateTextureHistogram(taskTextureData);
  TextureHistogram renderedHistogram = CreateTextureHistogram(renderedTextureData);

  float redSum = 0.0f;
  float greenSum = 0.0f;
  float blueSum = 0.0f;
  float alphaSum = 0.0f;

  for (int i = 0; i < 256; i++) {
    if (taskHistogram.RedChannel[i] + renderedHistogram.RedChannel[i] != 0) {
      redSum += ((float)(((taskHistogram.RedChannel[i] - renderedHistogram.RedChannel[i]) * (taskHistogram.RedChannel[i] - renderedHistogram.RedChannel[i])))
                 / (float)(taskHistogram.RedChannel[i] + renderedHistogram.RedChannel[i]));
    }

    if (taskHistogram.GreenChannel[i] + renderedHistogram.GreenChannel[i] != 0) {
      greenSum += ((float)(((taskHistogram.GreenChannel[i] - renderedHistogram.GreenChannel[i]) * (taskHistogram.GreenChannel[i] - renderedHistogram.GreenChannel[i])))
                   / (float)(taskHistogram.GreenChannel[i] + renderedHistogram.GreenChannel[i]));
    }

    if (taskHistogram.BlueChannel[i] + renderedHistogram.BlueChannel[i] != 0) {
      blueSum += ((float)(((taskHistogram.BlueChannel[i] - renderedHistogram.BlueChannel[i]) * (taskHistogram.BlueChannel[i] - renderedHistogram.BlueChannel[i])))
                  / (float)(taskHistogram.BlueChannel[i] + renderedHistogram.BlueChannel[i]));
    }

    if (taskHistogram.AlphaChannel[i] + renderedHistogram.AlphaChannel[i] != 0) {
      alphaSum += ((float)(((taskHistogram.AlphaChannel[i] - renderedHistogram.AlphaChannel[i]) * (taskHistogram.AlphaChannel[i] - renderedHistogram.AlphaChannel[i])))
                   / (float)(taskHistogram.AlphaChannel[i] + renderedHistogram.AlphaChannel[i]));
    }
  }

  return redSum + greenSum + blueSum + alphaSum;
}

double ImageComparator::CalculateSSIMForColorChannel(const std::vector<uint8_t> &taskTextureColor,
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

double ImageComparator::CalculateSSIM(const std::vector<uint8_t>& taskTextureData, const std::vector<uint8_t>& renderedTextureData) {
  std::vector<uint8_t> taskRedColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::RED);
  std::vector<uint8_t> taskGreenColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::GREEN);
  std::vector<uint8_t> taskBlueColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::BLUE);
  std::vector<uint8_t> taskAlphaColorChannel = ExtractColorChannel(taskTextureData, RGBAComponent::ALPHA);

  std::vector<uint8_t> renderedRedColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::RED);
  std::vector<uint8_t> renderedGreenColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::GREEN);
  std::vector<uint8_t> renderedBlueColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::BLUE);
  std::vector<uint8_t> renderedAlphaColorChannel = ExtractColorChannel(renderedTextureData, RGBAComponent::ALPHA);

  double redSSIM = CalculateSSIMForColorChannel(taskRedColorChannel, renderedRedColorChannel);
  double greenSSIM = CalculateSSIMForColorChannel(taskGreenColorChannel, renderedGreenColorChannel);
  double blueSSIM = CalculateSSIMForColorChannel(taskBlueColorChannel, renderedBlueColorChannel);
  double alphaSSIM = CalculateSSIMForColorChannel(taskAlphaColorChannel, renderedAlphaColorChannel);

  return redSSIM + greenSSIM + blueSSIM + alphaSSIM;
}


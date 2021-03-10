#include <utility>
#include "ImageComparator.h"

ImageComparator::ImageComparator(std::vector<uint8_t> taskTextureData, std::vector<uint8_t> renderedTextureData) :
  m_TaskTextureData(std::move(taskTextureData)),
  m_RenderedTextureData(std::move(renderedTextureData))
{
}

ImageComparator::~ImageComparator() {
}


float ImageComparator::CompareImages() {
  return CalculateChiSquareDistance(m_TaskTextureData, m_RenderedTextureData);
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

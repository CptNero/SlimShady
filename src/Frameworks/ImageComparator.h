#ifndef SLIMSHADY_IMAGECOMPARATOR_H
#define SLIMSHADY_IMAGECOMPARATOR_H


#include <GL/glew.h>
#include <vector>
#include <sstream>

class ImageComparator {

public:
    ImageComparator();
    ~ImageComparator();

    double CalculateChiSquareDistance(std::vector<uint8_t>& taskTextureData, std::vector<uint8_t>& renderedTextureData);
    double CalculateSSIM(std::vector<uint8_t>& taskTextureData, std::vector<uint8_t>& renderedTextureData);

private:

    enum RGBAComponent {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
        ALPHA = 3
    };

    struct TextureHistogram {
        double RedChannel[256] = {0};
        double GreenChannel[256] = {0};
        double BlueChannel[256] = {0};
        double AlphaChannel[256] = {0};

        std::string toString() {
          std::stringstream redChannelStream;
          std::stringstream greenChannelStream;
          std::stringstream blueChannelStream;
          std::stringstream alphaChannelStream;

          redChannelStream << "Red: ";
          greenChannelStream << "Green: ";
          blueChannelStream << "Blue: ";
          alphaChannelStream << "Alpha: ";

          for (int i = 0; i < 255; i++) {
            redChannelStream << RedChannel[i] << " ";
            greenChannelStream << GreenChannel[i] << " ";
            blueChannelStream << BlueChannel[i] << " ";
            alphaChannelStream << AlphaChannel[i] << " ";
          }

          redChannelStream << '\n';
          greenChannelStream << '\n';
          blueChannelStream << '\n';
          alphaChannelStream << '\n';

          return redChannelStream.str() + greenChannelStream.str() + blueChannelStream.str() + alphaChannelStream.str();
        }
    };

    std::vector<uint8_t> ExtractColorChannel(const std::vector<uint8_t>& textureData, RGBAComponent component);
    double CalculateChiSquareDistanceForColorValue(double taskColorValue, double renderedColorValue);
    double CalculateSSIMForColorChannel(const std::vector<uint8_t> &taskTextureColor, const std::vector<uint8_t> &renderedTextureColor);
    TextureHistogram CreateTextureHistogram(std::vector<uint8_t>& textureData);
};


#endif //SLIMSHADY_IMAGECOMPARATOR_H

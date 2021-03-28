#ifndef SLIMSHADY_IMAGECOMPARATOR_H
#define SLIMSHADY_IMAGECOMPARATOR_H


#include <GL/glew.h>
#include <vector>
#include <sstream>

class ImageComparator {

public:
    std::vector<uint8_t> m_TaskTextureData;
    std::vector<uint8_t> m_RenderedTextureData;

    ImageComparator(std::vector<uint8_t> taskTextureData, std::vector<uint8_t> renderedTextureData);
    ~ImageComparator();

    double CompareImages();
    float CalculateChiSquareDistance(const std::vector<uint8_t>& taskTextureData, const std::vector<uint8_t>& renderedTextureData);
    double CalculateSSIM(const std::vector<uint8_t>& taskTextureData, const std::vector<uint8_t>& renderedTextureData);
private:

    enum RGBAComponent {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
        ALPHA = 3
    };

    struct TextureHistogram {
        int RedChannel[256] = {0};
        int GreenChannel[256] = {0};
        int BlueChannel[256] = {0};
        int AlphaChannel[256] = {0};

        std::string toString() {
          std::stringstream redChannelStream;
          std::stringstream greenChannelStream;
          std::stringstream blueChannelStream;
          std::stringstream alphaChannelStream;

          redChannelStream << "Red: ";
          greenChannelStream << "Green: ";
          blueChannelStream << "Blue: ";
          alphaChannelStream << "Alpha: ";

          for (int i = 0; i < 256; i++) {
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
    double CalculateSSIMForColorChannel(const std::vector<uint8_t> &taskTextureColor, const std::vector<uint8_t> &renderedTextureColor);
    TextureHistogram CreateTextureHistogram(std::vector<uint8_t> textureData);


};


#endif //SLIMSHADY_IMAGECOMPARATOR_H

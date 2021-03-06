/*
 * Copyright (c) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @brief a header file with common samples functionality
 * @file common.hpp
 */

#pragma once

#include <time.h>
#include <cctype>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <algorithm>
#include <chrono>

///#include <cpp/ie_cnn_net_reader.h>
///#include <cpp/ie_infer_request.hpp>
/// #include <ie_device.hpp>
///#include <ie_plugin_dispatcher.hpp>
///#include <ie_plugin_ptr.hpp>

#ifndef UNUSED
#ifdef WIN32
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif
#endif

/**
 * @brief Trims from both ends (in place)
 * @param s - string to trim
 * @return trimmed string
 */
inline std::string& trim(std::string& s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// TargetDevice is not in openvino2020.3.341
#if 0 
/**
* @brief Converts string to TargetDevice
* @param deviceName - string value representing device
* @return TargetDevice value that corresponds to input string.
*         eDefault in case no corresponding value was found
*/
static InferenceEngine::TargetDevice getDeviceFromStr(const std::string& deviceName)
{
  return InferenceEngine::TargetDeviceInfo::fromStr(deviceName);
}

/**
* @brief Loads plugin from directories
* @param pluginDirs - plugin paths
* @param plugin - plugin name
* @param device - device to infer on
* @return Plugin pointer
*/
static InferenceEngine::InferenceEnginePluginPtr selectPlugin(const std::vector<std::string>& pluginDirs,
                                                              const std::string& plugin,
                                                              InferenceEngine::TargetDevice device)
{
  InferenceEngine::PluginDispatcher dispatcher(pluginDirs);

  if (!plugin.empty())
  {
    return dispatcher.getPluginByName(plugin);
  }
  else
  {
    return dispatcher.getSuitablePlugin(device);
  }
}

/**
 * @brief Loads plugin from directories
 * @param pluginDirs - plugin paths
 * @param plugin - plugin name
 * @param device - string representation of device to infer on
 * @return Plugin pointer
 */
static UNUSED InferenceEngine::InferenceEnginePluginPtr selectPlugin(const std::vector<std::string>& pluginDirs,
                                                                     const std::string& plugin,
                                                                     const std::string& device)
{
  return selectPlugin(pluginDirs, plugin, getDeviceFromStr(device));
}
#endif

/**
 * @brief Gets filename without extension
 * @param filepath - full file name
 * @return filename without extension
 */
static UNUSED std::string fileNameNoExt(const std::string& filepath)
{
  auto pos = filepath.rfind('.');
  if (pos == std::string::npos)
    return filepath;
  return filepath.substr(0, pos);
}

/**
* @brief Get extension from filename
* @param filename - name of the file which extension should be extracted
* @return string with extracted file extension
*/
inline std::string fileExt(const std::string& filename)
{
  auto pos = filename.rfind('.');
  if (pos == std::string::npos)
    return "";
  return filename.substr(pos + 1);
}

/*
static UNUSED std::ostream& operator<<(std::ostream& os, const InferenceEngine::Version* version)
{
  os << "\n\tAPI version ............ ";
  if (nullptr == version)
  {
    os << "UNKNOWN";
  }
  else
  {
    os << version->apiVersion.major << "." << version->apiVersion.minor;
    if (nullptr != version->buildNumber)
    {
      os << "\n\t"
         << "Build .................. " << version->buildNumber;
    }
    if (nullptr != version->description)
    {
      os << "\n\t"
         << "Description ....... " << version->description;
    }
  }
  return os;
}

*/
/**
 * @class PluginVersion
 * @brief A PluginVersion class stores plugin version and initialization status
 */
/*
struct PluginVersion : public InferenceEngine::Version
{
  bool initialized = false;

  explicit PluginVersion(const InferenceEngine::Version* ver)
  {
    if (nullptr == ver)
    {
      return;
    }
    InferenceEngine::Version::operator=(*ver);
    initialized = true;
  }

  operator bool() const noexcept
  {
    return initialized;
  }
};

static UNUSED std::ostream& operator<<(std::ostream& os, const PluginVersion& version)
{
  os << "\tPlugin version ......... ";
  if (!version)
  {
    os << "UNKNOWN";
  }
  else
  {
    os << version.apiVersion.major << "." << version.apiVersion.minor;
  }

  os << "\n\tPlugin name ............ ";
  if (!version || version.description == nullptr)
  {
    os << "UNKNOWN";
  }
  else
  {
    os << version.description;
  }

  os << "\n\tPlugin build ........... ";
  if (!version || version.buildNumber == nullptr)
  {
    os << "UNKNOWN";
  }
  else
  {
    os << version.buildNumber;
  }

  return os;
}

inline void printPluginVersion(InferenceEngine::InferenceEnginePluginPtr ptr, std::ostream& stream)
{
  const PluginVersion* pluginVersion;
  ptr->GetVersion((const InferenceEngine::Version*&)pluginVersion);
  stream << pluginVersion << std::endl;
}

*/
// TargetDevice is not in openvino2020
#if 0
static UNUSED std::vector<std::vector<size_t>> blobToImageOutputArray(InferenceEngine::TBlob<float>::Ptr output,
                                                                      size_t* pWidth, size_t* pHeight,
                                                                      size_t* pChannels)
{
  std::vector<std::vector<size_t>> outArray;
  size_t W = output->dims().at(0);
  size_t H = output->dims().at(1);
  size_t C = output->dims().at(2);

  // Get classes
  const float* outData = output->data();
  for (unsigned h = 0; h < H; h++)
  {
    std::vector<size_t> row;
    for (unsigned w = 0; w < W; w++)
    {
      float max_value = outData[h * W + w];
      size_t index = 0;
      for (size_t c = 1; c < C; c++)
      {
        size_t dataIndex = c * H * W + h * W + w;
        if (outData[dataIndex] > max_value)
        {
          index = c;
          max_value = outData[dataIndex];
        }
      }
      row.push_back(index);
    }
    outArray.push_back(row);
  }

  if (pWidth != nullptr)
    *pWidth = W;
  if (pHeight != nullptr)
    *pHeight = H;
  if (pChannels != nullptr)
    *pChannels = C;

  return outArray;
}
#endif

/**
 * @class Color
 * @brief A Color class stores channels of a given color
 */
class Color
{
private:
  unsigned char _r;
  unsigned char _g;
  unsigned char _b;

public:
  /**
   * A default constructor.
   * @param r - value for red channel
   * @param g - value for green channel
   * @param b - value for blue channel
   */
  Color(unsigned char r, unsigned char g, unsigned char b) : _r(r), _g(g), _b(b)
  {
  }

  inline unsigned char red()
  {
    return _r;
  }

  inline unsigned char blue()
  {
    return _b;
  }

  inline unsigned char green()
  {
    return _g;
  }
};

// TODO : keep only one version of writeOutputBMP

/**
 * @brief Writes output data to image
 * @param name - image name
 * @param data - output data
 * @param classesNum - the number of classes
 * @return false if error else true
 */
static UNUSED void writeOutputBmp(std::vector<std::vector<size_t>> data, size_t classesNum, std::ostream& outFile)
{
  unsigned int seed = (unsigned int)time(NULL);
  // Known colors for training classes from Cityscape dataset
  static std::vector<Color> colors = { { 128, 64, 128 },  { 232, 35, 244 },  { 70, 70, 70 },   { 156, 102, 102 },
                                       { 153, 153, 190 }, { 153, 153, 153 }, { 30, 170, 250 }, { 0, 220, 220 },
                                       { 35, 142, 107 },  { 152, 251, 152 }, { 180, 130, 70 }, { 60, 20, 220 },
                                       { 0, 0, 255 },     { 142, 0, 0 },     { 70, 0, 0 },     { 100, 60, 0 },
                                       { 90, 0, 0 },      { 230, 0, 0 },     { 32, 11, 119 },  { 0, 74, 111 },
                                       { 81, 0, 81 } };

  while (classesNum > colors.size())
  {
    static std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 255);
    Color color(dist(rng), dist(rng), dist(rng));
    colors.push_back(color);
  }

  unsigned char file[14] = {
    'B',     'M',        // magic
    0,       0,   0, 0,  // size in bytes
    0,       0,          // app data
    0,       0,          // app data
    40 + 14, 0,   0, 0   // start of data offset
  };
  unsigned char info[40] = {
    40,   0,    0, 0,  // info hd size
    0,    0,    0, 0,  // width
    0,    0,    0, 0,  // height
    1,    0,           // number color planes
    24,   0,           // bits per pixel
    0,    0,    0, 0,  // compression is none
    0,    0,    0, 0,  // image bits size
    0x13, 0x0B, 0, 0,  // horz resolution in pixel / m
    0x13, 0x0B, 0, 0,  // vert resolution (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
    0,    0,    0, 0,  // #colors in palette
    0,    0,    0, 0,  // #important colors
  };

  auto height = data.size();
  auto width = data.at(0).size();

/*
  if (height > (size_t)std::numeric_limits<int32_t>::max || width > (size_t)std::numeric_limits<int32_t>::max)
  {
    THROW_IE_EXCEPTION << "File size is too big: " << height << " X " << width;
  }
*/
  int padSize = static_cast<int>(4 - (width * 3) % 4) % 4;
  int sizeData = static_cast<int>(width * height * 3 + height * padSize);
  int sizeAll = sizeData + sizeof(file) + sizeof(info);

  file[2] = (unsigned char)(sizeAll);
  file[3] = (unsigned char)(sizeAll >> 8);
  file[4] = (unsigned char)(sizeAll >> 16);
  file[5] = (unsigned char)(sizeAll >> 24);

  info[4] = (unsigned char)(width);
  info[5] = (unsigned char)(width >> 8);
  info[6] = (unsigned char)(width >> 16);
  info[7] = (unsigned char)(width >> 24);

  int32_t negativeHeight = -(int32_t)height;
  info[8] = (unsigned char)(negativeHeight);
  info[9] = (unsigned char)(negativeHeight >> 8);
  info[10] = (unsigned char)(negativeHeight >> 16);
  info[11] = (unsigned char)(negativeHeight >> 24);

  info[20] = (unsigned char)(sizeData);
  info[21] = (unsigned char)(sizeData >> 8);
  info[22] = (unsigned char)(sizeData >> 16);
  info[23] = (unsigned char)(sizeData >> 24);

  outFile.write(reinterpret_cast<char*>(file), sizeof(file));
  outFile.write(reinterpret_cast<char*>(info), sizeof(info));

  unsigned char pad[3] = { 0, 0, 0 };

  for (size_t y = 0; y < height; y++)
  {
    for (size_t x = 0; x < width; x++)
    {
      unsigned char pixel[3];
      size_t index = data.at(y).at(x);
      pixel[0] = colors.at(index).red();
      pixel[1] = colors.at(index).green();
      pixel[2] = colors.at(index).blue();
      outFile.write(reinterpret_cast<char*>(pixel), 3);
    }
    outFile.write(reinterpret_cast<char*>(pad), padSize);
  }
}

/**
* @brief Writes output data to BMP image
* @param name - image name
* @param data - output data
* @param height - height of the target image
* @param width - width of the target image
* @return false if error else true
*/
static UNUSED bool writeOutputBmp(std::string name, unsigned char* data, size_t height, size_t width)
{
  std::ofstream outFile;
  outFile.open(name, std::ofstream::binary);
  if (!outFile.is_open())
  {
    return false;
  }

  unsigned char file[14] = {
    'B',     'M',        // magic
    0,       0,   0, 0,  // size in bytes
    0,       0,          // app data
    0,       0,          // app data
    40 + 14, 0,   0, 0   // start of data offset
  };
  unsigned char info[40] = {
    40,   0,    0, 0,  // info hd size
    0,    0,    0, 0,  // width
    0,    0,    0, 0,  // height
    1,    0,           // number color planes
    24,   0,           // bits per pixel
    0,    0,    0, 0,  // compression is none
    0,    0,    0, 0,  // image bits size
    0x13, 0x0B, 0, 0,  // horz resolution in pixel / m
    0x13, 0x0B, 0, 0,  // vert resolution (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
    0,    0,    0, 0,  // #colors in palette
    0,    0,    0, 0,  // #important colors
  };
/*
  if (height > (size_t)std::numeric_limits<int32_t>::max || width > (size_t)std::numeric_limits<int32_t>::max)
  {
    THROW_IE_EXCEPTION << "File size is too big: " << height << " X " << width;
  }
*/
  int padSize = static_cast<int>(4 - (width * 3) % 4) % 4;
  int sizeData = static_cast<int>(width * height * 3 + height * padSize);
  int sizeAll = sizeData + sizeof(file) + sizeof(info);

  file[2] = (unsigned char)(sizeAll);
  file[3] = (unsigned char)(sizeAll >> 8);
  file[4] = (unsigned char)(sizeAll >> 16);
  file[5] = (unsigned char)(sizeAll >> 24);

  info[4] = (unsigned char)(width);
  info[5] = (unsigned char)(width >> 8);
  info[6] = (unsigned char)(width >> 16);
  info[7] = (unsigned char)(width >> 24);

  int32_t negativeHeight = -(int32_t)height;
  info[8] = (unsigned char)(negativeHeight);
  info[9] = (unsigned char)(negativeHeight >> 8);
  info[10] = (unsigned char)(negativeHeight >> 16);
  info[11] = (unsigned char)(negativeHeight >> 24);

  info[20] = (unsigned char)(sizeData);
  info[21] = (unsigned char)(sizeData >> 8);
  info[22] = (unsigned char)(sizeData >> 16);
  info[23] = (unsigned char)(sizeData >> 24);

  outFile.write(reinterpret_cast<char*>(file), sizeof(file));
  outFile.write(reinterpret_cast<char*>(info), sizeof(info));

  unsigned char pad[3] = { 0, 0, 0 };

  for (size_t y = 0; y < height; y++)
  {
    for (size_t x = 0; x < width; x++)
    {
      unsigned char pixel[3];
      pixel[0] = data[y * width * 3 + x * 3];
      pixel[1] = data[y * width * 3 + x * 3 + 1];
      pixel[2] = data[y * width * 3 + x * 3 + 2];

      outFile.write(reinterpret_cast<char*>(pixel), 3);
    }
    outFile.write(reinterpret_cast<char*>(pad), padSize);
  }
  return true;
}

/**
 * Write output data to image
 * \param name - image name
 * \param data - output data
 * \param classesNum - the number of classes
 * \return false if error else true
 */

static UNUSED bool writeOutputBmp(unsigned char* data, size_t height, size_t width, std::ostream& outFile)
{
  unsigned char file[14] = {
    'B',     'M',        // magic
    0,       0,   0, 0,  // size in bytes
    0,       0,          // app data
    0,       0,          // app data
    40 + 14, 0,   0, 0   // start of data offset
  };
  unsigned char info[40] = {
    40,   0,    0, 0,  // info hd size
    0,    0,    0, 0,  // width
    0,    0,    0, 0,  // height
    1,    0,           // number color planes
    24,   0,           // bits per pixel
    0,    0,    0, 0,  // compression is none
    0,    0,    0, 0,  // image bits size
    0x13, 0x0B, 0, 0,  // horz resolution in pixel / m
    0x13, 0x0B, 0, 0,  // vert resolution (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
    0,    0,    0, 0,  // #colors in palette
    0,    0,    0, 0,  // #important colors
  };
/*
  if (height > (size_t)std::numeric_limits<int32_t>::max || width > (size_t)std::numeric_limits<int32_t>::max)
  {
    THROW_IE_EXCEPTION << "File size is too big: " << height << " X " << width;
  }
*/
  int padSize = static_cast<int>(4 - (width * 3) % 4) % 4;
  int sizeData = static_cast<int>(width * height * 3 + height * padSize);
  int sizeAll = sizeData + sizeof(file) + sizeof(info);

  file[2] = (unsigned char)(sizeAll);
  file[3] = (unsigned char)(sizeAll >> 8);
  file[4] = (unsigned char)(sizeAll >> 16);
  file[5] = (unsigned char)(sizeAll >> 24);

  info[4] = (unsigned char)(width);
  info[5] = (unsigned char)(width >> 8);
  info[6] = (unsigned char)(width >> 16);
  info[7] = (unsigned char)(width >> 24);

  int32_t negativeHeight = -(int32_t)height;
  info[8] = (unsigned char)(negativeHeight);
  info[9] = (unsigned char)(negativeHeight >> 8);
  info[10] = (unsigned char)(negativeHeight >> 16);
  info[11] = (unsigned char)(negativeHeight >> 24);

  info[20] = (unsigned char)(sizeData);
  info[21] = (unsigned char)(sizeData >> 8);
  info[22] = (unsigned char)(sizeData >> 16);
  info[23] = (unsigned char)(sizeData >> 24);

  outFile.write(reinterpret_cast<char*>(file), sizeof(file));
  outFile.write(reinterpret_cast<char*>(info), sizeof(info));

  unsigned char pad[3] = { 0, 0, 0 };

  for (size_t y = 0; y < height; y++)
  {
    for (size_t x = 0; x < width; x++)
    {
      unsigned char pixel[3];
      pixel[0] = data[y * width * 3 + x * 3];
      pixel[1] = data[y * width * 3 + x * 3 + 1];
      pixel[2] = data[y * width * 3 + x * 3 + 2];
      outFile.write(reinterpret_cast<char*>(pixel), 3);
    }
    outFile.write(reinterpret_cast<char*>(pad), padSize);
  }

  return true;
}

inline double getDurationOf(std::function<void()> func)
{
  auto t0 = std::chrono::high_resolution_clock::now();
  func();
  auto t1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> fs = t1 - t0;
  return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1000>>>(fs).count();
}

/*
static UNUSED void
printPerformanceCounts(const std::map<std::string, InferenceEngine::InferenceEngineProfileInfo>& performanceMap,
                       std::ostream& stream, bool bshowHeader = true)
{
  long long totalTime = 0;
  // Print performance counts
  if (bshowHeader)
  {
    stream << std::endl << "performance counts:" << std::endl << std::endl;
  }
  for (const auto& it : performanceMap)
  {
    std::string toPrint(it.first);
    const int maxLayerName = 30;

    if (it.first.length() >= maxLayerName)
    {
      toPrint = it.first.substr(0, maxLayerName - 4);
      toPrint += "...";
    }

    stream << std::setw(maxLayerName) << std::left << toPrint;
    switch (it.second.status)
    {
      case InferenceEngine::InferenceEngineProfileInfo::EXECUTED:
        stream << std::setw(15) << std::left << "EXECUTED";
        break;
      case InferenceEngine::InferenceEngineProfileInfo::NOT_RUN:
        stream << std::setw(15) << std::left << "NOT_RUN";
        break;
      case InferenceEngine::InferenceEngineProfileInfo::OPTIMIZED_OUT:
        stream << std::setw(15) << std::left << "OPTIMIZED_OUT";
        break;
    }
    stream << std::setw(30) << std::left << "layerType: " + std::string(it.second.layer_type) + " ";
    stream << std::setw(20) << std::left << "realTime: " + std::to_string(it.second.realTime_uSec);
    stream << std::setw(20) << std::left << " cpu: " + std::to_string(it.second.cpu_uSec);
    stream << " execType: " << it.second.exec_type << std::endl;
    if (it.second.realTime_uSec > 0)
    {
      totalTime += it.second.realTime_uSec;
    }
  }
  stream << std::setw(20) << std::left << "Total time: " + std::to_string(totalTime) << " microseconds" << std::endl;
}

static UNUSED void printPerformanceCounts(InferenceEngine::InferRequest request, std::ostream& stream)
{
  auto perfomanceMap = request.GetPerformanceCounts();
  printPerformanceCounts(perfomanceMap, stream);
}
*/

// TargetDevice is not in openvino2020
#if 0
/**
 * @deprecated
 */
static UNUSED void printPerformanceCountsPlugin(InferenceEngine::InferenceEnginePluginPtr plugin, std::ostream& stream)
{
  std::map<std::string, InferenceEngine::InferenceEngineProfileInfo> perfomanceMap;
  plugin->GetPerformanceCounts(perfomanceMap, nullptr);
  printPerformanceCounts(perfomanceMap, stream);
}
#endif

/**
 * @brief This class represents an object that is found by an object detection
 * net
 */
class DetectedObject
{
public:
  int objectType;
  float xmin, xmax, ymin, ymax, prob;
  bool difficult;

  DetectedObject(int objectType, float xmin, float ymin, float xmax, float ymax, float prob, bool difficult = false)
    : objectType(objectType), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), prob(prob), difficult(difficult)
  {
  }

  DetectedObject(const DetectedObject& other) = default;

  static float ioU(const DetectedObject& detectedObject1_, const DetectedObject& detectedObject2_)
  {
    // Add small space to eliminate empty squares
    float epsilon = 0;  // 1e-5f;

    DetectedObject detectedObject1(detectedObject1_.objectType, (detectedObject1_.xmin - epsilon),
                                   (detectedObject1_.ymin - epsilon), (detectedObject1_.xmax - epsilon),
                                   (detectedObject1_.ymax - epsilon), detectedObject1_.prob);
    DetectedObject detectedObject2(detectedObject2_.objectType, (detectedObject2_.xmin + epsilon),
                                   (detectedObject2_.ymin + epsilon), (detectedObject2_.xmax), (detectedObject2_.ymax),
                                   detectedObject2_.prob);

    if (detectedObject1.objectType != detectedObject2.objectType)
    {
      // objects are different, so the result is 0
      return 0.0f;
    }

    if (detectedObject1.xmax < detectedObject1.xmin)
      return 0.0;
    if (detectedObject1.ymax < detectedObject1.ymin)
      return 0.0;
    if (detectedObject2.xmax < detectedObject2.xmin)
      return 0.0;
    if (detectedObject2.ymax < detectedObject2.ymin)
      return 0.0;

    float xmin = (std::max)(detectedObject1.xmin, detectedObject2.xmin);
    float ymin = (std::max)(detectedObject1.ymin, detectedObject2.ymin);
    float xmax = (std::min)(detectedObject1.xmax, detectedObject2.xmax);
    float ymax = (std::min)(detectedObject1.ymax, detectedObject2.ymax);

    // Caffe adds 1 to every length if the box isn't normalized. So do we...
    float addendum;
    if (xmax > 1 || ymax > 1)
      addendum = 1;
    else
      addendum = 0;

    // intersection
    float intr;
    if ((xmax >= xmin) && (ymax >= ymin))
    {
      intr = (addendum + xmax - xmin) * (addendum + ymax - ymin);
    }
    else
    {
      intr = 0.0f;
    }

    // union
    float square1 = (addendum + detectedObject1.xmax - detectedObject1.xmin) *
                    (addendum + detectedObject1.ymax - detectedObject1.ymin);
    float square2 = (addendum + detectedObject2.xmax - detectedObject2.xmin) *
                    (addendum + detectedObject2.ymax - detectedObject2.ymin);

    float unn = square1 + square2 - intr;

    return static_cast<float>(intr) / unn;
  }

  DetectedObject scale(float scale_x, float scale_y) const
  {
    return DetectedObject(objectType, xmin * scale_x, ymin * scale_y, xmax * scale_x, ymax * scale_y, prob, difficult);
  }
};

class ImageDescription
{
public:
  const std::list<DetectedObject> alist;
  const bool check_probs;

  explicit ImageDescription(const std::list<DetectedObject>& alist, bool check_probs = false)
    : alist(alist), check_probs(check_probs)
  {
  }

  ImageDescription scale(float scale_x, float scale_y) const
  {
    std::list<DetectedObject> slist;
    for (auto& dob : alist)
    {
      slist.push_back(dob.scale(scale_x, scale_y));
    }
    return ImageDescription(slist, check_probs);
  }
};

struct AveragePrecisionCalculator
{
private:
  enum MatchKind
  {
    TruePositive,
    FalsePositive
  };

  /**
   * Here we count all TP and FP matches for all the classes in all the images.
   */
  std::map<int, std::vector<std::pair<double, MatchKind>>> matches;

  std::map<int, int> N;

  double threshold;

  static bool SortBBoxDescend(const DetectedObject& bbox1, const DetectedObject& bbox2)
  {
    return bbox1.prob > bbox2.prob;
  }

  static bool SortPairDescend(const std::pair<double, MatchKind>& p1, const std::pair<double, MatchKind>& p2)
  {
    return p1.first > p2.first;
  }

public:
  AveragePrecisionCalculator(double threshold) : threshold(threshold)
  {
  }

  // gt_bboxes -> des
  // bboxes -> det
};

/**
* @brief Adds colored rectangles to the image
* @param data - data where rectangles are put
* @param height - height of the rectangle
* @param width - width of the rectangle
* @param detectedObjects - vector of detected objects
*/
static UNUSED void addRectangles(unsigned char* data, size_t height, size_t width,
                                 std::vector<DetectedObject> detectedObjects)
{
  std::vector<Color> colors = { { 128, 64, 128 },  { 232, 35, 244 },  { 70, 70, 70 },   { 156, 102, 102 },
                                { 153, 153, 190 }, { 153, 153, 153 }, { 30, 170, 250 }, { 0, 220, 220 },
                                { 35, 142, 107 },  { 152, 251, 152 }, { 180, 130, 70 }, { 60, 20, 220 },
                                { 0, 0, 255 },     { 142, 0, 0 },     { 70, 0, 0 },     { 100, 60, 0 },
                                { 90, 0, 0 },      { 230, 0, 0 },     { 32, 11, 119 },  { 0, 74, 111 },
                                { 81, 0, 81 } };

  for (size_t i = 0; i < detectedObjects.size(); i++)
  {
    int cls = detectedObjects[i].objectType % colors.size();

    int xmin = detectedObjects[i].xmin * width;
    int xmax = detectedObjects[i].xmax * width;
    int ymin = detectedObjects[i].ymin * height;
    int ymax = detectedObjects[i].ymax * height;

    size_t shift_first = ymin * width * 3;
    size_t shift_second = ymax * width * 3;
    for (int x = xmin; x < xmax; x++)
    {
      data[shift_first + x * 3] = colors.at(cls).red();
      data[shift_first + x * 3 + 1] = colors.at(cls).green();
      data[shift_first + x * 3 + 2] = colors.at(cls).blue();
      data[shift_second + x * 3] = colors.at(cls).red();
      data[shift_second + x * 3 + 1] = colors.at(cls).green();
      data[shift_second + x * 3 + 2] = colors.at(cls).blue();
    }

    shift_first = xmin * 3;
    shift_second = xmax * 3;
    for (int y = ymin; y < ymax; y++)
    {
      data[shift_first + y * width * 3] = colors.at(cls).red();
      data[shift_first + y * width * 3 + 1] = colors.at(cls).green();
      data[shift_first + y * width * 3 + 2] = colors.at(cls).blue();
      data[shift_second + y * width * 3] = colors.at(cls).red();
      data[shift_second + y * width * 3 + 1] = colors.at(cls).green();
      data[shift_second + y * width * 3 + 2] = colors.at(cls).blue();
    }
  }
}

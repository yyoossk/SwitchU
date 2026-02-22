#include "Texture.hpp"
#include "GpuDevice.hpp"
#include "Renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdio>
#include <cstring>

namespace ui {

bool Texture::loadFromPixels(GpuDevice& gpu, Renderer& ren,
                             const uint8_t* rgba, int w, int h)
{
    m_width  = w;
    m_height = h;

    dk::ImageLayout layout;
    dk::ImageLayoutMaker{gpu.device()}
        .setFlags(0)
        .setFormat(DkImageFormat_RGBA8_Unorm)
        .setDimensions(w, h)
        .initialize(layout);

    m_mem = gpu.allocImageMemory(layout.getSize());
    m_image.initialize(layout, m_mem, 0);

    gpu.uploadTexture(m_image, rgba, w * h * 4, w, h);

    dk::ImageView view{m_image};
    m_slot = ren.registerTexture(view);
    m_valid = true;
    return true;
}

bool Texture::loadFromFile(GpuDevice& gpu, Renderer& ren, const std::string& path) {
    int w, h, ch;
    uint8_t* data = stbi_load(path.c_str(), &w, &h, &ch, 4);
    if (!data) {
        std::printf("[Texture] stbi_load FAILED: %s\n", path.c_str());
        return false;
    }
    bool ok = loadFromPixels(gpu, ren, data, w, h);
    stbi_image_free(data);
    return ok;
}

bool Texture::loadFromMemory(GpuDevice& gpu, Renderer& ren,
                             const uint8_t* data, size_t dataSize)
{
    int w, h, ch;
    uint8_t* pixels = stbi_load_from_memory(data, (int)dataSize, &w, &h, &ch, 4);
    if (!pixels) return false;
    bool ok = loadFromPixels(gpu, ren, pixels, w, h);
    stbi_image_free(pixels);
    return ok;
}

bool Texture::loadFromSurface(GpuDevice& gpu, Renderer& ren,
                              const uint8_t* data, int w, int h, int pitch)
{
    // Convert to tightly packed RGBA if pitch != w*4
    if (pitch == w * 4) {
        return loadFromPixels(gpu, ren, data, w, h);
    }
    std::vector<uint8_t> tight(w * h * 4);
    for (int y = 0; y < h; ++y)
        std::memcpy(tight.data() + y * w * 4, data + y * pitch, w * 4);
    return loadFromPixels(gpu, ren, tight.data(), w, h);
}

} // namespace ui

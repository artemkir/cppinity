#pragma once

#include <Engine.h>

#include <catch2/catch_all.hpp>

class MockShaderManager : public ShaderManager {
public:
};

class MockMaterialManager : public MaterialManager {
public:
    MockMaterialManager() : MaterialManager(&mockShaderManager) {}
private:
    MockShaderManager mockShaderManager;
};

class MockResourceManager : public ResourceManager {
public:
    MockResourceManager(IRenderer* renderer) : ResourceManager(renderer) {}
};

class MockRenderer : public IRenderer {
public:
    void BeginPass() const override {}
    void DrawRect(float x, float y, float w, float h, float col[4]) const override {}
    void EndPass() const override {}
    int GetW() const override { return 800; }
    int GetH() const override { return 600; }
    float GetFW() const override { return 800.0f; }
    float GetFH() const override { return 600.0f; }
    TextureData CreateRGBATextureFromPixelData(uint32_t view, int width, int height, const uint8_t* pixelData) const override {
        return TextureData();
    }
    TextureData CreateTextureFromGrayscalePixelData(uint32_t view, int width, int height, const uint8_t* pixelData) const override {
        return TextureData();
    }
    TextureData CreateTextureFromMemoryFile(uint32_t view, const uint8_t* data, size_t size) const override {
        return TextureData();
    }
    TextureData PreallocateEmptyTexture() const override {
        return TextureData();
    }
    void DestroyTexture(const TextureData& texture) const override {}
    void ApplyMaterial(const Material* material) override {}
    void ApplyTexture(const SharedPtr<Texture>& texture) override {}
    void Draw(int num_elements) override {}
};

class SceneTestFixture {
public:
    MockRenderer renderer;
    MockMaterialManager materialManager;
    MockResourceManager resourceManager;
    Scene scene;

    SceneTestFixture()
        : resourceManager(&renderer)
        , materialManager()
        , scene(&renderer, &materialManager, &resourceManager) {
    }
};
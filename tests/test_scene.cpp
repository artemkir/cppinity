// tests/test_scene.cpp
#include <iostream>
#include <catch2/catch_all.hpp>
#include <Engine.h> // Assumes Engine.h includes Scene.h, GameObject.h, MaterialManager.h, etc.
#include <memory>
#include <windows.h>

// Mock ShaderManager (minimal implementation)
class MockShaderManager : public ShaderManager {
public:
    // Add methods if required by MaterialManager or tests
};

// Mock MaterialManager
class MockMaterialManager : public MaterialManager {
public:
    MockMaterialManager() : MaterialManager(&mockShaderManager) {}
private:
    MockShaderManager mockShaderManager;
};

// Mock ResourceManager
class MockResourceManager : public ResourceManager {
public:
    MockResourceManager(IRenderer* renderer) : ResourceManager(renderer) {}
};

// Mock IRenderer with all required methods
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

// Custom component to add GameObjects during Update
class TestComponent : public BaseComponent {
public:
    using AddCallback = std::function<void(Scene*)>;
    AddCallback onUpdate;

    TestComponent(AddCallback callback) : onUpdate(std::move(callback)) {}
    void Awake() override {}
    void Start() override {}
    void Update(float deltaTime) override {
        if (onUpdate) {
            onUpdate(gameObject->GetScene());
        }
    }
    void OnCollide(GameObject* other) override {}
};

// Test fixture for Scene
class SceneTestFixture {
public:
    MockRenderer renderer;
    MockMaterialManager materialManager;
    MockResourceManager resourceManager;
    Scene scene;

    SceneTestFixture()
        : resourceManager(&renderer)    // Initialize MockResourceManager with renderer
        , materialManager()             // Already initialized with MockShaderManager
        , scene(&renderer, &materialManager, &resourceManager) {}
};

void gameInit(Scene* scene)
{
    
}

TEST_CASE_METHOD(SceneTestFixture, "Adding GameObject during Update is deferred", "[Scene]") {
    // Create a GameObject with a TestComponent that adds another GameObject in Update
    auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
		.WithComponent<TestComponent>([&](Scene* scene) {
		    scene->CreateGameObjectBuilder("TestGO2", 0)
                .AddToScene();
			})
        .AddToScene();

    REQUIRE(scene.FindGameObjectByName("TestGO1") == nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);

    // Simulate a frame
    scene.Frame(0.016f);

    // Verify initial state
    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);
        
    // Simulate a frame
    scene.Frame(0.016f);

    // TestGO2 should now exist due to pending add queue
    REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "Pending add queue processes multiple objects", "[Scene]") {
    // Create a GameObject with a TestComponent that adds multiple GameObjects
    auto go1 = std::make_unique<GameObject>("TestGO1", 0);
    go1->SetScene(&scene);
    go1->AddComponent(std::make_unique<TestComponent>([&](Scene* scene) {
        scene->CreateGameObjectBuilder("TestGO2", 0).AddToScene();
        scene->CreateGameObjectBuilder("TestGO3", 0).AddToScene();
        }));

    scene.AddGameObject(std::move(go1), true);

    // Verify initial state
    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO3") == nullptr);

    // Simulate a frame
    scene.Frame(0.016f);

    // Both TestGO2 and TestGO3 should now exist
    REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO3") != nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "GameObjects added during Update are not updated in same frame", "[Scene]") {
    // Track if TestGO2's Update was called
    bool wasUpdated = false;

    // Create a GameObject with a TestComponent that adds TestGO2
    auto go1 = std::make_unique<GameObject>("TestGO1", 0);
    go1->SetScene(&scene);
    go1->AddComponent(std::make_unique<TestComponent>([&](Scene* scene) {
        auto builder = scene->CreateGameObjectBuilder("TestGO2", 0);
        auto newGO = builder.AddToScene();
        newGO->AddComponent(std::make_unique<TestComponent>([&](Scene*) {
            wasUpdated = true;
            }));
        }));

    scene.AddGameObject(std::move(go1), true);

    // Simulate a frame
    scene.Frame(0.016f);

    // TestGO2 should exist but not have been updated
    REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
    REQUIRE_FALSE(wasUpdated);
}
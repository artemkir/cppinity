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

// Flag component for testing lifecycle methods
class FlagComponent : public BaseComponent {
public:
	bool awoke = false;
	bool started = false;
	int updates = 0;
	bool active = true;

	void Awake() override { awoke = true; }
	void Start() override { started = true; }
	void Update(float deltaTime) override { if (active) updates++; }
	void OnCollide(GameObject* other) override {}
};

// Custom component to destroy self during Update
class DestroySelfComponent : public BaseComponent {
public:
	void Update(float deltaTime) override {
		gameObject->GetScene()->Destroy(gameObject);
	}
	void Awake() override {}
	void Start() override {}
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
		: resourceManager(&renderer)
		, materialManager()
		, scene(&renderer, &materialManager, &resourceManager) {
	}
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

	// Verify state after first frame
	REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
	REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);

	// Simulate another frame
	scene.Frame(0.016f);

	// TestGO2 should now exist due to pending add queue
	REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "Pending add queue processes multiple objects", "[Scene]") {
	// Create a GameObject with a TestComponent that adds multiple GameObjects
	auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
		.WithComponent<TestComponent>([&](Scene* scene) {
		scene->CreateGameObjectBuilder("TestGO2", 0).AddToScene();
		scene->CreateGameObjectBuilder("TestGO3", 0).AddToScene();
			})
		.AddToScene();

	// Verify initial state
	REQUIRE(scene.FindGameObjectByName("TestGO1") == nullptr);
	REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);
	REQUIRE(scene.FindGameObjectByName("TestGO3") == nullptr);

	// Simulate a frame
	scene.Frame(0.016f);

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
	auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
		.WithComponent<TestComponent>([&](Scene* scene) {
		auto builder = scene->CreateGameObjectBuilder("TestGO2", 0)
			.WithComponent<TestComponent>([&](Scene*) {
			wasUpdated = true;
				})
			.AddToScene();
			})
		.AddToScene();

	scene.Frame(0.016f);

	// Verify initial state
	REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
	REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);

	// Simulate a frame
	scene.Frame(0.016f);

	// TestGO2 should exist but not have been updated
	REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
	REQUIRE_FALSE(wasUpdated);
}

TEST_CASE_METHOD(SceneTestFixture, "Lifecycle methods are called correctly", "[Scene]") {
	auto go = scene.CreateGameObjectBuilder("TestGO", 0)
		.WithComponent<FlagComponent>()
		.AddToScene();

	// Simulate: first frame: should call Awake only
	scene.Frame(0.016f);

	auto comp = dynamic_cast<FlagComponent*>(go->GetComponents()[0].get());

	REQUIRE(comp != nullptr);
	REQUIRE(comp->awoke == true);

	REQUIRE(comp->started == false);
	REQUIRE(comp->updates == 0);

	// Simulate first frame: should call Awake, Start, Update
	scene.Frame(0.016f);

	REQUIRE(comp->awoke == true);
	REQUIRE(comp->started == true);
	REQUIRE(comp->updates == 1);

	// Simulate second frame: should call Update again
	scene.Frame(0.016f);

	REQUIRE(comp->updates == 2);
}

TEST_CASE_METHOD(SceneTestFixture, "SetActive affects components and children", "[Scene]") {
	auto parent = scene.CreateGameObjectBuilder("Parent", 0)
		.WithComponent<FlagComponent>()
		.AddToScene();

	auto child = parent->CreateGameObjectBuilder("Child", 0)
		.WithComponent<FlagComponent>()
		.AddToScene();

	// Simulate first frame to actually add to scene
	scene.Frame(0.016f);

	auto parentComp = dynamic_cast<FlagComponent*>(parent->GetComponents()[0].get());
	auto childComp = dynamic_cast<FlagComponent*>(child->GetComponents()[0].get());

	REQUIRE(parentComp != nullptr);
	REQUIRE(childComp != nullptr);

	// Simulate frame: both update
	scene.Frame(0.016f);
	REQUIRE(parentComp->updates == 1);
	REQUIRE(childComp->updates == 1);

	// Deactivate parent
	parent->SetActive(false);

	// Simulate frame: no updates
	scene.Frame(0.016f);
	REQUIRE(parentComp->updates == 1);
	REQUIRE(childComp->updates == 1);

	// Reactivate parent
	parent->SetActive(true);

	// Simulate frame: updates again
	scene.Frame(0.016f);
	REQUIRE(parentComp->updates == 2);
	REQUIRE(childComp->updates == 2);
}

TEST_CASE_METHOD(SceneTestFixture, "Destroying GameObject during Update is deferred", "[Scene]") {
	auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
		.WithComponent<DestroySelfComponent>()
		.AddToScene();

	// Simulate a frame: Add object
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);

	// Simulate a frame: Update calls Destroy, at the end of frame object should be destroyed
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("TestGO1") == nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "Cycle detection in parent-child hierarchy", "[GameObject]") {
	auto go1 = scene.CreateGameObjectBuilder("TestGO", 0).AddToScene();

	REQUIRE_NOTHROW(go1->CreateGameObjectBuilder("Child", 0).AddToScene());

	REQUIRE_THROWS_AS(go1->CreateGameObjectBuilder("TestGO", 0).AddToScene(), std::runtime_error);
}
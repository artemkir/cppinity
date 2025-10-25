#include "test_mocks.h"

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
	std::function<void()> onDestroyCallback;

	void virtual Awake() override { awoke = true; }
	void virtual Start() override { started = true; }
	void virtual Update(float deltaTime) override { if (active) updates++; }
	void OnCollide(GameObject* other) override {}
	void OnDestroy() override {
		if (onDestroyCallback) {
			onDestroyCallback();
		}
	}
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

class AddInAwakeComponent : public BaseComponent {
public:
	void Awake() override {
		for (int i = 0; i < 100; i++)
		{
			gameObject->GetScene()->CreateGameObjectBuilder("AddedInAwake"+i, 0)
				.AddToScene();
		}
	}
	void Start() override {}
	void Update(float deltaTime) override {}
	void OnCollide(GameObject* other) override {}
};

TEST_CASE_METHOD(SceneTestFixture, "Adding GameObject during Awake is deferred correctly", "[Scene]") {
	auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
		.WithComponent<AddInAwakeComponent>()
		.AddToScene();

	// Initial state
	REQUIRE(scene.FindGameObjectByName("TestGO1") == nullptr);
	REQUIRE(scene.FindGameObjectByName("AddedInAwake") == nullptr);

	// Simulate frame: Process pending adds, call Awake which adds another to pendingAdd
	scene.Frame(0.016f);

	// After first frame: TestGO1 added, AddedInAwake still pending
	REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
	REQUIRE(scene.FindGameObjectByName("AddedInAwake") == nullptr);

	// Simulate another frame: Process the new pending add
	scene.Frame(0.016f);

	// Now AddedInAwake should be added
	REQUIRE(scene.FindGameObjectByName("AddedInAwake") != nullptr);
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

	auto comp = go->GetComponent<FlagComponent>();

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

	// Simulate first frame to actually add objects to scene
	scene.Frame(0.016f);

	auto parentComp = parent->GetComponent<FlagComponent>();
	auto childComp = child->GetComponent<FlagComponent>();

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

TEST_CASE_METHOD(SceneTestFixture, "Destroying parent deletes children", "[Scene]") {
	auto parent = scene.CreateGameObjectBuilder("Parent", 0)
		.AddToScene();

	auto child = parent->CreateGameObjectBuilder("Child", 0)
		.AddToScene();

	// Simulate a frame to add objects
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("Parent") != nullptr);
	REQUIRE(scene.FindGameObjectByName("Child") != nullptr);

	scene.Destroy(parent);

	// Simulate a frame to process destruction
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("Parent") == nullptr);
	REQUIRE(scene.FindGameObjectByName("Child") == nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "Destroying child removes from parent", "[Scene]") {
	auto parent = scene.CreateGameObjectBuilder("Parent", 0)
		.AddToScene();

	auto child = parent->CreateGameObjectBuilder("Child", 0)
		.AddToScene();

	// Simulate a frame to add objects
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("Parent") != nullptr);
	REQUIRE(scene.FindGameObjectByName("Child") != nullptr);
	REQUIRE(!parent->GetChildren().empty());

	scene.Destroy(child);

	// Simulate a frame to process destruction
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("Parent") != nullptr);
	REQUIRE(scene.FindGameObjectByName("Child") == nullptr);
	REQUIRE(parent->GetChildren().empty());
}

TEST_CASE_METHOD(SceneTestFixture, "OnDestroy called when object is about to be destroyed", "[Scene]") {
	bool onDestroyCalled = false;

	auto go = scene.CreateGameObjectBuilder("TestGO", 0)
		.WithComponent<FlagComponent>()
		.AddToScene();

	// Simulate a frame to add object
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("TestGO") != nullptr);

	auto comp = go->GetComponent<FlagComponent>();
	REQUIRE(comp != nullptr);

	comp->onDestroyCallback = [&]() { onDestroyCalled = true; };

	scene.Destroy(go);

	// Simulate a frame to process destruction
	scene.Frame(0.016f);

	REQUIRE(scene.FindGameObjectByName("TestGO") == nullptr);
	REQUIRE(onDestroyCalled);
}

TEST_CASE_METHOD(SceneTestFixture, "Deactivating GameObject in another's Start prevents Start until reactivated", "[Scene]") {
	// Component to deactivate another GameObject during Start
	class DeactivateOtherComponent : public FlagComponent {
	public:
		GameObject* target = nullptr;

		DeactivateOtherComponent(GameObject* target) : target(target) {}
		void Awake() override {
			FlagComponent::Awake();
		}

		void Start() override {
			
			FlagComponent::Start();

			if (target) {
				target->SetActive(false);
			}
		}
		void Update(float deltaTime) override {}
		void OnCollide(GameObject* other) override {}
	};

	// Create GO2 with FlagComponent to track lifecycle
	auto go2 = scene.CreateGameObjectBuilder("TestGO2", 0)
		.WithComponent<FlagComponent>()
		.AddToScene();

	// Create GO1 which will deactivate GO2 in its Start
	auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
		.WithComponent<DeactivateOtherComponent>(go2)
		.AddToScene();

	// Simulate first frame: GO1 and GO2 added
	scene.Frame(0.016f);

	auto comp1 = go1->GetComponent<DeactivateOtherComponent>();
	auto comp2 = go2->GetComponent<FlagComponent>();
	REQUIRE(comp1 != nullptr);
	REQUIRE(comp2 != nullptr);

	REQUIRE(comp1->awoke == true);
	REQUIRE(comp2->awoke == true);
	REQUIRE(comp1->started == false);
	REQUIRE(comp2->started == false);

	// Verify initial state
	REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
	REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);

	// Simulate: GO1's Start deactivates GO2
	scene.Frame(0.016f);
	
	REQUIRE(comp2->started == false);   // Start should not be called due to deactivation
	REQUIRE(comp2->updates == 0);       // No updates while deactivated

	// Simulate another frame: GO2 remains deactivated
	scene.Frame(0.016f);
	REQUIRE(comp2->started == false);   // Still no Start
	REQUIRE(comp2->updates == 0);       // Still no updates

	// Reactivate GO2
	go2->SetActive(true);

	// Simulate another frame: GO2 should now call Start and Update
	scene.Frame(0.016f);
	REQUIRE(comp2->awoke == true);
	REQUIRE(comp2->started == true);    // Start should now be called
	REQUIRE(comp2->updates == 1);       // Update should be called once
}
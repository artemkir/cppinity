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
    // New: Add OnEnable/OnDisable for intermediate mode
    //void OnEnable() override {}  // Placeholder, can add logic if needed
    //void OnDisable() override {} // Placeholder
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
        for (int i = 0; i < 100; i++) {
            gameObject->GetScene()->CreateGameObjectBuilder("AddedInAwake" + std::to_string(i), 0)
                .AddToScene();
        }
    }
    void Start() override {}
    void Update(float deltaTime) override {}
    void OnCollide(GameObject* other) override {}
};

TEST_CASE_METHOD(SceneTestFixture, "Adding GameObject during Awake is processed immediately if safe", "[Scene]") {
    auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
        .WithComponent<AddInAwakeComponent>()
        .AddToScene();

    // Initial state: In intermediate mode, non-iteration adds are immediate
    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);  // Changed: Now immediate
    REQUIRE(scene.FindGameObjectByName("AddedInAwake0") == nullptr);  // Still pending if in Awake

    // Simulate frame: Process any pending, call Awake (adds to queue or immediate), then Start/Update
    scene.Frame(0.016f);

    // After frame: AddedInAwake should be added (immediate or queued flush)
    REQUIRE(scene.FindGameObjectByName("AddedInAwake0") != nullptr);
    REQUIRE(scene.FindGameObjectByName("AddedInAwake99") != nullptr);  // Check multiple
}

TEST_CASE_METHOD(SceneTestFixture, "Adding GameObject during Update is deferred", "[Scene]") {
    auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
        .WithComponent<TestComponent>([&](Scene* scene) {
        scene->CreateGameObjectBuilder("TestGO2", 0)
            .AddToScene();
            })
        .AddToScene();

    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);  // Changed: Immediate add
    REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);

    // Simulate a frame: Update adds to queue
    scene.Frame(0.016f);

    // TestGO2 now exists (processed at frame end/start)
    REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "Pending add queue processes multiple objects", "[Scene]") {
    auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
        .WithComponent<TestComponent>([&](Scene* scene) {
        scene->CreateGameObjectBuilder("TestGO2", 0).AddToScene();
        scene->CreateGameObjectBuilder("TestGO3", 0).AddToScene();
            })
        .AddToScene();

    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);  // Immediate
    REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO3") == nullptr);

    scene.Frame(0.016f);

    REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO3") != nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "GameObjects added during Update are not updated in same frame", "[Scene]") {
    bool wasUpdated = false;
    auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
        .WithComponent<TestComponent>([&](Scene* scene) {
        scene->CreateGameObjectBuilder("TestGO2", 0)
            .WithComponent<TestComponent>([&](Scene*) {
            wasUpdated = true;
                })
            .AddToScene();
            })
        .AddToScene();

    scene.Frame(0.016f);  // First frame: Add go1, call Awake/Start/Update, add TestGO2 to queue

    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO2") == nullptr);  // Still deferred during Update

    scene.Frame(0.016f);  // Second frame: Process add, call Awake/Start for TestGO2, but no Update yet if same-frame rule

    REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);
    REQUIRE_FALSE(wasUpdated);  // Still no same-frame Update
}

TEST_CASE_METHOD(SceneTestFixture, "Lifecycle methods are called correctly with Awake+Start same frame", "[Scene]") {
    auto go = scene.CreateGameObjectBuilder("TestGO", 0)
        .WithComponent<FlagComponent>()
        .AddToScene();

    // Initial: Immediate add, but no calls yet
    REQUIRE(scene.FindGameObjectByName("TestGO") != nullptr);

    // First frame: Awake + Start (if active), no Update yet?
    scene.Frame(0.016f);
    auto comp = go->GetComponent<FlagComponent>();
    REQUIRE(comp != nullptr);
    REQUIRE(comp->awoke == true);
    REQUIRE(comp->started == true);  // Changed: Start in same frame as Awake
    REQUIRE(comp->updates == 0);  // Update not yet

    // Second frame: Update
    scene.Frame(0.016f);
    REQUIRE(comp->updates == 1);
}

TEST_CASE_METHOD(SceneTestFixture, "SetActive affects components and children", "[Scene]") {
    auto parent = scene.CreateGameObjectBuilder("Parent", 0)
        .WithComponent<FlagComponent>()
        .AddToScene();
    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .WithComponent<FlagComponent>()
        .AddToScene();

    // Immediate adds
    REQUIRE(scene.FindGameObjectByName("Parent") != nullptr);
    REQUIRE(scene.FindGameObjectByName("Child") != nullptr);

    scene.Frame(0.016f);  // Awake/Start for both

    auto parentComp = parent->GetComponent<FlagComponent>();
    auto childComp = child->GetComponent<FlagComponent>();
    REQUIRE(parentComp != nullptr);
    REQUIRE(childComp != nullptr);
    REQUIRE(parentComp->awoke == true);
    REQUIRE(parentComp->started == true);
    REQUIRE(childComp->awoke == true);
    REQUIRE(childComp->started == true);

    // Simulate frame: Updates
    scene.Frame(0.016f);
    REQUIRE(parentComp->updates == 1);
    REQUIRE(childComp->updates == 1);

    // Deactivate parent: Calls OnDisable immediately
    parent->SetActive(false);

    // Simulate: No updates
    scene.Frame(0.016f);
    REQUIRE(parentComp->updates == 1);
    REQUIRE(childComp->updates == 1);

    // Reactivate: Calls OnEnable, potentially resumes Start if pending, but already done
    parent->SetActive(true);

    // Simulate: Updates again
    scene.Frame(0.016f);
    REQUIRE(parentComp->updates == 2);
    REQUIRE(childComp->updates == 2);
}

TEST_CASE_METHOD(SceneTestFixture, "Destroying GameObject during Update is deferred", "[Scene]") {
    auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
        .WithComponent<DestroySelfComponent>()
        .AddToScene();

    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);  // Immediate

    scene.Frame(0.016f);  // Awake/Start/Update: Queues destroy

    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);  // Still exists mid-frame

    scene.Frame(0.016f);  // Processes destroy
    REQUIRE(scene.FindGameObjectByName("TestGO1") == nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "Cycle detection in parent-child hierarchy", "[GameObject]") {
    auto go1 = scene.CreateGameObjectBuilder("TestGO", 0).AddToScene();
    REQUIRE_NOTHROW(go1->CreateGameObjectBuilder("Child", 0).AddToScene());
    REQUIRE_THROWS_AS(go1->CreateGameObjectBuilder("TestGO", 0).AddToScene(), std::runtime_error);
    // No change needed, as hierarchy checks are unchanged
}

TEST_CASE_METHOD(SceneTestFixture, "Destroying parent deletes children", "[Scene]") {
    auto parent = scene.CreateGameObjectBuilder("Parent", 0)
        .AddToScene();
    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .AddToScene();

    REQUIRE(scene.FindGameObjectByName("Parent") != nullptr);
    REQUIRE(scene.FindGameObjectByName("Child") != nullptr);

    scene.Frame(0.016f);  // Awake/Start

    scene.Destroy(parent);

    scene.Frame(0.016f);  // Process destroy
    REQUIRE(scene.FindGameObjectByName("Parent") == nullptr);
    REQUIRE(scene.FindGameObjectByName("Child") == nullptr);
}

TEST_CASE_METHOD(SceneTestFixture, "Destroying child removes from parent", "[Scene]") {
    auto parent = scene.CreateGameObjectBuilder("Parent", 0)
        .AddToScene();
    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .AddToScene();

    REQUIRE(scene.FindGameObjectByName("Parent") != nullptr);
    REQUIRE(scene.FindGameObjectByName("Child") != nullptr);

    scene.Frame(0.016f);

    REQUIRE(!parent->GetChildren().empty());

    scene.Destroy(child);

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

    REQUIRE(scene.FindGameObjectByName("TestGO") != nullptr);

    scene.Frame(0.016f);  // Awake/Start

    auto comp = go->GetComponent<FlagComponent>();
    REQUIRE(comp != nullptr);
    comp->onDestroyCallback = [&]() { onDestroyCalled = true; };

    scene.Destroy(go);

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
        void Awake() override { FlagComponent::Awake(); }
        void Start() override {
            FlagComponent::Start();
            if (target) {
                target->SetActive(false);  // Calls OnDisable immediately
            }
        }
        void Update(float deltaTime) override {}
        void OnCollide(GameObject* other) override {}
    };

    auto go2 = scene.CreateGameObjectBuilder("TestGO2", 0)
        .WithComponent<FlagComponent>()
        .AddToScene();

    auto go1 = scene.CreateGameObjectBuilder("TestGO1", 0)
        .WithComponent<DeactivateOtherComponent>(go2)
        .AddToScene();

    REQUIRE(scene.FindGameObjectByName("TestGO1") != nullptr);
    REQUIRE(scene.FindGameObjectByName("TestGO2") != nullptr);

    // First frame: Awake + Start for both; go1's Start deactivates go2, preventing go2's Start if order allows
    scene.Frame(0.016f);

    auto comp1 = go1->GetComponent<DeactivateOtherComponent>();
    auto comp2 = go2->GetComponent<FlagComponent>();
    REQUIRE(comp1 != nullptr);
    REQUIRE(comp2 != nullptr);
    REQUIRE(comp1->awoke == true);
    REQUIRE(comp1->started == true);
    REQUIRE(comp2->awoke == true);
    REQUIRE(comp2->started == false);  // Prevented due to deactivation in same frame

    // Second frame: go2 deactivated, no Update
    scene.Frame(0.016f);
    REQUIRE(comp2->started == false);
    REQUIRE(comp2->updates == 0);

    // Reactivate: Calls OnEnable immediately, then Start in same/next frame
    go2->SetActive(true);

    // Third frame: Start and Update for go2
    scene.Frame(0.016f);
    REQUIRE(comp2->started == true);
    REQUIRE(comp2->updates == 1);
}
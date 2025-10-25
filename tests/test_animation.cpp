#include "test_mocks.h"
#include "Components/Animation.h"
#include "Components/Transform.h"
#include "GameObject.h"

TEST_CASE_METHOD(SceneTestFixture, "Animation initializes correctly", "[Animation]") {
    auto go = scene.CreateGameObjectBuilder("AnimatedObject", 0)
        .WithComponent<MockTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Animation>(1.0f, Vector2{ 2.0f, 2.0f }, 1)
        .AddToScene();

    scene.Frame(0.0f); // Trigger Awake

    auto anim = go->GetComponent<Animation>();
    REQUIRE(anim != nullptr);
    auto transform = go->GetComponent<MockTransform>();
    REQUIRE(transform != nullptr);

    REQUIRE(transform->GetScale().x == Catch::Approx(1.0f));
    REQUIRE(transform->GetScale().y == Catch::Approx(1.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Animation progresses halfway", "[Animation]") {
    auto go = scene.CreateGameObjectBuilder("AnimatedObject", 0)
        .WithComponent<MockTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Animation>(1.0f, Vector2{ 2.0f, 2.0f }, 1)
        .AddToScene();

    scene.Frame(0.0f); // Trigger Awake
    scene.Frame(0.5f); // Progress to t = 0.5

    auto transform = go->GetComponent<MockTransform>();
    REQUIRE(transform != nullptr);

    // EaseInOutQuad(0.5) = 2 * 0.5 * 0.5 = 0.5
    // newScale = start + (target - start) * easedT = 1.0 + (2.0 - 1.0) * 0.5 = 1.5
    REQUIRE(transform->GetScale().x == Catch::Approx(1.5f));
    REQUIRE(transform->GetScale().y == Catch::Approx(1.5f));
}

TEST_CASE_METHOD(SceneTestFixture, "Animation completes single loop", "[Animation]") {
    auto go = scene.CreateGameObjectBuilder("AnimatedObject", 0)
        .WithComponent<MockTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Animation>(1.0f, Vector2{ 2.0f, 2.0f }, 1)
        .AddToScene();

    scene.Frame(0.0f); // Trigger Awake
    scene.Frame(1.0f); // Complete animation

    auto anim = go->GetComponent<Animation>();
    auto transform = go->GetComponent<MockTransform>();
    REQUIRE(anim != nullptr);
    REQUIRE(transform != nullptr);

    REQUIRE(transform->GetScale().x == Catch::Approx(2.0f));
    REQUIRE(transform->GetScale().y == Catch::Approx(2.0f));
    REQUIRE_FALSE(anim->IsActive()); // Animation should be inactive
}

TEST_CASE_METHOD(SceneTestFixture, "Animation loops twice", "[Animation]") {
    auto go = scene.CreateGameObjectBuilder("AnimatedObject", 0)
        .WithComponent<MockTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Animation>(1.0f, Vector2{ 2.0f, 2.0f }, 2)
        .AddToScene();

    scene.Frame(0.0f); // Trigger Awake
    scene.Frame(1.0f); // Complete first loop
    scene.Frame(0.5f); // Halfway through second loop (back to start)

    auto anim = go->GetComponent<Animation>();
    auto transform = go->GetComponent<MockTransform>();
    REQUIRE(anim != nullptr);
    REQUIRE(transform != nullptr);

    // After first loop, scales to (2.0,2.0), swaps start/target, animates back
    // At t = 0.5 (second loop), easedT = 0.5, newScale = 2.0 + (1.0 - 2.0) * 0.5 = 1.5
    REQUIRE(transform->GetScale().x == Catch::Approx(1.5f));
    REQUIRE(transform->GetScale().y == Catch::Approx(1.5f));
    REQUIRE(anim->IsActive()); // Still active after first loop
}

TEST_CASE_METHOD(SceneTestFixture, "Animation with infinite loops", "[Animation]") {
    auto go = scene.CreateGameObjectBuilder("AnimatedObject", 0)
        .WithComponent<MockTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Animation>(1.0f, Vector2{ 2.0f, 2.0f }, -1)
        .AddToScene();

    scene.Frame(0.0f); // Trigger Awake
	scene.Frame(1.0f); // Forward to target
	scene.Frame(1.0f); // Back to start
	scene.Frame(1.0f); // Forward to target again

    auto anim = go->GetComponent<Animation>();
    auto transform = go->GetComponent<MockTransform>();
    REQUIRE(anim != nullptr);
    REQUIRE(transform != nullptr);

    // After two loops, should be back at target (2.0,2.0)
    REQUIRE(transform->GetScale().x == Catch::Approx(2.0f));
    REQUIRE(transform->GetScale().y == Catch::Approx(2.0f));
    REQUIRE(anim->IsActive()); // Still active with infinite loops
}

TEST_CASE_METHOD(SceneTestFixture, "Inactive animation does not update", "[Animation]") {
    auto go = scene.CreateGameObjectBuilder("AnimatedObject", 0)
        .WithComponent<MockTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Animation>(1.0f, Vector2{ 2.0f, 2.0f }, 1)
        .AddToScene();

    scene.Frame(0.0f); // Trigger Awake

    auto anim = go->GetComponent<Animation>();
    REQUIRE(anim != nullptr);
    anim->SetActive(false); // Disable animation

    scene.Frame(0.5f); // Try to update

    auto transform = go->GetComponent<MockTransform>();
    REQUIRE(transform != nullptr);

    REQUIRE(transform->GetScale().x == Catch::Approx(1.0f));
    REQUIRE(transform->GetScale().y == Catch::Approx(1.0f));
}
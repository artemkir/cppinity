#include "test_mocks.h"

TEST_CASE_METHOD(SceneTestFixture, "Canvas with TopLeft anchor and full screen", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        //.WithComponent<ScreenTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 200.0f }, Vector2{ 50.0f, 60.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(100.0f));
    REQUIRE(final.pos.y == Catch::Approx(200.0f));
    REQUIRE(final.size.x == Catch::Approx(50.0f));
    REQUIRE(final.size.y == Catch::Approx(60.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with Center anchor and partial screen", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<ScreenTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Canvas>(Vector2{ 0.5f, 0.5f }, Vector2{ 0.5f, 0.5f }, AnchorType::Center, Vector2{ 400.0f, 300.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 200.0f, 150.0f }, Vector2{ 100.0f, 100.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(400.0f));
    REQUIRE(final.pos.y == Catch::Approx(300.0f));
    REQUIRE(final.size.x == Catch::Approx(100.0f));
    REQUIRE(final.size.y == Catch::Approx(100.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with TopLeft anchor and offset position", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<ScreenTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Canvas>(Vector2{ 0.8f, 0.8f }, Vector2{ 0.1f, 0.1f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 200.0f }, Vector2{ 50.0f, 60.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(80.0f + 100.0f * 0.8f));
    REQUIRE(final.pos.y == Catch::Approx(60.0f + 200.0f * 0.8f));
    REQUIRE(final.size.x == Catch::Approx(50.0f * 0.8f));
    REQUIRE(final.size.y == Catch::Approx(60.0f * 0.8f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with parent-child hierarchy and scale", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<ScreenTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto parent = canvas_go->CreateGameObjectBuilder("Parent", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 100.0f }, Vector2{ 200.0f, 200.0f }, Vector2{ 1.5f, 1.5f })
        .AddToScene();

    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 50.0f, 50.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 2.0f, 2.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(175.0f));
    REQUIRE(final.pos.y == Catch::Approx(175.0f));
    REQUIRE(final.size.x == Catch::Approx(450.0f));
    REQUIRE(final.size.y == Catch::Approx(450.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with aspect-preserving relative size", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<ScreenTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Canvas>(Vector2{ -1.0f, 0.5f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 400.0f, 300.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 200.0f, 150.0f }, Vector2{ 100.0f, 100.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(150.0f));
    REQUIRE(final.pos.y == Catch::Approx(150.0f));
    REQUIRE(final.size.x == Catch::Approx(75.0f));
    REQUIRE(final.size.y == Catch::Approx(100.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Nested parent-child under Canvas with Center anchor", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<ScreenTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Canvas>(Vector2{ 0.5f, 0.5f }, Vector2{ 0.5f, 0.5f }, AnchorType::Center, Vector2{ 400.0f, 300.0f })
        .AddToScene();

    auto parent = canvas_go->CreateGameObjectBuilder("Parent", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 100.0f }, Vector2{ 200.0f, 200.0f }, Vector2{ 1.0f, 1.0f })
        .AddToScene();

    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 50.0f, 50.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(350.0f));
    REQUIRE(final.pos.y == Catch::Approx(250.0f));
    REQUIRE(final.size.x == Catch::Approx(100.0f));
    REQUIRE(final.size.y == Catch::Approx(100.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with GridTransform and TopLeft anchor", "[Canvas][GridTransform]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<ScreenTransform>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f })
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<GridTransform>(Vector2{ 2.0f, 3.0f }, Vector2{ 50.0f, 50.0f }, Vector2{ 1.0f, 1.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<GridTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // GridTransform pos (2,3) with cell_size (50,50) means pos = (2*50, 3*50) = (100, 150)
    // Size = (1*50, 1*50) = (50, 50), canvas scale = 1, no offset
    REQUIRE(final.pos.x == Catch::Approx(100.0f));
    REQUIRE(final.pos.y == Catch::Approx(150.0f));
    REQUIRE(final.size.x == Catch::Approx(50.0f));
    REQUIRE(final.size.y == Catch::Approx(50.0f));
}
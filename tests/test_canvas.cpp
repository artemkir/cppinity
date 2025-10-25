#include "test_mocks.h"

TEST_CASE_METHOD(SceneTestFixture, "Canvas with TopLeft anchor and full screen", "[Canvas]") {

    REQUIRE(scene.GetRenderer()->GetW() == 800);
    REQUIRE(scene.GetRenderer()->GetH() == 600);

    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 1600.0f, 1200.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 200.0f, 400.0f }, Vector2{ 100.0f, 120.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(100.0f));
    REQUIRE(final.pos.y == Catch::Approx(200.0f));
    REQUIRE(final.size.x == Catch::Approx(50.0f));
    REQUIRE(final.size.y == Catch::Approx(60.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.0f));
    REQUIRE(final.pivot.y == Catch::Approx(0.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with Center anchor and partial screen", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 0.5f, 0.5f }, Vector2{ 0.5f, 0.5f }, AnchorType::Center, Vector2{ 1600.0f, 1200.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 800.0f, 600.0f }, Vector2{ 400.0f, 400.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // Canvas: size (0.5,0.5) of 800x600 = 400x300, pos (0.5,0.5) center => screen pos (200,150)
    // Child: pos (800,600) in canvas space (1600x1200) => (800/1600*400, 600/1200*300) = (200,150)
    // Final pos = canvas_pos + child_pos = (200,150) + (200,150) = (400,300)
    // Size = (400/1600*400, 400/1200*300) = (100,100)
    REQUIRE(final.pos.x == Catch::Approx(400.0f));
    REQUIRE(final.pos.y == Catch::Approx(300.0f));
    REQUIRE(final.size.x == Catch::Approx(100.0f));
    REQUIRE(final.size.y == Catch::Approx(100.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.0f));
    REQUIRE(final.pivot.y == Catch::Approx(0.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with TopLeft anchor and offset position", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 0.8f, 0.8f }, Vector2{ 0.1f, 0.1f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 200.0f }, Vector2{ 50.0f, 60.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(80.0f + 100.0f * 0.8f));
    REQUIRE(final.pos.y == Catch::Approx(60.0f + 200.0f * 0.8f));
    REQUIRE(final.size.x == Catch::Approx(50.0f * 0.8f));
    REQUIRE(final.size.y == Catch::Approx(60.0f * 0.8f));
    REQUIRE(final.pivot.x == Catch::Approx(0.0f));
    REQUIRE(final.pivot.y == Catch::Approx(0.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with parent-child hierarchy and scale", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto parent = canvas_go->CreateGameObjectBuilder("Parent", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 100.0f }, Vector2{ 200.0f, 200.0f }, Vector2{ 1.5f, 1.5f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 50.0f, 50.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 2.0f, 2.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // Parent: pos (100,100), size (200,200), scale (1.5,1.5), pivot (0,0)
    // Child: pos (50,50), size (100,100), scale (2,2), pivot (0,0)
    // Child relative to parent: pos = 50*1.5 + 100 = 175, size = 100*1.5*2 = 300
    REQUIRE(final.pos.x == Catch::Approx(175.0f));
    REQUIRE(final.pos.y == Catch::Approx(175.0f));
    REQUIRE(final.size.x == Catch::Approx(300.0f));
    REQUIRE(final.size.y == Catch::Approx(300.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.0f));
    REQUIRE(final.pivot.y == Catch::Approx(0.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with aspect-preserving relative size", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ -1.0f, 0.5f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 400.0f, 300.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 200.0f, 150.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    REQUIRE(final.pos.x == Catch::Approx(150.0f));
    REQUIRE(final.pos.y == Catch::Approx(150.0f));
    REQUIRE(final.size.x == Catch::Approx(75.0f));
    REQUIRE(final.size.y == Catch::Approx(100.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.0f));
    REQUIRE(final.pivot.y == Catch::Approx(0.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Nested parent-child under Canvas with Center anchor", "[Canvas]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 0.5f, 0.5f }, Vector2{ 0.5f, 0.5f }, AnchorType::Center, Vector2{ 400.0f, 300.0f })
        .AddToScene();

    auto parent = canvas_go->CreateGameObjectBuilder("Parent", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 100.0f }, Vector2{ 200.0f, 200.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 50.0f, 50.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // Canvas: size (0.5,0.5) of 800x600 = 400x300, center anchor => screen pos (200,150)
    // Parent: pos (100,100), size (200,200), scale (1,1)
    // Child: pos (50,50), size (100,100), scale (1,1)
    // Child relative to parent: pos = 50*1 + 100 = 150
    // Canvas scale: (400/400, 300/300) = (1,1)
    // Final pos = canvas_pos + parent_pos = (200,150) + (150,150) = (350,300)
    REQUIRE(final.pos.x == Catch::Approx(350.0f));
    REQUIRE(final.pos.y == Catch::Approx(300.0f));
    REQUIRE(final.size.x == Catch::Approx(100.0f));
    REQUIRE(final.size.y == Catch::Approx(100.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.0f));
    REQUIRE(final.pivot.y == Catch::Approx(0.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "Canvas with GridTransform and TopLeft anchor", "[Canvas][GridTransform]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<GridTransform>(Vector2{ 2.0f, 3.0f }, Vector2{ 50.0f, 50.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<GridTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // GridTransform: pos (2,3), size (50,50), scale (1,1), pivot (0,0)
    // Screen pos = (2*50, 3*50) = (100,150), size = (50,50)
    REQUIRE(final.pos.x == Catch::Approx(100.0f));
    REQUIRE(final.pos.y == Catch::Approx(150.0f));
    REQUIRE(final.size.x == Catch::Approx(50.0f));
    REQUIRE(final.size.y == Catch::Approx(50.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.0f));
    REQUIRE(final.pivot.y == Catch::Approx(0.0f));
}

TEST_CASE_METHOD(SceneTestFixture, "ScreenTransform with Center pivot under Canvas", "[Canvas][ScreenTransform]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 200.0f, 400.0f }, Vector2{ 100.0f, 120.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.5f, 0.5f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // Pivot at center: pos adjusted by -size/2 = -(100/2, 120/2) = (-50,-60)
    // Final pos = (200-50, 400-60) = (150,340)
    REQUIRE(final.pos.x == Catch::Approx(150.0f));
    REQUIRE(final.pos.y == Catch::Approx(340.0f));
    REQUIRE(final.size.x == Catch::Approx(100.0f));
    REQUIRE(final.size.y == Catch::Approx(120.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.5f));
    REQUIRE(final.pivot.y == Catch::Approx(0.5f));
}

TEST_CASE_METHOD(SceneTestFixture, "ScreenTransform with Center pivot and scale", "[Canvas][ScreenTransform]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 200.0f, 400.0f }, Vector2{ 100.0f, 120.0f }, Vector2{ 2.0f, 2.0f }, Vector2{ 0.5f, 0.5f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // Scale (2,2), size (100,120) => scaledSize (200,240)
    // Pivot offset = (200*0.5, 240*0.5) = (100,120)
    // Pos = (200-100, 400-120) = (100,280)
    // Size = (100*2, 120*2) = (200,240)
    REQUIRE(final.pos.x == Catch::Approx(100.0f));
    REQUIRE(final.pos.y == Catch::Approx(280.0f));
    REQUIRE(final.size.x == Catch::Approx(200.0f));
    REQUIRE(final.size.y == Catch::Approx(240.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.5f));
    REQUIRE(final.pivot.y == Catch::Approx(0.5f));
}

TEST_CASE_METHOD(SceneTestFixture, "ScreenTransform with custom pivot and parent", "[Canvas][ScreenTransform]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto parent = canvas_go->CreateGameObjectBuilder("Parent", 0)
        .WithComponent<ScreenTransform>(Vector2{ 100.0f, 100.0f }, Vector2{ 200.0f, 200.0f }, Vector2{ 1.5f, 1.5f }, Vector2{ 0.0f, 0.0f })
        .AddToScene();

    auto child = parent->CreateGameObjectBuilder("Child", 0)
        .WithComponent<ScreenTransform>(Vector2{ 50.0f, 50.0f }, Vector2{ 100.0f, 100.0f }, Vector2{ 2.0f, 2.0f }, Vector2{ 0.5f, 0.5f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<ScreenTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // Parent: pos (100,100), size (200,200), scale (1.5,1.5), pivot (0,0)
    // Child: pos (50,50), size (100,100), scale (2,2), pivot (0.5,0.5)
    // Child local: scaledSize = (100*2,100*2) = (200,200), pivotOffset = (200*0.5,200*0.5) = (100,100)
    // Child pos = (50-100,50-100) = (-50,-50)
    // Relative to parent: pos = (-50*1.5 + 100, -50*1.5 + 100) = (25,25)
    // Size = (100*2*1.5, 100*2*1.5) = (300,300)
    REQUIRE(final.pos.x == Catch::Approx(25.0f));
    REQUIRE(final.pos.y == Catch::Approx(25.0f));
    REQUIRE(final.size.x == Catch::Approx(300.0f));
    REQUIRE(final.size.y == Catch::Approx(300.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.5f));
    REQUIRE(final.pivot.y == Catch::Approx(0.5f));
}

TEST_CASE_METHOD(SceneTestFixture, "GridTransform with Center pivot under Canvas", "[Canvas][GridTransform]") {
    auto canvas_go = scene.CreateGameObjectBuilder("Canvas", 0)
        .WithComponent<Canvas>(Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, AnchorType::TopLeft, Vector2{ 800.0f, 600.0f })
        .AddToScene();

    auto child = canvas_go->CreateGameObjectBuilder("Child", 0)
        .WithComponent<GridTransform>(Vector2{ 2.0f, 3.0f }, Vector2{ 50.0f, 50.0f }, Vector2{ 1.0f, 1.0f }, Vector2{ 0.5f, 0.5f })
        .AddToScene();

    scene.Frame(0.016f);

    auto child_trans = child->GetComponent<GridTransform>();
    REQUIRE(child_trans != nullptr);

    auto final = child_trans->GetFinalScreenTransform();
    // GridTransform: pos (2,3), size (50,50), scale (1,1), pivot (0.5,0.5)
    // Screen pos = (2*50, 3*50) - (50*1*0.5, 50*1*0.5) = (100-25, 150-25) = (75,125)
    // Size = (50,50)
    REQUIRE(final.pos.x == Catch::Approx(75.0f));
    REQUIRE(final.pos.y == Catch::Approx(125.0f));
    REQUIRE(final.size.x == Catch::Approx(50.0f));
    REQUIRE(final.size.y == Catch::Approx(50.0f));
    REQUIRE(final.pivot.x == Catch::Approx(0.5f));
    REQUIRE(final.pivot.y == Catch::Approx(0.5f));
}
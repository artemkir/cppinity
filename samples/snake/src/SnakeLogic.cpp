#include "Engine.h"

#include "Game.h"

void SnakeLogic::Start()
{
    inputHandler = gameObject->GetComponent<IInputHandler>();
    mazeGenerator = gameObject->GetScene()->FindGameObjectByName("maze_generator")->GetComponent<MazeGenerator>();
    apple = gameObject->GetScene()->FindGameObjectByName("apple")->GetComponent<AppleLogic>();
    rect = gameObject->GetComponent<RectRenderer>();
    
    auto scene = gameObject->GetScene();
    auto canvas = scene->FindGameObjectByName("MainCanvas");
    auto canvasSize = canvas->GetComponent<Canvas>()->GetCanvasSize();

    blockSize = canvasSize / Vector2{ MAZE_WIDTH, MAZE_HEIGHT };

	gameObject->SetActive(false);
}

void SnakeLogic::OnActive(bool active)
{
    //start new level
    if (active)
    {
        auto pos = mazeGenerator->GetRandomEmptyPosition();
        gameObject->GetTransform()->SetPosition(pos.x, pos.y);
        gameObject->GetTransform()->SetSize(blockSize.x, blockSize.y);
		inputHandler->Stop();
    }
    else
    {
		auto scene = gameObject->GetScene();
        
        for (size_t i = 0; i < tail.size(); i++)
        {
            scene->Destroy(tail[i]);
        }        

        tail.clear();
    }
}

void SnakeLogic::Update(float deltaTime)
{
    updateTimer += deltaTime;

    if (updateTimer < update_interval)
        return;

    updateTimer -= update_interval;

    auto dir = inputHandler->GetDirection();
    auto headTransform = gameObject->GetTransform();

    if (dir == Direction::STOP)
        return;

    int prevX = headTransform->GetPos().x;
    int prevY = headTransform->GetPos().y;

    switch (dir)
    {
    case Direction::LEFT:
        headTransform->SetPosition(prevX - 1, prevY);
        break;
    case Direction::RIGHT:
        headTransform->SetPosition(prevX + 1, prevY);
        break;
    case Direction::UP:
        headTransform->SetPosition(prevX, prevY - 1);
        break;
    case Direction::DOWN:
        headTransform->SetPosition(prevX, prevY + 1);
        break;
    default:
        break;
    }

    // Wrap around logic
    if (headTransform->GetPos().x < 0)
        headTransform->SetPosition(MAZE_WIDTH - 1, headTransform->GetPos().y);
    if (headTransform->GetPos().x >= MAZE_WIDTH)
        headTransform->SetPosition(0, headTransform->GetPos().y);
    if (headTransform->GetPos().y < 0)
        headTransform->SetPosition(headTransform->GetPos().x, MAZE_HEIGHT - 1);
    if (headTransform->GetPos().y >= MAZE_HEIGHT)
        headTransform->SetPosition(headTransform->GetPos().x, 0);

    for (size_t i = 0; i < tail.size(); i++)
    {
        auto tailTransform = tail[i]->GetTransform();
        int tempX = tailTransform->GetPos().x;
        int tempY = tailTransform->GetPos().y;
        tailTransform->SetPosition(prevX, prevY);
        prevX = tempX;
        prevY = tempY;

        tail[i]->GetComponent<SimpleCollider>()->SetActive(true);
    }
}

void SnakeLogic::OnCollide(GameObject *other)
{
    if (!other)
        return;

    if (other->GetName() == "apple")
    {
        score += 10;

        apple->RegenerateApple();

        auto headTransform = gameObject->GetTransform();
        
        int prevX = headTransform->GetPos().x;
        int prevY = headTransform->GetPos().y;
        
        if (!tail.empty())
        {
            prevX = tail.back()->GetTransform()->GetPos().x;
            prevY = tail.back()->GetTransform()->GetPos().y;
        }
        
        auto scene = gameObject->GetScene();
        auto canvas = scene->FindGameObjectByName("MainCanvas");
       
        auto color = rect->GetColor();

        auto newTail = canvas->CreateGameObjectBuilder("tail_" + std::to_string(tail.size()), OBSTACLE_TAG)
			.WithComponent<RectRenderer>((int)(color.r * 0.75f), (int)(color.g * 0.75f), (int)(color.b * 0.75f))
            .WithComponent<GridTransform>(Vector2{ prevX, prevY }, blockSize)
			.WithComponent<SimpleCollider>(false)
			.AddToScene();        

        tail.push_back(newTail);

        //if (tail.size() > 0 && update_interval > min_update_interval)
        //{
        //    update_interval -= tail_change_interval;
        //}
    }
    else if (other->GetTag() & OBSTACLE_TAG)
    {
        auto stateManager = gameObject->GetScene()->FindGameObjectByName("StateMachineRoot")->GetComponent<GameStateManager>();
        if (stateManager)
        {
            stateManager->TransitionTo(GameState::EndScreen);
        }
    }
}

int SnakeLogic::GetScore() { return score; }
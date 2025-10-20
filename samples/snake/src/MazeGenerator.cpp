#include "Engine.h"

#include "Game.h"

void MazeGenerator::AddGridToScene()
{
	std::mt19937 g(rd());
	std::uniform_int_distribution<> color(100, 140);

	float blockSizeW = (float)gameObject->GetScene()->GetRenderer()->GetW() / MAZE_WIDTH;
	float blockSizeH = (float)gameObject->GetScene()->GetRenderer()->GetH() / MAZE_HEIGHT;

	// Add walls to Scene
	for (int y = 0; y < MAZE_HEIGHT; ++y)
	{
		for (int x = 0; x < MAZE_WIDTH; ++x)
		{
			if (!grid[y][x])
			{
				int c = color(g);

				gameObject->GetScene()->CreateGameObjectBuilder("maze_block_" + std::to_string(x) + "_" + std::to_string(y), OBSTACLE_TAG)
					.WithComponent<GridTransform>(Vector2{ x, y }, Vector2{ blockSizeW, blockSizeH })
					.WithComponent<RectRenderer>(c, c, c)
					.WithComponent<SimpleCollider>()
					.AddToScene();
			}
		}
	}
}

void MazeGenerator::GenerateMaze(int startX, int startY)
{
	std::mt19937 g(rd());

	grid.resize(MAZE_HEIGHT, List<bool>(MAZE_WIDTH, true)); // All walls initially
	std::stack<std::pair<int, int>> stack;
	stack.push({ startX, startY });
	grid[startY][startX] = false; // Starting point is empty

	int _steps = 6;
	List<std::pair<int, int>> directions = { {-_steps, 0}, {_steps, 0}, {0, -_steps}, {0, _steps} };

	while (!stack.empty())
	{
		int x = stack.top().first;
		int y = stack.top().second;
		stack.pop();

		std::shuffle(directions.begin(), directions.end(), g);

		for (auto [dx, dy] : directions)
		{
			int nx = x + dx;
			int ny = y + dy;

			if (nx >= 0 && nx < MAZE_WIDTH && ny >= 0 && ny < MAZE_HEIGHT && grid[ny][nx])
			{
				int stepX = dx / _steps;
				int stepY = dy / _steps;
				int steps = _steps;

				for (int i = 1; i <= steps; ++i)
				{
					int mx = x + stepX * i;
					int my = y + stepY * i;
					grid[my][mx] = false;
				}

				stack.push({ nx, ny });
			}
		}
	}

	// Random holes
	int numHoles = 20;
	std::uniform_int_distribution<> disX(0, MAZE_WIDTH - 1);
	std::uniform_int_distribution<> disY(0, MAZE_HEIGHT - 1);

	for (int i = 0; i < numHoles; ++i)
	{
		int attempts = 20;
		while (attempts-- > 0)
		{
			int hx = disX(g);
			int hy = disY(g);
			if (!grid[hy][hx])
			{
				grid[hy][hx] = true;
				break;
			}
		}
	}

	AddGridToScene();
}

bool MazeGenerator::IsObstacle(int x, int y)
{
	auto& sceneObjects = gameObject->GetScene()->GetGameObjects();

	for (const auto& go : sceneObjects)
	{
		if (go->GetTag() & OBSTACLE_TAG &&
			go->GetTransform()->GetX() == x &&
			go->GetTransform()->GetY() == y)
		{
			return true;
		}
	}

	return false;
}

Vector2i MazeGenerator::GetRandomEmptyPosition()
{
	std::mt19937 g(rd());
	std::uniform_int_distribution<> disX(0, MAZE_WIDTH - 1);
	std::uniform_int_distribution<> disY(0, MAZE_HEIGHT - 1);

	int x = 0;
	int y = 0;

	const int MAX_ATTEMPTS = 1000;
	int attempts = 0;

	while (attempts < MAX_ATTEMPTS)
	{
		attempts++;
		x = disX(g);
		y = disY(g);

		if (IsObstacle(x, y))
			continue;

		break;
	}

	return { x, y };
}

void MazeGenerator::Awake()
{
	GenerateMaze(MAZE_WIDTH / 2, MAZE_HEIGHT / 2);
}
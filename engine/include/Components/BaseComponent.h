#pragma once

class GameObject;

// Base Component Class
class BaseComponent
{
protected:
	GameObject *gameObject = nullptr; // Reference to root GameObject
	bool active = true;

public:
	virtual ~BaseComponent() = default;
	void SetGameObject(GameObject *go) { gameObject = go; }
	GameObject *GetGameObject() { return gameObject; }
	virtual void Awake() {} // Called when object added to scene
	virtual void Start() {} // Called at the first scene update
	virtual void Update(float deltaTime) {}
	virtual void OnCollide(GameObject *other) {}
	void SetActive(bool active_)
	{
		active = active_;
		OnActive(active);
	}
	bool IsActive() const { return active; }
	virtual void OnActive(bool active_) {}
};
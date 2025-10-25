#pragma once

#include "Std.h"

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
	
	// Lifecycle methods, can be overridden by derived classes
	virtual void Awake() {} // Called when object added to scene
	virtual void Start() {} // Called at the first scene update
	virtual void Update(float deltaTime) {}
	virtual void OnCollide(GameObject *other) {}
	virtual void OnDestroy() {}
	
	void SetActive(bool active_)
	{		
		active = active_;
		OnActive(active);
	}

	bool IsActive() const { return active; }
	virtual void OnActive(bool active_) {}

    // Add this public getter to access gameObject from derived classes
    GameObject* GetGameObject() const { return gameObject; }
};
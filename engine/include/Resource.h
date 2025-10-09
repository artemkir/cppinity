// Resource.h
#pragma once

#include "Std.h"

class ResourceManager;  // Forward declare

enum class ResourceState { Unloaded, Loading, Loaded, Failed };

class Resource {
public:
    virtual ~Resource() = default;
    explicit Resource(ResourceManager* mgr) : manager_(mgr) {}

    ResourceState GetState() const { return state_; }
    const String& GetError() const { return error_; }
    const String& GetPath() const { return path_; }

    String path_;
    ResourceState state_ = ResourceState::Unloaded;
    String error_;
    ResourceManager* manager_ = nullptr;

protected:
    Resource() = default;
        
    virtual void CreateFromFileData(const uint8_t* data, size_t size) = 0;
    virtual void CreateFromMemory(const uint8_t* data, size_t size) = 0;
    virtual void HandleFailure(const String& err) {
        state_ = ResourceState::Failed;
        error_ = err;
    }

    friend class ResourceManager;
};
// Resource.h
#pragma once

#include <string>
#include <memory>
#include <stdexcept>

class ResourceManager;  // Forward declare

enum class ResourceState { Unloaded, Loading, Loaded, Failed };

class Resource {
public:
    virtual ~Resource() = default;
    Resource(ResourceManager* mgr) : manager_(mgr) {}

    ResourceState GetState() const { return state_; }
    const std::string& GetError() const { return error_; }
    const std::string& GetPath() const { return path_; }

protected:
    Resource() = default;

    virtual void ProcessData(const void* data, size_t size) = 0;
    virtual void HandleFailure(const std::string& err) {
        state_ = ResourceState::Failed;
        error_ = err;
    }

    std::string path_;
    ResourceState state_ = ResourceState::Unloaded;
    std::string error_;
    ResourceManager* manager_ = nullptr;

    friend class ResourceManager;
};
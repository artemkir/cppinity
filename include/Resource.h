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
    explicit Resource(ResourceManager* mgr) : manager_(mgr) {}

    ResourceState GetState() const { return state_; }
    const std::string& GetError() const { return error_; }
    const std::string& GetPath() const { return path_; }

    std::string path_;
    ResourceState state_ = ResourceState::Unloaded;
    std::string error_;
    ResourceManager* manager_ = nullptr;

protected:
    Resource() = default;
        
    virtual void CreateFromFileData(const uint8_t* data, size_t size) = 0;
    virtual void CreateFromMemory(const uint8_t* data, size_t size) = 0;
    virtual void HandleFailure(const std::string& err) {
        state_ = ResourceState::Failed;
        error_ = err;
    }

    friend class ResourceManager;
};
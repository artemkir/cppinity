// ResourceManager.h
#pragma once

#include "Resource.h"
#include <unordered_map>
#include <atomic>

extern "C" void sokol_fetch_request(const char* path, void* user_data, size_t buffer_size);

class IRenderer;

class ResourceManager {
public:
    ResourceManager(IRenderer* renderer_);
    ~ResourceManager();

    void Update();

    struct FetchUser {
        uint32_t id;
    };

    template <typename T>
    std::shared_ptr<T> Load(const std::string& path)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

        auto res = Get<T>(path);
        if (res) {
            return res;
        }

        res = std::make_shared<T>(this);
        res->path_ = path;
        res->state_ = ResourceState::Loading;

        // Keep it alive while fetching
        uint32_t id = next_id_++;
        pending_.emplace(id, res);

        FetchUser u{ id };
		size_t user_size = sizeof(FetchUser);

        sokol_fetch_request(path.c_str(), &u, user_size);

        cache_[path] = res;
        return res;
    }

    // Create resource from memory data
    template <typename T>
    std::shared_ptr<T> Create(const std::string& id, const void* data, size_t size)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

        auto res = Get<T>(path);
        if (res) {
            return res;
        }

        res = std::make_shared<T>(this);
        res->path_ = id; // Use ID as key for in-memory resources
        res->state_ = ResourceState::Loading;

        try
        {
            res->CreateFromMemory(data, size);
            res->state_ = ResourceState::Loaded;
        }
        catch (const std::exception& e)
        {
            res->HandleFailure(e.what());
        }

        cache_[key] = res;
        return res;
    }
    
    template <typename T>
    std::shared_ptr<T> CreateEmpty(const std::string& id)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

        auto res = Get<T>(id);
        if (res) {
            throw std::runtime_error("Resource already exists: " + id);
        }

        res = std::make_shared<T>(this);
        res->path_ = id;
        res->state_ = ResourceState::Loaded;

        cache_[id] = res;
        return res;
    }

    template <typename T>
    std::shared_ptr<T> Get(const std::string& key)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

        auto it = cache_.find(key);
        if (it != cache_.end())
        {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    IRenderer* GetRenderer() const { return renderer_; }

private:
    std::atomic<uint32_t> next_id_{ 1 };
    IRenderer* renderer_ = nullptr;

	//Using strong refs here, all resources are cached forever!
    std::unordered_map<std::string, std::shared_ptr<Resource>> cache_;

	//Refs to resources that are currently loading
    std::unordered_map<uint32_t, std::shared_ptr<Resource>> pending_;

public:
    void OnFetchComplete(const void* data, size_t size, bool failed, const char* error, void* user);
};
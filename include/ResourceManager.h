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

        auto key = path;
        auto it = cache_.find(key);
        if (it != cache_.end())
        {
            if (auto sp = it->second.lock())
            {
                return std::static_pointer_cast<T>(sp);
            }
        }

        auto res = std::make_shared<T>(this);
        res->path_ = path;
        res->state_ = ResourceState::Loading;

        // Keep it alive while fetching
        uint32_t id = next_id_++;
        pending_.emplace(id, res);

        FetchUser u{ id };
		size_t user_size = sizeof(FetchUser);

        sokol_fetch_request(path.c_str(), &u, user_size);

        cache_[key] = res;
        return res;
    }

    IRenderer* GetRenderer() const { return renderer_; }

private:
    std::atomic<uint32_t> next_id_{ 1 };
    IRenderer* renderer_ = nullptr;

	//Weak ptr to allow resources to be freed when no longer used
    std::unordered_map<std::string, std::weak_ptr<Resource>> cache_;

	//Own only strong refs to resources that are currently loading
    std::unordered_map<uint32_t, std::shared_ptr<Resource>> pending_;

public:
    void OnFetchComplete(const void* data, size_t size, bool failed, const char* error, void* user);
};
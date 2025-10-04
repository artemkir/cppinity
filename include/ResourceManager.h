// ResourceManager.h
#pragma once

#include "Resource.h"
#include <unordered_map>

extern "C" void sokol_fetch_request(const char* path, void* user_data, size_t buffer_size);

class IRenderer;

class ResourceManager {
public:
    ResourceManager(IRenderer* renderer_);
    ~ResourceManager();

    void Update();

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

        sokol_fetch_request(path.c_str(), res.get(), sizeof(res.get()));

        cache_[key] = res;
        return res;
    }

    IRenderer* GetRenderer() const { return renderer_; }

private:
    IRenderer* renderer_ = nullptr;
    std::unordered_map<std::string, std::weak_ptr<Resource>> cache_;

public:
    void OnFetchComplete(const void* data, size_t size, bool failed, const char* error, void* user);
};
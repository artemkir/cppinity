// ResourceManager.cpp
#include "ResourceManager.h"

extern "C" void sokol_fetch_setup(int max_requests, int num_channels, int num_lanes);
extern "C" void sokol_fetch_shutdown();
extern "C" void sokol_fetch_update();

/*template <typename T>
std::shared_ptr<T> ResourceManager::Load(const std::string &path)
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

    sokol_fetch_request(path.c_str(), res.get());

    cache_[key] = res;
    return res;
}*/

ResourceManager::ResourceManager(IRenderer* renderer_) : renderer_(renderer_)
{
    sokol_fetch_setup(128, 1, 1);
}

ResourceManager::~ResourceManager()
{
    sokol_fetch_shutdown();
}

void ResourceManager::Update()
{
    sokol_fetch_update();
}

void ResourceManager::OnFetchComplete(const void *data, size_t size, bool failed, const char *error, void *user)
{
    FetchUser u{};
    memcpy(&u, user, sizeof(FetchUser));

    auto it = pending_.find(u.id);

    // Request was cancelled or resource already released.
    if (it == pending_.end()) {
        return;
    }
    auto res = it->second; // strong ref

    if (failed)
    {
        res->HandleFailure(error ? error : "Unknown error");
    }
    else
    {
        try
        {
            res->ProcessData(data, size);
            res->state_ = ResourceState::Loaded;
        }
        catch (const std::exception &e)
        {
            res->HandleFailure(e.what());
        }
    }

    pending_.erase(it); // drop the strong ref when done
}
// ResourceManager.cpp
#include "ResourceManager.h"

#include "sokol_wrapper.h"

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

void ResourceManager::OnFetchComplete(const uint8_t*data, size_t size, bool failed, const char *error, void *user)
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
            res->CreateFromFileData(data, size);
            res->state_ = ResourceState::Loaded;
        }
        catch (const std::exception &e)
        {
            res->HandleFailure(e.what());
        }
    }

    pending_.erase(it); // drop the strong ref when done
}
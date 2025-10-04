#include "Resource.h"
#include "IRenderer.h"
#include "Texture.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

class ResourceManager;

class TextureResource : public Resource
{
public:
    explicit TextureResource(ResourceManager* mgr);

    void SetTarget(std::shared_ptr<Texture> target) { texture_ = target; }
    

    std::shared_ptr<Texture> GetTexture() const;

protected:
    void ProcessData(const void* data, size_t size) override;

private:
    std::shared_ptr<Texture> texture_;
};
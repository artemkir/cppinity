// Material.cpp
#include "Material.h"
#include "Shader.h"
#include <cstring>  // For memcpy
#include <stdexcept>

extern "C" uint32_t sokol_create_pipeline(uint32_t shader_id, int num_attrs, const int* attr_formats, int index_type);
extern "C" void sokol_destroy_pipeline(uint32_t id);

Material::Material(std::shared_ptr<Shader> shader)
	: shader_(shader)
{
	//No material without shader
	if (!shader) 
	{
		throw std::runtime_error("Material requires a valid shader");
	}

	// Prepare layout from shader attrs
	const auto& attrs = shader->GetAttributes();
	std::vector<int> attr_formats(attrs.size());
	for (size_t i = 0; i < attrs.size(); ++i) {
		attr_formats[i] = static_cast<int>(attrs[i].format);
	}

	// Create pipeline immediately
	pipeline_id_ = sokol_create_pipeline(shader->GetID(), attrs.size(), attr_formats.data(), static_cast<std::underlying_type_t<IndexType>>(IndexType::UINT16));

	uniform_buffer_.resize(GetUniformSize(), 0);
}

Material::~Material() 
{
	if (pipeline_id_ != 0) 
	{
		sokol_destroy_pipeline(pipeline_id_);
	}
}

void Material::SetUniform(const std::string& name, const std::vector<float>& value) 
{
	uniform_values_[name] = value;
	dirty_ = true;
}

void Material::SetTexture(const std::string& name, std::shared_ptr<ITexture> texture)
{
	if (shader_->GetImageSlots().find(name) == shader_->GetImageSlots().end()) {
		throw std::runtime_error("Texture sampler not found: " + name);
	}
	textures_[name] = texture;
}

const unsigned char* Material::GetUniformData() const 
{
	if (!dirty_) 
	{
		return uniform_buffer_.data();
	}

	std::fill(uniform_buffer_.begin(), uniform_buffer_.end(), 0);
	const auto& block = shader_->GetVSUniformBlock();
	
	for (const auto& u : block.uniforms) 
	{
		auto val_it = uniform_values_.find(u.name);
		
		if (val_it == uniform_values_.end()) 
		{
			continue;
		}
				
		memcpy(&uniform_buffer_[u.offset], val_it->second.data(), RenderTypeUtils::GetSize(u.type));
	}
	
	dirty_ = false;

	return uniform_buffer_.data();
}

std::shared_ptr<ITexture> Material::GetTexture(const std::string& name) const 
{
	auto it = textures_.find(name);
	return (it != textures_.end()) ? it->second : nullptr;
}
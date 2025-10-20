#include "sokol_wrapper.h"
#include <stdio.h>

// Stub implementations: empty or return defaults. No Sokol includes.
void sokol_draw_screen_quad(const float pos[2], const float size[2], const float color[4]) {
}

void sokol_begin_pass() {
}

void sokol_end_pass() {
}

uint32_t sokol_create_texture_from_memory_file(const void* data, size_t size) {
    return 0;
}

uint32_t sokol_create_texture_from_grayscale(int width, int height, const uint8_t* grayscaleData) {
    return 0;
}

uint32_t sokol_create_rgba_texture(int width, int height, const uint8_t* rgbaPixels) {
    return 0;
}

void sokol_destroy_texture(uint32_t id) {
}

uint32_t sokol_alloc_empty_view() {
    return 0;
}

uint32_t sokol_create_view(uint32_t view, uint32_t texture_id) {
    return 0;
}

void sokol_destroy_view(uint32_t view_id) {
}

void sokol_apply_view(uint32_t view_id) {
}

void sokol_unbind_texture(void) {
}

uint32_t sokol_create_shader(
    const char* vs_source,
    const char* fs_source,
    int num_attrs,
    const char** attr_names,
    const int* attr_formats,
    int uniform_block_size,
    int num_uniforms,
    const char** uniform_names,
    const int* uniform_types,
    int num_images,
    const char** image_names) {
    return 0;
}

void sokol_destroy_shader(uint32_t id) {
}

uint32_t sokol_create_pipeline(uint32_t shader_id, int num_attrs, const int* attr_formats, int index_type, bool alphaBlending) {
    return 0;
}

void sokol_destroy_pipeline(uint32_t id) {
}

void sokol_apply_pipeline(uint32_t id) {
}

void sokol_apply_uniforms(const void* data, int size) {
}

int sokol_get_screen_width() {
    return 0;
}

int sokol_get_screen_height() {
    return 0;
}

void sokol_draw(int num_elements) {
}

void sokol_fetch_setup(int max_requests, int num_channels, int num_lanes) {
}

void sokol_fetch_shutdown(void) {
}

void sokol_fetch_update(void) {
}

void sokol_fetch_request(const char* path, void* user_data, size_t buffer_size) {
    printf("Stub fetch request: %s\n", path);
}

void sokol_setup() {
}

#if !defined(SOKOL_GLES3)
#define SOKOL_GLCORE
#endif

#define SOKOL_IMPL
#define SOKOL_APP_IMPL

#include "InputEvent.h"
//#include "Scripts/GameConsts.h"

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "sokol_log.h"
#include "sokol_fetch.h"
#include "stb/stb_image.h"

#include "sokol_wrapper.h"

extern void app_init(int w, int h);
extern bool app_frame(float deltaTime);
extern void app_cleanup(void);
extern void app_event(const InputEvent *event);
extern void app_fetch_completed(const uint8_t* data, size_t size, void* user);
extern void app_fetch_failed(void* user);

static struct
{
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;
    int screen_width;
    int screen_height;
    uint8_t file_buffer[1024*1024*4];
} app_state;

typedef struct {
    float bottom_left[2];
    float size[2];
    float color[4];
    float screen_size[2];
} vs_uniforms_t;

static uint64_t last_time = 0;

void init(void)
{
    app_init(app_state.screen_width, app_state.screen_height);

    stm_setup();
    last_time = stm_now();
    srand(stm_sec(stm_now()));
}

void create_default_quad_buffers()
{
    float vertices[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    app_state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc) {
        .data = SG_RANGE(vertices),
            .label = "vertex-buffer"
    });

    // create an index buffer for the cube
    const uint16_t indices[] = {
        0,
        1,
        2,
        0,
        2,
        3,
    };

    app_state.bind.index_buffer = sg_make_buffer(&(sg_buffer_desc) {
        .usage.index_buffer = true,
            .data = SG_RANGE(indices),
            .label = "quad-indices"
    });
}

void cleanup_default_quad_buffers()
{
    sg_destroy_buffer(app_state.bind.vertex_buffers[0]);
    sg_destroy_buffer(app_state.bind.index_buffer);
}

void frame(void)
{
    uint64_t now = stm_now();
    float deltaTime = stm_sec(stm_diff(now, last_time));
    last_time = now;

    if (!app_frame(deltaTime))
    {
        sapp_request_quit();
    }
}

void sokol_draw_screen_quad(const float pos[2], const float size[2], const float color[4])
{
    vs_uniforms_t vs_params = {
		    .bottom_left = {pos[0], pos[1]},
			.size = {size[0], size[1]},
			.color = {color[0], color[1], color[2], color[3]},
			.screen_size = {(float)app_state.screen_width, (float)app_state.screen_height}
        };
            
    sg_apply_uniforms(0, &SG_RANGE(vs_params));

    sg_draw(0, 6, 1);
}

void sokol_begin_pass()
{
    sg_begin_pass(&(sg_pass){
        .action = app_state.pass_action,
        .swapchain = sglue_swapchain()});

    //sg_apply_pipeline(app_state.pip);

    //sg_apply_bindings(&app_state.bind);
}

void sokol_end_pass()
{
    sg_end_pass();
    sg_commit();
}

void cleanup(void)
{
    cleanup_default_quad_buffers();

    app_cleanup();
    sg_shutdown();
}

static void event(const sapp_event *e)
{
    InputEvent custom;

    switch (e->type)
    {
    case SAPP_EVENTTYPE_KEY_DOWN:
        custom.type = EventType_KeyDown;
        custom.key_code = (int)e->key_code;
        break;
    case SAPP_EVENTTYPE_KEY_UP:
        custom.type = EventType_KeyUp;
        custom.key_code = (int)e->key_code;
        break;
    case SAPP_EVENTTYPE_QUIT_REQUESTED:
        custom.type = EventType_QuitRequested;
        break;
    default:
        custom.type = EventType_Unknown;
        break;
    }

    app_event(&custom);
}

uint32_t sokol_create_texture_from_memory_file(const void* data, size_t size)
{
    int png_width = 0;
    int png_height = 0;
    int num_channels = 0;
    const int desired_channels = 4;

    stbi_uc* pixels = stbi_load_from_memory(
        data,
        (int)size,
        &png_width, &png_height,
        &num_channels, desired_channels);

    if (pixels) {
        // create an image object from the loaded texture date
        sg_image img = sg_make_image(&(sg_image_desc) {
            .width = png_width,
                .height = png_height,
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .data.mip_levels[0] = {
                    .ptr = pixels,
                    .size = (size_t)(png_width * png_height * 4),
            },
            .label = "png-image",
        });
        stbi_image_free(pixels);

        // ...and initialize the pre-allocated texture view handle with that image
        //sg_init_view(state.bind.views[VIEW_tex], &(sg_view_desc){
        //    .texture = { .image = img },
        //        .label = "png-texture-view",
        //});

        return img.id;
    }

    return 0;
}

uint32_t sokol_create_texture_from_grayscale(int width, int height, const uint8_t* grayscaleData)
{
    size_t pixel_count = (size_t)(width * height);
    size_t buffer_size = sizeof(uint32_t) * pixel_count;
    uint32_t *rgbaPixels = (uint32_t*)malloc(buffer_size);

    if (!rgbaPixels)
        return 0;

    int offset = 0;
    for (int i = 0; i < pixel_count; i++) {
        uint8_t value = grayscaleData[i];
        uint32_t rgba = (uint32_t)value |
            ((uint32_t)value << 8) |
            ((uint32_t)value << 16) |
            ((value == 0 ? 0u : 255u) << 24);
        rgbaPixels[i] = rgba;
    }

    sg_image im = sg_make_image(&(sg_image_desc) {
        .width = width,
        .height = height,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .data.mip_levels[0] = (sg_range){.ptr = rgbaPixels, .size = buffer_size}
    });

    free(rgbaPixels);

    return im.id;
}

uint32_t sokol_create_rgba_texture(int width, int height, const uint8_t* rgbaPixels)
{
    if (!rgbaPixels)
        return 0;

    size_t pixel_count = (size_t)(width * height);
    size_t buffer_size = sizeof(uint32_t) * pixel_count;
    
    sg_image im = sg_make_image(&(sg_image_desc) {
            .width = width,
            .height = height,
            .pixel_format = SG_PIXELFORMAT_RGBA8,
            .data.mip_levels[0] = (sg_range){ .ptr = rgbaPixels, .size = buffer_size }
    });

    return im.id;
}

// Destroy texture by ID
void sokol_destroy_texture(uint32_t id)
{
    sg_image img = {.id = id};
    sg_destroy_image(img);
}

uint32_t sokol_alloc_empty_view()
{
    return sg_alloc_view().id;
}

uint32_t sokol_create_view(uint32_t view, uint32_t texture_id)
{
    //no view before
    if (view == 0)
    {
        return sg_make_view(&(sg_view_desc) { .texture.image = texture_id }).id;
    }
    //view preallocated before
    else
    {
        sg_init_view((sg_view) { .id = view }, & (sg_view_desc) { .texture.image = texture_id });
        return view;
    }
}

void sokol_destroy_view(uint32_t view_id)
{
    sg_destroy_view((sg_view){ .id = view_id });
}

void sokol_apply_view(uint32_t view_id)
{
    app_state.bind.views[0] = (sg_view){ .id = view_id };
}

void sokol_unbind_texture(void)
{
    // sg_texture(sg_make_image(&(sg_image_desc) { 0 }));
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
    const char** image_names) 
{
    sg_shader_desc desc = {0};
    desc.vertex_func.source = vs_source;
    desc.fragment_func.source = fs_source;

    for (int i = 0; i < num_attrs; ++i) {
        desc.attrs[i].glsl_name = attr_names[i];
    }

    desc.uniform_blocks[0].size = uniform_block_size;
    
    for (int i = 0; i < num_uniforms; ++i) {

        desc.uniform_blocks[0].stage = SG_SHADERSTAGE_VERTEX;
        desc.uniform_blocks[0].glsl_uniforms[i].glsl_name = uniform_names[i];
        desc.uniform_blocks[0].glsl_uniforms[i].type = (sg_uniform_type)uniform_types[i];
    }

    for (int i = 0; i < num_images; ++i) {
        desc.views[i].texture.stage = SG_SHADERSTAGE_FRAGMENT;
        desc.samplers[i].stage = SG_SHADERSTAGE_FRAGMENT;
        desc.texture_sampler_pairs[i] = (sg_shader_texture_sampler_pair){
                .stage = SG_SHADERSTAGE_FRAGMENT,
                .view_slot = 0,
                .sampler_slot = 0,
                .glsl_name = image_names[i]
            };
        }

    sg_shader shd = sg_make_shader(&desc);
    return shd.id;
}

void sokol_destroy_shader(uint32_t id) {
    sg_destroy_shader((sg_shader){.id = id});
}

uint32_t sokol_create_pipeline(uint32_t shader_id, int num_attrs, const int* attr_formats, int index_type, bool alphaBlending) {
    sg_pipeline_desc desc = {0};

    desc.shader.id = shader_id;
    for (int i = 0; i < num_attrs; ++i) {
        desc.layout.attrs[i].format = (sg_vertex_format)attr_formats[i];
    }
    desc.index_type = (sg_index_type)index_type;
    
    if (alphaBlending)
    {
        desc.colors[0].blend.enabled = true;
        desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
        desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        desc.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_ONE;  // SRC_ALPHA?
        desc.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    }

    sg_pipeline pip = sg_make_pipeline(&desc);
    return pip.id;
}

void sokol_destroy_pipeline(uint32_t id) {
    sg_destroy_pipeline((sg_pipeline){.id = id});
}

void sokol_apply_pipeline(uint32_t id) {
    sg_apply_pipeline((sg_pipeline){.id = id});
}

void sokol_apply_uniforms(const void* data, int size) {
    sg_range u = {.ptr = data, .size = (size_t)size};
    sg_apply_uniforms(0, &u);
}

int sokol_get_screen_width() {
    return app_state.screen_width;
}

int sokol_get_screen_height() {
    return app_state.screen_height;
}

void sokol_draw(int num_elements) {
    sg_apply_bindings(&app_state.bind);
    sg_draw(0, num_elements, 1);
}

void sokol_fetch_setup(int max_requests, int num_channels, int num_lanes) {
    sfetch_desc_t desc = {
        .max_requests = max_requests,
        .num_channels = num_channels,
        .num_lanes = num_lanes,
        .logger.func = slog_func
    };
    sfetch_setup(&desc);
}

void sokol_fetch_shutdown(void) {
    sfetch_shutdown();
}

void sokol_fetch_update(void) {
    sfetch_dowork();
}

void fetch_loaded_callback(const uint8_t* data, size_t size, void* user) {
    app_fetch_completed(data,size,user);
}

void fetch_failed_callback(void* user) {
    app_fetch_failed(user);
}

void fetch_callback(const sfetch_response_t* response) {
    if (response->fetched) {
        fetch_loaded_callback(response->data.ptr, response->data.size, response->user_data);
    }
    else if (response->failed) {
        fetch_failed_callback(response->user_data);
    }

    char buffer[256];
    sprintf(buffer, "Fetch completed: %s (fetched: %d, failed: %d, size: %zu, error: %d)\n", response->path, response->fetched, response->failed, response->data.size, response->error_code);

    printf("%s",buffer);
}

void sokol_fetch_request(const char* path, 
                         void* user_data,
                         size_t buffer_size)
{
    sfetch_request_t req = {
        .path = path,
        .callback = fetch_callback,  
        .user_data = (sfetch_range_t){.ptr = user_data, .size = buffer_size},
		.buffer = SFETCH_RANGE(app_state.file_buffer),
    };

    sfetch_handle_t f = sfetch_send(&req);

	printf("Fetch request sent: %s (handle: %d)\n", path, f.id);
}

void sokol_setup()
{
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func});
       
    app_state.pass_action = (sg_pass_action){
        .colors[0] = {.load_action = SG_LOADACTION_CLEAR, .clear_value = {0.2f, 0.3f, 0.3f, 1.0f}}};

    app_state.bind.samplers[0] = sg_make_sampler(&(sg_sampler_desc) {
        .min_filter = SG_FILTER_LINEAR,
        .mag_filter = SG_FILTER_LINEAR,
    });

    create_default_quad_buffers();
}

sapp_desc sokol_main(int argc, char *argv[])
{
    app_state.screen_width = 1066;
    app_state.screen_height = 600;

    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = app_state.screen_width,
        .height = app_state.screen_height,
        .high_dpi = true,
        .window_title = "Application",
#if defined(WIN32) && defined(_DEBUG)
		.win32_console_create = true,
#endif
    };
}
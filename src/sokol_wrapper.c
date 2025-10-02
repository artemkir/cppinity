#define SOKOL_GLCORE
#define SOKOL_IMPL
#define SOKOL_NO_DEPRECATED
#define SOKOL_CPP_NO_SHORTCUTS

#include "InputEvent.h"

#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "sokol_log.h"

#include "Shaders\triangle-sapp.glsl.h"

extern void app_init(void);
extern bool app_frame(uint64_t deltaTime);
extern void app_cleanup(void);
extern void app_event(const InputEvent *event);

static struct {
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;
} state;

static uint64_t last_time = 0;

void init(void) {
    app_init();

    stm_setup();
    last_time = stm_now();
    srand(stm_sec(stm_now()));
}

void sokol_setup() {
    
    sg_setup(&(sg_desc) {
        .environment = sglue_environment(),
        .logger.func = slog_func
    });

    state.pass_action = (sg_pass_action){
        .colors[0] = {.load_action = SG_LOADACTION_CLEAR, .clear_value = {0.2f, 0.3f, 0.3f, 1.0f} }
    };

    // create shader from code-generated sg_shader_desc
    sg_shader shd = sg_make_shader(triangle_shader_desc(sg_query_backend()));

    // create a pipeline object (default render states are fine for triangle)
    state.pip = sg_make_pipeline(&(sg_pipeline_desc) {
        .shader = shd,
            // if the vertex layout doesn't have gaps, don't need to provide strides and offsets
            .layout = {
                .attrs = {
                    [ATTR_triangle_position].format = SG_VERTEXFORMAT_FLOAT3,
                    [ATTR_triangle_color0].format = SG_VERTEXFORMAT_FLOAT4
                }
        },
            .index_type = SG_INDEXTYPE_UINT16,
            .label = "triangle-pipeline"
    });
}

void createRect()
{
    // a vertex buffer with 3 vertices and view for binding
    float vertices[] = {
        // positions            // colors
         -1.0f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,
         -1.0f, -1.0f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
          1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,
          1.0f, 1.0f, 0.0f,      0.0f, 1.0f, 1.0f, 1.0f
    };
    state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc) {
        .data = SG_RANGE(vertices),
            .label = "vertex-buffer"
    });

    // create an index buffer for the cube
    const uint16_t indices[] = {
        0, 1, 2,  0, 2, 3,
    };

    state.bind.index_buffer = sg_make_buffer(&(sg_buffer_desc) {
        .usage.index_buffer = true,
            .data = SG_RANGE(indices),
            .label = "cube-indices"
    });
}

void cleanupRect()
{
	sg_destroy_buffer(state.bind.vertex_buffers[0]);
	sg_destroy_buffer(state.bind.index_buffer);
}

void frame(void) {

    uint64_t now = stm_now();
    float deltaTime = stm_sec(stm_diff(now, last_time));
    last_time = now;

    if (!app_frame(deltaTime))
    {
         sapp_request_quit();
    }
}

void sokol_begin_pass() {
    sg_begin_pass(&(sg_pass) {
        .action = state.pass_action,
            .swapchain = sglue_swapchain()
    });

    sg_apply_pipeline(state.pip);

    for (int i = 0; i < 20; i++)
    {
        createRect();

        sg_apply_bindings(&state.bind);

        sg_draw(0, 6, 1);

        cleanupRect();
    }
}

void sokol_end_pass() {
    sg_end_pass();
	sg_commit();
}

void cleanup(void) {
    app_cleanup();
    sg_shutdown();
}

static void event(const sapp_event* e) {
    InputEvent custom;
    
    switch (e->type) {
        case SAPP_EVENTTYPE_KEY_DOWN:
            custom.type = EventType_KeyDown;  // Use enum values from CustomEvent.h (include it)
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

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc) {
            .init_cb = init,
            .frame_cb = frame,
            .cleanup_cb = cleanup,
            .event_cb = event,
            .width = 800,
            .height = 600,
            .high_dpi = true,
            .window_title = "Application",
    };
}

//Texture wrappers for C interface
uint32_t sokol_create_texture(int width, int height, const unsigned char* pixelData) {
    /*std::vector<uint32_t> rgbaPixels(width * height);
    for (int i = 0; i < width * height; ++i) {
        uint8_t value = pixelData[i];
        rgbaPixels[i] = value | (value << 8) | (value << 16) | (value == 0 ? 0 : 255 << 24);  // RGBA
    }

    sg_image_desc desc = (sg_image_desc){ 0 };
    desc.width = width;
    desc.height = height;
    desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    desc.min_filter = SG_FILTER_NEAREST;
    desc.mag_filter = SG_FILTER_NEAREST;
    desc.content.subimage[0][0].ptr = rgbaPixels.data();
    desc.content.subimage[0][0].size = width * height * 4;

    sg_image img = sg_make_image(&desc);
    if (sg_query_image_state(img) != SG_RESOURCESTATE_VALID) {
        // Handle error, e.g., return 0
        return 0;
    }
    return img.id;*/

	return 0; // Placeholder return value
}

// Destroy texture by ID
void sokol_destroy_texture(uint32_t id) {
    sg_image img = { .id = id };
    sg_destroy_image(img);
}

// Bind texture for drawing (call before draw quads)
void sokol_bind_texture(uint32_t id) {
    //sg_image img = { .id = id };
    //sg_texture(img);
}

// Unbind texture (call after draw)
void sokol_unbind_texture(void) {
    //sg_texture(sg_make_image(&(sg_image_desc) { 0 }));
}
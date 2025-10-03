
#if !defined(SOKOL_GLES3)
#define SOKOL_GLCORE
#endif

#define SOKOL_IMPL

#include "InputEvent.h"
#include "Scripts/GameConsts.h"

#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "sokol_log.h"

extern void app_init(void);
extern bool app_frame(float deltaTime);
extern void app_cleanup(void);
extern void app_event(const InputEvent *event);

static struct
{
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;
    int screen_width;
    int screen_height;
} app_state;

static uint64_t last_time = 0;

void init(void)
{
    app_init();

    stm_setup();
    last_time = stm_now();
    srand(stm_sec(stm_now()));
}

sg_shader make_custom_shader()
{
    return sg_make_shader(&(sg_shader_desc){
        .attrs = {
            [0].glsl_name = "pos",
            [1].glsl_name = "color0",
        },
        .vertex_func.source = "#version 300 es\n"
                              "in vec4 pos;"
                              "in vec4 color0;"
                              "out vec4 color;"
                              "void main() {"
                              "  gl_Position = pos;\n"
                              "  color = color0;\n"
                              "}\n",
        .fragment_func.source = "#version 300 es\n"
                                "precision mediump float;\n"
                                "in vec4 color;\n"
                                "out vec4 frag_color;\n"
                                "void main() {\n"
                                "  frag_color = color;\n"
                                "}\n"});
}

typedef struct {
    float bottom_left[2];
    float size[2];
    float color[4];
    float screen_size[2];
} vs_uniforms_t;

sg_shader make_shader_for_2d_quads()
{
    return sg_make_shader(&(sg_shader_desc) {
        .attrs = {
            [0].glsl_name = "pos"
        },
        .uniform_blocks[0] = {
            .stage = SG_SHADERSTAGE_VERTEX,
            .size = sizeof(vs_uniforms_t),
            .glsl_uniforms = {
                [0] = { .glsl_name = "u_bottom_left", .type = SG_UNIFORMTYPE_FLOAT2 },
                [1] = { .glsl_name = "u_size", .type = SG_UNIFORMTYPE_FLOAT2 },
                [2] = { .glsl_name = "u_color", .type = SG_UNIFORMTYPE_FLOAT4},
                [3] = { .glsl_name = "u_screen_size", .type = SG_UNIFORMTYPE_FLOAT2},
			},
		},
        .vertex_func.source = "#version 300 es\n"
            "uniform vec2 u_bottom_left;\n"
            "uniform vec2 u_size;\n"
            "uniform vec4 u_color;\n"
            "uniform vec2 u_screen_size;\n"
            "in vec2 pos;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "  float pixel_x = u_bottom_left.x + pos.x * u_size.x;\n"
            "  float pixel_y = u_bottom_left.y + (1.0 - pos.y) * u_size.y;\n"
            "  float norm_x = (pixel_x / u_screen_size.x) * 2.0 - 1.0;\n"
            "  float norm_y = 1.0 - (pixel_y / u_screen_size.y) * 2.0;\n"
            "  gl_Position = vec4(norm_x, norm_y, 0.0, 1.0);\n"
            "  color = u_color;\n"
            "}\n",
         .fragment_func.source = "#version 300 es\n"
            "precision mediump float;\n"
            "in vec4 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "  frag_color = color;\n"
            "}\n"
    });
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
            .label = "cube-indices"
    });
}

void sokol_setup()
{
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func});

    app_state.pass_action = (sg_pass_action){
        .colors[0] = {.load_action = SG_LOADACTION_CLEAR, .clear_value = {0.2f, 0.3f, 0.3f, 1.0f}}};

    app_state.pip = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = make_shader_for_2d_quads(),
        .layout = {
            .attrs = {
                [0].format = SG_VERTEXFORMAT_FLOAT2}},
        .index_type = SG_INDEXTYPE_UINT16,
        .label = "triangle-pipeline"});

    create_default_quad_buffers();
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

    sg_apply_pipeline(app_state.pip);

    sg_apply_bindings(&app_state.bind);
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
        custom.type = EventType_KeyDown; // Use enum values from CustomEvent.h (include it)
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

sapp_desc sokol_main(int argc, char *argv[])
{
    app_state.screen_width = WIDTH * TILE_SIZE + 2;
    app_state.screen_height = HEIGHT * TILE_SIZE + 2;

    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = app_state.screen_width,
        .height = app_state.screen_height,
        .high_dpi = true,
        .window_title = "Application",
    };
}

// Texture wrappers for C interface
uint32_t sokol_create_texture(int width, int height, const unsigned char *pixelData)
{
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
void sokol_destroy_texture(uint32_t id)
{
    sg_image img = {.id = id};
    sg_destroy_image(img);
}

// Bind texture for drawing (call before draw quads)
void sokol_bind_texture(uint32_t id)
{
    // sg_image img = { .id = id };
    // sg_texture(img);
}

// Unbind texture (call after draw)
void sokol_unbind_texture(void)
{
    // sg_texture(sg_make_image(&(sg_image_desc) { 0 }));
}
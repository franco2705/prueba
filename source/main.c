#include <switch.h>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct {
    int x;
    int y;
    float depth;
} ScreenPoint;

static const Vec3 cube_vertices[8] = {
    {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},
};

static const uint8_t cube_edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6},
    {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7},
};

static inline void put_pixel(uint32_t *pixels, uint32_t width, uint32_t height,
                             int x, int y, uint32_t color) {
    if (x >= 0 && y >= 0 && (uint32_t)x < width && (uint32_t)y < height) {
        pixels[(uint32_t)y * width + (uint32_t)x] = color;
    }
}

static void draw_line(uint32_t *pixels, uint32_t width, uint32_t height,
                      ScreenPoint a, ScreenPoint b, uint32_t color) {
    int dx = abs(b.x - a.x);
    int sx = a.x < b.x ? 1 : -1;
    int dy = -abs(b.y - a.y);
    int sy = a.y < b.y ? 1 : -1;
    int error = dx + dy;

    for (;;) {
        put_pixel(pixels, width, height, a.x, a.y, color);
        if (a.x == b.x && a.y == b.y) break;
        int twice_error = 2 * error;
        if (twice_error >= dy) {
            error += dy;
            a.x += sx;
        }
        if (twice_error <= dx) {
            error += dx;
            a.y += sy;
        }
    }
}

static ScreenPoint project_vertex(Vec3 vertex, float rotation_x, float rotation_y,
                                  uint32_t width, uint32_t height) {
    float cos_y = cosf(rotation_y);
    float sin_y = sinf(rotation_y);
    float x = vertex.x * cos_y - vertex.z * sin_y;
    float z = vertex.x * sin_y + vertex.z * cos_y;

    float cos_x = cosf(rotation_x);
    float sin_x = sinf(rotation_x);
    float y = vertex.y * cos_x - z * sin_x;
    z = vertex.y * sin_x + z * cos_x + 5.0f;

    const float focal_length = 430.0f;
    return (ScreenPoint){
        .x = (int)((float)width * 0.5f + x * focal_length / z),
        .y = (int)((float)height * 0.5f - y * focal_length / z),
        .depth = z,
    };
}

int main(void) {
    consoleDebugInit(debugDevice_SVC);

    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    Framebuffer framebuffer;
    framebufferCreate(&framebuffer, nwindowGetDefault(), 1280, 720,
                      PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&framebuffer);

    float rotation_x = 0.35f;
    float rotation_y = 0.0f;

    while (appletMainLoop()) {
        padUpdate(&pad);
        uint64_t pressed = padGetButtonsDown(&pad);
        uint64_t held = padGetButtons(&pad);
        HidAnalogStickState stick = padGetStickPos(&pad, HidNpadButton_StickL);

        if (pressed & HidNpadButton_Plus) break;
        if (pressed & HidNpadButton_A) {
            rotation_x = 0.35f;
            rotation_y = 0.0f;
        }

        rotation_y += 0.012f + (float)stick.x / 32768.0f * 0.045f;
        rotation_x += (float)stick.y / 32768.0f * 0.045f;
        if (held & HidNpadButton_Left) rotation_y -= 0.035f;
        if (held & HidNpadButton_Right) rotation_y += 0.035f;
        if (held & HidNpadButton_Up) rotation_x += 0.035f;
        if (held & HidNpadButton_Down) rotation_x -= 0.035f;

        uint32_t width, height;
        uint32_t *pixels = framebufferBegin(&framebuffer, &width, &height);
        for (uint32_t i = 0; i < width * height; i++) pixels[i] = 0xFF1A102A;

        ScreenPoint projected[8];
        for (int i = 0; i < 8; i++) {
            projected[i] = project_vertex(cube_vertices[i], rotation_x, rotation_y,
                                          width, height);
        }
        for (int i = 0; i < 12; i++) {
            ScreenPoint a = projected[cube_edges[i][0]];
            ScreenPoint b = projected[cube_edges[i][1]];
            uint32_t color = (a.depth + b.depth < 10.0f) ? 0xFFFFA840 : 0xFF63E6FF;
            draw_line(pixels, width, height, a, b, color);
        }
        framebufferEnd(&framebuffer);
    }

    framebufferClose(&framebuffer);
    consoleExit(NULL);
    return 0;
}

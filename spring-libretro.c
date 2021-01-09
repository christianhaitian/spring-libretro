#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include "libretro.h"

#define LOG_PREFIX "Spring - "
#define MAX_PATH 1024

static uint32_t *frame_buf;
static struct retro_log_callback logging;
static retro_log_printf_t log_cb;

static char cmd[MAX_PATH];

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
    (void)level;
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
}

void retro_init(void)
{
    frame_buf = calloc(320 * 240, sizeof(uint32_t));
}

void retro_deinit(void)
{
    free(frame_buf);
    frame_buf = NULL;
}

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
    (void)port;
    (void)device;
}

void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = "Spring";
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
    info->library_version  = "0.2.0" GIT_VERSION;
    info->need_fullpath    = true;
    info->valid_extensions = NULL;
}

static retro_video_refresh_t video_cb;
static retro_environment_t environ_cb;

void retro_get_system_av_info(struct retro_system_av_info *info)
{
    float aspect = 4.0f / 3.0f;
    float sampling_rate = 30000.0f;

    info->timing = (struct retro_system_timing) {
        .fps = 60.0,
        .sample_rate = sampling_rate,
    };

    info->geometry = (struct retro_game_geometry) {
        .base_width   = 320,
        .base_height  = 240,
        .max_width    = 320,
        .max_height   = 240,
        .aspect_ratio = aspect,
    };
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;

    bool no_content = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_content);

    if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
        log_cb = logging.log;
    else
        log_cb = fallback_log;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
    (void)cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
    (void)cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
    (void)cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
    (void)cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
    video_cb = cb;
}

void retro_reset(void)
{
}

void retro_run(void)
{
//    // Clear the display.
//    unsigned stride = 320;
//    video_cb(frame_buf, 320, 240, stride << 2);
//
//
    environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
}

bool retro_load_game(const struct retro_game_info *info)
{
    char line[MAX_PATH];
    char cmd_info[MAX_PATH];
    char safe_path[MAX_PATH];
    char core_info[MAX_PATH] = {0};
    char tmp[MAX_PATH];

    const char* path;
    environ_cb(RETRO_ENVIRONMENT_GET_LIBRETRO_PATH, &path);

    strcpy(tmp, path);
    char* ptr = tmp + strlen(tmp)-1;
    while (*ptr != '.')
        --ptr;
    strcpy(ptr, ".info");

    if (access(tmp, F_OK) == 0) {
        strcpy(core_info, tmp);
    }
    else {
        strcat(core_info, "/usr/share/libretro/info");
        while (*ptr != '/')
            --ptr;
        strcat(core_info, ptr);
    }

    printf("core_info path [%s]\n", core_info);
    printf("reload_load_game path [%s]\n", info->path);

    snprintf(safe_path, sizeof(safe_path)-1, "'%s'", info->path);

    FILE* f = fopen(core_info, "r");
    if (!f) {
        log_cb(RETRO_LOG_ERROR, LOG_PREFIX "Failed to open core_info");
        return false;
    }

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "command = \"%[^\"]\"", cmd_info);
    }

    fclose(f);

    snprintf(cmd, sizeof(cmd)-1, cmd_info, safe_path);
    printf("Command [%s]\n", cmd);

    return true;
}

void retro_unload_game(void)
{
    char* args[4] = { "sh", "-c", cmd, 0 };

    for (int i = 0; i < 3; i++) {
        if (args[i])
            log_cb(RETRO_LOG_DEBUG, LOG_PREFIX "arg %d [%s]\n", i, args[i]);
    }

    execvp(args[0], args);
}

unsigned retro_get_region(void)
{
    return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    return retro_load_game(info);
}

size_t retro_serialize_size(void)
{
    return 0;
}

bool retro_serialize(void *data_, size_t size)
{
    return true;
}

bool retro_unserialize(const void *data_, size_t size)
{
    return true;
}

void *retro_get_memory_data(unsigned id)
{
    (void)id;
    return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
    (void)id;
    return 0;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
    (void)index;
    (void)enabled;
    (void)code;
}

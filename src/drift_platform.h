#ifndef DRIFT_PLATFORM_H
#define DRIFT_PLATFORM_H

// Time
static f32 drift_get_elapsed_time(u64 previous_counter, u64 current_counter);

// Window Settings
static void drift_load_app_defaults(drift_app_t *drift_app);
static int drift_get_window_refresh_rate(SDL_Window *window);

// Events
static void drift_reset_frame_based_input(drift_platform_t *platform);
static void drift_handle_event(drift_platform_t *platform, SDL_Event *event);

#endif

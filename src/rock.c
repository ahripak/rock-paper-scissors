#include <pebble.h>

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )

static Window *window;
static BitmapLayer *image_layer;
static GBitmap *image;
static uint32_t imgs[3];

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  psleep(1000);
  vibes_double_pulse();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Should work");
  psleep(1000);
  vibes_double_pulse();
  psleep(1000);
  vibes_double_pulse();
  psleep(500);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  image = gbitmap_create_with_resource(imgs[rand() % ARR_SIZE(imgs)]);

  image_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(image_layer, image);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
}

static void window_load(Window *window) {
  light_enable(true);
  accel_tap_service_subscribe(accel_tap_handler);
}

static void window_unload(Window *window) {
  light_enable(false);
  bitmap_layer_destroy(image_layer);
  gbitmap_destroy(image);
  accel_tap_service_unsubscribe();
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  imgs[0] = RESOURCE_ID_IMAGE_ROCK;
  imgs[1] = RESOURCE_ID_IMAGE_PAPER;
  imgs[2] = RESOURCE_ID_IMAGE_SCISSORS;
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

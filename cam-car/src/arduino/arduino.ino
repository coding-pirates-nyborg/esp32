/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-cam-projects-ebook/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h"          // disable brownout problems
#include "soc/rtc_cntl_reg.h" // disable brownout problems
#include "esp_http_server.h"
#include "index_html.h"
#include "constants.h"
#include "GPIO.h"
#include "neo_pixel_control.h"
#include "Context.h"

#include "Forward.h"
#include "Left.h"
#include "Right.h"
#include "Backward.h"
#include "Stop.h"

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

#define NUM_LEDS 8

NeoPixel pixels[NUM_LEDS] = {
    {0, 50, CRGB::Red},
    {1, 50, CRGB::Green},
    {2, 50, CRGB::Blue},
    {3, 50, CRGB::Yellow},
    {4, 50, CRGB::Purple},
    {5, 50, CRGB::Cyan},
    {6, 50, CRGB::White},
    {7, 50, CRGB::Orange}};

CRGB leds[NUM_LEDS] = {
    pixels[0].rgb_color,
    pixels[1].rgb_color,
    pixels[2].rgb_color,
    pixels[3].rgb_color,
    pixels[4].rgb_color,
    pixels[5].rgb_color,
    pixels[6].rgb_color,
    pixels[7].rgb_color
  };

#define LED_PIN 22  

NeoPixelControl neoPixelControl(pixels, leds, NUM_LEDS, LED_PIN);

static esp_err_t index_handler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

static esp_err_t stream_handler(httpd_req_t *req)
{
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t *_jpg_buf = NULL;
  char *part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK)
  {
    return res;
  }

  while (true)
  {
    fb = esp_camera_fb_get();
    if (!fb)
    {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    }
    else
    {
      if (fb->width > 400)
      {
        if (fb->format != PIXFORMAT_JPEG)
        {
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!jpeg_converted)
          {
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        }
        else
        {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if (res == ESP_OK)
    {
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK)
    {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (res == ESP_OK)
    {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (fb)
    {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    }
    else if (_jpg_buf)
    {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK)
    {
      break;
    }
    // Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}

static esp_err_t cmd_handler(httpd_req_t *req)
{
  int res = 0;
  char *buf;
  size_t buf_len;
  char cmd[32] = {
      0,
  };

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1)
  {
    buf = (char *)malloc(buf_len);
    if (!buf)
    {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
    {
      if (httpd_query_key_value(buf, "cmd", cmd, sizeof(cmd)) == ESP_OK)
      {
        Context *context = new Context();
        Serial.printf("Received command: %s ",cmd);
        if (strcmp(cmd, "forward") == 0)
        {
          context->setStrategy(new Forward());
          context->executeStrategy();
        }
        else if (strcmp(cmd, "left") == 0)
        {
          context->setStrategy(new Left());
          context->executeStrategy();
        }
        else if (strcmp(cmd, "right") == 0)
        {
          context->setStrategy(new Right());
          context->executeStrategy();
        }
        else if (strcmp(cmd, "backward") == 0)
        {
          context->setStrategy(new Backward());
          context->executeStrategy();
        }
        else if (strcmp(cmd, "stop") == 0)
        {
          context->setStrategy(new Stop());
          context->executeStrategy();
        }
        else if (strcmp(cmd, "killLEDS") == 0){
          neoPixelControl.turn_off(); 
        }
        else if (strcmp(cmd, "turnOnLEDS") == 0){
          neoPixelControl.turn_on();
        }
        else if (strcmp(cmd, "blinkLEDS") == 0){
          neoPixelControl.blink(500, 5);
        }
        else
        {
          res = 1;
        }
        delete context;
      }
      else
      {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
      free(buf);
    }
    else
    {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
  }
  else
  {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
  sensor_t *s = esp_camera_sensor_get();

  if (res)
  {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void startCameraServer()
{
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
      .uri = "/",
      .method = HTTP_GET,
      .handler = index_handler,
      .user_ctx = NULL};

  httpd_uri_t cmd_uri = {
      .uri = "/action",
      .method = HTTP_GET,
      .handler = cmd_handler,
      .user_ctx = NULL};
  httpd_uri_t stream_uri = {
      .uri = "/stream",
      .method = HTTP_GET,
      .handler = stream_handler,
      .user_ctx = NULL};
  if (httpd_start(&camera_httpd, &config) == ESP_OK)
  {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
  }
  config.server_port += 1;
  config.ctrl_port += 1;
  if (httpd_start(&stream_httpd, &config) == ESP_OK)
  {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  pinMode(CAMERA_FLASH, OUTPUT); //

  pinMode(NEO_PIXEL_PIN, OUTPUT);
  // pinMode(NEO_PIXEL_REAR, OUTPUT); VIRKER IKKE MED CAM! GPIO 16 TODO find alternativ

  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);

  Serial.begin(115200);
  Serial.setDebugOutput(false);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())
  {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Replace with your network credentials
  const char *ssid = "frey-pirates-bil";
  const char *password = "codingpirates";
  const int channel = 9; // should be 3 or 9 to avoid interference with other wifi networks

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Wi-Fi connection
  WiFi.softAP(ssid, password, channel);

  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.softAPIP());

  // turn_off_all_lights();

  // Start streaming web server
  startCameraServer();

  neoPixelControl.setup();
}

void loop()
{

}

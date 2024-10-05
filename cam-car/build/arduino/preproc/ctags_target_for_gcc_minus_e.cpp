# 1 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-cam-projects-ebook/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

# 10 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2
# 11 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2

# 13 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2
# 14 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2
# 15 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2

# 17 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2
# 18 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2
# 19 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2
# 20 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2
# 21 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 2


httpd_handle_t camera_httpd = 
# 23 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                             __null
# 23 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                                 ;
httpd_handle_t stream_httpd = 
# 24 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                             __null
# 24 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                                 ;

/*-----------------------CAMERA STREAM---------------*/
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" "123456789000000000000987654321";
static const char* _STREAM_BOUNDARY = "\r\n--" "123456789000000000000987654321" "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = 
# 37 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                    __null
# 37 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                        ;
  esp_err_t res = 0 /*!< esp_err_t value indicating success (no error) */;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = 
# 40 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                      __null
# 40 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                          ;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != 0 /*!< esp_err_t value indicating success (no error) */){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial0.println("Camera capture failed");
      res = -1 /*!< Generic esp_err_t code indicating failure */;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = 
# 58 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
              __null
# 58 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                  ;
          if(!jpeg_converted){
            Serial0.println("JPEG compression failed");
            res = -1 /*!< Generic esp_err_t code indicating failure */;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == 0 /*!< esp_err_t value indicating success (no error) */){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == 0 /*!< esp_err_t value indicating success (no error) */){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == 0 /*!< esp_err_t value indicating success (no error) */){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = 
# 81 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
          __null
# 81 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
              ;
      _jpg_buf = 
# 82 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                __null
# 82 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                    ;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = 
# 85 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                __null
# 85 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                    ;
    }
    if(res != 0 /*!< esp_err_t value indicating success (no error) */){
      break;
    }
    //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}

static esp_err_t cmd_handler(httpd_req_t *req){
  char* buf;
  size_t buf_len;
  char variable[32] = {0,};

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return -1 /*!< Generic esp_err_t code indicating failure */;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == 0 /*!< esp_err_t value indicating success (no error) */) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == 0 /*!< esp_err_t value indicating success (no error) */) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return -1 /*!< Generic esp_err_t code indicating failure */;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return -1 /*!< Generic esp_err_t code indicating failure */;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return -1 /*!< Generic esp_err_t code indicating failure */;
  }

  sensor_t * s = esp_camera_sensor_get();
  int res = 0;

  if(!strcmp(variable, "forward")) {
    Serial0.println("Forward");
    digitalWrite(14, 1);
    digitalWrite(15, 0);
    digitalWrite(13, 1);
    digitalWrite(12, 0);
  }
  else if(!strcmp(variable, "left")) {
    Serial0.println("Left");
    digitalWrite(14, 0);
    digitalWrite(15, 1);
    digitalWrite(13, 1);
    digitalWrite(12, 0);
  }
  else if(!strcmp(variable, "right")) {
    Serial0.println("Right");
    digitalWrite(14, 1);
    digitalWrite(15, 0);
    digitalWrite(13, 0);
    digitalWrite(12, 1);
  }
  else if(!strcmp(variable, "backward")) {
    Serial0.println("Backward");
    digitalWrite(14, 0);
    digitalWrite(15, 1);
    digitalWrite(13, 0);
    digitalWrite(12, 1);
  }
  else if(!strcmp(variable, "stop")) {
    Serial0.println("Stop");
    digitalWrite(14, 0);
    digitalWrite(15, 0);
    digitalWrite(13, 0);
    digitalWrite(12, 0);
  }
  else {
    res = -1;
  }

  if(res){
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, 
# 172 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                             __null
# 172 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                                 , 0);
}

void startCameraServer(){
  httpd_config_t config = { .task_priority = ( ( UBaseType_t ) 0U )+5, .stack_size = 4096, .core_id = ( 0x7FFFFFFF ), .server_port = 80, .ctrl_port = (32768) /*!< HTTP Server control socket port*/, .max_open_sockets = 7, .max_uri_handlers = 8, .max_resp_headers = 8, .backlog_conn = 5, .lru_purge_enable = false, .recv_wait_timeout = 5, .send_wait_timeout = 5, .global_user_ctx = 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                         __null
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                         , .global_user_ctx_free_fn = 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                         __null
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                         , .global_transport_ctx = 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                         __null
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                         , .global_transport_ctx_free_fn = 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                         __null
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                         , .enable_so_linger = false, .linger_timeout = 0, .keep_alive_enable = false, .keep_alive_idle = 0, .keep_alive_interval = 0, .keep_alive_count = 0, .open_fn = 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                         __null
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                         , .close_fn = 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                         __null
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                         , .uri_match_fn = 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                         __null 
# 176 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
                         };
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = index_handler,
    .user_ctx = 
# 182 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                __null
  
# 183 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
 };

  httpd_uri_t cmd_uri = {
    .uri = "/action",
    .method = HTTP_GET,
    .handler = cmd_handler,
    .user_ctx = 
# 189 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                __null
  
# 190 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
 };
  httpd_uri_t stream_uri = {
    .uri = "/stream",
    .method = HTTP_GET,
    .handler = stream_handler,
    .user_ctx = 
# 195 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
                __null
  
# 196 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
 };
  if (httpd_start(&camera_httpd, &config) == 0 /*!< esp_err_t value indicating success (no error) */) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
  }
  config.server_port += 1;
  config.ctrl_port += 1;
  if (httpd_start(&stream_httpd, &config) == 0 /*!< esp_err_t value indicating success (no error) */) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

void setup() {
  do { do { if (__builtin_constant_p(!(((((0x3ff48000 + 0xd4))) >= 0x3ff00000) && (((0x3ff48000 + 0xd4))) <= 0x3ff13FFC)) && !(!(((((0x3ff48000 + 0xd4))) >= 0x3ff00000) && (((0x3ff48000 + 0xd4))) <= 0x3ff13FFC))) { extern __attribute__((error("(Cannot use WRITE_PERI_REG for DPORT registers use DPORT_WRITE_PERI_REG)"))) void failed_compile_time_assert(void); failed_compile_time_assert(); } 
# 209 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
 (__builtin_expect(!!(
# 209 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
 "(Cannot use WRITE_PERI_REG for DPORT registers use DPORT_WRITE_PERI_REG)" && (!(((((0x3ff48000 + 0xd4))) >= 0x3ff00000) && (((0x3ff48000 + 0xd4))) <= 0x3ff13FFC))
# 209 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
 ), 1) ? (void)0 : __assert_func ((__builtin_strrchr( "/" "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino", '/') + 1), 209, __PRETTY_FUNCTION__, 
# 209 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
 "\"(Cannot use WRITE_PERI_REG for DPORT registers use DPORT_WRITE_PERI_REG)\" && (!(((((0x3ff48000 + 0xd4))) >= 0x3ff00000) && (((0x3ff48000 + 0xd4))) <= 0x3ff13FFC))"
# 209 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino" 3 4
 ))
# 209 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/arduino.ino"
 ; } while(0);; (*((volatile uint32_t *)((0x3ff48000 + 0xd4)))) = (uint32_t)(0); } while(0); //disable brownout detector

  pinMode(4, 0x03); // 

  pinMode(2, 0x03);
  pinMode(16, 0x03);

  pinMode(14, 0x03);
  pinMode(15, 0x03);
  pinMode(13, 0x03);
  pinMode(12, 0x03);

  Serial0.begin(115200);
  Serial0.setDebugOutput(false);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Replace with your network credentials
  const char* ssid = "coding-pirates-bil";
  const char* password = "codingpirates";
  const int channel = 9;

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != 0 /*!< esp_err_t value indicating success (no error) */) {
    Serial0.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Wi-Fi connection
  WiFi.softAP(ssid, password, channel);

  Serial0.println("");
  Serial0.println("WiFi connected");

  Serial0.print("Camera Stream Ready! Go to: http://");
  Serial0.println(WiFi.softAPIP());

 // turn_off_all_lights();

  // Start streaming web server
  startCameraServer();

}

void loop() {

}

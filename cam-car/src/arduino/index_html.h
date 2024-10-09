static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>
  <head>
    <title>ESP32-CAM Robot</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      table { margin-left: auto; margin-right: auto; }
      td { padding: 8 px; }
      .button {
        background-color: #354597;
        border: none;
        color: white;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(139, 32, 32, 0);
      }
      .button-blue {
        background-color: #416e1e;
        border: none;
        color: white;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }

      img { 
        width: auto ;
        max-width: 100% ;
        height: auto ; 
      }

      .avatar {
        width: auto;
        max-width: 50%;
        height: 100;
      }
    </style>
  </head>
  <body>
    <h1>Coding Pirates Nyborg ESP32-CAM Robot</h1>
    <!-- <img src="" id="cam" > -->
    <img src="" class="avatar" id="cam" alt="Camera stream not available" onerror="this.onerror=null;this.src='avatar.jpeg';">
    
    <table>
      <tr><td colspan="3" align="center"><button class="button-blue" onmousedown="move('forward');" ontouchstart="move('forward');" onmouseup="move('stop');" ontouchend="move('stop');">Forward</button></td></tr>
      <tr><td align="center"><button class="button" onmousedown="move('left');" ontouchstart="move('left');" onmouseup="move('stop');" ontouchend="move('stop');">Left</button></td><td align="center"><button class="button" onmousedown="move('stop');" ontouchstart="move('stop');">Stop</button></td><td align="center"><button class="button" onmousedown="move('right');" ontouchstart="move('right');" onmouseup="move('stop');" ontouchend="move('stop');">Right</button></td></tr>
      <tr><td colspan="3" align="center"><button class="button" onmousedown="move('backward');" ontouchstart="move('backward');" onmouseup="move('stop');" ontouchend="move('stop');">Backward</button></td></tr>                   
    </table>
    <!-- <img src="avatar.jpeg" id="avatar" > -->
    <script>
      function move(direction) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?move=" + direction, true);
     xhr.send();
   }
   window.onload = document.getElementById("cam").src = window.location.href.slice(0, -1) + ":81/stream";
  </script>
  </body>
</html>

)rawliteral";

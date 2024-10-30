static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>

<head>
  <title>ESP32-CAM Robot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin: 0px auto;
      padding-top: 30px;
    }

    table {
      margin-left: auto;
      margin-right: auto;
    }

    td {
      padding: 8 px;
    }

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
      -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
    }

    img {
      width: auto;
      max-width: 100%;
      height: auto;
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
  <!-- <div id="cam"> -->
    <img src="" class="avatar" id="cam" alt="Camera stream not available"
      onerror="this.onerror=null;this.src='avatar.jpeg';">
  <!-- </div> -->
  <!-- <div id="carControl"> -->
    <table>
      <tr>
        <td colspan="3" align="center"><button class="button-blue" onmousedown="sendCommand('forward');"
            ontouchstart="sendCommand('forward');" onmouseup="sendCommand('stop');" ontouchend="sendCommand('stop');">Forward</button></td>
      </tr>
      <tr>
        <td align="center"><button class="button" onmousedown="sendCommand('left');" ontouchstart="sendCommand('left');"
            onmouseup="sendCommand('stop');" ontouchend="sendCommand('stop');">Left</button></td>
        <td align="center"><button class="button" onmousedown="sendCommand('stop');" ontouchstart="sendCommand('stop');">Stop</button>
        </td>
        <td align="center"><button class="button" onmousedown="sendCommand('right');" ontouchstart="sendCommand('right');"
            onmouseup="sendCommand('stop');" ontouchend="sendCommand('stop');">Right</button></td>
      </tr>
      <tr>
        <td colspan="3" align="center"><button class="button" onmousedown="sendCommand('backward');"
            ontouchstart="sendCommand('backward');" onmouseup="sendCommand('stop');" ontouchend="sendCommand('stop');">Backward</button></td>
      </tr>
    </table>

    <table>
      <tr>
        <td rowspan="3" align="center"><button class="button" onmousedown="sendCommand('killLEDS');">Kill All LEDS</button></td>
        <td rowspan="3" align="center"><button class="button" onmousedown="sendCommand('turnOnLEDS');">Turn on LED's</button></td>
        <td rowspan="3" align="center"><button class="button" onmousedown="sendCommand('blinkLEDS');">Blink</button></td>
      </tr>
    </table>
  <!-- </div> -->
  <script>
    // localStorage.setItem('showNeopixelControl', true);
    if (localStorage.getItem('showNeopixelControl') === 'true') {
      document.write(`
        <div id="neopixelControl">
          <td colspan="3" align="center"><button class="button" onmouseup="getNeopixels();">Load NeoPixels</button></td>
        </div>
      `);
    }
  </script>

  <script>
    function getNeopixels() {
      console.log("Getting NeoPixels");
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/action?neopixels", true);
      xhr.onload = function () {
        if (xhr.status >= 200 && xhr.status < 300) {
          console.log("Response status: " + xhr.status);
          console.log(xhr.responseText); 
          var responseArray = JSON.parse(xhr.responseText);
          console.log(responseArray); // Log the array to the console
          // You can now use responseArray as a JavaScript array of objects
        }else{
          console.log("Error: " + xhr.status);
        }
      }; xhr.send();
    }

    function sendCommand(cmd) {
      console.log(`Sending cmd: ${cmd} to ESP32-CAM`);
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/action?cmd=" + cmd, true);
      xhr.send();
    }

    window.onload = document.getElementById("cam").src = window.location.href.slice(0, -1) + ":81/stream";
  </script>
</body>

</html>
)rawliteral";

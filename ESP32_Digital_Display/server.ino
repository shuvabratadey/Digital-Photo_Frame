// List all files in SPIFFS
String listFiles()
{
  String fileList = "<ul>";
  fs::File root = SPIFFS.open("/");
  fs::File file = root.openNextFile();
  while (file)
  {
    String fname = file.name();
    String displayName = fname;
    if (displayName.startsWith("/")) displayName.remove(0, 1);

    // Create image preview for JPG/JPEG files
    if (displayName.endsWith(".jpg") || displayName.endsWith(".jpeg") ||
        displayName.endsWith(".JPG") || displayName.endsWith(".JPEG"))
    {
      fileList += "<li>"
                  "<img src=\"" + fname + "\" alt=\"" + displayName + 
                  "\" style=\"max-width:80px; max-height:80px; border-radius:5px; margin-right:10px;\">"
                  "<span>" + displayName + "</span> "
                  "<button onclick='deleteFile(\"" + fname + "\")'>Delete</button></li>";
    }
    else
    {
      // For non-image files, just show the name
      fileList += "<li><a href=\"" + fname + "\">" + displayName + "</a> "
                  "<button onclick='deleteFile(\"" + fname + "\")'>Delete</button></li>";
    }

    file = root.openNextFile();
  }
  fileList += "</ul>";
  return fileList;
}

// Serve HTML page
String htmlMainPage()
{
  return R"rawliteral(
    <!DOCTYPE html>
    <html>    
    <head>
      <meta charset="UTF-8">
      <title>ESP32 File Manager</title>
      <style>
        body {
          font-family: Arial, sans-serif;
          background: linear-gradient(to right, #74ebd5, #acb6e5);
          color: #333;
          padding: 20px;
          max-width: 700px;
          margin: auto;
          border-radius: 10px;
          box-shadow: 0 0 15px rgba(0, 0, 0, 0.2);
        }
    
        h2,
        h3 {
          text-align: center;
          color: #222;
        }
    
        form {
          background-color: #ffffffbb;
          padding: 15px;
          border-radius: 10px;
          text-align: center;
          margin-bottom: 20px;
        }
    
        input[type="file"] {
          margin: 10px 0;
          padding: 6px;
          border-radius: 5px;
        }
    
        input[type="submit"] {
          background-color: #4CAF50;
          border: none;
          color: white;
          padding: 10px 18px;
          border-radius: 5px;
          cursor: pointer;
          font-size: 16px;
        }
    
        input[type="submit"]:hover {
          background-color: #45a049;
        }
    
        ul {
          list-style-type: none;
          padding: 0;
        }
    
        li {
          background: #fff;
          margin: 8px 0;
          padding: 10px;
          border-radius: 8px;
          display: flex;
          justify-content: space-between;
          align-items: center;
          box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);
        }
    
        a {
          text-decoration: none;
          color: #007BFF;
          font-weight: bold;
        }
    
        button {
          background-color: #ff4d4d;
          border: none;
          color: white;
          padding: 6px 12px;
          border-radius: 5px;
          cursor: pointer;
          font-size: 14px;
        }
    
        button:hover {
          background-color: #e60000;
        }
    
        #preview {
          display: block;
          margin: 10px auto;
          border: 2px solid #444;
          border-radius: 6px;
          max-width: 161px;
        }
      </style>
      <script>
        function deleteFile(fname) {
          fetch('/delete?file=' + fname, { method: 'GET' })
            .then(response => {
              if (response.ok) {
                location.reload();
              } else {
                alert("Delete failed.");
              }
            });
        }
    
        function deleteAllFiles() {
          if (confirm("Are you sure you want to delete ALL files?")) {
            fetch('/delete_all', { method: 'GET' })
              .then(response => response.text())
              .then(result => {
                alert(result);
                location.reload();
              });
          }
        }
    
        window.addEventListener('DOMContentLoaded', () => {
          const form = document.getElementById("uploadForm");
          const fileInput = document.getElementById("imageInput");
          const canvas = document.getElementById("resizeCanvas");
          const previewImg = document.getElementById("preview");
          let resizedBlob = null;
    
          fileInput.addEventListener("change", () => {
            const file = fileInput.files[0];
            if (!file || !file.type.startsWith("image/jpeg")) {
              alert("Please select a JPG image.");
              return;
            }
    
            const reader = new FileReader();
            reader.onload = function (e) {
              const img = new Image();
              img.onload = function () {
                const targetWidth = 161;
                const targetHeight = 130;
    
                canvas.width = targetWidth;
                canvas.height = targetHeight;
    
                const ctx = canvas.getContext("2d");
                ctx.clearRect(0, 0, targetWidth, targetHeight);
                ctx.drawImage(img, 0, 0, targetWidth, targetHeight);
    
                canvas.toBlob(function (blob) {
                  resizedBlob = blob;
                  // Show preview
                  previewImg.src = URL.createObjectURL(blob);
                  previewImg.style.display = "block";
                }, "image/jpeg", 0.9);
              };
              img.src = e.target.result;
            };
            reader.readAsDataURL(file);
          });
    
          form.addEventListener("submit", function (e) {
            e.preventDefault();
            if (!resizedBlob) {
              alert("Image not ready yet. Please wait.");
              return;
            }
    
            const formData = new FormData();
            const originalFileName = fileInput.files[0].name;
            formData.append("upload", resizedBlob, originalFileName);
    
            fetch("/upload", {
              method: "POST",
              body: formData,
            })
              .then((response) => {
                if (response.ok) {
                  alert("Upload successful!");
                  location.reload();
                } else if (response.status === 409) {
                  alert("Upload failed: File already exists.");
                } else {
                  alert("Upload failed.");
                }
              })
              .catch((err) => {
                console.error(err);
                alert("Upload error.");
              });
          });
        });
      </script>
    </head>
    
    <body>
      <div style="position: absolute; top: 20px; right: 20px;">
        <button
          style="background-color:#007BFF; color:white; padding:10px 16px; border:none; border-radius:5px; cursor:pointer;"
          onclick="window.location.href = '/wifi';">WiFi Credentials</button>
      </div>
    
      <h2>ESP32 SPIFFS File Manager</h2>
    
      <form id="uploadForm" enctype="multipart/form-data">
        <input type="file" id="imageInput" accept=".jpg, .jpeg" required>
        <br>
        <img id="preview" style="display:none;" alt="Preview (161x130)">
        <input type="submit" value="Upload Resized Image">
      </form>
      <canvas id="resizeCanvas" style="display:none;"></canvas>
    
      <hr>
      <h3>Stored Files</h3>
      %FILE_LIST%
    
      <button
        style="background-color:#cc0000; color:white; padding:10px 20px; border:none; border-radius:5px; cursor:pointer;"
        onclick="deleteAllFiles()">Delete All Files</button>
    
      <center>
        <button
          style="background-color:blue; color:white; padding:10px 20px; border:none; border-radius:5px; cursor:pointer;"
          onclick="window.location.href = '/exit';">Done</button>
      </center>
    </body>
    </html>
  )rawliteral";
}

// Serve HTML page
String htmlWiFiCredentialsPage()
{
  return R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>ESP32 WiFi Setup</title>
    <style>
      body {
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        background: #f4f6f8;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
      }
      .container {
        background: #fff;
        padding: 2rem 3rem;
        border-radius: 8px;
        box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
        width: 100%;
        max-width: 400px;
        text-align: center;
      }
      h2 {
        margin-bottom: 1.5rem;
        color: #333;
      }
      input[type="text"],
      input[type="password"],
      select {
        width: 100%;
        padding: 0.8rem;
        margin: 0.5rem 0 1rem;
        border: 1px solid #ccc;
        border-radius: 5px;
        font-size: 1rem;
        box-sizing: border-box;
      }
      button,
      .back-link {
        padding: 0.8rem 1.5rem;
        border: none;
        border-radius: 5px;
        font-size: 1rem;
        cursor: pointer;
        text-decoration: none;
        display: inline-block;
      }
      button {
        background-color: #007bff;
        color: white;
        margin-right: 0.5rem;
      }
      button:hover {
        background-color: #0056b3;
      }
      .back-link {
        background-color: #6c757d;
        color: white;
      }
      .back-link:hover {
        background-color: #5a6268;
      }
      .button-group {
        display: flex;
        justify-content: center;
        gap: 1rem;
        margin-top: 1rem;
      }
      .footer {
        margin-top: 2rem;
        font-size: 0.8rem;
        color: #aaa;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h2>Connect to WiFi</h2>
      <form action="/save" method="POST">
        <input type="text" name="ssid" placeholder="WiFi SSID" required />
        <input type="password" name="password" placeholder="WiFi Password" required />
        <select name="wifi_mode" required>
          <option value="WiFi_STA">WiFi_STA</option>
          <option value="WiFi_AP">WiFi_AP</option>
        </select>
        <div class="button-group">
          <button type="submit">Save & Reboot</button>
          <a href="/" class="back-link">Back</a>
        </div>
      </form>
      <div class="footer">ESP32 Setup Page</div>
    </div>
  </body>
  </html>
  )rawliteral";
}

void scheduleRestart() {
  restartTimer.once(2, []() {
    ESP.restart();
  });
}

void ExtraTasks(void *param)
{
  if (MODE == DEFUALT_AP_MODE)
  {
    Serial.println("AP_Mode");
    Serial.print("SSID: ");
    Serial.println("ESP32_AP");
    Serial.print("PASS: ");
    Serial.println("12345678");
    WiFi.softAP("ESP32_AP", "12345678");
  }
  else if (MODE == USER_MODE)
  {
    if (AP_OR_STA_MODE == AP_MODE)
    {
      Serial.println("AP_Mode");
      Serial.print("SSID: ");
      Serial.println(ssid);
      Serial.print("PASS: ");
      Serial.println(password);
      WiFi.softAP(ssid, password);
    }
    else
    {
      Serial.println("STA_Mode");
      Serial.print("SSID: ");
      Serial.println(ssid);
      Serial.print("PASS: ");
      Serial.println(password);
      WiFi.begin(ssid, password);
      Serial.println("Connecting...");
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
    }
  }

  if ((AP_OR_STA_MODE == AP_MODE) || (MODE == DEFUALT_AP_MODE))
  {
    // Initialize mDNS
    if (!MDNS.begin("esp32"))
    { // Set the hostname to "esp32.local"
      Serial.println("Error setting up MDNS responder!");
      while (1)
      {
        delay(500);
      }
    }
    Serial.print("\r\nConnected to WiFi\r\nConnect with http://esp32.local or IP: http://");
    Serial.println(WiFi.softAPIP());
  }
  else if ((AP_OR_STA_MODE == USER_MODE) || (MODE == STA_MODE))
  {
    // Initialize mDNS
    if (!MDNS.begin("esp32"))
    { // Set the hostname to "esp32.local"
      Serial.println("Error setting up MDNS responder!");
      while (1)
      {
        delay(500);
      }
    }
    MDNS.addService("http", "tcp", 80);
    Serial.print("\r\nConnected to WiFi\r\nConnect with http://esp32.local or IP: http://");
    Serial.println(WiFi.localIP());
    showIP(WiFi.localIP().toString());
  }

  // Serve root with file list
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    pause_display = true;
    String html = htmlMainPage();
    html.replace("%FILE_LIST%", listFiles());
    request->send(200, "text/html", html);
  });

  // Serve static files with automatic content type
  server.serveStatic("/", SPIFFS, "/");

  // Handle file uploads
  server.on("/upload", HTTP_POST,
  [](AsyncWebServerRequest * request) {
    if (rejectUpload) {
      rejectUpload = false; // reset for next request
      request->send(409, "text/plain", "File already exists");
    } else {
      request->redirect("/");
    }
  },
  [](AsyncWebServerRequest * request, String filename, size_t index,
     uint8_t *data, size_t len, bool final) {

    if (index == 0) {
      Serial.printf("UploadStart: %s\n", filename.c_str());

      // Check if file already exists
      if (SPIFFS.exists("/" + filename)) {
        Serial.println("File exists. Rejecting upload.");
        rejectUpload = true;
        return; // Don't create or write the file
      }

      rejectUpload = false; // file does not exist
    }

    if (rejectUpload) return; // skip all chunks

    // Proceed with writing
    fs::File file = SPIFFS.open("/" + filename, index ? FILE_APPEND : FILE_WRITE);
    if (file) {
      file.write(data, len);
      file.close();
    }

    if (final) {
      Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
    }
  });

  // Handle file deletion
  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    if (request->hasParam("file")) {
      String filename = request->getParam("file")->value();
      Serial.println("Delete requested: " + filename);

      // Ensure filename starts with "/"
      if (!filename.startsWith("/")) {
        filename = "/" + filename;
      }

      if (SPIFFS.exists(filename)) {
        SPIFFS.remove(filename);
        Serial.println("Deleted: " + filename);
        request->send(200, "text/plain", "Deleted");
        return;
      } else {
        Serial.println("File not found: " + filename);
      }
    }
    request->send(400, "text/plain", "Invalid Request");
  });

  // Handle all files deletion
  server.on("/delete_all", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    fs::File root = SPIFFS.open("/");
    int deleted = 0;

    while (true) {
      fs::File file = root.openNextFile();
      if (!file) break;

      String path = file.name();
      file.close();

      if (SPIFFS.remove("/" + path)) {
        Serial.println("Deleted: " + path);
        deleted++;
      } else {
        Serial.println("Failed to delete: " + path);
      }
    }

    request->send(200, "text/plain", "Deleted " + String(deleted) + " file(s).");
  });

  server.on("/exit", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/html", "<center><h1><font color='red'>Data Updated Now Please Close The Page</font></h1></center>");
    scheduleRestart();
  });

  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    pause_display = false;
    String html = htmlWiFiCredentialsPage();
    request->send(200, "text/html", html);
  });

  server.on("/save", HTTP_POST, [](AsyncWebServerRequest * request)
  {
    pause_display = false;

    if (request->hasParam("ssid", true)) {
      memset(ssid, 0, sizeof(ssid));
      strcpy(ssid, request->getParam("ssid", true)->value().c_str());
      Serial.println("SSID: " + String(ssid));
    }

    if (request->hasParam("password", true)) {
      memset(password, 0, sizeof(password));
      strcpy(password, request->getParam("password", true)->value().c_str());
      Serial.println("Password: " + String(password));
    }

    if (request->hasParam("wifi_mode", true)) {
      String mode = request->getParam("wifi_mode", true)->value();
      if (mode == "WiFi_STA") {
        AP_OR_STA_MODE = STA_MODE;
      } else {
        AP_OR_STA_MODE = AP_MODE;
      }
      Serial.println("WiFi Mode: " + mode);
    }
    MODE = USER_MODE;
    save_credentials();
    request->send(200, "text/html", "<center><h1><font color='red'>Data Updated Now Please Close The Page</font></h1></center>");
    scheduleRestart();
  });

  server.begin();
  while (1)
  {
    delay(50);
  }
}

void save_credentials()
{
  EEPROM.begin(EEPROM_SIZE);

  EEPROM.write(WIFI_MODE_SAVE_ADDR, AP_OR_STA_MODE);
  EEPROM.write(MODE_SAVE_ADDR, MODE);

  // Write SSID
  for (int i = 0; i < SSID_LEN; i++)
  {
    EEPROM.write(SSID_ADDR + i, i < strlen(ssid) ? ssid[i] : 0);
  }

  // Write Password
  for (int i = 0; i < PASS_LEN; i++)
  {
    EEPROM.write(PASS_ADDR + i, i < strlen(password) ? password[i] : 0);
  }

  EEPROM.commit();
  EEPROM.end();

  Serial.println("Credentials saved to EEPROM.");
}

void retrive_credentials()
{
  EEPROM.begin(EEPROM_SIZE);

  AP_OR_STA_MODE = EEPROM.read(WIFI_MODE_SAVE_ADDR);
  MODE = EEPROM.read(MODE_SAVE_ADDR);

  // Write SSID
  for (int i = 0; i < SSID_LEN; i++)
  {
    ssid[i] = EEPROM.read(SSID_ADDR + i);
  }

  // Write Password
  for (int i = 0; i < PASS_LEN; i++)
  {
    password[i] = EEPROM.read(PASS_ADDR + i);
  }

  ssid[SSID_LEN] = '\0';
  password[PASS_LEN] = '\0';

  EEPROM.end();
  if (ssid[0] == '\0' || ssid[0] == 255)
  {
    strcpy(ssid, "ESP32_AP");
    strcpy(password, "12345678");
    AP_OR_STA_MODE = AP_MODE;
    MODE = DEFUALT_AP_MODE;
    save_credentials();
  }
}

# Arduino Code

## Add the `secrets` library

To add the secrets library, copy the `libraries/secrets/` directory to your local Arduino libraries directory using the command below. Replace `/path/to/sketchbook` with your Arduino sketchbook location (found in Arduino preferences)
```
cp -r arduino/libraries/secrets/ /path/to/sketchbook/libraries/secrets
```

Then open `/path/to/sketchbook/libraries/secrets/secrets.h` and fill in your secret info.
 - `SECRET_SSID` and `SECRET_PASS`: your router's ssid and password. These were used in the `client/` and `server/` projects, which are no longer used
 - `broadcastAddress1` and `broadcastAddress2`: the MAC addresses of your ESP8266 boards, used in `sender/` and `receiver/`. [How to get ESP8266 Mac Address](https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/)

# makerlabs-acm
MakerLabs Access Control Management (ACM) System

## How-to obtain this repo:
# Clone this repo recursively:
```
git clone --recursive https://github.com/MakerLabsVan/makerlabs-acm
cd makerlabs-acm
```

# Pre-requisites (macOS):

- Electrical:
  Install KiCad 5.0+:
  http://downloads.kicad-pcb.org/osx/nightly/

- Firmware/Software:
  Install homebrew macOS package manager:
  ```
  /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
  ```

  Install `yarn`:
  ```
  brew install yarn
  ```

  Install dependencies:
  ```
  yarn deps
  yarn install
  ```

# Electrical

- Open the KiCad project:
  ```
  open -a kicad electrical/makerlabs-acm-reader-lock_v1.0.0.pro
  ```

# Firmware

- Configure firmware settings (Wifi SSID/password, Google Spreadsheet ID, etc):
  ```
  yarn firmware menuconfig
  ```

- Build a firmware image, flash it to the connected board:
  ```
  yarn firmware flash
  ```

- Same as above, but also monitor the serial data and reboot after flashing:
  ```
  yarn firmware flash monitor
  ```

# WebComponents

- Add WebComponent dependencies in webcomponents, via `bower`:
  ```
  yarn webcomponents-add <component names>
  ```

- Fetch/update WebComponent versions from latest packages:
  ```
  yarn webcomponents
  ```

Commit and push the updated files in `webcomponents/` and `webcomponents/bower_components/`

# Google Apps Script Webapp / Frontend

- Login to the Google Apps Script / online UI:
  ```
  yarn login
  ```

- Pull changes from Google Apps Script / online UI:
  ```
  yarn pull
  ```

- (Make desired changes to files in `webapp/`)

- Push changes to Google Apps Script
  ```
  yarn push
  ```

- Deploy the latest changes as a webapp:
  ```
  yarn deploy
  ```

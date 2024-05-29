- Version: v0.5.24
  - Fix about typo. See: https://github.com/KangLin/RabbitCommon/tree/25e00a2d98488fa2d63f54b34ac9628603cab448
  - Modify save windows state
  - Modify find RabbitCommon

- Version: v0.5.23
  - Added left bar dockwidget
  - Add save settings:
    - Data bit
    - Parity
    - Stop bit
    - Flow control

- Version: v0.5.22
  - Use VCPKG_MANIFEST_DIR overlay openssl 1.1
  - Use [RabbitCommon v2.2.1](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.1)

- Version: v0.5.21
  - Fix deploy.sh bug
  - CI: copy openssl dll in windows. See: https://github.com/KangLin/RabbitCommon/commit/673aa9198ec1f88eb1f9f265d0b85f613ee86418
  - Fix android resource
  - MacOS can now be successfully compiled in CI.
    but the loading dynamic library path is not set correctly.
    Please help you set it.
  - Use RabbitCommon v2.1.0
  - Add version output log

- Version: v0.5.20
  - Modify deploy.sh

- Version: v0.5.19
  - Modify ci to windows xp

- Version: v0.5.18
  - Modify UI

- Version: v0.5.17
  - Modify MimeType in org.Rabbit.Calendar.desktop
  - Modify debian package
  - Remove pro
  - Use the update of RabbitCommon v2.0.0

- Version: v0.5.16
  - FIX: DockDebugLog core dump. See: https://github.com/KangLin/RabbitCommon/commit/af2e434e39af35876113436ead2a946efcf74199

- Version: v0.5.15
  - Use RabbitCommon v2
  - Add CMakeLists.txt

- Version: v0.5.14
  - Fix: log output file

- Version: v0.5.13
  - Fix: Fixed a bug where the correct dependency library could not be loaded
     when there were different versions of the dependency library under Linux.
  - Add save and restore main window state

- Version: v0.5.12
  - Fix typos
  - github actions:
    - msvc.yml works
  - About window: add revisioin, and be opened from main repository

- Version: v0.5.11
  - Update the code of RabbitCommon
  - Support Qt6
  - Modify github actions
  - Rename tag.sh to deploy.sh

- Version: v0.5.10
  - FIX: divide by 0 crash. Affects all previous releases.
    See https://github.com/KangLin/RabbitCommon/commit/7ed300a206d8a36c30003b6c19ad181682d0ff5f
  
- Version: v0.5.9
  - Add mirror repository
  - Modify ci
    - Add github actions
  
- Version: v0.5.8
  - Use update RabbitCommon::CStyle
  
- Version: v0.5.7
  - Modify style use RabbitCommon::CStyle
  - Add QUIWidget
  
- Version: v0.5.6
  - Modify UI: Add push button
  - Add outpin status
  - CI: Add github actions
  
- Version: v0.5.5
  - Modify same things
  
- Version: v0.5.4
  - Modify same bugs
  - Add open save file and open send file
  
- Version: v0.5.3
  - Modify UI
    - Add scrollarea
    - Modify ui for encode
    - Modify SendHexChar
    - Modify send file

- Version: v0.5.2
  - Modify install
  
- Version: v0.5.1
  - Modify ci
  - Update RabbitCommon interface
  - Modify updater
  - Add android sign

- Version: v0.5.0
  - Modify donation

- Version: v0.4.9
  - Modify documents
  
- Version: v0.4.8
  - Modify build
  
- Version: v0.4.7
  - Modify pro file

- Version: v0.4.6
  - Modify ci appimage

- Version: v0.4.5
  - Modify language BUG
    
- Version: v0.4.4
  - Modify ci BUG
  - Modify style
    
- Version: v0.4.3
  - Modify the project file, RabbitCommon is added to the main project file in library project mode.
  - Increase the refresh serial port tool. See: Issues #3

- Version: v0.4.2
  - Modify linux CI dependency library error
  - Modify the install file

- Version: v0.4.1 
  - Using RabbitCommon v1.0.0
  - Use RabbitCommon to translate

- Version: v0.4.0
  - Modify Install.sh

- Version: v0.3.9
  - Only do the update test under linux

- Version: v0.3.8
  - Modify tar package content

- Version: v0.3.7
  - Increase update_linux_appimage.xml

- Version: v0.3.6
  - Test new version number update

- Version: v0.3.5
  - Reuse version number vX.X.X

- Version: 0.3.7
  - Modify donation

- Version: 0.3.6
  - Modify download updates

- Version: 0.3.5
  - Increase LINUX distribution

- Version: v0.3.4
  - Use GlobalDir in the RabbitCommon library

- Version: v0.3.3
  - Modify the send code BUG
  - Add online updates
  - Add DEB package

- Version: v0.3.2
  - Increase load file
  - Increase log file
  - Increase receive save to file
  - Increase the number of loops sent
  - Modify installation
  
- Version: v0.3.1
  - Modify tag.sh

- Version: v0.3.0
  - Modify automated compilation

- Version: v0.2.0
  - Automatic ci upload changed to qt5.7, because windowsxp is no longer supported after qt5.8

- Version: v0.1.0
  - Modifying the open serial port (not starting with COM) failed

- Version: v0.0.9
  - Modify the version number generation
  - Modify the sending statistics
  - Limit the length of QTextEdit
  - Modify automatic compilation

- Version: v0.0.8
  - Increase style

- Version: v0.0.7
  - Modify the installation file
  - Modify automatic compilation

- Version: v0.0.6
  - Modify about dialog
  - Modify the code display
  - Modify language menu
  - Edit copyright information

- Version: v0.0.5
  - Modify app recently sent
  - Send failure LOG
  - Modify display time format

- Version: v0.0.4
  - Modify the installation package garbled error (Install.nsi must be UTF8-[utf8 BOM] encoding)

- Version: v0.0.3
  - Save Settings
  - Add English README
  - Modify some bugs
  - Add tag script, modify version number, Install.nsi

- Version: v0.0.2
  - Perfect user interface

- Version:0.0.1
  - Complete basic serial port function


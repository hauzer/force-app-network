## How To Install

You can download the latest [release](https://github.com/hauzer/force-app-network/releases) or build from source.

## How To Use

* Copy the files into the app's directory.
* Configure `force-app-network.json`:
  * Set `app` to the path of the app you want to run.
  * Set `network` to the host name of the network you want to force.
* Start your app by executing `force-network-app.exe` from that directory.
* Mind the bitness of your app; use the appropriate `.exe`.

NOTE: Currently probably works with only a very limited number of apps, since only one specific WinAPI function is targeted. Built for and tested with Age of Mythology: Gold Edition (*not* Enhanced Edition) using Hamachi.

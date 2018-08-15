# Problem Solving

## How To Bring KREMEBOt Back To Life



1. Remove old dfu-util: sudo apt-get remove dfu-util

2. Download newest dfu-util from https://sourceforge.net/projects/dfu-util/

3. Install dfu-util from source (see INSTALL section)

4. Connect with usb to pc (robot power should be down) put krembot in DFU mode (reset+setup -> release reset -> wait for yellow and release setup)

5. Save KREMEBOt ID from console

6. In terminal:

```
    $ particle keys server
```

```
    $ particle keys doctor <photon id>
```

If needed setup wifi again.

If photon stuck on breathing green:
Enter photon into safe mode ()
Flash blink led example

To update photon firmware:
Download cli api to your pc
Download firmware bin files (usually comes in 2 files)
Enter photon to dfu mode ()
In command line write:
particle flash --usb [path to bin file part 2]

particle flash --usb [path to bin file part 1]

It is important to flash part 2 before part 1 (otherwise flash may fail)

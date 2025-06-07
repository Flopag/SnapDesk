# SnapDesk

SnapDesk is a Debian application that makes fingerprints of access point using frames following these steps:

1. Read the last frame from beacon-sniffer, a character device inserted in a device driver that makes frames available using a managed mode Wi-Fi interface.
2. Decode the frame.
3. Construct the fingerprint using a custom language.
4. Update the database in /database folder.
5. Notify if this is a new entry.

## SnapDesk installation instructions

This can be used following these steps:
1. Load the beacon-sniffer character device corresponding to the targeted interface
2. `make run`

To enable SnapDesk to run at boot time, run the scrypt `run_at_boot.sh`

The custom code must be written in code.txt ([custom language syntax](#custom-language-syntax))

## beacon-sniffer installation instructions

see [Rtl8188eu instructions](/beacon-sniffer/Rtl8188/Readme.md) and [Ath9k instructions](/beacon-sniffer/Ath9k/Readme.md)

## Current Support

Currently, beacon-sniffer only capture beacon frames and SnapDesk can only analyse beacon frames. This can be easily change by adding a new decoder in SnapDesk and change the filter in beacon-sniffer.

## Custom language syntax

- A comment begins with # <comment> and is ignored by the compiler.
- The script ends at the first blank line or the end of the file. Any content after a blank line is ignored.
- The beginning of a function is: <function name> {
  - The function Sha256: Concatenates all arguments and returns their SHA-256 hash.
  - The function Cut_bit: Extracts a bit slice from the first argument, starting at the bit given by the second argument and get the number of bits specified in the third.
  - The function cut_Byte: Similar to Cut_bit but operates on bytes instead of bits.
- The end of a function is: }
- Each function argument must appear on its own line, between the opening and closing braces.
- A getter is written as ><field>, where <field> can be a named field or an IE element id.
- Any line not matching the syntax for functions or getters is treated as a static string.
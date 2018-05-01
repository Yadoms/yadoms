# **SomfyIO Situo Protocol**

The protocol used for the SomfyIO Situo remote control adapter is based on RS232 protocol (9600 bauds, 8 data bits, 1 stop bit, no parity, no flow control).
Each message contains `<STX>` and `<ETX>` characters.
Each response contains at least an ACK.

### Commands:

 - `S`: Status. Return the name and version of the firmware
Example: 
--> `<STX>S<ETX>` 
<-- `<STX>sSituo5-v01.01<ETX>`

 - `C` : Configuration. Allow to change some parameters of the adapter.
	 - `D`: Debug mode. Give some log in the return flow.
		 - 0 : OFF
		 - 1 : ON
Example:
--> `<STX>CD1<ETX>` 
<-- `<STX>c-DebugON-<ETX>`

 - `L`: Get current channel / Change the active channel. Return a number between 0 to 5. 0 means that no LED is light. Otherwise it returns the value of the channel. After the first call, it changes the channel to the next one. If the current channel is the fifth, the next change will put the active channel to 1.
Example: if the current channel is the third
--> `<STX>L<ETX>` 
<-- `<STX>l3<ETX>`
--> `<STX>L<ETX>` 
<-- `<STX>l4<ETX>`
// The new active channel is the forth.

 - `U`: Up. Simulate pressing Up command of the remote control. It will use the current channel.
Example:
--> `<STX>U<ETX>` 
// The shutters of the current channel is opening
<-- `<STX>U<ETX>`

 - `ux`: Quick Up. Simulate pressing Up command of the remote control. It will change the channel to `x`beforehand.
Example:
--> `<STX>u3<ETX>` 
// Change to channel 3, then open the shutters
<-- `<STX>U<ETX>`

 - `D`: Down. Simulate pressing Down command of the remote control. It will use the current channel.
Example:
--> `<STX>D<ETX>` 
// The shutters of the current channel is closing
<-- `<STX>D<ETX>`

 - `dx`: Quick Down. Simulate pressing Down command of the remote control. It will change the channel to `x`beforehand.
Example:
--> `<STX>d5<ETX>` 
// Change to channel 5, then close the shutters
<-- `<STX>d<ETX>`

 - `M`: My/Stop. Simulate pressing My/Stop command of the remote control. It will use the current channel.
Example:
--> `<STX>M<ETX>` 
// The shutters of the current channel move to the prerecorded position or stop if they were moving.
<-- `<STX>M<ETX>`

 - `mx`: Quick My/Stop. Simulate pressing My/Stop command of the remote control. It will change the channel to `x`beforehand.
Example:
--> `<STX>m2<ETX>` 
// Change to channel 2, then move the shutters to there prerecorded position or stop them.
<-- `<STX>m<ETX>`

 - `P`: Add/Remove an application. Simulate pressing Prog command of the remote control. It will use the current channel.
Example:
--> `<STX>P<ETX>` 
// If a shutter is in programming mode, it is add to the current channel. If the shutter was already in the current channel, it's removed.
<-- `<STX>P<ETX>`

 - `px`: Add/Remove an application. Simulate pressing My/Stop command of the remote control. It will change the channel to `x`beforehand.
Example:
--> `<STX>p1<ETX>` 
// Change to channel 1, then add or remove the shutter which is in programming mode.
<-- `<STX>p<ETX>`

### Error management:
If something went wrong, an error `e` is sending back instead of the ACK. It's followed by a number corresponding of the error.

 1. Unknown command
 2. frame too small
 3. frame too long
 4. character `<ETX>` not found
 5. Wrong data received
 6. Wrong data length received

### LED management
![Somfy IO Situo 5](https://i62.servimg.com/u/f62/13/77/38/12/icon10.png)
With a Somfy IO Situo 5, there is one LED for the battery (which normally indicates when the battery is almost empty) and 4 LED to indicates the active channel.
On a Somfy IO Situo 1, there is only one LED for the battery and one LED for the active channel.

#### Battery LED
The use of the battery LED is overload. It's used to show activity as follow :

 - At the start of the adapter, the LED blinks 10 times (quick blink).
 - When an error occurred, the LED blinks 5 times (quick blink).
 - When wakening, the LED blinks 2 times (slow blink).

#### Active Channel
Active channel is shown by the lightening of one of the 4 channel LED (or the one for the Situo 1). Each LED is tied in one channel.
With the Situo 5, if all the LED are lighted, it means that the active channel is the fifth.
The adapter uses these LED to determine the current active channel. 
 

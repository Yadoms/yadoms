EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:cp2102
LIBS:PIC16F688
LIBS:ESD_Protection
LIBS:CP2102N-A01-GQFN20
LIBS:serial
LIBS:SomfySituo5IoAdapter-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PIC16F688 U2
U 1 1 59F85470
P 8600 4550
F 0 "U2" H 8099 5001 50  0000 L BNN
F 1 "PIC16F688" H 8099 4049 50  0000 L BNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 8600 4550 50  0001 L BNN
F 3 "1.62 USD" H 8600 4550 50  0001 L BNN
F 4 "Microchip" H 8600 4550 50  0001 L BNN "MF"
F 5 "DIP-14 Microchip" H 8600 4550 50  0001 L BNN "Package"
F 6 "Good" H 8600 4550 50  0001 L BNN "Availability"
F 7 "PIC16F688-E/P" H 8600 4550 50  0001 L BNN "MP"
	1    8600 4550
	1    0    0    -1  
$EndComp
$Comp
L USB_OTG J1
U 1 1 59F865FD
P 1000 4350
F 0 "J1" H 800 4800 50  0000 L CNN
F 1 "USB_OTG" H 800 4700 50  0000 L CNN
F 2 "Connectors:USB_Micro-B" H 1150 4300 50  0001 C CNN
F 3 "" H 1150 4300 50  0001 C CNN
	1    1000 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 59F86672
P 1000 4850
F 0 "#PWR01" H 1000 4900 30  0001 C CNN
F 1 "GND" H 1000 4725 30  0001 C CNN
F 2 "" H 1000 4850 60  0000 C CNN
F 3 "" H 1000 4850 60  0000 C CNN
	1    1000 4850
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 59F88552
P 3100 4700
F 0 "C4" H 3125 4800 50  0000 L CNN
F 1 "47pF" H 3125 4600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3138 4550 50  0001 C CNN
F 3 "" H 3100 4700 50  0001 C CNN
	1    3100 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 59F88558
P 3100 4900
F 0 "#PWR02" H 3100 4950 30  0001 C CNN
F 1 "GND" H 3100 4775 30  0001 C CNN
F 2 "" H 3100 4900 60  0000 C CNN
F 3 "" H 3100 4900 60  0000 C CNN
	1    3100 4900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59F8867D
P 2750 4900
F 0 "#PWR03" H 2750 4950 30  0001 C CNN
F 1 "GND" H 2750 4775 30  0001 C CNN
F 2 "" H 2750 4900 60  0000 C CNN
F 3 "" H 2750 4900 60  0000 C CNN
	1    2750 4900
	1    0    0    -1  
$EndComp
Text GLabel 10250 4350 2    60   Input ~ 0
Somfy_led1
Text GLabel 10250 4450 2    60   Input ~ 0
Somfy_led2
Text GLabel 10250 4550 2    60   Input ~ 0
Somfy_led3
Text GLabel 7250 4450 0    60   Input ~ 0
Somfy_led4
$Comp
L GND #PWR04
U 1 1 59FB3D60
P 4900 5900
F 0 "#PWR04" H 4900 5950 30  0001 C CNN
F 1 "GND" H 4900 5775 30  0001 C CNN
F 2 "" H 4900 5900 60  0000 C CNN
F 3 "" H 4900 5900 60  0000 C CNN
	1    4900 5900
	1    0    0    -1  
$EndComp
Text GLabel 10250 4650 2    60   Output ~ 0
Somfy_sw_top
Text GLabel 10250 4750 2    60   Output ~ 0
Somfy_sw_bottom
Text GLabel 10250 4850 2    60   Output ~ 0
Somfy_sw_my
Text GLabel 7250 4850 0    60   Output ~ 0
Somfy_sw_channel
Text GLabel 7250 4350 0    60   Output ~ 0
Somfy_sw_prog
$Comp
L GND #PWR05
U 1 1 59FB86F0
P 9400 4250
F 0 "#PWR05" H 9400 4300 30  0001 C CNN
F 1 "GND" H 9400 4125 30  0001 C CNN
F 2 "" H 9400 4250 60  0000 C CNN
F 3 "" H 9400 4250 60  0000 C CNN
	1    9400 4250
	0    -1   -1   0   
$EndComp
Text GLabel 10250 4100 2    60   Input ~ 0
ICSP_CLK
Text GLabel 10250 4000 2    60   Input ~ 0
ICSP_DATA
Text GLabel 7250 4550 0    60   Input ~ 0
ICSP_VPP
Text GLabel 5750 1600 0    60   Input ~ 0
ICSP_VPP
$Comp
L C C7
U 1 1 59FB5762
P 8050 3600
F 0 "C7" H 7900 3500 50  0000 L CNN
F 1 "100nF" H 7800 3700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8088 3450 50  0001 C CNN
F 3 "" H 8050 3600 50  0001 C CNN
	1    8050 3600
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 59FB59F0
P 8050 3800
F 0 "#PWR06" H 8050 3850 30  0001 C CNN
F 1 "GND" H 8050 3675 30  0001 C CNN
F 2 "" H 8050 3800 60  0000 C CNN
F 3 "" H 8050 3800 60  0000 C CNN
	1    8050 3800
	1    0    0    -1  
$EndComp
Text GLabel 5750 1900 0    60   Input ~ 0
ICSP_DATA
$Comp
L GND #PWR07
U 1 1 59FB7AF1
P 4900 1800
F 0 "#PWR07" H 4900 1850 30  0001 C CNN
F 1 "GND" H 4900 1675 30  0001 C CNN
F 2 "" H 4900 1800 60  0000 C CNN
F 3 "" H 4900 1800 60  0000 C CNN
	1    4900 1800
	1    0    0    -1  
$EndComp
Text GLabel 5750 2000 0    60   Input ~ 0
ICSP_CLK
NoConn ~ 6000 2100
$Comp
L Conn_01x01 J2
U 1 1 59FD8F28
P 9000 1450
F 0 "J2" H 9100 1450 50  0000 C CNN
F 1 "Conn_01x01" H 9450 1450 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 1450 50  0001 C CNN
F 3 "" H 9000 1450 50  0001 C CNN
	1    9000 1450
	1    0    0    -1  
$EndComp
Text Notes 8900 2750 0    60   ~ 0
Somfy
$Comp
L Conn_01x01 J3
U 1 1 59FD9464
P 9000 1550
F 0 "J3" H 9100 1550 50  0000 C CNN
F 1 "Conn_01x01" H 9450 1550 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 1550 50  0001 C CNN
F 3 "" H 9000 1550 50  0001 C CNN
	1    9000 1550
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J4
U 1 1 59FD94F5
P 9000 1650
F 0 "J4" H 9100 1650 50  0000 C CNN
F 1 "Conn_01x01" H 9450 1650 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 1650 50  0001 C CNN
F 3 "" H 9000 1650 50  0001 C CNN
	1    9000 1650
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J5
U 1 1 59FD9589
P 9000 1750
F 0 "J5" H 9100 1750 50  0000 C CNN
F 1 "Conn_01x01" H 9450 1750 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 1750 50  0001 C CNN
F 3 "" H 9000 1750 50  0001 C CNN
	1    9000 1750
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J6
U 1 1 59FD9620
P 9000 1850
F 0 "J6" H 9100 1850 50  0000 C CNN
F 1 "Conn_01x01" H 9450 1850 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 1850 50  0001 C CNN
F 3 "" H 9000 1850 50  0001 C CNN
	1    9000 1850
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J7
U 1 1 59FD96E8
P 9000 1950
F 0 "J7" H 9100 1950 50  0000 C CNN
F 1 "Conn_01x01" H 9450 1950 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 1950 50  0001 C CNN
F 3 "" H 9000 1950 50  0001 C CNN
	1    9000 1950
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J8
U 1 1 59FD9785
P 9000 2050
F 0 "J8" H 9100 2050 50  0000 C CNN
F 1 "Conn_01x01" H 9450 2050 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 2050 50  0001 C CNN
F 3 "" H 9000 2050 50  0001 C CNN
	1    9000 2050
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J9
U 1 1 59FD9825
P 9000 2150
F 0 "J9" H 9100 2150 50  0000 C CNN
F 1 "Conn_01x01" H 9450 2150 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 2150 50  0001 C CNN
F 3 "" H 9000 2150 50  0001 C CNN
	1    9000 2150
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J10
U 1 1 59FD98C8
P 9000 2250
F 0 "J10" H 9100 2250 50  0000 C CNN
F 1 "Conn_01x01" H 9450 2250 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 2250 50  0001 C CNN
F 3 "" H 9000 2250 50  0001 C CNN
	1    9000 2250
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J11
U 1 1 59FD996E
P 9000 2350
F 0 "J11" H 9100 2350 50  0000 C CNN
F 1 "Conn_01x01" H 9450 2350 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 2350 50  0001 C CNN
F 3 "" H 9000 2350 50  0001 C CNN
	1    9000 2350
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01 J12
U 1 1 59FD9A19
P 9000 2450
F 0 "J12" H 9100 2450 50  0000 C CNN
F 1 "Conn_01x01" H 9450 2450 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 2450 50  0001 C CNN
F 3 "" H 9000 2450 50  0001 C CNN
	1    9000 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 59FD9DE1
P 8700 2600
F 0 "#PWR08" H 8700 2650 30  0001 C CNN
F 1 "GND" H 8700 2475 30  0001 C CNN
F 2 "" H 8700 2600 60  0000 C CNN
F 3 "" H 8700 2600 60  0000 C CNN
	1    8700 2600
	1    0    0    -1  
$EndComp
Text GLabel 8500 1650 0    60   Output ~ 0
Somfy_led1
Text GLabel 8500 1750 0    60   Output ~ 0
Somfy_led2
Text GLabel 8500 1850 0    60   Output ~ 0
Somfy_led3
Text GLabel 8500 1950 0    60   Output ~ 0
Somfy_led4
Text GLabel 8500 2050 0    60   Input ~ 0
Somfy_sw_top
Text GLabel 8500 2150 0    60   Input ~ 0
Somfy_sw_my
Text GLabel 8500 2250 0    60   Input ~ 0
Somfy_sw_bottom
Text GLabel 8500 2350 0    60   Input ~ 0
Somfy_sw_channel
Text GLabel 8500 2450 0    60   Input ~ 0
Somfy_sw_prog
$Comp
L PWR_FLAG #FLG1
U 1 1 59FE1117
P 1800 4150
F 0 "#FLG1" H 1800 4450 60  0001 C CNN
F 1 "PWR_FLAG" H 1800 4100 40  0001 C CNN
F 2 "" H 1700 4350 60  0001 C CNN
F 3 "" H 1800 4450 60  0001 C CNN
	1    1800 4150
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5A00EFA7
P 2750 4700
F 0 "C3" H 2775 4800 50  0000 L CNN
F 1 "47pF" H 2775 4600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2788 4550 50  0001 C CNN
F 3 "" H 2750 4700 50  0001 C CNN
	1    2750 4700
	1    0    0    -1  
$EndComp
$Comp
L FT230X U1
U 1 1 5A031EF0
P 4500 4550
F 0 "U1" H 4550 4600 60  0000 C CNN
F 1 "FT230X" H 4650 3550 60  0000 C CNN
F 2 "Housings_SSOP:SSOP-16_3.9x4.9mm_Pitch0.635mm" H 5250 4600 60  0001 C CNN
F 3 "" H 4500 4550 60  0000 C CNN
	1    4500 4550
	1    0    0    -1  
$EndComp
NoConn ~ 5700 5150
NoConn ~ 5700 5250
NoConn ~ 5700 5350
NoConn ~ 5700 5450
$Comp
L R R1
U 1 1 5A0366E7
P 3450 4350
F 0 "R1" V 3400 4550 50  0000 C CNN
F 1 "27" V 3450 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3380 4350 50  0001 C CNN
F 3 "" H 3450 4350 50  0001 C CNN
	1    3450 4350
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5A036A65
P 3450 4450
F 0 "R2" V 3400 4650 50  0000 C CNN
F 1 "27" V 3450 4450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3380 4450 50  0001 C CNN
F 3 "" H 3450 4450 50  0001 C CNN
	1    3450 4450
	0    1    1    0   
$EndComp
$Comp
L Ferrite_Bead L1
U 1 1 5A0374A6
P 3000 4150
F 0 "L1" V 2750 4200 50  0000 C CNN
F 1 "Ferrite_Bead" V 2850 4250 50  0000 C CNN
F 2 "Inductors_SMD:L_0805_HandSoldering" V 2930 4150 50  0001 C CNN
F 3 "" H 3000 4150 50  0001 C CNN
	1    3000 4150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 5A038931
P 2200 4900
F 0 "#PWR09" H 2200 4950 30  0001 C CNN
F 1 "GND" H 2200 4775 30  0001 C CNN
F 2 "" H 2200 4900 60  0000 C CNN
F 3 "" H 2200 4900 60  0000 C CNN
	1    2200 4900
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5A038938
P 2200 4700
F 0 "C2" H 2225 4800 50  0000 L CNN
F 1 "10nF" H 2225 4600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2238 4550 50  0001 C CNN
F 3 "" H 2200 4700 50  0001 C CNN
	1    2200 4700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR010
U 1 1 5A038EBE
P 4900 1700
F 0 "#PWR010" H 4900 1925 30  0001 C CNN
F 1 "VCC" V 4850 1700 30  0001 C CNN
F 2 "" H 4800 1450 60  0001 C CNN
F 3 "" H 4900 1550 60  0001 C CNN
	1    4900 1700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 5A038F6C
P 4900 3400
F 0 "#PWR011" H 4900 3625 30  0001 C CNN
F 1 "VCC" V 4850 3400 30  0001 C CNN
F 2 "" H 4800 3150 60  0001 C CNN
F 3 "" H 4900 3250 60  0001 C CNN
	1    4900 3400
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5A0398D1
P 3800 5600
F 0 "C5" H 3825 5700 50  0000 L CNN
F 1 "100nF" H 3825 5500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3838 5450 50  0001 C CNN
F 3 "" H 3800 5600 50  0001 C CNN
	1    3800 5600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5A03A5FD
P 5200 3800
F 0 "#PWR012" H 5200 3850 30  0001 C CNN
F 1 "GND" H 5200 3675 30  0001 C CNN
F 2 "" H 5200 3800 60  0000 C CNN
F 3 "" H 5200 3800 60  0000 C CNN
	1    5200 3800
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5A03A607
P 5200 3600
F 0 "C6" H 5225 3700 50  0000 L CNN
F 1 "100nF" H 5225 3500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5238 3450 50  0001 C CNN
F 3 "" H 5200 3600 50  0001 C CNN
	1    5200 3600
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR013
U 1 1 5A03A93A
P 4300 1500
F 0 "#PWR013" H 4300 1725 30  0001 C CNN
F 1 "VCC" V 4250 1500 30  0001 C CNN
F 2 "" H 4200 1250 60  0001 C CNN
F 3 "" H 4300 1350 60  0001 C CNN
	1    4300 1500
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5A03AA8A
P 4300 1850
F 0 "C1" H 4325 1950 50  0000 L CNN
F 1 "4.7uF" H 4325 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4338 1700 50  0001 C CNN
F 3 "" H 4300 1850 50  0001 C CNN
	1    4300 1850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5A03ABBC
P 4300 2150
F 0 "#PWR014" H 4300 2200 30  0001 C CNN
F 1 "GND" H 4300 2025 30  0001 C CNN
F 2 "" H 4300 2150 60  0000 C CNN
F 3 "" H 4300 2150 60  0000 C CNN
	1    4300 2150
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR015
U 1 1 5A03B4F9
P 3800 5050
F 0 "#PWR015" H 3800 5275 30  0001 C CNN
F 1 "+3V3" V 3725 5050 30  0001 C CNN
F 2 "" H 3800 5050 60  0000 C CNN
F 3 "" H 3800 5050 60  0000 C CNN
	1    3800 5050
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5A03D8BD
P 6000 5000
F 0 "R3" V 5950 5200 50  0000 C CNN
F 1 "10k" V 6000 5000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5930 5000 50  0001 C CNN
F 3 "" H 6000 5000 50  0001 C CNN
	1    6000 5000
	-1   0    0    1   
$EndComp
$Comp
L R R4
U 1 1 5A03DEB1
P 6150 5000
F 0 "R4" V 6100 5200 50  0000 C CNN
F 1 "10k" V 6150 5000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6080 5000 50  0001 C CNN
F 3 "" H 6150 5000 50  0001 C CNN
	1    6150 5000
	-1   0    0    1   
$EndComp
$Comp
L +3V3 #PWR016
U 1 1 5A03E2E8
P 6400 5300
F 0 "#PWR016" H 6400 5525 30  0001 C CNN
F 1 "+3V3" V 6325 5300 30  0001 C CNN
F 2 "" H 6400 5300 60  0000 C CNN
F 3 "" H 6400 5300 60  0000 C CNN
	1    6400 5300
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR017
U 1 1 5A8EBAE1
P 8700 1450
F 0 "#PWR017" H 8700 1675 30  0001 C CNN
F 1 "+3V3" V 8625 1450 30  0001 C CNN
F 2 "" H 8700 1450 60  0000 C CNN
F 3 "" H 8700 1450 60  0000 C CNN
	1    8700 1450
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 59FB766C
P 9950 4450
F 0 "R6" V 10000 4650 50  0000 C CNN
F 1 "100" V 9950 4450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 9880 4450 50  0001 C CNN
F 3 "" H 9950 4450 50  0001 C CNN
	1    9950 4450
	0    1    -1   0   
$EndComp
$Comp
L R R5
U 1 1 59FB4FD2
P 9950 4350
F 0 "R5" V 10000 4550 50  0000 C CNN
F 1 "100" V 9950 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 9880 4350 50  0001 C CNN
F 3 "" H 9950 4350 50  0001 C CNN
	1    9950 4350
	0    1    -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG3
U 1 1 5A8F0A07
P 4300 2100
F 0 "#FLG3" H 4300 2400 60  0001 C CNN
F 1 "PWR_FLAG" H 4300 2050 40  0001 C CNN
F 2 "" H 4200 2300 60  0001 C CNN
F 3 "" H 4300 2400 60  0001 C CNN
	1    4300 2100
	0    -1   -1   0   
$EndComp
Text GLabel 8500 1550 0    60   Input ~ 0
ICSP_VPP
$Comp
L Conn_01x01 J13
U 1 1 5AA95685
P 9000 2550
F 0 "J13" H 9100 2550 50  0000 C CNN
F 1 "Conn_01x01" H 9450 2550 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 9000 2550 50  0001 C CNN
F 3 "" H 9000 2550 50  0001 C CNN
	1    9000 2550
	1    0    0    -1  
$EndComp
Text Notes 7200 1550 0    60   ~ 0
Somfy_led_battery
Text Label 1350 4150 0    60   ~ 0
VCC_IN
Wire Wire Line
	1000 4750 1000 4850
Wire Wire Line
	900  4750 900  4800
Wire Wire Line
	900  4800 1300 4800
Connection ~ 1000 4800
Wire Wire Line
	1300 4800 1300 4550
Wire Wire Line
	3100 4850 3100 4900
Wire Wire Line
	3100 4550 3100 4450
Connection ~ 3100 4450
Wire Wire Line
	2750 4900 2750 4850
Wire Wire Line
	5700 4650 7900 4650
Wire Wire Line
	5700 4750 7900 4750
Wire Wire Line
	9300 4650 10250 4650
Wire Wire Line
	9300 4750 10250 4750
Wire Wire Line
	9300 4850 10250 4850
Wire Wire Line
	7250 4850 7900 4850
Wire Wire Line
	2750 4350 2750 4550
Wire Wire Line
	4900 5900 4900 5800
Wire Wire Line
	10100 4350 10250 4350
Wire Wire Line
	10100 4450 10250 4450
Wire Wire Line
	9300 4550 10250 4550
Wire Wire Line
	7250 4450 7900 4450
Wire Wire Line
	7250 4350 7900 4350
Wire Wire Line
	9300 4250 9400 4250
Wire Wire Line
	7750 4250 7900 4250
Wire Wire Line
	9300 4450 9800 4450
Wire Wire Line
	9800 4350 9300 4350
Wire Wire Line
	9750 4100 10250 4100
Wire Wire Line
	9750 4100 9750 4450
Connection ~ 9750 4450
Wire Wire Line
	9650 4000 10250 4000
Wire Wire Line
	9650 4000 9650 4350
Connection ~ 9650 4350
Wire Wire Line
	7250 4550 7900 4550
Wire Wire Line
	8050 3800 8050 3750
Wire Wire Line
	8050 3400 8050 3450
Wire Wire Line
	6000 1600 5750 1600
Wire Wire Line
	6000 1900 5750 1900
Wire Wire Line
	5750 2000 6000 2000
Connection ~ 1800 4150
Wire Wire Line
	8700 2600 8700 2550
Wire Wire Line
	8700 2550 8800 2550
Wire Wire Line
	8500 2450 8800 2450
Wire Wire Line
	8800 2350 8500 2350
Wire Wire Line
	8500 2250 8800 2250
Wire Wire Line
	8800 2150 8500 2150
Wire Wire Line
	8500 2050 8800 2050
Wire Wire Line
	8800 1950 8500 1950
Wire Wire Line
	8500 1850 8800 1850
Wire Wire Line
	8800 1750 8500 1750
Wire Wire Line
	8500 1650 8800 1650
Wire Wire Line
	5700 4850 5800 4850
Wire Wire Line
	5800 4850 5800 4950
Wire Wire Line
	5800 4950 5700 4950
Wire Wire Line
	3800 5850 5000 5850
Wire Wire Line
	5000 5850 5000 5800
Connection ~ 4900 5850
Wire Wire Line
	4150 4450 4150 4800
Wire Wire Line
	4150 4800 4200 4800
Wire Wire Line
	4200 4900 4050 4900
Wire Wire Line
	4050 4900 4050 4350
Wire Wire Line
	7750 4250 7750 3400
Wire Wire Line
	7750 3400 8050 3400
Connection ~ 7750 4150
Wire Wire Line
	4900 3400 4900 4250
Connection ~ 4900 4150
Wire Wire Line
	4150 4450 3600 4450
Wire Wire Line
	4050 4350 3600 4350
Wire Wire Line
	3300 4350 1300 4350
Wire Wire Line
	1300 4450 3300 4450
Connection ~ 2750 4350
Wire Wire Line
	1300 4150 2850 4150
Wire Wire Line
	3150 4150 7750 4150
Wire Wire Line
	2200 4900 2200 4850
Wire Wire Line
	2200 4550 2200 4150
Connection ~ 2200 4150
Wire Wire Line
	3800 5050 3800 5450
Wire Wire Line
	3800 5100 4200 5100
Wire Wire Line
	3800 5200 4200 5200
Connection ~ 3800 5200
Wire Wire Line
	3800 5300 4200 5300
Connection ~ 3800 5300
Wire Wire Line
	3800 5750 3800 5850
Wire Wire Line
	4300 1500 4300 1700
Wire Wire Line
	4300 2000 4300 2150
Connection ~ 3800 5100
Wire Wire Line
	6000 4850 6000 4750
Connection ~ 6000 4750
Wire Wire Line
	6150 4850 6150 4650
Connection ~ 6150 4650
Wire Wire Line
	6400 5300 6000 5300
Wire Wire Line
	6000 5300 6000 5150
Wire Wire Line
	6150 5150 6150 5300
Connection ~ 6150 5300
Wire Wire Line
	8700 1450 8800 1450
Wire Wire Line
	4900 1700 6000 1700
Wire Wire Line
	6000 1800 4900 1800
Wire Wire Line
	5200 3800 5200 3750
Wire Wire Line
	4900 3400 5200 3400
Wire Wire Line
	5200 3400 5200 3450
Connection ~ 4900 3400
Connection ~ 4300 2100
Wire Wire Line
	8500 1550 8800 1550
$Comp
L Conn_01x06 J15
U 1 1 5AAA98B4
P 6200 1800
F 0 "J15" H 6200 2100 50  0000 C CNN
F 1 "ICSP" H 6200 1400 50  0000 C CNN
F 2 "AVX 00-9188 006 receiver:AVX_00-9188_006_receiver" H 6200 1800 50  0001 C CNN
F 3 "" H 6200 1800 50  0001 C CNN
	1    6200 1800
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG2
U 1 1 5AAA9CFB
P 4300 1600
F 0 "#FLG2" H 4300 1900 60  0001 C CNN
F 1 "PWR_FLAG" H 4300 1550 40  0001 C CNN
F 2 "" H 4200 1800 60  0001 C CNN
F 3 "" H 4300 1900 60  0001 C CNN
	1    4300 1600
	0    -1   -1   0   
$EndComp
Connection ~ 4300 1600
$EndSCHEMATC

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
LIBS:ICSP_PicKit2
LIBS:PicKit2Adapter-cache
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
L Conn_01x06 J1
U 1 1 5ABB7664
P 4350 1400
F 0 "J1" H 4350 1700 50  0000 C CNN
F 1 "AVX 00-9188 006" V 4500 1350 50  0000 C CNN
F 2 "AVX 00-9188 006:AVX_00-9188_006" H 4350 1400 50  0001 C CNN
F 3 "" H 4350 1400 50  0001 C CNN
	1    4350 1400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3350 1800 5000 1800
Wire Wire Line
	3350 1900 5000 1900
Wire Wire Line
	3350 2000 5000 2000
Wire Wire Line
	3350 2100 5000 2100
Wire Wire Line
	3350 2200 5000 2200
Wire Wire Line
	3350 2300 5000 2300
$Comp
L GND #PWR1
U 1 1 5ABB76A8
P 3650 2650
F 0 "#PWR1" H 3650 2400 50  0001 C CNN
F 1 "GND" H 3650 2500 50  0000 C CNN
F 2 "" H 3650 2650 50  0001 C CNN
F 3 "" H 3650 2650 50  0001 C CNN
	1    3650 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2000 3650 2650
Connection ~ 3650 2000
$Comp
L Conn_01x08 J3
U 1 1 5AEC404D
P 5200 2100
F 0 "J3" H 5200 2500 50  0000 C CNN
F 1 "AVX 00-9188 008" H 5200 1550 50  0000 C CNN
F 2 "AVX 00-9188 006:AVX_00-9188_008" H 5200 2100 50  0001 C CNN
F 3 "" H 5200 2100 50  0001 C CNN
	1    5200 2100
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x04 J2
U 1 1 5AEC4080
P 4250 2850
F 0 "J2" H 4250 3050 50  0000 C CNN
F 1 "AVX 00-9188 004" V 4400 2800 50  0000 C CNN
F 2 "AVX 00-9188 006:AVX_00-9188_004" H 4250 2850 50  0001 C CNN
F 3 "" H 4250 2850 50  0001 C CNN
	1    4250 2850
	0    -1   1    0   
$EndComp
$Comp
L Conn_01x08 P1
U 1 1 5AEC40EF
P 3150 2100
F 0 "P1" H 3150 2500 50  0000 C CNN
F 1 "Conn_01x08" H 3150 1600 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x08_Pitch2.54mm" H 3150 2100 50  0001 C CNN
F 3 "" H 3150 2100 50  0001 C CNN
	1    3150 2100
	-1   0    0    -1  
$EndComp
Text Notes 2900 1700 1    60   ~ 0
ICSP_PicKit2 mapping
Text Notes 2800 1850 0    60   ~ 0
VPP
Text Notes 2800 2050 0    60   ~ 0
GND
Text Notes 2650 2150 0    60   ~ 0
ICSPDAT
Text Notes 2650 2250 0    60   ~ 0
ICSPCLK
Text Notes 2600 2350 0    60   ~ 0
AUXILIARY
Text Notes 2800 1950 0    60   ~ 0
VDD
Wire Wire Line
	4150 2650 4150 1600
Wire Wire Line
	4250 1600 4250 2650
Wire Wire Line
	4350 2650 4350 1600
Wire Wire Line
	4450 2650 4450 1600
Wire Wire Line
	4550 2200 4550 1600
Wire Wire Line
	4650 2300 4650 1600
Connection ~ 4450 2100
Connection ~ 4550 2200
Connection ~ 4650 2300
Wire Wire Line
	3350 2400 5000 2400
Wire Wire Line
	5000 2500 3350 2500
Connection ~ 4350 2000
Connection ~ 4250 1900
Connection ~ 4150 1800
$EndSCHEMATC

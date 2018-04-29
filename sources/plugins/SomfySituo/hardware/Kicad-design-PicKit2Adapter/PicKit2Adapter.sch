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
L ICSP_PicKit2 P1
U 1 1 5ABB75B3
P 3000 2050
F 0 "P1" H 2850 2400 60  0000 C CNN
F 1 "ICSP_PicKit2" H 2850 1700 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x06_Pitch2.54mm" H 3000 2050 60  0001 C CNN
F 3 "" H 3000 2050 60  0001 C CNN
	1    3000 2050
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x06 J1
U 1 1 5ABB7664
P 4250 2000
F 0 "J1" H 4250 2300 50  0000 C CNN
F 1 "AVX 00-9188 006" H 4250 1600 50  0000 C CNN
F 2 "AVX 00-9188 006:AVX_00-9188_006" H 4250 2000 50  0001 C CNN
F 3 "" H 4250 2000 50  0001 C CNN
	1    4250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1800 4050 1800
Wire Wire Line
	3350 1900 4050 1900
Wire Wire Line
	3350 2000 4050 2000
Wire Wire Line
	4050 2100 3350 2100
Wire Wire Line
	3350 2200 4050 2200
Wire Wire Line
	4050 2300 3350 2300
$Comp
L GND #PWR01
U 1 1 5ABB76A8
P 3650 2450
F 0 "#PWR01" H 3650 2200 50  0001 C CNN
F 1 "GND" H 3650 2300 50  0000 C CNN
F 2 "" H 3650 2450 50  0001 C CNN
F 3 "" H 3650 2450 50  0001 C CNN
	1    3650 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2450 3650 2000
Connection ~ 3650 2000
$EndSCHEMATC

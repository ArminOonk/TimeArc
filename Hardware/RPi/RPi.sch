EESchema Schematic File Version 2
LIBS:power
LIBS:device
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
LIBS:dual_buffer_nc7wz16
LIBS:rpi_interface
LIBS:RPi-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "TimeArc RPi interface"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R R2
U 1 1 56BB967C
P 1250 1950
F 0 "R2" V 1330 1950 50  0000 C CNN
F 1 "100" V 1250 1950 50  0000 C CNN
F 2 "" V 1180 1950 50  0000 C CNN
F 3 "" H 1250 1950 50  0000 C CNN
	1    1250 1950
	0    -1   -1   0   
$EndComp
$Comp
L C C3
U 1 1 56BB9B0E
P 3850 2100
F 0 "C3" H 3875 2200 50  0000 L CNN
F 1 "100n" H 3875 2000 50  0000 L CNN
F 2 "" H 3888 1950 50  0000 C CNN
F 3 "" H 3850 2100 50  0000 C CNN
	1    3850 2100
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 56BB9BC3
P 1550 2200
F 0 "R4" V 1630 2200 50  0000 C CNN
F 1 "470" V 1550 2200 50  0000 C CNN
F 2 "" V 1480 2200 50  0000 C CNN
F 3 "" H 1550 2200 50  0000 C CNN
	1    1550 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR3
U 1 1 56BB9D2C
P 1550 2450
F 0 "#PWR3" H 1550 2200 50  0001 C CNN
F 1 "GND" H 1550 2300 50  0000 C CNN
F 2 "" H 1550 2450 50  0000 C CNN
F 3 "" H 1550 2450 50  0000 C CNN
	1    1550 2450
	1    0    0    -1  
$EndComp
$Comp
L Dual_buffer_NC7WZ16 U1
U 1 1 56BBA89C
P 2050 1750
F 0 "U1" H 2100 2100 60  0000 C CNN
F 1 "Dual_buffer_NC7WZ16" H 2200 1400 60  0000 C CNN
F 2 "" H 1850 1750 60  0000 C CNN
F 3 "" H 1850 1750 60  0000 C CNN
	1    2050 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 1950 1550 1950
Wire Wire Line
	1550 1800 1550 2050
Wire Wire Line
	1550 2500 1550 2350
$Comp
L GND #PWR1
U 1 1 56BBAB3E
P 1350 1700
F 0 "#PWR1" H 1350 1450 50  0001 C CNN
F 1 "GND" H 1350 1550 50  0000 C CNN
F 2 "" H 1350 1700 50  0000 C CNN
F 3 "" H 1350 1700 50  0000 C CNN
	1    1350 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 1700 1550 1700
$Comp
L +2V5 #PWR4
U 1 1 56BBAB98
P 2750 1700
F 0 "#PWR4" H 2750 1550 50  0001 C CNN
F 1 "+2V5" H 2750 1840 50  0000 C CNN
F 2 "" H 2750 1700 50  0000 C CNN
F 3 "" H 2750 1700 50  0000 C CNN
	1    2750 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1700 2850 1700
$Comp
L C C1
U 1 1 56BBABDE
P 3000 1700
F 0 "C1" H 3025 1800 50  0000 L CNN
F 1 "C" H 3025 1600 50  0000 L CNN
F 2 "" H 3038 1550 50  0000 C CNN
F 3 "" H 3000 1700 50  0000 C CNN
	1    3000 1700
	0    1    1    0   
$EndComp
$Comp
L GND #PWR5
U 1 1 56BBAC60
P 3250 1700
F 0 "#PWR5" H 3250 1450 50  0001 C CNN
F 1 "GND" H 3250 1550 50  0000 C CNN
F 2 "" H 3250 1700 50  0000 C CNN
F 3 "" H 3250 1700 50  0000 C CNN
	1    3250 1700
	1    0    0    -1  
$EndComp
Connection ~ 2750 1700
Wire Wire Line
	3250 1700 3150 1700
$Comp
L R R1
U 1 1 56BBAECF
P 1250 1450
F 0 "R1" V 1330 1450 50  0000 C CNN
F 1 "100" V 1250 1450 50  0000 C CNN
F 2 "" V 1180 1450 50  0000 C CNN
F 3 "" H 1250 1450 50  0000 C CNN
	1    1250 1450
	0    -1   1    0   
$EndComp
$Comp
L R R3
U 1 1 56BBAED5
P 1550 1200
F 0 "R3" V 1630 1200 50  0000 C CNN
F 1 "470" V 1550 1200 50  0000 C CNN
F 2 "" V 1480 1200 50  0000 C CNN
F 3 "" H 1550 1200 50  0000 C CNN
	1    1550 1200
	1    0    0    1   
$EndComp
$Comp
L GND #PWR2
U 1 1 56BBAEDB
P 1550 950
F 0 "#PWR2" H 1550 700 50  0001 C CNN
F 1 "GND" H 1550 800 50  0000 C CNN
F 2 "" H 1550 950 50  0000 C CNN
F 3 "" H 1550 950 50  0000 C CNN
	1    1550 950 
	1    0    0    1   
$EndComp
Wire Wire Line
	1400 1450 1550 1450
Wire Wire Line
	1550 1350 1550 1600
Wire Wire Line
	1550 900  1550 1050
Connection ~ 1550 1450
Connection ~ 1550 1950
$Comp
L R R6
U 1 1 56BBB4FF
P 3600 1900
F 0 "R6" V 3680 1900 50  0000 C CNN
F 1 "100" V 3600 1900 50  0000 C CNN
F 2 "" V 3530 1900 50  0000 C CNN
F 3 "" H 3600 1900 50  0000 C CNN
	1    3600 1900
	0    -1   1    0   
$EndComp
$Comp
L R R8
U 1 1 56BBB8DE
P 4100 2100
F 0 "R8" V 4180 2100 50  0000 C CNN
F 1 "100" V 4100 2100 50  0000 C CNN
F 2 "" V 4030 2100 50  0000 C CNN
F 3 "" H 4100 2100 50  0000 C CNN
	1    4100 2100
	-1   0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 56BBB934
P 4350 1900
F 0 "C5" H 4375 2000 50  0000 L CNN
F 1 "47u" H 4375 1800 50  0000 L CNN
F 2 "" H 4388 1750 50  0000 C CNN
F 3 "" H 4350 1900 50  0000 C CNN
	1    4350 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 1900 4200 1900
Wire Wire Line
	3850 1900 3850 1950
Wire Wire Line
	4100 1900 4100 1950
Connection ~ 3850 1900
Connection ~ 4100 1900
$Comp
L GND #PWR7
U 1 1 56BBB9D2
P 3850 2300
F 0 "#PWR7" H 3850 2050 50  0001 C CNN
F 1 "GND" H 3850 2150 50  0000 C CNN
F 2 "" H 3850 2300 50  0000 C CNN
F 3 "" H 3850 2300 50  0000 C CNN
	1    3850 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 2300 3850 2250
Wire Wire Line
	3850 2250 4100 2250
Connection ~ 3850 2250
Text GLabel 1100 1450 0    60   Input ~ 0
PWM0
Text GLabel 1100 1950 0    60   Input ~ 0
PWM1
$Comp
L C C2
U 1 1 56BBC624
P 3850 1300
F 0 "C2" H 3875 1400 50  0000 L CNN
F 1 "100n" H 3875 1200 50  0000 L CNN
F 2 "" H 3888 1150 50  0000 C CNN
F 3 "" H 3850 1300 50  0000 C CNN
	1    3850 1300
	1    0    0    1   
$EndComp
$Comp
L R R5
U 1 1 56BBC62A
P 3600 1500
F 0 "R5" V 3680 1500 50  0000 C CNN
F 1 "100" V 3600 1500 50  0000 C CNN
F 2 "" V 3530 1500 50  0000 C CNN
F 3 "" H 3600 1500 50  0000 C CNN
	1    3600 1500
	0    -1   -1   0   
$EndComp
$Comp
L R R7
U 1 1 56BBC630
P 4100 1300
F 0 "R7" V 4180 1300 50  0000 C CNN
F 1 "100" V 4100 1300 50  0000 C CNN
F 2 "" V 4030 1300 50  0000 C CNN
F 3 "" H 4100 1300 50  0000 C CNN
	1    4100 1300
	-1   0    0    1   
$EndComp
$Comp
L C C4
U 1 1 56BBC636
P 4350 1500
F 0 "C4" H 4375 1600 50  0000 L CNN
F 1 "47u" H 4375 1400 50  0000 L CNN
F 2 "" H 4388 1350 50  0000 C CNN
F 3 "" H 4350 1500 50  0000 C CNN
	1    4350 1500
	0    1    -1   0   
$EndComp
Wire Wire Line
	3750 1500 4200 1500
Wire Wire Line
	3850 1500 3850 1450
Wire Wire Line
	4100 1500 4100 1450
Connection ~ 3850 1500
Connection ~ 4100 1500
$Comp
L GND #PWR6
U 1 1 56BBC641
P 3850 1100
F 0 "#PWR6" H 3850 850 50  0001 C CNN
F 1 "GND" H 3850 950 50  0000 C CNN
F 2 "" H 3850 1100 50  0000 C CNN
F 3 "" H 3850 1100 50  0000 C CNN
	1    3850 1100
	1    0    0    1   
$EndComp
Wire Wire Line
	3850 1100 3850 1150
Wire Wire Line
	3850 1150 4100 1150
Connection ~ 3850 1150
Connection ~ 3250 1700
Wire Wire Line
	2600 1500 2600 1600
Wire Wire Line
	2600 1500 3450 1500
Wire Wire Line
	3450 1900 2600 1900
Wire Wire Line
	2600 1900 2600 1800
$Comp
L RPi_Interface U?
U 1 1 56C2444B
P 6600 1650
F 0 "U?" H 6600 2650 60  0000 C CNN
F 1 "RPi_Interface" H 6600 500 60  0000 C CNN
F 2 "" H 6550 1250 60  0000 C CNN
F 3 "" H 6550 1250 60  0000 C CNN
	1    6600 1650
	1    0    0    -1  
$EndComp
Text GLabel 5800 2150 0    60   Input ~ 0
PWM0
Text GLabel 7400 1050 2    60   Input ~ 0
PWM1
$Comp
L GND #PWR?
U 1 1 56C24D5C
P 5350 1250
F 0 "#PWR?" H 5350 1000 50  0001 C CNN
F 1 "GND" H 5350 1100 50  0000 C CNN
F 2 "" H 5350 1250 50  0000 C CNN
F 3 "" H 5350 1250 50  0000 C CNN
	1    5350 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 1050 5350 1050
Wire Wire Line
	5350 1050 5350 1250
Wire Wire Line
	5350 1250 5800 1250
Connection ~ 5350 1250
$Comp
L GND #PWR?
U 1 1 56C24DFC
P 5350 1750
F 0 "#PWR?" H 5350 1500 50  0001 C CNN
F 1 "GND" H 5350 1600 50  0000 C CNN
F 2 "" H 5350 1750 50  0000 C CNN
F 3 "" H 5350 1750 50  0000 C CNN
	1    5350 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 1750 5350 1750
$Comp
L GND #PWR?
U 1 1 56C24E6A
P 5350 2050
F 0 "#PWR?" H 5350 1800 50  0001 C CNN
F 1 "GND" H 5350 1900 50  0000 C CNN
F 2 "" H 5350 2050 50  0000 C CNN
F 3 "" H 5350 2050 50  0000 C CNN
	1    5350 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2050 5350 2050
$Comp
L GND #PWR?
U 1 1 56C24EE9
P 5350 2450
F 0 "#PWR?" H 5350 2200 50  0001 C CNN
F 1 "GND" H 5350 2300 50  0000 C CNN
F 2 "" H 5350 2450 50  0000 C CNN
F 3 "" H 5350 2450 50  0000 C CNN
	1    5350 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2450 5350 2450
$Comp
L GND #PWR?
U 1 1 56C24F59
P 7850 750
F 0 "#PWR?" H 7850 500 50  0001 C CNN
F 1 "GND" H 7850 600 50  0000 C CNN
F 2 "" H 7850 750 50  0000 C CNN
F 3 "" H 7850 750 50  0000 C CNN
	1    7850 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 750  7400 750 
$Comp
L GND #PWR?
U 1 1 56C25001
P 7850 1450
F 0 "#PWR?" H 7850 1200 50  0001 C CNN
F 1 "GND" H 7850 1300 50  0000 C CNN
F 2 "" H 7850 1450 50  0000 C CNN
F 3 "" H 7850 1450 50  0000 C CNN
	1    7850 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1450 7400 1450
$Comp
L GND #PWR?
U 1 1 56C25073
P 7850 2250
F 0 "#PWR?" H 7850 2000 50  0001 C CNN
F 1 "GND" H 7850 2100 50  0000 C CNN
F 2 "" H 7850 2250 50  0000 C CNN
F 3 "" H 7850 2250 50  0000 C CNN
	1    7850 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 2250 7850 2250
$Comp
L +5V #PWR?
U 1 1 56C2514E
P 5600 2650
F 0 "#PWR?" H 5600 2500 50  0001 C CNN
F 1 "+5V" H 5600 2790 50  0000 C CNN
F 2 "" H 5600 2650 50  0000 C CNN
F 3 "" H 5600 2650 50  0000 C CNN
	1    5600 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2650 5800 2650
Wire Wire Line
	5800 2650 5800 2550
Connection ~ 5800 2650
$Comp
L +3.3V #PWR?
U 1 1 56C25212
P 7850 2650
F 0 "#PWR?" H 7850 2500 50  0001 C CNN
F 1 "+3.3V" H 7850 2790 50  0000 C CNN
F 2 "" H 7850 2650 50  0000 C CNN
F 3 "" H 7850 2650 50  0000 C CNN
	1    7850 2650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 56C25251
P 7850 1850
F 0 "#PWR?" H 7850 1700 50  0001 C CNN
F 1 "+3.3V" H 7850 1990 50  0000 C CNN
F 2 "" H 7850 1850 50  0000 C CNN
F 3 "" H 7850 1850 50  0000 C CNN
	1    7850 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1850 7400 1850
Wire Wire Line
	7850 2650 7400 2650
$Comp
L AP1117E25 U?
U 1 1 56C25F6C
P 1900 3800
F 0 "U?" H 2000 3550 50  0000 C CNN
F 1 "AP1117E25" H 1900 4050 50  0000 C CNN
F 2 "" H 1900 3800 50  0000 C CNN
F 3 "" H 1900 3800 50  0000 C CNN
	1    1900 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56C2629B
P 1900 4250
F 0 "#PWR?" H 1900 4000 50  0001 C CNN
F 1 "GND" H 1900 4100 50  0000 C CNN
F 2 "" H 1900 4250 50  0000 C CNN
F 3 "" H 1900 4250 50  0000 C CNN
	1    1900 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4250 1900 4100
$Comp
L +5V #PWR?
U 1 1 56C2652F
P 900 3700
F 0 "#PWR?" H 900 3550 50  0001 C CNN
F 1 "+5V" H 900 3840 50  0000 C CNN
F 2 "" H 900 3700 50  0000 C CNN
F 3 "" H 900 3700 50  0000 C CNN
	1    900  3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  3700 900  3800
$Comp
L +2V5 #PWR?
U 1 1 56C265AE
P 2500 3650
F 0 "#PWR?" H 2500 3500 50  0001 C CNN
F 1 "+2V5" H 2500 3790 50  0000 C CNN
F 2 "" H 2500 3650 50  0000 C CNN
F 3 "" H 2500 3650 50  0000 C CNN
	1    2500 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 3800 2500 3800
Wire Wire Line
	2500 3800 2500 3650
$Comp
L C C?
U 1 1 56C2662E
P 2500 3950
F 0 "C?" H 2525 4050 50  0000 L CNN
F 1 "47u" H 2525 3850 50  0000 L CNN
F 2 "" H 2538 3800 50  0000 C CNN
F 3 "" H 2500 3950 50  0000 C CNN
	1    2500 3950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 56C266CF
P 900 3950
F 0 "C?" H 925 4050 50  0000 L CNN
F 1 "47u" H 925 3850 50  0000 L CNN
F 2 "" H 938 3800 50  0000 C CNN
F 3 "" H 900 3950 50  0000 C CNN
	1    900  3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  4100 2500 4100
Connection ~ 1900 4100
Connection ~ 900  3800
Connection ~ 2500 3800
$Comp
L R R?
U 1 1 56C26E4B
P 1300 3800
F 0 "R?" V 1380 3800 50  0000 C CNN
F 1 "0" V 1300 3800 50  0000 C CNN
F 2 "" V 1230 3800 50  0000 C CNN
F 3 "" H 1300 3800 50  0000 C CNN
	1    1300 3800
	0    -1   1    0   
$EndComp
Wire Wire Line
	900  3800 1150 3800
Wire Wire Line
	1450 3800 1600 3800
$EndSCHEMATC

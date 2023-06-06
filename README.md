# Car-Black-Box
The black box is a device that is installed in a vehicle to record data such as speed, distance traveled, acceleration, and braking patterns. It also records video and audio from within the vehicle, which can be used in the event of an accident or other incident.
Here I am designing a prototype of this black box using PIC18F4580.Which has three parts: 


1)Dashboard: where time,gear and speed will be displayed
time is fetched from RTC
MKP1->Collision occurance,MKP2->increment gear,MKP3->decrement gear
stores events to the external EEPROM whenever any of these keys are pressed
MKP11->come to password part

2)Password: 4digit password should be entered so as to go to Menu part
after three wrong attempts block user for 2 minutes and go back to dashboard
MKP11->0 MKP12->1 (default password is 1111)

3)Menu: In menu you will be having 5 options-> a)view log b)download log c)clear log d)set time e)change password
view log: we will be seeing(CLCD) the latest 10 events and for scrolling up & down we will be using MKP11 & MKP12.
download log: Read all the evnts in a 2-D array & display those strings using UART in screen.
Clear log: Here we will be clearing the logged events.
Set time: This option is to set the time manualy
Change password: This option is to change to password which is used to access the menu part.

/*
 *NAME : Boney Kuriakose
 *DATE : 24/04/2023
 * DESCRIPTION : CAR BLACK BOX
 * Black Box implementation in an Automotive System to log critical events. This will help in pro-active vehicle monitoring and maintenance.
 */



#include <xc.h>
#include "main.h"

unsigned char store_data[11],count=0,log_flag;                         //these all are the required variables declare globally.
static unsigned char menu=0;
unsigned char clock_reg[3];
unsigned char time[9];
unsigned char arr[9][3]={"ON","GN","G1","G2","G3","G4","G5","GR","CO"};
unsigned char i=0,rpm,clear=0;
unsigned char choose_flag=0;
unsigned char password[5];
unsigned char password1[5]="1111";
unsigned char menu_1[6][19] = {"View log        ","download log       ","Clear log    ","Set time       ","Change password ","                "};
unsigned char logs[10][17];
unsigned char log;



/*
 * this function will do the change the time 
 * if u press the switch 11 means it will increase the particular values 
 * if u press the switch 12 means it will change the field like hr to min to sec like that
 * if u press the switch 11 in long time it will set the time and that time it will write in rtc 
 * first we store the change time in change time array and that time we converted in to BCD after that we write in rtc and it will go the dash board
 *if u press the switch 12 long press then it will not save the time and go to menu.
 */
void set_time(void)
{
    unsigned char hour = 0,min = 0,sec = 0;
    unsigned static change;
    static long int delay;
    unsigned char *change_time = time;
    unsigned static int val1 = 0,val2 = 0;
    unsigned char set_time_key = read_switches(LEVEL_CHANGE);
    clcd_print(time, LINE2(0));
    if ( set_time_key ==  MK_SW12)
    {
    	val1++;
    }
    if ( set_time_key ==  MK_SW11)
    {
    	val2++;
    }
    if( val1 < 200 && val1 > 0 && set_time_key == ALL_RELEASED)
    {
    	val1 = 0;

    	change++;
    	change= change % 3;
    }
    else if ( val1 > 200 && set_time_key == ALL_RELEASED )
    {
    	val1 = 0;
    	choose_flag = 2;
    }
    if ( val2 < 200 && val2 > 0 && set_time_key== ALL_RELEASED)
    {
    	val2 = 0;
    	if ( change == 0 )
    	{
    	    change_time[1]++;
    	    if ( change_time[1] > '9' )
    	    {
    	    	change_time[1] = '0';
	        	change_time[0]++;
	        }
	        if( change_time[0] == '2' && change_time[1] == '4')
	        {
	        	change_time[0] = '0';
        		change_time[1] = '0';
	        }

    	}
    	else if ( change == 1 )
    	{
    	    change_time[4]++;
    	    if ( change_time[4] > '9' )
    	    {
        		change_time[4] = '0';
	        	change_time[3]++;
	        }
	       if( change_time[3] == '6' )
	       {
	        	change_time[3] = '0';
        		change_time[4] = '0';
	        }

    	}
    	else if ( change == 2 )
    	{
       	    change_time[7]++;
	       if ( change_time[7] > '9' )
	       {
	        	change_time[7] = '0';
	        	change_time[6]++;
	        }
	       if( change_time[6] == '6')
	       {
	        	change_time[6] = '0';
        		change_time[7] = '0';
	        }
    	}
    }
    else if(val2 > 200 && set_time_key == ALL_RELEASED)
    {
        if ( val2 > 200)
    	{
    	    val2 = 0;
    	    clcd_print("SET TIME SUCCESS",LINE1(0));
    	    clcd_print("                  ",LINE2(0));
    	    hour = (change_time[0] - '0') << 4;
    	    hour = (change_time[1] - '0' ) | hour;
    	    write_ds1307(HOUR_ADDR,hour);
    	    min = (change_time[3] - '0') << 4;
    	    min = (change_time[4] - '0' ) | min;
    	    write_ds1307(MIN_ADDR,min);
    	    sec = (change_time[6] - '0') << 4;
    	    sec = (change_time[7] - '0' ) | sec;
    	    write_ds1307(SEC_ADDR,sec);
    	    for(delay=200000;delay--;);
    	    CLEAR_DISP_SCREEN;

    	    choose_flag = 0;
    	}
    }
}



/*
 * this function will do the display the logs by pressing switches
 * first store the events in log array with the help of read_external_eeprom function
 * if u press the switch 11 means it will display the logs in first to last
 * if u press the switch 12 means it will display the logs in last to first
 * if u press the switch 12 in long time  means it will go to menu
 */
void view_log(void)
{
    static unsigned int view_select1,view_select2 = 0,view_key;
    static long int delay;
    view_key = read_switches(LEVEL_CHANGE);
    if ( clear == 0)
    {
    	logs[log][0]=read_ext_eeprom((log*10)+0);
    	logs[log][1]=read_ext_eeprom((log*10)+1);
    	logs[log][2] = ':';

    	logs[log][3]=read_ext_eeprom((log*10)+2);
    	logs[log][4]=read_ext_eeprom((log*10)+3);
    	logs[log][5] = ':';

    	logs[log][6]=read_ext_eeprom((log*10)+4);
    	logs[log][7]=read_ext_eeprom((log*10)+5);

    	logs[log][8] = ' ';
    	logs[log][9] = ' ';
    	logs[log][10]=read_ext_eeprom((log*10)+6);
    	logs[log][11]=read_ext_eeprom((log*10)+7);
    	logs[log][12] = ' ';

    	logs[log][13]=read_ext_eeprom((log*10)+8);
    	logs[log][14]=read_ext_eeprom((log*10)+9);

    	clcd_print("Log           ",LINE1(0));
        clcd_putch(log+48,LINE2(0));
    	clcd_print(logs[log],LINE2(1));

    }
    if(clear == 1)
    {
    	clcd_print("LOGS_ARE_CLEARED",LINE1(0));
    	clcd_print("               ",LINE2(0));
        for(delay=800000;delay--;);
        CLEAR_DISP_SCREEN;
        choose_flag=2;
    }
    if ( view_key ==  MK_SW11)
    {
    	view_select1++;
    }
    if ( view_key ==  MK_SW12)
    {
    	view_select2++;
    	if (view_select2 > 200)
    	{
    	    choose_flag = 2;
    	    view_select2 = 0;
    	}
    }
    if( view_select1 < 200 && view_select1 > 0 && view_key == ALL_RELEASED)
    {

    	view_select1 = 0;
    	if(log<=9)
    	    log++;
    }
    else if ( view_select1 > 200 && view_key == ALL_RELEASED )
    {
    	view_select1 = 0;
    }
    if ( view_select2 < 200 && view_select2 > 0 && view_key == ALL_RELEASED)
    {
    	view_select2 = 0;
    	if(log>=1)
            log--;

    }
    if( view_select2 > 200 && view_key == ALL_RELEASED)
    {
        CLEAR_DISP_SCREEN;
    	choose_flag = 2;
    	view_select2 = 0;

    }
}




/*
 * this function will do the download the logs in screen with the help of uart protocol
 * after completion of download logs it will go the view menu
 */
void download_log(void)
{
    static long int delay;
    unsigned char arr[10][17];
    if ( clear == 0 )
    {
    	for ( int i = 0; i < 10; i++ )
    	{
    	    arr[i][0] = read_ext_eeprom((i*10)+0);
    	    arr[i][1] = read_ext_eeprom((i*10)+1);
    	    arr[i][2] = ':';

    	    arr[i][3] = read_ext_eeprom((i*10)+2);
    	    arr[i][4] = read_ext_eeprom((i*10)+3);
    	    arr[i][5] = ':';
    	    arr[i][6] = read_ext_eeprom((i*10)+4);
    	    arr[i][7] = read_ext_eeprom((i*10)+5);
    	    arr[i][8] = ' ';
    	    arr[i][9] = ' ';

    	    arr[i][10] = read_ext_eeprom((i*10)+6);
    	    arr[i][11] = read_ext_eeprom((i*10)+7);
    	    arr[i][12] = ' ';
    	    arr[i][13] = read_ext_eeprom((i*10)+8);
    	    arr[i][14] = read_ext_eeprom((i*10)+9);
    	    puts("\n\r");
    	    puts(arr[i]);
    	}
         clcd_print("                ",LINE1(1));
         clcd_print("DOWNLOAD COMPLETE",LINE2(0));
         for(delay=800000;delay--;);
         CLEAR_DISP_SCREEN;
         choose_flag = 2;
    }
    else 
    {
        clcd_print("LOGS ARE CLEARED",LINE1(1));
        for(delay=800000;delay--;);
        CLEAR_DISP_SCREEN;
        choose_flag = 2;
    }
}



/*
 This function to change the password
 if password success then go to dashboard
 if password change fails then go to menu 
 */
void change_password(void)
{
    unsigned char new_password[5],key3;
    unsigned char re_enter[5],k;
    static unsigned char index1=0,index2=0;
    static long int delay;
   
    key3=read_switches(STATE_CHANGE);
    if(index1 < 4)
    {
         clcd_print("ENTER PASSWORD",LINE1(0));
        if(key3 == MK_SW11)
        {
            new_password[index1]='1';
            clcd_putch('*',LINE2(index1));
            index1++;
        }
        else if(key3 == MK_SW12)
        {
            new_password[index1]='0';
            clcd_putch('*',LINE2(index1));
            index1++;
        }
         if(index1 == 4)
         {
             for(delay=100000;delay--;);
             CLEAR_DISP_SCREEN;
         }
    }
    else if(index1 == 4)
    {
        new_password[index1]='\0';
        clcd_print("REENTER PASSWORD",LINE1(0));
        if(index2 < 4)
        {
            if(key3 == MK_SW11)
            {
                re_enter[index2]='1';
                clcd_putch('*',LINE2(index2));
                index2++;
            }
           else if(key3 == MK_SW12)
           {
                re_enter[index2]='0';
                clcd_putch('*',LINE2(index2));
                index2++;
            }
            if(index2 == 4)
            {
                for(delay=100000;delay--;);
                CLEAR_DISP_SCREEN;
            }
        }
        else if(index2 == 4)
        {
            re_enter[index2]='\0';
            if(!(strcmp(new_password,re_enter)))
            {
                clcd_print("SET PASSWORD",LINE1(0));
                clcd_print("SUCCESSFUL",LINE2(0));
                for(k=0;k<4;k++)
                {
                    write_ext_eeprom(k+200,new_password[k]);
                }
                for(delay=800000;delay--;);
                CLEAR_DISP_SCREEN;
                choose_flag=0;
            }
            else
            {
                clcd_print("SET PASSWORD",LINE1(0));
                clcd_print("FAILURE",LINE2(0));
                index1=0;
                index2=0;
                for(delay=800000;delay--;);
                CLEAR_DISP_SCREEN;
                choose_flag=2;
            }
            
        }
    }
    
}




/*
 * this function will do the clear the logs
 * after clearing the logs it will go the menu
 */
void clear_log(void)
{
    static long int delay;
    store_data[0];
    clear=1;
    clcd_print("CLEAR LOG",LINE1(0));
    clcd_print("SUCCESSFUL",LINE2(0));
    menu=0;
    for(delay=800000;delay--;);
    CLEAR_DISP_SCREEN;
    choose_flag=2;
}




/*
 * in this function will do the go and set the particular menu
 * its based on control flag
 * if log flag 1 means we go to the view log menu ( view log function calling will happen.
 * if log flag 2 means we go to the clear log menu ( clear log function calling will happen.
 * if log flag 3 means we go to the download log menu ( download log function calling will happen.
 * if log flag 4 means we go to the set time  menu ( set time function calling will happen.
 * if log flag 5 means we go to the change password menu ( change password function calling will happen.
 */
void setting_log(void)
{
    switch(log_flag)
    {
        case 0:
        {
            view_log();
            break;
        }
        case 1:
        {
            download_log();
            break;
        }
        case 2:
        {
            clear_log();
            break;
        }
        case 3:
        {
            set_time();
            break;
        }
        case 4:
        {
            change_password();
            break;
        }
            
    }
    
}




/* this function will do the change the menu 
 * we will read the switches in level change 
 * if u press switch 11 means menu will move up manner 
 * if u press switch 12 means menu will move in  down manner 
 * if u press the switch 11 in long time it will go particular menu
 * if u press the switch 12 in long time it will go dash board
 */
void change_menu(void)
{
   static unsigned static int t_count1 = 0,t_count2 = 0;
    unsigned char key2 = read_switches(LEVEL_CHANGE);
    if ( key2 ==  MK_SW11)
    {
	    t_count1++;
    }
    if (key2 ==  MK_SW12)
    {
	    t_count2++;
    }
    if( t_count1 < 200 && t_count1 > 0 && key2 == ALL_RELEASED)
    {

	    t_count1 = 0;
	    if( menu < 4)
	       menu++;
    }
    else if ( t_count1 > 200 && key2 == ALL_RELEASED )
    {
	    choose_flag = 3;
	    t_count1 = 0;
        CLEAR_DISP_SCREEN;
        log_flag=menu;
    }
    if ( t_count2 < 200 && t_count2 > 0 && key2 == ALL_RELEASED)
    {
    	t_count2 = 0;
    	if  ( menu >= 1)
    	    menu--;
    }
    else if(t_count2 > 200 && key2 == ALL_RELEASED)
    {
    	t_count2 = 0;
    	choose_flag = 0;
    	CLEAR_DISP_SCREEN;
    }
}




/*
 * this function will do the display the menu by pressing the switch 11 and switch 12 in clcd display
 * and change menu function calling
 * if u press switch 11 it will display the menu in up manner 
 * if u press switch 12 it will display the menu in down manner 
 * if u press the switch 11 in long time it will go particular menu
 * if u press the switch 12 in long time it will go to the dash board
 */
void view_menu(void)
{
    clcd_putch('*',LINE1(0));
    clcd_putch(' ',LINE2(0));
    clcd_print(menu_1[menu],LINE1(1));
    clcd_print(menu_1[menu+1],LINE2(1));
    change_menu();
}




/*
 * this function will do the read password 
 * if the password matches go to the view menu
 * if the password is not matches means it will give three chance
 */
void enter_password(void)
{
    unsigned char key1;
    unsigned char entered[5];
    static long int delay; 
    static unsigned char input = 0;
    unsigned char attempts = 3;
    if(attempts > 0)                                                  //check for no of attempts left
    {
        if(input==0)
        clcd_print("_ENTER_PASSWORD ", LINE1(0));
        key1=read_switches(STATE_CHANGE);
        if(input<4)
        {
            if(key1 == MK_SW11)                                      //if MK_SW11 is pressed then 1 is entered
            {
                entered[input]=(1+'0');
                clcd_putch('*',LINE2(input));
                input++;
            }
            else if(key1 == MK_SW12)                                 //if MK_SW12 is pressed then 0 is entered
            {
                entered[input]=(0+'0');
                clcd_putch('*',LINE2(input));
                input++;
            }
        }
        else if(input == 4)
        {
            for(delay=100000;delay--;);
            attempts--;
            entered[input]='\0';
            if(!strcmp(entered,password))                                //compare the entered and password strings if matches then
            {
                CLEAR_DISP_SCREEN;
                clcd_print("ENTERED_PASSWORD",LINE1(0));
                clcd_print("SUCCESSFULL",LINE2(0));                     //success message in clcd
                for(delay=500000;delay--;);
                choose_flag=2;
            }
            else                                                        //if not matches then 
            {
                CLEAR_DISP_SCREEN;
                input=0;
                clcd_print("TRY AGAIN",LINE1(0));                       //print try again and print no of attempts left
                clcd_putch(attempts+'0',LINE2(0));
                clcd_print("time left",LINE2(2));
                for(delay=500000;delay--;);
                CLEAR_DISP_SCREEN;
                clcd_print("_ENTER_PASSWORD",LINE1(0));
                input=0;
            }
        }
    }
    else if(attempts == 0)
    {
        CLEAR_DISP_SCREEN;
        clcd_print("USER BLOCKED",LINE1(0));                              //if more than 3 attempts blocked the user for 2 minutes
        clcd_print("wait for two min",LINE2(0));
        for(delay=8000000;delay--;);
        CLEAR_DISP_SCREEN;
        choose_flag=0;                                                   //again go back to dashboard
    }
}




/*
 * this function will stores event in array
 * when the event occurs that event stores in store_data array
 * only stores in 10 events
 * and we calling the write data into ext eeprom.
 */
void store_event(void)
{
    unsigned char j,k;
    store_data[0]=time[0];
    store_data[1]=time[1];
    store_data[2]=time[3];
    store_data[3]=time[4];
    store_data[4]=time[6];
    store_data[5]=time[7];
    store_data[6]=arr[i][0];
    store_data[7]=arr[i][1];
    store_data[8]=(rpm/10+48);
    store_data[9]=(rpm%10+48);
    store_data[10]='\0';
    for(j=0;j<10;j++)
    {
        write_ext_eeprom((count*10+j),store_data[j]);
    }
    count++;
    if(count==10)
    {
        count=0;
    }
}




/*
 * this function get time in rtc 
 * that we collected in clock_reg array
 * that time is in bcd format 
 * so we converted to character values and assign time array 
 */
int get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40)
   	{
   		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
   		time[1] = '0' + (clock_reg[0] & 0x0F);
   	}
    else
   	{
    	time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
   	}
   	time[2] = ':';
   	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
   	time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
   	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
   	time[7] = '0' + (clock_reg[2] & 0x0F);
   	time[8] = '\0';
}




/*
 In this dashboard function display the time in clcd using RTC and RPM using ADC potentiometer and set the gear values using matrix keypad
 */
int dashboard(void)
{
    clcd_print("TIME     EV  RPM",LINE1(0));
    get_time();                                                  //call the get time function
    clcd_print(time, LINE2(0));                                  //display time in clcd 2nd line
    unsigned char key;
    clcd_print(arr[i],LINE2(9));                                 //display gear state in clcd 2nd line
    key=read_switches(STATE_CHANGE);                             //read the swithes
    {
        if(key == MK_SW2)                                       //for key press we have call the store event function
        {
            if(i<=6)
            {
               i++;
               store_event();
            }
        }
        if(key == MK_SW3)
        {
            if(i>1 && i!=8)
            {
                i--;
                store_event();
            }
        }
        if(key == MK_SW1)
        {
            i=8;
            store_event();
        }
    }
    rpm=((read_adc(CHANNEL4))/10);                          //read the adc values
    clcd_putch(rpm/10+'0',LINE2(14));                       //print the adc values in clcd
    clcd_putch(rpm%10+'0',LINE2(15));
    if(key == MK_SW11)                                      //if MK_SW11 is pressed then 
    {
        CLEAR_DISP_SCREEN;
        choose_flag=1;                                      //set the choose_flag as 1
        enter_password();
    }
    
}



/*
 This function call all the initialization functions
 */
void init_config(void)                                //init_config function definition
{
    init_adc();                                       //call all the peripherals init function
    init_uart();
    init_i2c();
    init_clcd();
    init_ds1307();
    init_matrix_keypad();
}



void main(void)                                               //main function definition 
{
    
    init_config();                                            //call init_config function
    unsigned char k,l;       
    static int i=0;
    if(i == 0)
    {
       for (k=0;k<4;k++)
       {
           write_ext_eeprom(200+k,password1[k]);              //write my initial password (1111) into external eeprom from 200 address             
       }
       i=1;
    }    
    while(1)
    {
        if(choose_flag==0)                                    //if choose_flag == 0 then 
        {
             for(l=0;l<4;l++)
            {
               password[l]=read_ext_eeprom(200+l);            //read the password from Ext eeprom and store it in a password array
            }
            password[l]='\0';
            dashboard();                                      //call the dashboard function
        }
        else if(choose_flag==1)                               //if choose_flag == 1 then 
        {
            enter_password();                                 //call the enter_password function
        }
        else if(choose_flag==2)                               //if choose_flag == 2 then
        {
            view_menu();                                      //call the view_menu function
        }
        else if(choose_flag==3)                               //if choose_flag == 3 then 
        {
            setting_log();                                    //call the setting_log function
        }
    }
}

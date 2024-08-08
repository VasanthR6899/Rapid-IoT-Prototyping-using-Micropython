/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_mp.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_mp.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_MP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_MP_DATA app_mpData;
static char heap[64000];
static char strFileName[64];
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static int APP_MPCMDProcess
(
    SYS_CMD_DEVICE_NODE* pCmdIO, 
    int argc, 
    char** argv
);

/* TODO:  Add any necessary local functions.
*/
static const SYS_CMD_DESCRIPTOR AppMPCmdTbl[] =
{
    {"mp", (SYS_CMD_FNC) APP_MPCMDProcess, ": Socket mode SNTP commands processing"},
    
};


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_MP_Initialize ( void )

  Remarks:
    See prototype in app_mp.h.
 */

void APP_MP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_mpData.state = APP_MP_STATE_INIT;
    int stack_dummy;
    MP_STATE_THREAD(stack_top) = (char *)&stack_dummy;
    gc_init(heap, heap + sizeof(heap));
    micropython_init();
            ///mp_hal_init();
    readline_init0();
    
    SYS_CMD_ADDGRP(AppMPCmdTbl, sizeof (AppMPCmdTbl) / sizeof (*AppMPCmdTbl), "Enter mp help", " to know about the available mp commands"); 


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

static int APP_MPCMDProcess
(
    SYS_CMD_DEVICE_NODE* pCmdIO, 
    int argc, 
    char** argv
) 
{
   if ((argc >= 1) && (!strcmp(argv[1], "help")))
   {
        SYS_CONSOLE_PRINT("List of Supported python commands \r\n1. mp exec <script_name.py> - To execute a python script from the USB Drive\r\n2. mp command - To enter into Micropython command line mode \r\n3. mp reset - To reset the system\r\n");     
   }
   else if ((argc >= 1) && (!strcmp(argv[1], "command")))
   {
       app_mpData.state = APP_MP_STATE_COMMAND_TASKS;
       
   }
   else if ((argc >= 1) && (!strcmp(argv[1], "reset")))
   {
       app_mpData.state = APP_MP_STATE_RESET_TASKS;
       
   }
   else if ((argc >= 2) && (!strcmp(argv[1], "exec")))
   {
       if(strlen(argv[2]) <64 )
       {
           strcpy(strFileName, argv[2]);
           app_mpData.state = APP_MP_STATE_EXEC_TASKS;
       }
       
   }
   return 0;
}


/******************************************************************************
  Function:
    void APP_MP_Tasks ( void )

  Remarks:
    See prototype in app_mp.h.
 */

void APP_MP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_mpData.state )
    {
        /* Application's initial state. */
        case APP_MP_STATE_INIT:
        {
            bool appInitialized = true;

            extern pyexec_mode_kind_t pyexec_mode_kind;    
    
            vTaskDelay(1000 / portTICK_PERIOD_MS);
    
            SYS_CONSOLE_PRINT("Rapid Prototyping with Micropython... \r\n>");
            if (appInitialized)
            {

                app_mpData.state = APP_MP_STATE_SERVICE_TASKS;
            }
            break;
        }
        
         case APP_MP_STATE_SERVICE_TASKS:
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_MP_STATE_COMMAND_TASKS:
        {
            SYS_CONSOLE_PRINT("Entering Command Line mode with Micropython, Press CTRL + C to exit \r\n>");
            app_mpData.state =APP_MP_STATE_SERVICE_TASKS;
            if (pyexec_friendly_repl() != 0) {
                break;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }
        
        case APP_MP_STATE_EXEC_TASKS:
        {
            app_mpData.state =APP_MP_STATE_SERVICE_TASKS;
            if (pyexec_file_if_exists(strFileName) != 0) {
                break;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }
        case APP_MP_STATE_RESET_TASKS:
        {
            SYS_CONSOLE_PRINT("Resetting the System, Please wait !!!\r\n>");
            NVIC_SystemReset();
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */

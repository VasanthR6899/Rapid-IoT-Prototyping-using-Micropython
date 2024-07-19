/*******************************************************************************
  MPLAB Harmony Application Source File
  NVM FAT Single Disk Demo Application
  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.
    NVM FAT Single Disk Demo
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

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "app_fs.h"
#include "app.h"

/* This application showcases the File operations with NVM as the media. To
 * begin with the file system image contains a file called "FILE.TXT" with
 * 4-byte data "Data".
 * The application does the following:
 * 1. Mounts the file system present on the NVM media.
 * 2. Opens a file called "FILE.TXT" in READ PLUS mode.
 * 3. Retrieves the file stat information for the file.
 * 4. Gets the file size and compares it with the size information present in
 *    the file stat structure.
 * 5. Does a file seek to the end of the file.
 * 6. Checks if the EOF has reached.
 * 7. Sets the file pointer to the beginning of the file.
 * 8. Reads the 4 byte data and checks if the expected data is present in the
 *    file.
 * 9. Appends 13 bytes of data to the file. Performs a file seek to the
 *    beginning of the appended data.
 * 10. Reads 13 bytes and checks if the expected data is present in the file.
 * 11. If there is no error in any of the above steps then the application will
 *     go into Idle state.
 * 12. If there is an error then the application will go into Error state.
 * */

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define APP_MOUNT_NAME          "/mnt/E:"
#define APP_DEVICE_NAME         "/dev/mtda1"
#define APP_FS_TYPE             FAT



uint8_t CACHE_ALIGN work[SYS_FS_FAT_MAX_SS];
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_FS_DATA appfsData;


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

/* TODO:  Add any necessary local functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_FS_Initialize ( void )
{
    /* Initialize the app state to wait for
     * media attach. */
 
    appfsData.state = APP_SWITCH_PRESS_WAIT;
 
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_FS_Tasks ( void )
{
SYS_FS_FORMAT_PARAM opt;
    /* Check the application's current state. */
    switch ( appfsData.state )
    {
        case APP_SWITCH_PRESS_WAIT:
        {          
            {
                appfsData.state = APP_MOUNT_DISK;


            }
            break;
        }
        case APP_MOUNT_DISK:
        {
//#define QSPI_ERASE
#ifdef QSPI_ERASE
            {
                DRV_SST26_ChipErase(sysObj.drvSST26);
                SYS_CONSOLE_PRINT("\r\nErase is successful\r\n");
                while(1)
                {
            
                }
            }
#endif
            vTaskDelay(10000); //wait for device mount
           if(0)/*Temporary workaround to mount and format the device as AUTO_MOUNT is Enabled*///if(SYS_FS_Mount(APP_DEVICE_NAME, APP_MOUNT_NAME, APP_FS_TYPE, 0, NULL) != 0)
            {
                /* The disk could not be mounted. Try mounting again until
                 * mount is successful. */
                appfsData.state = APP_MOUNT_DISK;
            }
            else if (SYS_FS_ERROR_NO_FILESYSTEM==SYS_FS_Error())
            {
             opt.fmt = SYS_FS_FORMAT_FAT;
             opt.au_size = 0;
                /* Mount was successful. Try to format the disk */
    
             SYS_CONSOLE_PRINT("\r\nFormatting the Disk\r\n");
            if (SYS_FS_DriveFormat (SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0, &opt, (void *)work, SYS_FS_FAT_MAX_SS) != SYS_FS_RES_SUCCESS)
            {
                  SYS_CONSOLE_PRINT("\r\nFailed to format\r\n");
                /* Format of the disk failed. */
             
            }
            else 
            {
                 appfsData.state = APP_OPEN_FILE;
                 SYS_CONSOLE_PRINT("\r\nFormat Successful\r\n");
            }
            }
            else 
            {
              
                appfsData.state = APP_OPEN_FILE;
            }
            break;
        }
         case APP_OPEN_FILE:
         {
                 // init MicroPython runtime
              
         }

       

        default:
        {
            break;
        }
    }

} //End of APP_Tasks



void gc_collect(void) {
    // TODO possibly need to trace registers
    //gc_dump_info();
    void *dummy;
    SYS_CONSOLE_PRINT("[%s] 1\r\n", __func__);
    gc_collect_start();
    //gc_helper_collect_regs_and_stack();
    gc_collect_root(&dummy, ((mp_uint_t)MP_STATE_THREAD(stack_top) - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    //gc_collect_root(&dummy, ((mp_uint_t)&dummy - (mp_uint_t)MP_STATE_THREAD(stack_top)) / sizeof(mp_uint_t));
    #if MICROPY_PY_THREAD
    mp_thread_gc_others();
    #endif
    gc_collect_end();
#if 0   
    void *dummy;
    SYS-("[%s] In\r\n", __func__);
    gc_collect_start();
    // Node: stack is ascending
    gc_collect_root(&dummy, ((mp_uint_t)&dummy - (mp_uint_t)MP_STATE_THREAD(stack_top)) / sizeof(mp_uint_t));
    gc_collect_end();
#endif
}


typedef struct mp_reader_wfi32_t {
    bool close_fd;
    int fd;
    size_t len;
    size_t pos;
    byte buf[20];
} mp_reader_wfi32_t;


STATIC mp_uint_t mp_reader_wfi32_readbyte(void *data) {
    mp_reader_wfi32_t *reader = (mp_reader_wfi32_t *)data;
    if (reader->pos >= reader->len) {
        if (reader->len == 0) {
            return MP_READER_EOF;
        } else {
            MP_THREAD_GIL_EXIT();
            //int n = read(reader->fd, reader->buf, sizeof(reader->buf));
            int n = SYS_FS_FileRead(reader->fd, reader->buf, sizeof(reader->buf));
            MP_THREAD_GIL_ENTER();
            if (n <= 0) {
                reader->len = 0;
                return MP_READER_EOF;
            }
            reader->len = n;
            reader->pos = 0;
        }
    }
    return reader->buf[reader->pos++];
}

STATIC void mp_reader_wfi32_close(void *data) {
    mp_reader_wfi32_t *reader = (mp_reader_wfi32_t *)data;
    if (reader->close_fd) {
        MP_THREAD_GIL_EXIT();
        SYS_FS_FileClose(reader->fd);
        //close(reader->fd);
        MP_THREAD_GIL_ENTER();
    }
    m_del_obj(mp_reader_posix_t, reader);
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    //mp_raise_OSError(MP_ENOENT);

    mp_reader_t reader;
    SYS_FS_HANDLE fd = (SYS_FS_HANDLE)NULL;
    size_t n;
#ifdef DEV_DEBUG
    SYS_CONSOLE_MESSAGE("mp_lexer_new_from_file\r\n");
#endif
    //mp_reader_new_file(&reader, filename);
    
    MP_THREAD_GIL_EXIT();
    //int fd = open(filename, O_RDONLY, 0644);
    fd = SYS_FS_FileOpen(filename, SYS_FS_FILE_OPEN_READ);
    MP_THREAD_GIL_ENTER();
    if (fd < 0) {
        SYS_CONSOLE_PRINT("No script files 1..  %s",filename);
        mp_raise_OSError(MP_EPERM);
    }
    //mp_reader_new_file_from_fd(reader, fd, true);
    
    mp_reader_wfi32_t *rp = m_new_obj(mp_reader_wfi32_t);
    rp->close_fd = true;
    rp->fd = fd;
    MP_THREAD_GIL_EXIT();
    //int n = read(fd, rp->buf, sizeof(rp->buf));
    n = SYS_FS_FileRead(fd, rp->buf, sizeof(rp->buf));
    if (n == -1) {
        if (rp->close_fd) {
            //close(fd);
            SYS_FS_FileClose(fd);
        }
        MP_THREAD_GIL_ENTER();
        SYS_CONSOLE_PRINT("No script files 2.. ");
        mp_raise_OSError(MP_EPERM);
    }
    MP_THREAD_GIL_ENTER();
    rp->len = n;
    rp->pos = 0;
    reader.data = rp;
    reader.readbyte = mp_reader_wfi32_readbyte;
    reader.close = mp_reader_wfi32_close;
    
    
    
    return mp_lexer_new(qstr_from_str(filename), reader);

}

mp_import_stat_t mp_import_stat(const char *path) {

    //SYS_CONSOLE_PRINT("mp_import_stat, path= %s\r\n", path);
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT stat;
    char longFileName[300];
    char dir_path[30] = {0};
    bool second_lv = false;

    int i = 0;
    //SYS_CONSOLE_PRINT("path len= %d\r\n", strlen(path));
    for (i = 0; i<strlen(path); i++) {
        if (path[i] == '/')
        {
            memcpy(dir_path, path, i);
            second_lv = true;
            path += i + 1;
           
            break;
        }
    }
    
    // If long file name is used, the following elements of the "stat"
    // structure needs to be initialized with address of proper buffer.
    stat.lfname = longFileName;
    stat.lfsize = 300;
    
    dirHandle = SYS_FS_DirOpen("");

    if(dirHandle == SYS_FS_HANDLE_INVALID)
    {
        //SYS_CONSOLE_PRINT("directory open fail\r\n");
        return MP_IMPORT_STAT_NO_EXIST;
    }

    if(SYS_FS_DirSearch(dirHandle, path, SYS_FS_ATTR_DIR, &stat) == SYS_FS_RES_FAILURE)
    {
       SYS_FS_DirClose(dirHandle);
    }
    else
    {
       SYS_FS_DirClose(dirHandle);
       return MP_IMPORT_STAT_DIR;
    }
    
    if (second_lv)
    {
        dirHandle = SYS_FS_DirOpen(dir_path);
        if(dirHandle == SYS_FS_HANDLE_INVALID)
        {
            SYS_CONSOLE_PRINT("directory open fail\r\n");
            return MP_IMPORT_STAT_NO_EXIST;
        }


        if(SYS_FS_DirSearch(dirHandle, path, SYS_FS_ATTR_FILE, &stat) == SYS_FS_RES_FAILURE)
        {
           SYS_FS_DirClose(dirHandle);
           //SYS_CONSOLE_PRINT("directory open log3\r\n");
           return MP_IMPORT_STAT_NO_EXIST;
        }
        else
        {
           SYS_FS_DirClose(dirHandle);
           //SYS_CONSOLE_PRINT("directory open log4\r\n");
           return MP_IMPORT_STAT_FILE;
        }
        SYS_FS_DirClose(dirHandle);
    }
    return MP_IMPORT_STAT_NO_EXIST;

}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val) {
    while (1) {
        ;
    }
}

void NORETURN __fatal_error(const char *msg) {
    while (1) {
        ;
    }
}


/*******************************************************************************
  System Configuration Header

  File Name:
    configuration.h

  Summary:
    Build-time configuration header for the system defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "user.h"
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Configuration
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                            (0)
#define SYS_TIME_MAX_TIMERS                         (5)
#define SYS_TIME_HW_COUNTER_WIDTH                   (32)
#define SYS_TIME_TICK_FREQ_IN_HZ                    (1001.001)

#define SYS_CONSOLE_INDEX_0                       0





#define SYS_CMD_ENABLE
#define SYS_CMD_DEVICE_MAX_INSTANCES       SYS_CONSOLE_DEVICE_MAX_INSTANCES
#define SYS_CMD_PRINT_BUFFER_SIZE          1024U
#define SYS_CMD_BUFFER_DMA_READY

/* Command System Service RTOS Configurations*/
#define SYS_CMD_RTOS_STACK_SIZE                256
#define SYS_CMD_RTOS_TASK_PRIORITY             2


#define SYS_DEBUG_ENABLE
#define SYS_DEBUG_GLOBAL_ERROR_LEVEL       SYS_ERROR_DEBUG
#define SYS_DEBUG_BUFFER_DMA_READY
#define SYS_DEBUG_USE_CONSOLE



/* File System Service Configuration */

#define SYS_FS_MEDIA_NUMBER               (1U)
#define SYS_FS_VOLUME_NUMBER              (1U)

#define SYS_FS_AUTOMOUNT_ENABLE           true
#define SYS_FS_CLIENT_NUMBER              1U
#define SYS_FS_MAX_FILES                  (2U)
#define SYS_FS_MAX_FILE_SYSTEM_TYPE       (1U)
#define SYS_FS_MEDIA_MAX_BLOCK_SIZE       (512U)
#define SYS_FS_MEDIA_MANAGER_BUFFER_SIZE  (2048U)
#define SYS_FS_USE_LFN                    (1)
#define SYS_FS_FILE_NAME_LEN              (255U)
#define SYS_FS_CWD_STRING_LEN             (1024)

/* File System RTOS Configurations*/
#define SYS_FS_STACK_SIZE                 1024
#define SYS_FS_PRIORITY                   2

#define SYS_FS_FAT_VERSION                "v0.15"
#define SYS_FS_FAT_READONLY               false
#define SYS_FS_FAT_CODE_PAGE              437
#define SYS_FS_FAT_MAX_SS                 SYS_FS_MEDIA_MAX_BLOCK_SIZE





#define SYS_FS_MEDIA_TYPE_IDX0 				SYS_FS_MEDIA_TYPE_SPIFLASH
#define SYS_FS_TYPE_IDX0 					FAT
					
#define SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0 			"/mnt/E:"
#define SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0			"/dev/mtda1"
								

#define SYS_CONSOLE_DEVICE_MAX_INSTANCES   			(1U)
#define SYS_CONSOLE_UART_MAX_INSTANCES 	   			(1U)
#define SYS_CONSOLE_USB_CDC_MAX_INSTANCES 	   		(0U)
#define SYS_CONSOLE_PRINT_BUFFER_SIZE        		(8192U)




// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* Memory Driver Global Configuration Options */
#define DRV_MEMORY_INSTANCES_NUMBER          (1U)

/* Memory Driver Instance 0 Configuration */
#define DRV_MEMORY_INDEX_0                   0
#define DRV_MEMORY_CLIENTS_NUMBER_IDX0       2
#define DRV_MEMORY_BUF_Q_SIZE_IDX0    1
/* Memory Driver Instance 0 RTOS Configurations*/
#define DRV_MEMORY_STACK_SIZE_IDX0               1024
#define DRV_MEMORY_PRIORITY_IDX0                 1
#define DRV_MEMORY_RTOS_DELAY_IDX0               10U

/* SST26 Driver Instance Configuration */
#define DRV_SST26_INDEX                 (0U)
#define DRV_SST26_CLIENTS_NUMBER        (1U)
#define DRV_SST26_START_ADDRESS         (0x0U)
#define DRV_SST26_PAGE_SIZE             (256U)
#define DRV_SST26_ERASE_BUFFER_SIZE     (4096U)



// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************
/* Number of Endpoints used */
#define DRV_USBFSV1_ENDPOINTS_NUMBER                        2U

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER                         1U

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE                          64U


/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBFSV1_INSTANCES_NUMBER                        1U


/* Enables Device Support */
#define DRV_USBFSV1_DEVICE_SUPPORT                          true
    
/* Disable Host Support */
#define DRV_USBFSV1_HOST_SUPPORT                            false

/* Enable usage of Dual Bank */
#define DRV_USBFSV1_DUAL_BANK_ENABLE                        false

/* Alignment for buffers that are submitted to USB Driver*/ 
#define USB_ALIGN  __ALIGNED(CACHE_LINE_SIZE)

/* Maximum instances of MSD function driver */
#define USB_DEVICE_MSD_INSTANCES_NUMBER     1 

#define USB_DEVICE_MSD_NUM_SECTOR_BUFFERS 2


/* Number of Logical Units */
#define USB_DEVICE_MSD_LUNS_NUMBER      1





// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/

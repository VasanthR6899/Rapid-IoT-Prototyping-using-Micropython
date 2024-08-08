/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mperrno.h"
#include "py/obj.h"
#include "machine_i2c.h"
#include "modmachine.h"
#include "../port/same54p20a/same54p20a_i2c.h"
#include "definitions.h"



#define I2C_DEFAULT_TIMEOUT_US (10000) // 10ms



//#define MP_MACHINE_I2C_FLAG_READ (0x01) // if not set then it's a write
//#define MP_MACHINE_I2C_FLAG_STOP (0x02)

typedef struct _machine_i2c_obj_t {
    mp_obj_base_t base;
    int8_t port;
    int8_t scl;
    int8_t sda;
} machine_i2c_obj_t;

STATIC machine_i2c_obj_t machine_hw_i2c_obj[I2C_NUM_MAX];

uint8_t transferStatus;
// volatile



STATIC void machine_hw_i2c_init(machine_i2c_obj_t *self, uint32_t freq, uint32_t timeout_us) {
    SERCOM_I2C_TRANSFER_SETUP i2c_setup;
     i2c_setup.clkSpeed = freq;
     
     I2C_TransferSetup(self->port, &i2c_setup, 100000000UL);
       
}

int machine_hw_i2c_transfer(mp_obj_base_t *self_in, uint16_t addr, size_t n, mp_machine_i2c_buf_t *bufs, unsigned int flags) {
    bool res;
    machine_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    int data_len = 0;
    for (; n--; ++bufs) {
        if (flags & MP_MACHINE_I2C_FLAG_READ) {
            
            transferStatus = APP_TRANSFER_STATUS_IN_PROGRESS;
                       
            res = I2C_Read(self->port, addr, bufs->buf, bufs->len );
            
            if (!res)
                SYS_CONSOLE_PRINT("i2c read, fail\r\n");
            
            while (transferStatus == APP_TRANSFER_STATUS_IN_PROGRESS)
            {
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            
            if (transferStatus != APP_TRANSFER_STATUS_SUCCESS)
                mp_raise_ValueError(MP_ERROR_TEXT("fail to read I2C data"));
#ifdef PIC32MZW1_DEBUG           
            SYS_CONSOLE_PRINT("i2c read, data = 0x%x\r\n", bufs->buf[0]);
#endif
        }
        else{
#ifdef PIC32MZW1_DEBUG            
            SYS_CONSOLE_PRINT("i2c write, len = %d data=0x%x\r\n", bufs->len, bufs->buf[0]);
#endif
            transferStatus = APP_TRANSFER_STATUS_IN_PROGRESS;
            
            res = I2C_Write(self->port, addr, bufs->buf, bufs->len );
            
            if (!res)
                SYS_CONSOLE_PRINT("i2c write, fail\r\n");
            
            while (transferStatus == APP_TRANSFER_STATUS_IN_PROGRESS)
            {
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            if (transferStatus != APP_TRANSFER_STATUS_SUCCESS)
                mp_raise_ValueError(MP_ERROR_TEXT("fail to write I2C data"));

        }
                   
        data_len += bufs->len;
    }
    
    if (flags & MP_MACHINE_I2C_FLAG_STOP) {
        //i2c_master_stop(cmd);
    }
    
    return data_len;
}

/******************************************************************************/
// MicroPython bindings for machine API

STATIC void machine_hw_i2c_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    //machine_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);

}

mp_obj_t machine_hw_i2c_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    //MP_MACHINE_I2C_CHECK_FOR_LEGACY_SOFTI2C_CONSTRUCTION(n_args, n_kw, all_args);

    // Parse args
    enum { ARG_id, ARG_scl, ARG_sda, ARG_freq, ARG_timeout };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_id, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_scl, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_sda, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_freq, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 50000} },
        { MP_QSTR_timeout, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = I2C_DEFAULT_TIMEOUT_US} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Get I2C bus
    mp_int_t i2c_id = mp_obj_get_int(args[ARG_id].u_obj);
    if (i2c_id > I2C_NUM_MAX) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("I2C(%d) doesn't exist"), i2c_id);
    }

    // Get static peripheral object
    machine_i2c_obj_t *self = (machine_i2c_obj_t *)&machine_hw_i2c_obj[i2c_id-1];

    if (self->base.type == NULL) {
        // Created for the first time, set default pins
        self->base.type = &machine_i2c_type;
        self->port = i2c_id;
        
        if (self->port == I2C_NUM_1) {
            self->scl = I2C1_SCL_PIN;                              // SERCOM3 Pin Number for PA23_S3P1_I2C_SCL
            self->sda = I2C1_SDA_PIN;                              // SERCOM3 Pin Number for PA22_S3P0_I2C_SDA
        } 
        else if (self->port == I2C_NUM_2) { 
            self->scl = I2C2_SCL_PIN;                              // SERCOM7 Pin Number for PD09_S7P1_I2C_SCL                    
            self->sda = I2C2_SDA_PIN;                              // SERCOM7 Pin Number for PD08_S7P0_I2C_SDA
        }
    }

    
    
    // Initialise the I2C peripheral
    machine_hw_i2c_init(self, args[ARG_freq].u_int, args[ARG_timeout].u_int);
#ifdef PIC32MZW1_DEBUG    
    SYS_CONSOLE_PRINT("Register I2C1_CallbackRegister\r\n");
#endif
    
 
    I2C_CallbackRegister(self->port);
    
    return MP_OBJ_FROM_PTR(self);

}

#if MICROPY_PY_MACHINE_I2C || MICROPY_PY_MACHINE_SOFTI2C

// For use by ports that require a single buffer of data for a read/write transfer
int mp_machine_i2c_transfer_adaptor(mp_obj_base_t *self, uint16_t addr, size_t n, mp_machine_i2c_buf_t *bufs, unsigned int flags) {
    size_t len;
    uint8_t *buf;
    if (n == 1) {
        // Use given single buffer
        len = bufs[0].len;
        buf = bufs[0].buf;
    } else {
        // Combine buffers into a single one
        len = 0;
        for (size_t i = 0; i < n; ++i) {
            len += bufs[i].len;
        }
        buf = m_new(uint8_t, len);
        if (!(flags & MP_MACHINE_I2C_FLAG_READ)) {
            len = 0;
            for (size_t i = 0; i < n; ++i) {
                memcpy(buf + len, bufs[i].buf, bufs[i].len);
                len += bufs[i].len;
            }
        }
    }

    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    int ret = i2c_p->transfer_single(self, addr, len, buf, flags);

    if (n > 1) {
        if (flags & MP_MACHINE_I2C_FLAG_READ) {
            // Copy data from single buffer to individual ones
            len = 0;
            for (size_t i = 0; i < n; ++i) {
                memcpy(bufs[i].buf, buf + len, bufs[i].len);
                len += bufs[i].len;
            }
        }
        m_del(uint8_t, buf, len);
    }

    return ret;
}

STATIC int mp_machine_i2c_readfrom(mp_obj_base_t *self, uint16_t addr, uint8_t *dest, size_t len, bool stop) {
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    mp_machine_i2c_buf_t buf = {.len = len, .buf = dest};
    unsigned int flags = MP_MACHINE_I2C_FLAG_READ | (stop ? MP_MACHINE_I2C_FLAG_STOP : 0);
    return i2c_p->transfer(self, addr, 1, &buf, flags);
}

STATIC int mp_machine_i2c_writeto(mp_obj_base_t *self, uint16_t addr, const uint8_t *src, size_t len, bool stop) {
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    mp_machine_i2c_buf_t buf = {.len = len, .buf = (uint8_t *)src};
    unsigned int flags = stop ? MP_MACHINE_I2C_FLAG_STOP : 0;
    return i2c_p->transfer(self, addr, 1, &buf, flags);
}

/******************************************************************************/
// MicroPython bindings for generic machine.I2C

STATIC mp_obj_t machine_i2c_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(args[0]);
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    if (i2c_p->init == NULL) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("I2C operation not supported"));
    }
    i2c_p->init(self, n_args - 1, args + 1, kw_args);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_i2c_init_obj, 1, machine_i2c_init);

STATIC mp_obj_t machine_i2c_scan(mp_obj_t self_in) {
    mp_obj_base_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t list = mp_obj_new_list(0, NULL);
    // 7-bit addresses 0b0000xxx and 0b1111xxx are reserved
    for (int addr = 0x08; addr < 0x78; ++addr) {
        int ret = mp_machine_i2c_writeto(self, addr, NULL, 0, true);
        if (ret == 0) {
            mp_obj_list_append(list, MP_OBJ_NEW_SMALL_INT(addr));
        }
    }
    return list;
}
MP_DEFINE_CONST_FUN_OBJ_1(machine_i2c_scan_obj, machine_i2c_scan);


STATIC mp_obj_t machine_i2c_readinto(size_t n_args, const mp_obj_t *args) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(args[0]);
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    if (i2c_p->read == NULL) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("I2C operation not supported"));
    }

    // get the buffer to read into
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[1], &bufinfo, MP_BUFFER_WRITE);

    // work out if we want to send a nack at the end
    bool nack = (n_args == 2) ? true : mp_obj_is_true(args[2]);

    // do the read
    int ret = i2c_p->read(self, bufinfo.buf, bufinfo.len, nack);
    if (ret != 0) {
        mp_raise_OSError(-ret);
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_i2c_readinto_obj, 2, 3, machine_i2c_readinto);

STATIC mp_obj_t machine_i2c_write(mp_obj_t self_in, mp_obj_t buf_in) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(self_in);
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    if (i2c_p->write == NULL) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("I2C operation not supported"));
    }

    // get the buffer to write from
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);

    // do the write
    int ret = i2c_p->write(self, bufinfo.buf, bufinfo.len);
    if (ret < 0) {
        mp_raise_OSError(-ret);
    }

    // return number of acks received
    return MP_OBJ_NEW_SMALL_INT(ret);
}
MP_DEFINE_CONST_FUN_OBJ_2(machine_i2c_write_obj, machine_i2c_write);

STATIC mp_obj_t machine_i2c_readfrom(size_t n_args, const mp_obj_t *args) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(args[0]);
    mp_int_t addr = mp_obj_get_int(args[1]);
    vstr_t vstr;
    vstr_init_len(&vstr, mp_obj_get_int(args[2]));
    bool stop = (n_args == 3) ? true : mp_obj_is_true(args[3]);
    int ret = mp_machine_i2c_readfrom(self, addr, (uint8_t *)vstr.buf, vstr.len, stop);
    if (ret < 0) {
        mp_raise_OSError(-ret);
    }
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_i2c_readfrom_obj, 3, 4, machine_i2c_readfrom);

STATIC mp_obj_t machine_i2c_readfrom_into(size_t n_args, const mp_obj_t *args) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(args[0]);
    mp_int_t addr = mp_obj_get_int(args[1]);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[2], &bufinfo, MP_BUFFER_WRITE);
    bool stop = (n_args == 3) ? true : mp_obj_is_true(args[3]);
    int ret = mp_machine_i2c_readfrom(self, addr, bufinfo.buf, bufinfo.len, stop);
    if (ret < 0) {
        mp_raise_OSError(-ret);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_i2c_readfrom_into_obj, 3, 4, machine_i2c_readfrom_into);

STATIC mp_obj_t machine_i2c_writeto(size_t n_args, const mp_obj_t *args) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(args[0]);
    mp_int_t addr = mp_obj_get_int(args[1]);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[2], &bufinfo, MP_BUFFER_READ);
    bool stop = (n_args == 3) ? true : mp_obj_is_true(args[3]);
    int ret = mp_machine_i2c_writeto(self, addr, bufinfo.buf, bufinfo.len, stop);
    if (ret < 0) {
        mp_raise_OSError(-ret);
    }
    // return number of acks received
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_i2c_writeto_obj, 3, 4, machine_i2c_writeto);

STATIC mp_obj_t machine_i2c_writevto(size_t n_args, const mp_obj_t *args) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(args[0]);
    mp_int_t addr = mp_obj_get_int(args[1]);

    // Get the list of data buffer(s) to write
    size_t nitems;
    const mp_obj_t *items;
    mp_obj_get_array(args[2], &nitems, (mp_obj_t **)&items);

    // Get the stop argument
    bool stop = (n_args == 3) ? true : mp_obj_is_true(args[3]);

    // Extract all buffer data, skipping zero-length buffers
    size_t alloc = nitems == 0 ? 1 : nitems;
    size_t nbufs = 0;
    mp_machine_i2c_buf_t *bufs = mp_local_alloc(alloc * sizeof(mp_machine_i2c_buf_t));
    for (; nitems--; ++items) {
        mp_buffer_info_t bufinfo;
        mp_get_buffer_raise(*items, &bufinfo, MP_BUFFER_READ);
        if (bufinfo.len > 0) {
            bufs[nbufs].len = bufinfo.len;
            bufs[nbufs++].buf = bufinfo.buf;
        }
    }

    // Make sure there is at least one buffer, empty if needed
    if (nbufs == 0) {
        bufs[0].len = 0;
        bufs[0].buf = NULL;
        nbufs = 1;
    }

    // Do the I2C transfer
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    int ret = i2c_p->transfer(self, addr, nbufs, bufs, stop ? MP_MACHINE_I2C_FLAG_STOP : 0);
    mp_local_free(bufs);

    if (ret < 0) {
        mp_raise_OSError(-ret);
    }

    // Return number of acks received
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_i2c_writevto_obj, 3, 4, machine_i2c_writevto);

STATIC size_t fill_memaddr_buf(uint8_t *memaddr_buf, uint32_t memaddr, uint8_t addrsize) {
    size_t memaddr_len = 0;
    if ((addrsize & 7) != 0 || addrsize > 32) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid addrsize"));
    }
    for (int16_t i = addrsize - 8; i >= 0; i -= 8) {
        memaddr_buf[memaddr_len++] = memaddr >> i;
    }
    return memaddr_len;
}

STATIC int read_mem(mp_obj_t self_in, uint16_t addr, uint32_t memaddr, uint8_t addrsize, uint8_t *buf, size_t len) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(self_in);

    // Create buffer with memory address
    uint8_t memaddr_buf[4];
    size_t memaddr_len = fill_memaddr_buf(&memaddr_buf[0], memaddr, addrsize);

    #if MICROPY_PY_MACHINE_I2C_TRANSFER_WRITE1

    // Create partial write and read buffers
    mp_machine_i2c_buf_t bufs[2] = {
        {.len = memaddr_len, .buf = memaddr_buf},
        {.len = len, .buf = buf},
    };

    // Do write+read I2C transfer
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    return i2c_p->transfer(self, addr, 2, bufs,
        MP_MACHINE_I2C_FLAG_WRITE1 | MP_MACHINE_I2C_FLAG_READ | MP_MACHINE_I2C_FLAG_STOP);

    #else

    int ret = mp_machine_i2c_writeto(self, addr, memaddr_buf, memaddr_len, false);
    if (ret != memaddr_len) {
        // must generate STOP
        mp_machine_i2c_writeto(self, addr, NULL, 0, true);
        return ret;
    }
    return mp_machine_i2c_readfrom(self, addr, buf, len, true);

    #endif
}

STATIC int write_mem(mp_obj_t self_in, uint16_t addr, uint32_t memaddr, uint8_t addrsize, const uint8_t *buf, size_t len) {
    mp_obj_base_t *self = (mp_obj_base_t *)MP_OBJ_TO_PTR(self_in);

    // Create buffer with memory address
    uint8_t memaddr_buf[4];
    size_t memaddr_len = fill_memaddr_buf(&memaddr_buf[0], memaddr, addrsize);

    // Create partial write buffers
    mp_machine_i2c_buf_t bufs[2] = {
        {.len = memaddr_len, .buf = memaddr_buf},
        {.len = len, .buf = (uint8_t *)buf},
    };

    // Do I2C transfer
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)self->type->protocol;
    return i2c_p->transfer(self, addr, 2, bufs, MP_MACHINE_I2C_FLAG_STOP);
}

STATIC const mp_arg_t machine_i2c_mem_allowed_args[] = {
    { MP_QSTR_addr,    MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_memaddr, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_arg,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_addrsize, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
};

STATIC mp_obj_t machine_i2c_readfrom_mem(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_addr, ARG_memaddr, ARG_n, ARG_addrsize };
    mp_arg_val_t args[MP_ARRAY_SIZE(machine_i2c_mem_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args,
        MP_ARRAY_SIZE(machine_i2c_mem_allowed_args), machine_i2c_mem_allowed_args, args);

    // create the buffer to store data into
    vstr_t vstr;
    vstr_init_len(&vstr, mp_obj_get_int(args[ARG_n].u_obj));

    // do the transfer
    int ret = read_mem(pos_args[0], args[ARG_addr].u_int, args[ARG_memaddr].u_int,
        args[ARG_addrsize].u_int, (uint8_t *)vstr.buf, vstr.len);
    if (ret < 0) {
        mp_raise_OSError(-ret);
    }

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_i2c_readfrom_mem_obj, 1, machine_i2c_readfrom_mem);


STATIC mp_obj_t machine_i2c_readfrom_mem_into(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_addr, ARG_memaddr, ARG_buf, ARG_addrsize };
    mp_arg_val_t args[MP_ARRAY_SIZE(machine_i2c_mem_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args,
        MP_ARRAY_SIZE(machine_i2c_mem_allowed_args), machine_i2c_mem_allowed_args, args);

    // get the buffer to store data into
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_buf].u_obj, &bufinfo, MP_BUFFER_WRITE);

    // do the transfer
    int ret = read_mem(pos_args[0], args[ARG_addr].u_int, args[ARG_memaddr].u_int,
        args[ARG_addrsize].u_int, bufinfo.buf, bufinfo.len);
    if (ret < 0) {
        mp_raise_OSError(-ret);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_i2c_readfrom_mem_into_obj, 1, machine_i2c_readfrom_mem_into);

STATIC mp_obj_t machine_i2c_writeto_mem(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_addr, ARG_memaddr, ARG_buf, ARG_addrsize };
    mp_arg_val_t args[MP_ARRAY_SIZE(machine_i2c_mem_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args,
        MP_ARRAY_SIZE(machine_i2c_mem_allowed_args), machine_i2c_mem_allowed_args, args);

    // get the buffer to write the data from
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_buf].u_obj, &bufinfo, MP_BUFFER_READ);

    // do the transfer
    int ret = write_mem(pos_args[0], args[ARG_addr].u_int, args[ARG_memaddr].u_int,
        args[ARG_addrsize].u_int, bufinfo.buf, bufinfo.len);
    if (ret < 0) {
        mp_raise_OSError(-ret);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_i2c_writeto_mem_obj, 1, machine_i2c_writeto_mem);

STATIC const mp_rom_map_elem_t machine_i2c_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_i2c_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_scan), MP_ROM_PTR(&machine_i2c_scan_obj) },

    // primitive I2C operations
    { MP_ROM_QSTR(MP_QSTR_readinto), MP_ROM_PTR(&machine_i2c_readinto_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&machine_i2c_write_obj) },

    // standard bus operations
    { MP_ROM_QSTR(MP_QSTR_readfrom), MP_ROM_PTR(&machine_i2c_readfrom_obj) },
    { MP_ROM_QSTR(MP_QSTR_readfrom_into), MP_ROM_PTR(&machine_i2c_readfrom_into_obj) },
    { MP_ROM_QSTR(MP_QSTR_writeto), MP_ROM_PTR(&machine_i2c_writeto_obj) },
    { MP_ROM_QSTR(MP_QSTR_writevto), MP_ROM_PTR(&machine_i2c_writevto_obj) },

    // memory operations
    { MP_ROM_QSTR(MP_QSTR_readfrom_mem), MP_ROM_PTR(&machine_i2c_readfrom_mem_obj) },
    { MP_ROM_QSTR(MP_QSTR_readfrom_mem_into), MP_ROM_PTR(&machine_i2c_readfrom_mem_into_obj) },
    { MP_ROM_QSTR(MP_QSTR_writeto_mem), MP_ROM_PTR(&machine_i2c_writeto_mem_obj) },
};
MP_DEFINE_CONST_DICT(mp_machine_i2c_locals_dict, machine_i2c_locals_dict_table);

#endif

STATIC const mp_machine_i2c_p_t machine_hw_i2c_p = {
    .transfer = machine_hw_i2c_transfer,
};

const mp_obj_type_t machine_i2c_type = {
    { &mp_type_type },
    .name = MP_QSTR_I2C,
    .print = machine_hw_i2c_print,
    .make_new = machine_hw_i2c_make_new,
    .protocol = &machine_hw_i2c_p,
    .locals_dict = (mp_obj_dict_t *)&mp_machine_i2c_locals_dict,
};

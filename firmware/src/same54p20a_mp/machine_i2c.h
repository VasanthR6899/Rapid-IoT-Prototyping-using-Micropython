#ifndef MICROPY_INCLUDED_EXTMOD_MACHINE_I2C_H
#define MICROPY_INCLUDED_EXTMOD_MACHINE_I2C_H


#define MP_MACHINE_I2C_FLAG_READ (0x01) // if not set then it's a write
#define MP_MACHINE_I2C_FLAG_STOP (0x02)

#if MICROPY_PY_MACHINE_I2C_TRANSFER_WRITE1
// If set, the first mp_machine_i2c_buf_t in a transfer is a write.
#define MP_MACHINE_I2C_FLAG_WRITE1 (0x04)
#endif

typedef struct _mp_machine_i2c_buf_t {
    size_t len;
    uint8_t *buf;
} mp_machine_i2c_buf_t;

// I2C protocol
// - init must be non-NULL
// - start/stop/read/write can be NULL, meaning operation is not supported
// - transfer must be non-NULL
// - transfer_single only needs to be set if transfer=mp_machine_i2c_transfer_adaptor
typedef struct _mp_machine_i2c_p_t {
    void (*init)(mp_obj_base_t *obj, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
    int (*start)(mp_obj_base_t *obj);
    int (*stop)(mp_obj_base_t *obj);
    int (*read)(mp_obj_base_t *obj, uint8_t *dest, size_t len, bool nack);
    int (*write)(mp_obj_base_t *obj, const uint8_t *src, size_t len);
    int (*transfer)(mp_obj_base_t *obj, uint16_t addr, size_t n, mp_machine_i2c_buf_t *bufs, unsigned int flags);
    int (*transfer_single)(mp_obj_base_t *obj, uint16_t addr, size_t len, uint8_t *buf, unsigned int flags);
} mp_machine_i2c_p_t;


#endif
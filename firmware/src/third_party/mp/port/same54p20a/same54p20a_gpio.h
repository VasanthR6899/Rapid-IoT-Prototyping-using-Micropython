
typedef enum
{
    GPIO_PORT_SAME54P20A_GPIO_IN = 0,
    GPIO_PORT_SAME54P20A_GPIO_OUT = 1,
} GPIO_PORT;

typedef enum
{
    SAME54P20A_GPIO_PORT_A = 0,
    SAME54P20A_GPIO_PORT_B = 1,
    SAME54P20A_GPIO_PORT_C = 2,
    SAME54P20A_GPIO_PORT_D = 3,
} SAME54P20A_GPIO_PORT;

typedef enum
{
    SAME54P20A_GPIO_IN = 0,
    SAME54P20A_GPIO_OUT = 1,
} SAME54P20A_GPIO_DIR;

typedef enum
{
    SAME54P20A_IRQ_FALLING_TRIGGER = 0,
    SAME54P20A_IRQ_RISING_TRIGGER = 1,    
} SAME54P20A_IRQ_TRIGGER;

typedef enum
{
    SAME54P20A_INT_CHANN_0 = 0,
    SAME54P20A_INT_CHANN_1 = 1,
    SAME54P20A_INT_CHANN_2 = 2,
    SAME54P20A_INT_CHANN_3 = 3,
    SAME54P20A_INT_CHANN_4 = 4,
} SAME54P20A_INT_CHAN;

void gpio_set_dir(GPIO_PORT port, int pin, SAME54P20A_GPIO_DIR dir);
unsigned int gpio_get_level(SAME54P20A_GPIO_PORT port, int pin);
void gpio_put(GPIO_PORT port, int pin, int val);
void gpio_irq_disable( SAME54P20A_INT_CHAN irq_chan );
void gpio_irq_enable( SAME54P20A_INT_CHAN irq_chan );
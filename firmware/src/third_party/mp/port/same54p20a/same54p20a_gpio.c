
#include "same54p20a_gpio.h"
#include "definitions.h"
 

void gpio_set_dir(GPIO_PORT port, int pin, SAME54P20A_GPIO_DIR dir)
{
#if 0
	if (dir == SAME54P20A_GPIO_IN)
		GPIO_PortInputEnable(port, 1<<pin);
	else
		GPIO_PortOutputEnable(port, 1<<pin);
#endif
    if (dir == SAME54P20A_GPIO_IN)
	//	GPIO_PortInputEnable(port, 1<<pin);
        PORT_REGS->GROUP[port].PORT_DIRCLR =1<<pin;
	else
	//	GPIO_PortOutputEnable(port, 1<<pin);
        PORT_REGS->GROUP[port].PORT_DIRSET =1<<pin;
}

void gpio_put(GPIO_PORT port, int pin, int val)
{
    if (val == 0)
        (PORT_REGS->GROUP[port].PORT_OUTCLR = 1UL << pin);
     else
        (PORT_REGS->GROUP[port].PORT_OUTSET = 1UL << pin);          
}

unsigned int gpio_get_level(SAME54P20A_GPIO_PORT port, int pin)
{
    if(pin == 18)
        return ((PORT_REGS->GROUP[port].PORT_OUT >> pin) & 0x01);
    else if(pin ==31)
        return ((PORT_REGS->GROUP[port].PORT_IN >> pin) & 0x01);
        
            
}

unsigned int gpio_set_irq(int pin, SAME54P20A_INT_CHAN irq_chan, int irq_id, SAME54P20A_IRQ_TRIGGER trigger, void* handler)
{
#if 0
    //SAME54P20A_INT_CHAN * pirq_chan = NULL;
    //pirq_chan = malloc(sizeof(SAME54P20A_INT_CHAN));
    //*pirq_chan = irq_chan;
        
    /* Unlock system for PPS configuration */
    //SYSKEY = 0x00000000;
    //SYSKEY = 0xAA996655;
    //SYSKEY = 0x556699AA;
    
    CFGCON0bits.IOLOCK = 0;
    //INT2R = 10;
    switch (irq_chan)
    {
        case SAME54P20A_INT_CHANN_0:
            INT0R = irq_id;
            if (trigger == SAME54P20A_IRQ_RISING_TRIGGER)
                INTCONSET |= _INTCON_INT0EP_MASK;
            EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_0, handler, (uintptr_t) NULL); // need correct to EXTERNAL_INT_0
            break;
        
        case SAME54P20A_INT_CHANN_1:
            INT1R = irq_id;
            if (trigger == SAME54P20A_IRQ_RISING_TRIGGER)
                INTCONSET |= _INTCON_INT1EP_MASK;
            EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_1, handler, (uintptr_t) NULL); // need correct to EXTERNAL_INT_1
            break;
        case SAME54P20A_INT_CHANN_2:
            INT2R = irq_id;
            if (trigger == SAME54P20A_IRQ_RISING_TRIGGER)
                INTCONSET |= _INTCON_INT2EP_MASK;
            
            //printf("[%s] log1\r\n", __func__);
            EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_2, handler, (uintptr_t) NULL);
            break;
        case SAME54P20A_INT_CHANN_3:
            INT3R = irq_id;
            if (trigger == SAME54P20A_IRQ_RISING_TRIGGER)
                INTCONSET |= _INTCON_INT3EP_MASK;
            
            EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_3, handler, (uintptr_t) NULL);
            break;
        case SAME54P20A_INT_CHANN_4:
            INT4R = irq_id;
            if (trigger == SAME54P20A_IRQ_RISING_TRIGGER)
                INTCONSET |= _INTCON_INT4EP_MASK;
            
            EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_4, handler, (uintptr_t) NULL);
            break;
        default:
            break;
    }

    /* Lock back the system after PPS configuration */
    //CFGCON0bits.IOLOCK = 1;
    
    //SYSKEY = 0x00000000;
    
    /* Configure External Interrupt Edge Polarity */
    
#endif
    return 0;
            
}


void gpio_irq_enable( SAME54P20A_INT_CHAN irq_chan )
{
#if 0
    EXTERNAL_INT_PIN    irq_num;
    
    switch (irq_chan)
    {
        case SAME54P20A_INT_CHANN_0:
            irq_num = EXTERNAL_INT_0;
            break;
        case SAME54P20A_INT_CHANN_1:
            irq_num = EXTERNAL_INT_1;
            break;
        case SAME54P20A_INT_CHANN_2:
            irq_num = EXTERNAL_INT_2;
            break;
        case SAME54P20A_INT_CHANN_3:
            irq_num = EXTERNAL_INT_3;
            break;
        case SAME54P20A_INT_CHANN_4:
            irq_num = EXTERNAL_INT_4;
            break;
        default:
            irq_num = EXTERNAL_INT_0;
            break;
    }
    
    EVIC_ExternalInterruptEnable(irq_num);
#endif
}

void gpio_irq_disable( SAME54P20A_INT_CHAN irq_chan )
{
#if 0
    EXTERNAL_INT_PIN    irq_num;
    
    switch (irq_chan)
    {
        case SAME54P20A_INT_CHANN_0:
            irq_num = EXTERNAL_INT_0;
            break;
        case SAME54P20A_INT_CHANN_1:
            irq_num = EXTERNAL_INT_1;
            break;
        case SAME54P20A_INT_CHANN_2:
            irq_num = EXTERNAL_INT_2;
            break;
        case SAME54P20A_INT_CHANN_3:
            irq_num = EXTERNAL_INT_3;
            break;
        case SAME54P20A_INT_CHANN_4:
            irq_num = EXTERNAL_INT_4;
            break;
        default:
            irq_num = EXTERNAL_INT_0;
            break;
    }
    
    EVIC_ExternalInterruptDisable(irq_num);
#endif
}

#define MAX_PIN_NUM 128
bool gpio_inuse[MAX_PIN_NUM] = {0};
bool gpio_check_available( int pin )
{
    return gpio_inuse[pin-1] ? false: true;
}
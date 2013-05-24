/*
 * linux/arch/arm/mach-at91/gpio.c
 *
 * Copyright (C) 2005 HP Labs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <at91sam926x.h>
#include "gpio.h"

static unsigned long at91_pios[3] = {
	AT91_PIOA,
	AT91_PIOB,
	AT91_PIOC,
};

static inline void  *pin_to_controller(unsigned pin)
{
    pin -= PIN_BASE;
    pin /= 32;
    if (pin < 3)
        return (void *)(AT91_BASE_SYS + at91_pios[pin]);
    return RT_NULL;
}

static inline unsigned pin_to_mask(unsigned pin)
{
    pin -= PIN_BASE;
    return 1 << (pin % 32);
}

/*
 * mux the pin to the gpio controller (instead of "A" or "B" peripheral), and
 * configure it for an input.
 */
int  at91_set_gpio_input(unsigned pin, int use_pullup)
{
    void    *pio = pin_to_controller(pin);
    unsigned    mask = pin_to_mask(pin);

    if (!pio)
        return -RT_ERROR;

    writel(mask, pio + PIO_IDR);
    writel(mask, pio + (use_pullup ? PIO_PUER : PIO_PUDR));
    writel(mask, pio + PIO_ODR);
    writel(mask, pio + PIO_PER);
    return 0;
}


/*
 * mux the pin to the gpio controller (instead of "A" or "B" peripheral),
 * and configure it for an output.
 */
int  at91_set_gpio_output(unsigned pin, int value)
{
    void    *pio = pin_to_controller(pin);
    unsigned    mask = pin_to_mask(pin);

    if (!pio)
        return -RT_ERROR;

    writel(mask, pio + PIO_IDR);
    writel(mask, pio + PIO_PUDR);
    writel(mask, pio + (value ? PIO_SODR : PIO_CODR));
    writel(mask, pio + PIO_OER);
    writel(mask, pio + PIO_PER);
    return 0;
}


#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <MilkV_GPIO.hpp>

static MilkV_GPIO ledgpio(MilkV_GPIO::PWR_GPIO, 2);

int main()
{
    ledgpio.init(MilkV_GPIO::GPIO_OUTPUT);

    while(1)
    {
        ledgpio.writePin(1);
        printf("MilkV_GPIO state: %lu\n", ledgpio.readPin());
        usleep(50000);

        ledgpio.writePin(0);
        printf("MilkV_GPIO state: %lu\n", ledgpio.readPin());
        usleep(450000);
    }

    return 0;
}

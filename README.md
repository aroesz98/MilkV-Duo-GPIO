# Milk V Duo Bare Metal GPIO Driver

## Implementation Description:
The following implementation contains the `MilkV_GPIO` class, which provides the ability to manage General Purpose Input/Output (GPIO) ports on the Milk-V platform using the /dev/mem address space. The class provides methods for initializing pins, reading and writing pin states, managing interrupts, and enabling/disabling debounce functionality. The easiest way to compile that library is to clone duo-examples repository, follow the instruction to setup compiler and environment than add that project to examples.

## Functionality and Function Descriptions:

1. **Constructor and Destructor:**
   - **Constructor:** Initializes the `MilkV_GPIO` object with the specified GPIO port and pin number.
     - Parameters: 
       - `GPIOPort port`: The GPIO port (GPIOPort0, GPIOPort1, GPIOPort2, GPIOPort3, PowerGPIO).
       - `uint32_t pinNumber`: The pin number.
   - **Destructor:** Clears the GPIO list and releases resources.

2. **Initialization:**
   - **init(GPIODirection pinDirection):** Initializes the GPIO pin as either input or output.
     - Parameters:
       - `GPIODirection pinDirection`: The direction of the pin (GPIOInput, GPIOOutput).

3. **Pin Operations:**
   - **writePin(bool pinState):** Sets the pin state to high or low.
     - Parameters:
       - `bool pinState`: The state of the pin (true - high, false - low).
   - **readPin(void):** Reads the state of the pin (high or low).
     - Returns: The state of the pin (1 - high, 0 - low).

4. **Interrupt Management:**
   - **enableInterrupt(void):** Enables interrupts for the pin.
   - **disableInterrupt(void):** Disables interrupts for the pin.
   - **enableInterruptMask(void):** Enables the interrupt mask for the pin.
   - **disableInterruptMask(void):** Disables the interrupt mask for the pin.
   - **setInterruptLevelType(IntLevelType levelType):** Sets the interrupt level type (level sensitive or edge sensitive).
     - Parameters:
       - `IntLevelType levelType`: The interrupt level type (LevelSensitive, EdgeSensitive).
   - **setInterruptPolarity(IntPolarity polarity):** Sets the interrupt polarity (active low or high).
     - Parameters:
       - `IntPolarity polarity`: The interrupt polarity (ActiveLow, ActiveHigh).

5. **Debounce:**
   - **enableDebounce(void):** Enables the debounce function for the pin.
   - **disableDebounce(void):** Disables the debounce function for the pin.

6. **Interrupt Status:**
   - **getIntStatus(void):** Returns the interrupt status.
   - **getRawIntStatus(void):** Returns the raw interrupt status.

## Structures and Global Variables:

1. **Structure `GPIOReg`:**
   - This structure holds control registers for the GPIO port.

2. **Global Variables:**
   - **sGPIOList:** A static list of pointers to `GPIOReg` structures representing GPIO ports.

## Helper Functions:

1. **getGPIOAccess(void):**
   - This function gains access to the GPIO address space using mmap and opens /dev/mem.
   - Allocates memory for the GPIO registers and adds pointers to the `sGPIOList`.

## Flow diagram:
![codetoflow (10)](https://github.com/user-attachments/assets/f92d1617-1533-473d-8f18-8eeba9927153)

### Example code of usage MilkV GPIO Driver inside blink.cpp file.

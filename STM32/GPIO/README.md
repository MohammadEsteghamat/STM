
# STM32 GPIO HAL Functions

This repository provides an overview and examples for using GPIO (General Purpose Input/Output) functions in STM32 using the Hardware Abstraction Layer (HAL). The functions discussed here are used for managing input/output pins and handling interrupts.

## GPIO HAL Functions

### 1. `HAL_GPIO_Init(GPIOx, &GPIO_InitStruct)`
**Description:**  
This function is used to initialize one or more pins of a GPIO port. The pin configuration is set using the `GPIO_InitTypeDef` structure, which includes settings like pin mode, pull-up/down resistors, and output type.

**Example: Initializing Pin PA5 as Output**
```cpp
GPIO_InitTypeDef GPIO_InitStruct;
__HAL_RCC_GPIOA_CLK_ENABLE();  // Enable clock for port A

// Initialize pin PA5 as output
GPIO_InitStruct.Pin = GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```

### 2. `HAL_GPIO_DeInit(GPIOx, GPIO_PIN_x)`
**Description:**  
This function deinitializes the specified GPIO pin(s) and resets them to their default state.

**Example: Deinitialize Pin PA5**
```cpp
HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
```

### 3. `HAL_GPIO_ReadPin(GPIOx, GPIO_PIN_x)`
**Description:**  
This function reads the current state of a GPIO pin configured as an input. It returns either `GPIO_PIN_SET` (1) or `GPIO_PIN_RESET` (0).

**Example: Reading Button Status on Pin PC13**
```cpp
if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
    // Button is pressed
}
```

### 4. `HAL_GPIO_WritePin(GPIOx, GPIO_PIN_x, GPIO_PinState)`
**Description:**  
This function sets the state of a GPIO pin configured as an output. You can set the pin to either `GPIO_PIN_SET` (high) or `GPIO_PIN_RESET` (low).

**Example: Turning an LED On and Off**
```cpp
// Turn the LED on
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

// Turn the LED off
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
```

### 5. `HAL_GPIO_TogglePin(GPIOx, GPIO_PIN_x)`
**Description:**  
This function toggles the state of a GPIO pin configured as an output. It will change the pin from high to low or low to high.

**Example: Toggling an LED Every 500ms**
```cpp
while (1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);  // 500ms delay
}
```

### 6. `HAL_GPIO_LockPin(GPIOx, GPIO_PIN_x)`
**Description:**  
This function locks the configuration of a GPIO pin, preventing further changes to its settings. The pin can only be unlocked by a reset of the microcontroller.

**Example: Locking Pin PA5**
```cpp
HAL_GPIO_LockPin(GPIOA, GPIO_PIN_5);
```

**Description:**  
This function locks the configuration of a GPIO pin, preventing further changes to its settings. The pin can only be unlocked by a reset of the microcontroller.

**Use Case:**
- **Pin Protection:** When you have configured a pin for a specific function (e.g., SPI, I2C, or UART), you can lock that pin to prevent accidental changes.
- **Configuration Stability:** This ensures that the pin configuration remains unchanged even if other functions or configuration code is executed.
- **Prevention of Human or Code Errors:** Once locked, the pin's configuration cannot be altered until the microcontroller is reset.

**Example: Locking Pin PA5**
```cpp
// Lock pin PA5
HAL_GPIO_LockPin(GPIOA, GPIO_PIN_5);
```

After calling this function, Pin `PA5` will be locked, and its configuration cannot be changed until the microcontroller is reset.

**Notes:**
- Locking is useful for pins that are configured as input/output or for specific functions.
- Once a pin is locked, no further changes to its configuration can be made through HAL functions.


### 7. `HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_x)`
**Description:**  
This function is used to handle external interrupts triggered by a GPIO pin. It should be called inside the interrupt handler function (IRQ Handler).

**Example: Handling Interrupt for Pin PC13**
```cpp
void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
```

### 8. `HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)`
**Description:**  
This callback function is triggered when an external interrupt occurs on a GPIO pin. It should be implemented to handle the interrupt.

**Example: Toggling an LED on Interrupt**
```cpp
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_13) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // Toggle the LED
    }
}
```

## Table of GPIO Functions

| Function Name                       | Description                                                            |
|-------------------------------------|------------------------------------------------------------------------|
| `HAL_GPIO_Init()`                   | Initializes the GPIO pin(s) with specified settings.                   |
| `HAL_GPIO_DeInit()`                 | Deinitializes the GPIO pin(s) and resets them to their default state. |
| `HAL_GPIO_ReadPin()`                | Reads the state of an input GPIO pin.                                  |
| `HAL_GPIO_WritePin()`               | Sets the output state of a GPIO pin (high or low).                     |
| `HAL_GPIO_TogglePin()`              | Toggles the output state of a GPIO pin.                                |
| `HAL_GPIO_LockPin()`                | Locks the configuration of a GPIO pin.                                 |
| `HAL_GPIO_EXTI_IRQHandler()`        | Handles external interrupts for a GPIO pin in the IRQ handler.         |
| `HAL_GPIO_EXTI_Callback()`          | Callback function triggered on an external interrupt.                  |

## Conclusion

These GPIO functions are essential for controlling and managing the input/output pins on STM32 microcontrollers using HAL. With these functions, you can set up GPIO pins for digital I/O, handle interrupts, and control external devices like LEDs, buttons, and sensors.


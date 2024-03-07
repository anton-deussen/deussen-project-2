#include <ezButton.h>

/**
 * Global constants and variables
 */

// Buttons
ezButton button_one(25);
ezButton button_two(23);

// Pins
const int potentiometer_pin = 32;
const int led_pin = 33;

// Channels
const int pwm_channel = 0;

// Enums
enum SerialCommand
{
    initial,
    command_v,
    command_o
};

enum State
{
    off,
    on,
    adjustable
};

enum Key
{
    key_v = 'v',
    key_o = 'o'
};

// Initial values
int potentiometer_value = 0;
int led_value = 0;
int current_state = off;
int button_one_times_pressed = 0;
int button_two_times_pressed = 0;

/**
 * @brief Main program setup
 */
void setup()
{
    Serial.begin(9600);

    pinMode(potentiometer_pin, INPUT);

    ledcSetup(pwm_channel, 1000, 10);
    ledcAttachPin(led_pin, pwm_channel);
    ledcWrite(pwm_channel, 0);
}

/**
 * @brief Prints the current global variable values to the serial monitor.
 */
void Debug()
{
    Serial.print("Current state: ");
    Serial.print(current_state);
    Serial.print(", ");

    Serial.print("Button 1: ");
    Serial.print(button_one.getState());
    Serial.print(", ");

    Serial.print("Button 1 pressed: ");
    Serial.print(button_one_times_pressed);
    Serial.print(", ");

    Serial.print("Button 2: ");
    Serial.print(button_two.getState());
    Serial.print(", ");

    Serial.print("Button 2 pressed: ");
    Serial.print(button_two_times_pressed);
    Serial.print(", ");

    Serial.print("Potentiometer: ");
    Serial.print(potentiometer_value);
    Serial.print(", ");

    Serial.print("LED: ");
    Serial.print(led_value);
    Serial.print('\n');
}

/**
 * @brief
 */
void HandleSerialInput()
{
    if (Serial.available() > 0)
    {
        char pressed_key = Serial.read();

        if (pressed_key == key_v)
        {
            Debug();
        }

        if (pressed_key == key_o)
        {
            current_state = off;
            button_one_times_pressed = 2;
        }
    }
}

/**
 * @brief Main program loop
 */
void loop()
{
    button_one.loop();
    button_two.loop();

    potentiometer_value = analogRead(potentiometer_pin);

    HandleSerialInput();

    if (button_one.isPressed())
    {
        button_one_times_pressed++;
    }

    if (button_two.isPressed())
    {
        button_two_times_pressed++;
    }

    if (current_state == off)
    {
        SetStateToOff();
    }

    if (current_state == on)
    {
        SetStateToOn();
    }

    if (current_state == adjustable)
    {
        SetStateToAdjustable();
    }
}

/**
 * @brief Sets the global state to off.
 */
void SetStateToOff()
{
    ledcWrite(pwm_channel, 0);

    if (button_one_times_pressed % 2 != 0)
    {
        current_state = on;
    }
}

/**
 * @brief Sets the global state to on.
 */
void SetStateToOn()
{
    ledcWrite(pwm_channel, (int)(0.13 * 1023));

    if (button_one_times_pressed % 2 == 0)
    {
        current_state = off;
    }

    if (button_two_times_pressed % 2 != 0)
    {
        current_state = adjustable;
    }
}

/**
 *
 */
void SetStateToAdjustable()
{
    ledcWrite(pwm_channel, map(potentiometer_value, 0, 4095, 0, 1023));

    if (button_one.isPressed())
    {
        current_state = off;
        button_two_times_pressed = 0;
    }

    if (button_two_times_pressed % 2 == 0)
    {
        current_state = on;
        button_two_times_pressed = 0;
    }
}

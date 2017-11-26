#include "i2c_wrapper.h"

void
I2C_start(I2C_TypeDef *I2Cx, uint8_t address, uint8_t direction)
{
    // wait until I2C1 is not busy anymore
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;

    // Send I2C1 START condition
    I2C_GenerateSTART(I2Cx, ENABLE);

    // wait for I2C1 EV5 --> Slave has acknowledged start condition
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;

    // Send slave Address for write
    I2C_Send7bitAddress(I2Cx, address, direction);

    /* wait for I2C1 EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
            ;
    }
    else if (direction == I2C_Direction_Receiver)
    {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
            ;
    }
}

void
I2C_write(I2C_TypeDef *I2Cx, uint8_t data)
{
    I2C_SendData(I2Cx, data);
    // wait for I2C1 EV8_2 --> byte has been transmitted
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
}

uint8_t
I2C_read_ack(I2C_TypeDef *I2Cx)
{
    // enable acknowledge of recieved data
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    // wait until one byte has been received
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    // read data from I2C data register and return data byte
    uint8_t data = I2C_ReceiveData(I2Cx);
    return data;
}

uint8_t
I2C_read_nack(I2C_TypeDef *I2Cx)
{
    // disabe acknowledge of received data
    // nack also generates stop condition after last byte received
    // see reference manual for more info
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    // wait until one byte has been received
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    // read data from I2C data register and return data byte
    uint8_t data = I2C_ReceiveData(I2Cx);
    return data;
}

/* This funtion issues a stop condition and therefore
 * releases the bus
 */
void
I2C_stop(I2C_TypeDef *I2Cx)
{
    // Send I2C1 STOP Condition
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

void
I2C_write_byte(I2C_TypeDef *I2Cx, uint8_t address, uint8_t byte)
{
    // Start the transmission
    I2C_start(I2Cx, address << 1, I2C_Direction_Transmitter);
    // Send a byte
    I2C_write(I2Cx, byte);
    // Stop the transmission
    I2C_stop(I2Cx);
}

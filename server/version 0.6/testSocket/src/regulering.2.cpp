#include "regulering.hpp"

regulering::regulering(int* file)
    : file_i2c_(*file),
    mq_(MAX_QUEUE_SIZE)
{
    if (ioctl(file_i2c_, I2C_SLAVE, addr_) < 0) {
        //Error handling
        std::cout << "Failed to acquire bus acces and/or talk to slave" << std::endl;
        return;
    }
}

void regulering::run()
{
    while(1)
    {
        unsigned long id;
        osapi::Message* msg = mq_.receive(id);

        handleMsg(id, msg);

        delete msg;
    }
}

void regulering::handleMsg(unsigned long id, osapi::Message* msg)
{
    
}


void regulering::readI2C(unsigned char *buf, int length)
{
    if (read(file_i2c_, buf, length) != length) {
        std::cout << "regulering:\t "
            << "Failed to read from I2C bus. Error '"
            << errno
            << "'"
            << std::endl;
    }
    std::cout << file_i2c_ << std::endl;
}

void regulering::writeI2C(unsigned char addr, unsigned int temp, unsigned int humi)
{
    unsigned char I2C_WR_Buf[10] = {0};

    I2C_WR_Buf[0] = addr;
    I2C_WR_Buf[1] = temp;
    I2C_WR_Buf[2] = humi;

    int length = 3;

    if (write(file_i2c_, I2C_WR_Buf, length) != length) {
        std::cout << "regulering:\t "
            << "Failed to write to I2C bus. Error '"
            << errno
            << "'"
            << std::endl;
    }

    std::cout << "regulering:\t "
        << "Sent command '"
        << temp << " og " << humi <<
        << "' to address '"
        << addr
        << "'"
        << std::endl;
}

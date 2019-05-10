#include "PSoC_handler.hpp"

PSoCHandler::PSoCHandler(int* file)
    : file_i2c_(*file),
    mq_(MAX_QUEUE_SIZE)
{
    if (ioctl(file_i2c_, I2C_SLAVE, addr_) < 0) {
        //Error handling
        std::cout << "Failed to acquire bus acces and/or talk to slave" << std::endl;
        return;
    }
}

void PSoCHandler::run()
{
    while(1)
    {
        unsigned long id;
        osapi::Message* msg = mq_.receive(id);

        handleMsg(id, msg);

        delete msg;
    }
}

void PSoCHandler::handleMsg(unsigned long id, osapi::Message* msg)
{
    switch(id)
    {
        case ID_SET_TEMP:
        {
            psocSetTemp* req = static_cast<psocSetTemp*> (msg);
            
            std::cout << "PSoCHandler:\t "
                << "Received temperature command with '"
                << req->temp
                << "'"
                << std::endl;
            writeI2C('T',req->temp);
            
            req->mq->send(req->returnId);
        
            break;
        }
        case ID_SET_HUMI:
        {
            psocSetHumi* req = static_cast<psocSetHumi*> (msg);
            
            std::cout << "PSoCHandler:\t "
                << "Received humidity command with '"
                << req->humi
                << "'"
                << std::endl;
            writeI2C('H', req->humi);
        
            req->mq->send(req->returnId);
        
            break;
        }
        case ID_GET_TEMP:
        {
            std::cout << "PSoCHandler: Received temperature" << std::endl;
            // Convert the given request message
            psocGetTemp* req = static_cast<psocGetTemp*> (msg);

            // Get temperature from PSoC via I2C
            unsigned int buf[2];

            readI2C(buf, 2);

            if (buf == nullptr) {
                std::cout << "Buffer is nullptr in READ TEMP" << std::endl;
                break;
            }

            req->mq->send(req->returnId);

            break;
        }
        case ID_GET_HUMI:
        {
            

            std::cout << "PSoCHandler: Received humidity" << std::endl;
            // Convert the given request message
            psocGetHumi* req = static_cast<psocGetHumi*> (msg);

            // Get temperature from PSoC via I2C
            unsigned int buf[2];

            readI2C(buf, 2);

            if (buf == nullptr) {
                std::cout << "Buffer is nullptr in READ HUMI" << std::endl;
                break;
            }

            req->mq->send(req->returnId);
            break;
        }
        default:
        {

            break;
        }
    }
}


void PSoCHandler::readI2C(unsigned int *buf, int length)
{
    if (read(file_i2c_, buf, length) != length) {
        std::cout << "PSoCHandler:\t "
            << "Failed to read from I2C bus. Error '"
            << errno
            << "'"
            << std::endl;
    }
}

    void writeI2C(unsigned int addr, unsigned int temp, unsigned int humi)
{
    unsigned int I2C_WR_Buf[3] = {0};

    I2C_WR_Buf[0] = addr;
    I2C_WR_Buf[1] = temp;
    I2C_WR_Buf[1] = humi;
    

    int length = 3;

    if (write(file_i2c_, I2C_WR_Buf, length) != length) {
        std::cout << "PSoCHandler:\t "
            << "Failed to write to I2C bus. Error '"
            << errno
            << "'"
            << std::endl;
    }

    std::cout << "PSoCHandler:\t "
        << "Sent command '"
        << temp,humi
        << "' to address '"
        << addr
        << "'"
        << std::endl;
}

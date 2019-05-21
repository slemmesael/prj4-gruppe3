/**
 * PSOCHandler
 */

#ifndef CLASS_PSOCHANDLER_HPP
#define CLASS_PSOCHANDLER_HPP

//OS Api
#include <osapi/MsgQueue.hpp>
#include <osapi/ThreadFunctor.hpp>

//Needed for PSoCHandler
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>

/** Command messages */

/**
 * Set temperature
 */
struct psocTemp : public osapi::Message
{
    unsigned int temp; // 10 - 40
};
/**
 * Set humidity
 */
struct psocHumi : public osapi::Message
{
    unsigned int humi; // 10 - 60
};
/**
 * Set plant type
 */
struct planttype : public osapi::Message
{
    enum {
        ID_PLANT_1,
        ID_PLANT_2,
        ID_PLANT_3,
    };
    unsigned int plant; // 1-3
    //std::string plant; 
    
    // Optional for return message
    osapi::MsgQueue* mq;
    unsigned int returnId;
};


class PSoCHandler : public osapi::ThreadFunctor
{
public:
    enum {
        /** Commands that doesn't need a response */
        ID_SET_TEMP,
        ID_SET_HUMI,

        /** Requests that need response */
        ID_GET_TEMP,
        ID_GET_HUMI,
    };

    /**
     * Constructor for PSoCHandler
     * @param file Reference to I2C bus
     */
    PSoCHandler(int* file);

    /**
     * A constant that defines the max amount of messages that can be in the
     * local osapi message queue.
     */
    static const int MAX_QUEUE_SIZE = 20;

    /**
     * A simple getter for the osapi message queue
     * @return A reference to the local osapi message queue
     */
    osapi::MsgQueue* getMsgQueue()
    {
        return &mq_;
    }
private:
    /**
     * Handling the message queue
     * @param id  Message id
     * @param msg Message reference
     */
    void handleMsg(unsigned long id, osapi::Message* msg);

    /**
     * The ThreadFunctor, thread method (osapi)
     */
    void run();

    /**
     * Read the data input from the I2C bus
     * @param  buf
     */
    /**
     * Read the data input from the I2C bus
     * @param  buf     The buffer to set the read values
     * @param  length  The length of the buffer
     */
    void readI2C(unsigned int *buf, int length);

    /**
     * Write to I2C bus
     * @param  addr     [description]
     * @param  temp     [description]
     * @param  humi     [description]
     */
    void writeI2C(unsigned int addr, unsigned int temp, unsigned int humi);

    /**
     * File for I2C bus to read and write to
     */
    int file_i2c_;

    /**
     * Error number
     */
    int err_;

    /**
     * The slave address (PSoC5 I2C address)
     */
    int addr_ = 0x50;

    /**
     * The osapi Message Queue
     */
    osapi::MsgQueue mq_;
};

#endif

// Copyright 2015-2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once
#ifndef HW_CDC_H
#define HW_CDC_H

#include "sdkconfig.h"
#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32S3

#include "HardwareSerial.h"
#include <inttypes.h>
#include "esp_event.h"
#include "Stream.h"

ESP_EVENT_DECLARE_BASE(ARDUINO_HW_CDC_EVENTS);

typedef enum {
    ARDUINO_HW_CDC_ANY_EVENT = ESP_EVENT_ANY_ID,
    ARDUINO_HW_CDC_CONNECTED_EVENT = 0,
    ARDUINO_HW_CDC_BUS_RESET_EVENT,
    ARDUINO_HW_CDC_RX_EVENT,
    ARDUINO_HW_CDC_TX_EVENT,
    ARDUINO_HW_CDC_MAX_EVENT,
} arduino_hw_cdc_event_t;

typedef union {
    struct {
            size_t len;
    } rx;
    struct {
            size_t len;
    } tx;
} arduino_hw_cdc_event_data_t;

class HWCDC: public HardwareSerial
{
public:
    HWCDC();
    ~HWCDC();

    void onEvent(esp_event_handler_t callback);
    void onEvent(arduino_hw_cdc_event_t event, esp_event_handler_t callback);

    size_t setRxBufferSize(size_t);
    size_t setTxBufferSize(size_t);
    void setTxTimeoutMs(uint32_t timeout);
    virtual void begin(unsigned long baud, uint32_t config=0, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false, unsigned long timeout_ms = 20000UL, uint8_t rxfifo_full_thrhd = 112) override;
    virtual void end(bool fullyTerminate = true) override;
    
    virtual int available(void) override;
    virtual int availableForWrite(void) override;
    virtual int peek(void) override;
    virtual int read(void) override;
    virtual size_t read(uint8_t *buffer, size_t size) override;
    virtual size_t write(uint8_t) override;
    virtual size_t write(const uint8_t *buffer, size_t size) override;
    virtual void flush(void) override;
    
    inline size_t read(char * buffer, size_t size)
    {
        return read((uint8_t*) buffer, size);
    }
    inline size_t write(const char * buffer, size_t size)
    {
        return write((uint8_t*) buffer, size);
    }
    inline size_t write(const char * s)
    {
        return write((uint8_t*) s, strlen(s));
    }
    inline size_t write(unsigned long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t) n);
    }
    virtual operator bool() const override;
    virtual void setDebugOutput(bool) override;
    virtual uint32_t baudRate() override {return 115200;}

};

#if ARDUINO_USB_MODE
#if ARDUINO_USB_CDC_ON_BOOT//Serial used for USB CDC
extern HWCDC Serial;
#else
extern HWCDC USBSerial;
#endif
#endif

#endif /* CONFIG_IDF_TARGET_ESP32C3 */
#endif
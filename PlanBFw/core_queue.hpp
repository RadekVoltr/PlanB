#ifndef SRC_COREQUEUE_H_
#define SRC_COREQUEUE_H_

#ifdef ARDUINO_ARCH_ESP32 

#else
  #include <pico/util/queue.h>
#endif

//more dynamic version of multicore queue, needed for sending more data over queue
//updated version of arduino-pico/cores/rp2040/RP2040Support.h from https://github.com/earlephilhower/arduino-pico

class _CoreQueue {
private:
  #ifdef ARDUINO_ARCH_ESP32
    QueueHandle_t _queue;
  #else
    queue_t _queue;
  #endif
  
public:
    _CoreQueue() { /* noop */ };
    ~_CoreQueue() { /* noop */ };

    void begin(int queue_count) {
      #ifdef ARDUINO_ARCH_ESP32 
          _queue = xQueueCreate( queue_count, sizeof( uint32_t ) );
          Serial.print("Queue :");Serial.println((uint32_t)_queue);
        #else
          queue_init(&_queue, sizeof(uint32_t), queue_count);
        #endif
    }

    void push(uint32_t val) {
        while (!push_nb(val)) { /* noop */ }
    }

    bool push_nb(uint32_t val) {
        // Push to the other FIFO
        #ifdef ARDUINO_ARCH_ESP32 
          return xQueueSend(_queue, &val, (TickType_t)10);
        #else
          return queue_try_add(&_queue, &val);
        #endif
    }

    uint32_t pop() {
        uint32_t ret;
        while (!pop_nb(&ret)) { /* noop */ }
        return ret;
    }

    bool pop_nb(uint32_t *val) {
        // Pop from my own FIFO
        #ifdef ARDUINO_ARCH_ESP32 
          return xQueueReceive(_queue, val, ( TickType_t ) 10);
        #else
          return queue_try_remove(&_queue, val);
        #endif
    }

    int available() {
        #ifdef ARDUINO_ARCH_ESP32 
          return uxQueueMessagesWaiting(_queue);
        #else
          return queue_get_level(&_queue);
        #endif
    }

    void clear() {
        uint32_t val;

        #ifdef ARDUINO_ARCH_ESP32 
          while ( xQueueReceive(_queue, &val, ( TickType_t ) 10))
            {}
        #else
        while (queue_try_remove(&_queue, &val)) {
            tight_loop_contents();
                    }
        #endif

      }
};



#endif
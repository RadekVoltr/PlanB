#ifndef SRC_COREQUEUE_H_
#define SRC_COREQUEUE_H_

#include <pico/util/queue.h>

//more dynamic version of multicore queue, needed for sending more data over queue
//updated version of arduino-pico/cores/rp2040/RP2040Support.h from https://github.com/earlephilhower/arduino-pico

class _CoreQueue {
public:
    _CoreQueue() { /* noop */ };
    ~_CoreQueue() { /* noop */ };

    void begin(int queue_count) {
        queue_init(&_queue, sizeof(uint32_t), queue_count);
    }

    void push(uint32_t val) {
        while (!push_nb(val)) { /* noop */ }
    }

    bool push_nb(uint32_t val) {
        // Push to the other FIFO
        return queue_try_add(&_queue, &val);
    }

    uint32_t pop() {
        uint32_t ret;
        while (!pop_nb(&ret)) { /* noop */ }
        return ret;
    }

    bool pop_nb(uint32_t *val) {
        // Pop from my own FIFO
        return queue_try_remove(&_queue, val);
    }

    int available() {
        return queue_get_level(&_queue);
    }

    void clear() {
        uint32_t val;

        while (queue_try_remove(&_queue, &val)) {
            tight_loop_contents();
        }

    }

private:
    queue_t _queue;
};



#endif
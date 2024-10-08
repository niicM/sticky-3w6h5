/*
 * Copyright (c) 2022 Golioth, Inc.
 * Copyright (c) 2023 Nick Miller
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h> // memcpy

// This type must be atomic. If you are on an 8-bit CPU, you would change the type to uint8_t.
typedef uint32_t ringbuf_index_t;

typedef struct {
    volatile ringbuf_index_t write_index;
    volatile ringbuf_index_t read_index;
    /// User-allocated buffer for item storage.
    /// For an item_size X and Y total items, this must be at least (X * (Y + 1)) bytes.
    uint8_t* buffer;
    /// Size, in bytes, of buffer
    size_t buffer_size;
    /// Size, in bytes, of each item in the ringbuf
    size_t item_size;
} ringbuf_t;

#define RINGBUF_BUFFER_SIZE(item_sz, max_num_items) (item_sz * (max_num_items + 1))

// Convenience macro that defines and initializes two variables in the current scope:
//      uint8_t <name>_buffer[];
//      ringbuf_t <name>;
#define RINGBUF_DEFINE_AND_INIT(name, item_sz, max_num_items) \
    uint8_t name##_buffer[RINGBUF_BUFFER_SIZE(item_sz, max_num_items)]; \
    ringbuf_t name; \
    ringbuf_init(&name, name##_buffer, sizeof(name##_buffer), item_sz)

/// When ringbuf is empty, write_index == read_index.
/// When ringbuf is full, write_index == read_index - 1, modulo total_items

static inline void ringbuf_init(
        ringbuf_t* ringbuf,
        uint8_t* buffer,
        size_t buffer_size,
        size_t item_size) {
    ringbuf->write_index = 0;
    ringbuf->read_index = 0;
    ringbuf->buffer = buffer;
    ringbuf->buffer_size = buffer_size;
    ringbuf->item_size = item_size;
}

static inline size_t total_items(const ringbuf_t* ringbuf) {
    return ringbuf->buffer_size / ringbuf->item_size;
}

static inline ringbuf_index_t next_write_index(const ringbuf_t* ringbuf) {
    return (ringbuf->write_index + 1) % total_items(ringbuf);
}

static inline bool ringbuf_is_empty(const ringbuf_t* ringbuf) {
    return (ringbuf->write_index == ringbuf->read_index);
}

static inline bool ringbuf_is_full(const ringbuf_t* ringbuf) {
    ringbuf_index_t next_wr_index = (ringbuf->write_index + 1) % total_items(ringbuf);
    return (next_wr_index == ringbuf->read_index);
}

static inline bool ringbuf_put(ringbuf_t* ringbuf, const void* item) {
    if (!item) {
        return false;
    }

    if (ringbuf_is_full(ringbuf)) {
        return false;
    }

    uint8_t* buffer_wr_ptr = ringbuf->buffer + ringbuf->write_index * ringbuf->item_size;
    memcpy(buffer_wr_ptr, item, ringbuf->item_size);
    ringbuf->write_index = next_write_index(ringbuf);

    return true;
}

static inline bool ringbuf_get_internal(ringbuf_t* ringbuf, void* item, bool remove) {
    if (ringbuf_is_empty(ringbuf)) {
        return false;
    }

    if (item) {
        const uint8_t* buffer_rd_ptr = ringbuf->buffer + ringbuf->read_index * ringbuf->item_size;
        memcpy(item, buffer_rd_ptr, ringbuf->item_size);
    }

    if (remove) {
        ringbuf->read_index = (ringbuf->read_index + 1) % total_items(ringbuf);
    }

    return true;
}

static inline bool ringbuf_get(ringbuf_t* ringbuf, void* item) {
    return ringbuf_get_internal(ringbuf, item, true);
}

static inline bool ringbuf_peek(ringbuf_t* ringbuf, void* item) {
    return ringbuf_get_internal(ringbuf, item, false);
}

static inline size_t ringbuf_size(const ringbuf_t* ringbuf) {
    ringbuf_index_t write_index = ringbuf->write_index;
    ringbuf_index_t read_index = ringbuf->read_index;

    if (write_index > read_index) {
        return (write_index - read_index);
    }

    if (read_index > write_index) {
        return (write_index + total_items(ringbuf) - read_index);
    }

    // read_index == write_index
    return 0;
}

static inline size_t ringbuf_capacity(const ringbuf_t* ringbuf) {
    // When the ringbuf is full, write_index will be one before read_index,
    // meaning we will only be able to store a number of items that is one
    // less than total_items.
    return total_items(ringbuf) - 1;
}

static inline void ringbuf_reset(ringbuf_t* ringbuf) {
    ringbuf->write_index = 0;
    ringbuf->read_index = 0;
}
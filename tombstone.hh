/*
 * Copyright (C) 2015 Cloudius Systems, Ltd.
 */

#pragma once

#include "timestamp.hh"
#include "gc_clock.hh"

/**
 * Represents deletion operation. Can be commuted with other tombstones via apply() method.
 * Can be empty.
 */
struct tombstone final {
    api::timestamp_type timestamp;
    gc_clock::time_point ttl;

    tombstone(api::timestamp_type timestamp, gc_clock::time_point ttl)
        : timestamp(timestamp)
        , ttl(ttl)
    { }

    tombstone()
        : tombstone(api::missing_timestamp, {})
    { }

    int compare(const tombstone& t) const {
        if (timestamp < t.timestamp) {
            return -1;
        } else if (timestamp > t.timestamp) {
            return 1;
        } else if (ttl < t.ttl) {
            return -1;
        } else if (ttl > t.ttl) {
            return 1;
        } else {
            return 0;
        }
    }

    bool operator<(const tombstone& t) const {
        return compare(t) < 0;
    }

    bool operator<=(const tombstone& t) const {
        return compare(t) <= 0;
    }

    bool operator>(const tombstone& t) const {
        return compare(t) > 0;
    }

    bool operator>=(const tombstone& t) const {
        return compare(t) >= 0;
    }

    bool operator==(const tombstone& t) const {
        return compare(t) == 0;
    }

    bool operator!=(const tombstone& t) const {
        return compare(t) != 0;
    }

    explicit operator bool() const {
        return timestamp != api::missing_timestamp;
    }

    void apply(const tombstone& t) {
        if (*this < t) {
            *this = t;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const tombstone& t) {
        return out << "{timestamp=" << t.timestamp << ", ttl=" << t.ttl.time_since_epoch().count() << "}";
    }
};




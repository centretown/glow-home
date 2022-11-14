#pragma once

#include "esphome.h"

namespace glow
{
  const static uint16_t upper_limit = 0x3ffe;
  const static uint16_t out_of_bounds = 0x3fff;

  class Range
  {
  private:
    union
    {
      uint32_t pack;
      struct
      {
        uint begin : 14;
        uint reverse : 1;
        uint reserved_1 : 1;
        uint end : 14;
        uint reserved_2 : 1;
        uint reserved_3 : 1;
      };
    };

  public:
    Range(uint32_t pack = 0)
    {
      Pack(pack);
    }

    Range(uint16_t first, uint16_t last)
    {
      resize(first, last);
    }

    Range(Range &range) : Range(range.Pack()) {}

    // access
    inline uint16_t Begin() const { return begin; }
    inline uint16_t End() const { return end; }
    inline uint16_t Length() const { return end - begin; }
    inline uint32_t Pack() const { return pack; }
    inline bool Reversed() const { return (reverse == 1) ? true : false; }

    // modify
    inline uint32_t Pack(uint32_t v)
    {
      pack = v;
      return resize(begin, end);
    }

    inline void Copy(Range range)
    {
      Pack(range.Pack());
    }

    inline void Reversed(bool is_reversed)
    {
      reverse = (is_reversed == true) ? 1 : 0;
    }

    inline uint32_t operator()() const
    {
      return pack;
    }

    inline uint32_t operator()(uint32_t v)
    {
      return Pack(v);
    }

    inline uint32_t operator()(uint16_t first, uint16_t last)
    {
      return Resize(first, last);
    }

    inline Range &operator=(Range other)
    {
      Pack(other());
      return *this;
    }

    inline Range &operator=(uint32_t other)
    {
      Pack(other);
      return *this;
    }

    // resize
    inline Range &operator++()
    {
      if (end < upper_limit)
      {
        ++end;
        resize(begin, end);
      }
      return *this;
    }

    inline Range &operator--()
    {
      if (end > begin)
      {
        --end;
        resize(begin, end);
      }
      return *this;
    }

    inline uint32_t operator+(uint16_t v) const
    {
      uint16_t last = end + v;
      if (last < end)
      {
        last = upper_limit;
      }
      return Range(begin, last)
          .Pack();
    }

    inline uint32_t operator-(uint16_t v) const
    {
      uint16_t last = end - v;
      if (last < begin || last > end)
      {
        last = begin;
      }
      return Range(begin, last)
          .Pack();
    }

    inline Range &operator+=(uint16_t v)
    {
      uint16_t last = v + end;
      if (last < end)
      {
        last = upper_limit;
      }
      resize(begin, last);
      return *this;
    }

    inline Range &operator-=(uint16_t v)
    {
      uint16_t last = end - v;
      if (last < begin || last > end)
      {
        last = begin;
      }
      resize(begin, last);
      return *this;
    }

    // move
    inline uint32_t operator<<(const uint16_t v) const
    {
      uint16_t delta = v;
      if (delta > begin)
      {
        delta = begin;
      }
      return Range(begin - delta,
                   end - delta)
          .Pack();
    }

    inline uint32_t operator>>(const uint16_t v) const
    {
      uint16_t delta = v;
      uint16_t last = end + delta;
      if (last < end)
      {
        delta = upper_limit - end;
        last = end + delta;
      }
      return Range(begin + delta, last)
          .Pack();
    }

    inline Range &operator<<=(const uint16_t v)
    {
      uint16_t delta = v;
      if (delta > begin)
      {
        delta = begin;
      }
      resize(begin - delta, end - delta);
      return *this;
    }

    inline Range &operator>>=(const uint16_t v)
    {
      uint16_t delta = v;
      uint16_t last = end + v;
      if (last < end)
      {
        delta = upper_limit - end;
        last = end + delta;
      }
      resize(begin + delta, last);
      return *this;
    }

  private:
    inline uint32_t resize(uint16_t first, uint16_t last)
    {
      begin = first;
      end = last;
      return pack;
    }

  public:
    inline uint32_t Resize(uint16_t first, uint16_t last)
    {
      if (last < first)
      {
        uint16_t v = last;
        last = first;
        first = v;
        reverse = 1;
      }
      else
      {
        reverse = 0;
      }

      return resize(first, last);
    }

    // implement
    inline uint16_t Map(uint16_t index) { return index; }

    template <typename PUT>
    inline void forward(PUT &put)
    {
      for (uint16_t i = begin; i < end; i++)
      {
        put(i);
      }
    }

    template <typename PUT>
    inline void backward(PUT &put)
    {
      const uint16_t length = Length();
      for (uint16_t i = 1; i <= length; i++)
      {
        put(end - i);
      }
    }

    template <typename PUT, typename PUTTER>
    inline void spin(PUT &put, PUTTER &putter)
    {
      if (reverse == 1)
      {
        backward(put);
      }
      else
      {
        forward(put);
      }
      putter.Update();
    }

    template <typename PUTTER, typename VALUE>
    inline void Spin(PUTTER &putter, VALUE value)
    {
      auto put = [&](uint16_t index)
      {
        putter.Put(index, value);
      };
      spin(put, putter);
    }

    template <typename PUTTER, typename MAPPER, typename VALUE>
    inline void Spin(PUTTER &putter, MAPPER &mapper, VALUE value)
    {
      auto put = [&](uint16_t index)
      {
        putter.Put(mapper.Map(index), value);
      };

      spin(put, putter);
    }

    template <typename PUTTER, typename VALUE>
    inline void SpinValues(PUTTER &putter, VALUE value)
    {
      auto put = [&](uint16_t index)
      {
        putter.Put(index, value.Map(index));
      };

      spin(put, putter);
    }

    template <typename PUTTER, typename MAPPER, typename VALUE>
    inline void SpinValues(PUTTER &putter, MAPPER &mapper, VALUE value)
    {
      auto put = [&](uint16_t index)
      {
        putter.Put(mapper.Map(index), value.Map(index));
      };

      spin(put, putter);
    }
  };
}

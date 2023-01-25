#pragma once

#include "base.h"

namespace glow
{
  class Filer
  {
  private:
    char key[40];
    char value[20];

  public:
    bool split(char *buffer)
    {
      char *pequals = strchr(buffer, '=');
      if (!pequals)
      {
        return false;
      }
      size_t key_len = pequals - buffer;
      strncpy(key, buffer, key_len);
      key[key_len] = 0;
      strcpy(value, pequals + 1);
      return true;
    }

    template <typename PROPERTIES>
    void read(const char *filename, PROPERTIES &properties)
    {
      FILE *handle = fopen(filename, "r");
      if (handle == NULL)
      {
        write(filename, properties);
        return;
      }

      int status = 0;
      char buffer[60] = {0};
      do
      {
        status = fscanf(handle, "%60s\n", buffer);
        // printf("status=%d buffer=%s\n", status, buffer);
        if (status > 0)
        {
          if (split(buffer))
          {
            // printf("key=%s value=%s\n", key, value);
            properties.set(key, value);
          }
        }
      } while (status > 0);
      fclose(handle);
    }

    template <typename PROPERTIES>
    void write(const char *filename, PROPERTIES &properties)
    {
      FILE *handle = fopen(filename, "w");
      if (handle == NULL)
      {
        return;
      }

      for (uint8_t id = 0; id < properties.count(); id++)
      {
        properties.get_key(id, key, sizeof(key));
        properties.get(id, value, sizeof(value));
        fprintf(handle, "%s=%s\n", key, value);
      }

      fclose(handle);
    }
  };

}

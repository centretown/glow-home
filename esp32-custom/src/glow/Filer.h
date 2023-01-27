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
      const char *delim = " ,=:{}'\"\n\t";
      char *pch = strtok(buffer, delim);
      if (pch == NULL)
      {
        return false;
      }
      strncpy(key, pch, sizeof(key));

      pch = strtok(NULL, delim);
      if (pch == NULL)
      {
        return false;
      }
      strncpy(value, pch, sizeof(value));

      return true;
    }

    template <typename PROPERTIES>
    bool read(const char *filename, PROPERTIES &properties)
    {
      FILE *handle = fopen(filename, "r");
      if (handle == NULL)
      {
        return write(filename, properties);
      }

      char buffer[60] = {0};
      while (fgets(buffer, sizeof(buffer), handle))
      {
        if (split(buffer))
        {
          // printf("key=%s value=%s\n", key, value);
          properties.set(key, value);
        }
      }

      fclose(handle);
      return true;
    }

    template <typename PROPERTIES>
    bool write(const char *filename, PROPERTIES &properties)
    {
      FILE *handle = fopen(filename, "w");
      if (handle == NULL)
      {
        return false;
      }

      for (uint8_t id = 0; id < properties.count(); id++)
      {
        properties.get_key(id, key, sizeof(key));
        properties.get(id, value, sizeof(value));
        fprintf(handle, "%s=%s\n", key, value);
      }

      fclose(handle);
      return true;
    }
  };
}

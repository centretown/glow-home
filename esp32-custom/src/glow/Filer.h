#pragma once

#include "base.h"
#include "Parser.h"
// #include "Properties.h"

namespace glow
{
  class Filer
  {
  private:
    char key[40];
    char value[40];
    Parser parser;

  public:
    template <typename PROPERTIES>
    bool read_create(const char *filename, PROPERTIES &properties)
    {
      if (read(filename, properties))
      {
        return true;
      }
      return write(filename, properties);
    }

    template <typename PROPERTIES>
    bool read(const char *filename, PROPERTIES &properties)
    {
      FILE *handle = fopen(filename, "r");
      if (handle == NULL)
      {
        return false;
      }

      char buffer[60] = {0};
      while (fgets(buffer, sizeof(buffer), handle))
      {
        auto status = parser.parse(buffer,
                                   key, sizeof(key),
                                   value, sizeof(value));
        switch (status)
        {
        case Parser::Value:
          properties.set(key, value);
          break;

        case Parser::Layer:
          printf("add layer %s\n", value);
          break;

        case Parser::Incomplete:
          printf("Incomplete %s\n", key);
          break;

        case Parser::Ignore:
        default:
          break;
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

        auto io = properties.get_io(id);
        io.get(properties, value, sizeof(value));
        fprintf(handle, "%s\n%s=%s\n\n",
                io.comment.c_str(), key, value);
      }

      fclose(handle);
      return true;
    }
  };
}

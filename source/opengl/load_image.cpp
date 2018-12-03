#include "opengl/load_image.hpp"

#include <fstream>
#include <memory>
#include <sstream>

namespace opengl
{
  Texture loadTexture(std::string const &name)
  {
    auto bytesToInt = [](char *bytes)
		      {
			return (static_cast<unsigned char>(bytes[3]) << 24u)
			  | (static_cast<unsigned char>(bytes[2]) << 16u)
			  | (static_cast<unsigned char>(bytes[1]) << 8u)
			  | static_cast<unsigned char>(bytes[0]);
		      };
    std::ifstream file(name, std::ios::binary);

    if (!file)
      throw std::runtime_error("'" + name + "': failed to open");
    file.exceptions(std::ios::badbit);

    char readBuf[4];
    std::array<unsigned int, 2u> dim{0u, 0u};
    try {
      file.seekg(10);
      file.read(readBuf, sizeof(readBuf));
      unsigned int offset(bytesToInt(readBuf));

      file.seekg(14);
      file.read(readBuf, sizeof(readBuf));

      file.read(readBuf, sizeof(readBuf));
      dim[0] = bytesToInt(readBuf);

      file.read(readBuf, sizeof(readBuf));
      dim[1] = bytesToInt(readBuf);

      file.seekg(offset);

      std::unique_ptr<char[]> data(new char[dim[0] * dim[1] * sizeof(unsigned int)]);

      file.read(&data[0], std::streamsize(dim[0] * dim[1] * sizeof(unsigned int)));

      std::streamsize r(file.gcount());

      if (r != std::streamsize(dim[0] * dim[1] * sizeof(unsigned int)))
	{
	  std::stringstream s;

	  s << name << ": file seems truncated, " << r << std::string(" bytes read. Expected ") << std::streamsize(dim[0] * dim[1] * sizeof(unsigned int));
	  throw std::runtime_error(s.str());
	}
      file.exceptions(std::ios::goodbit);

      for (auto it = &data[0]; it < &data[dim[0] * dim[1] * sizeof(unsigned int)]; it += sizeof(unsigned int))
	{
	  std::swap(it[0], it[3]);
	  std::swap(it[1], it[2]);
	}
      Texture texture;

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexImage2D(GL_TEXTURE_2D,
		   0,
		   GL_RGBA,
		   static_cast<GLsizei>(dim[0]),
		   static_cast<GLsizei>(dim[1]),
		   0,
		   GL_RGBA,
		   GL_UNSIGNED_BYTE,
		   static_cast<void *>(data.get()));
      glBindTexture(GL_TEXTURE_2D, 0);
      return texture;
    } catch (std::exception const &e) {
      throw std::runtime_error(name + ": failed to load texture(" + std::string(e.what()) + ")");
    }
  }

}

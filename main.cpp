#include <fstream>
#include <cstdint>
#include <cstdio>
#include <new>

void generateHeader(size_t dataBytesCount, unsigned char * data) {
  printf(
    "#pragma once\n"
    "\n"
    "#include <stdint.h>\n"
    "\n"
    "size_t resourceGetBytesCount() {\n"
    "  return %zu;\n"
    "}\n"
    "\n"
    "unsigned char resource[] = {\n",
    dataBytesCount
  );
  for (size_t i = 0; i < dataBytesCount; i += 1) {
    printf("  %d,\n", data[i]);
  }
  printf("};\n");
}

int main(int argsCount, const char ** args) {
  if (argsCount < 2) {
    printf("Usage example: headergen.exe C:/path/to/some.file");
    return 0;
  }

  const char * filepath = args[1];

  std::ifstream file(filepath, std::ifstream::binary);
  if (file.fail() == false) {
    std::filebuf * fileFb = file.rdbuf();
    if (fileFb != 0) {
      size_t fileDataBytesCount = fileFb->pubseekoff(0, file.end, file.in);
      fileFb->pubseekpos(0, file.in);
      if (fileDataBytesCount > 0) {
        char * fileData = new char[fileDataBytesCount];
        fileFb->sgetn(fileData, fileDataBytesCount);

        generateHeader(fileDataBytesCount, (unsigned char *)fileData);

        delete[] fileData;
      }
    }
  }
  file.close();

  return 0;
}

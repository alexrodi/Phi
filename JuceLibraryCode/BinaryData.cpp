/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== Speaker_Icon.svg ==================
static const unsigned char temp_binary_data_0[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<svg width=\"600px\" height=\"600px\" viewBox=\"0 0 600 600\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
"    <title>Speaker_Icon</title>\n"
"    <g id=\"Page-1\" stroke=\"none\" stroke-width=\"1\" fill=\"none\" fill-rule=\"evenodd\">\n"
"        <g id=\"Speaker_Icon\" transform=\"translate(86, 136.7933)\" stroke=\"#000000\" stroke-width=\"33.3333333\">\n"
"            <polygon id=\"Path\" fill=\"#000000\" fill-rule=\"nonzero\" stroke-linejoin=\"round\" points=\"222.593333 0 108.233333 98.9133333 0 98.9133333 0 226.2 106.593333 226.2 222.593333 326.54\"></polygon>\n"
"            <path d=\"M280,92.2066667 C308.425359,135.514553 308.425359,191.565447 280,234.873333 M327.333333,44.8733333 C379.344836,115.43572 379.344836,211.64428 327.333333,282.206667 M370.666667,1.54 C446.683196,96.1638131 446.683196,230.916187 37"
"0.666667,325.54\" id=\"Shape\" stroke-linecap=\"round\"></path>\n"
"        </g>\n"
"    </g>\n"
"</svg>";

const char* Speaker_Icon_svg = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x89e74e1e:  numBytes = 949; return Speaker_Icon_svg;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "Speaker_Icon_svg"
};

const char* originalFilenames[] =
{
    "Speaker_Icon.svg"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}

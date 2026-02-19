#include "../../include/DIPAL/IO/PNGImageIO.hpp"

namespace DIPAL {

Result<std::unique_ptr<Image>> PNGImageIO::load(std::string_view filename) {
    (void)filename;
    return makeErrorResult<std::unique_ptr<Image>>(
        ErrorCode::NotImplemented,
        "PNG I/O not yet implemented"
    );
}

VoidResult PNGImageIO::save(const Image& image,
                            std::string_view filename,
                            int compressionLevel) {
    (void)image;
    (void)filename;
    (void)compressionLevel;
    return makeVoidErrorResult(
        ErrorCode::NotImplemented,
        "PNG I/O not yet implemented"
    );
}

}  // namespace DIPAL

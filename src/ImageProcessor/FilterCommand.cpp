// src/ImageProcessor/FilterCommand.cpp
#include "../../include/DIPAL/ImageProcessor/FilterCommand.hpp"

#include <stdexcept>

namespace DIPAL {

FilterCommand::FilterCommand(std::unique_ptr<FilterStrategy> filter) 
    : m_filter(std::move(filter)) {
    if (!m_filter) {
        throw std::invalid_argument("Filter cannot be null");
    }
}

Result<std::unique_ptr<Image>> FilterCommand::execute(const Image& image) {
    return m_filter->apply(image);
}

std::string_view FilterCommand::getName() const {
    return m_filter->getName();
}

bool FilterCommand::isUndoable() const {
    return false;  // Filters are generally not undoable
}

} // namespace DIPAL

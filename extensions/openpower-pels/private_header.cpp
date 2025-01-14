#include "private_header.hpp"

#include <phosphor-logging/log.hpp>

namespace openpower
{
namespace pels
{

using namespace phosphor::logging;

PrivateHeader::PrivateHeader(Stream& pel)
{
    try
    {
        pel >> *this;
        validate();
    }
    catch (const std::exception& e)
    {
        log<level::ERR>("Cannot unflatten private header",
                        entry("ERROR=%s", e.what()));
        _valid = false;
    }
}

void PrivateHeader::validate()
{
    bool failed = false;

    if (header().id != privateHeaderSectionID)
    {
        log<level::ERR>("Invalid private header section ID",
                        entry("ID=0x%X", header().id));
        failed = true;
    }

    if (header().version != privateHeaderVersion)
    {
        log<level::ERR>("Invalid private header version",
                        entry("VERSION=0x%X", header().version));
        failed = true;
    }

    if (_sectionCount < minSectionCount)
    {
        log<level::ERR>("Invalid section count in private header",
                        entry("SECTION_COUNT=0x%X", _sectionCount));
        failed = true;
    }

    _valid = (failed) ? false : true;
}

Stream& operator>>(Stream& s, PrivateHeader& ph)
{
    s >> ph._header >> ph._createTimestamp >> ph._commitTimestamp >>
        ph._creatorID >> ph._logType >> ph._reservedByte >> ph._sectionCount >>
        ph._obmcLogID >> ph._creatorVersion >> ph._plid >> ph._id;
    return s;
}

Stream& operator<<(Stream& s, PrivateHeader& ph)
{
    s << ph._header << ph._createTimestamp << ph._commitTimestamp
      << ph._creatorID << ph._logType << ph._reservedByte << ph._sectionCount
      << ph._obmcLogID << ph._creatorVersion << ph._plid << ph._id;
    return s;
}

Stream& operator>>(Stream& s, CreatorVersion& cv)
{
    for (size_t i = 0; i < sizeof(CreatorVersion); i++)
    {
        s >> cv.version[i];
    }
    return s;
}

Stream& operator<<(Stream& s, CreatorVersion& cv)
{
    for (size_t i = 0; i < sizeof(CreatorVersion); i++)
    {
        s << cv.version[i];
    }
    return s;
}

} // namespace pels
} // namespace openpower

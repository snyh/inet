//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include <algorithm>
#include "inet/common/packet/ByteCountChunk.h"
#include "inet/common/packet/ChunkBuffer.h"
#include "inet/common/packet/SequenceChunk.h"

namespace inet {

ChunkBuffer::ChunkBuffer(const char *name) :
    cNamedObject(name)
{
}

ChunkBuffer::ChunkBuffer(const ChunkBuffer& other) :
    cNamedObject(other),
    regions(other.regions)
{
}

void ChunkBuffer::eraseEmptyRegions(std::vector<Region>::iterator begin, std::vector<Region>::iterator end)
{
    regions.erase(std::remove_if(begin, end, [](const Region& region) { return region.data == nullptr; }), regions.end());
}

void ChunkBuffer::sliceRegions(Region& newRegion)
{
    if (!regions.empty()) {
        auto lowerit = std::lower_bound(regions.begin(), regions.end(), newRegion, Region::compareStartOffset);
        if (lowerit != regions.begin())
            lowerit--;
        auto upperit = std::upper_bound(regions.begin(), regions.end(), newRegion, Region::compareEndOffset);
        if (upperit != regions.end())
            upperit++;
        for (auto it = lowerit; it < upperit; it++) {
            auto& oldRegion = *it;
            if (newRegion.getEndOffset() <= oldRegion.getStartOffset() || oldRegion.getEndOffset() <= newRegion.getStartOffset())
                // no intersection
                continue;
            else if (newRegion.getStartOffset() <= oldRegion.getStartOffset() && oldRegion.getEndOffset() <= newRegion.getEndOffset()) {
                // new totally covers old
                oldRegion.data = nullptr;
                regions.erase(it--);
                upperit--;
            }
            else if (oldRegion.getStartOffset() < newRegion.getStartOffset() && newRegion.getEndOffset() < oldRegion.getEndOffset()) {
                // new splits old into two parts
                Region previousRegion(oldRegion.getStartOffset(), oldRegion.data->peek(0, newRegion.getStartOffset() - oldRegion.getStartOffset()));
                Region nextRegion(newRegion.getEndOffset(), oldRegion.data->peek(newRegion.getEndOffset() - oldRegion.getStartOffset(), oldRegion.getEndOffset() - newRegion.getEndOffset()));
                oldRegion.offset = nextRegion.offset;
                oldRegion.data = nextRegion.data;
                regions.insert(it, previousRegion);
                return;
            }
            else {
                // new cuts beginning or end of old
                int64_t peekStartOffset = std::min(newRegion.getStartOffset(), oldRegion.getStartOffset());
                int64_t peekEndOffset = std::min(newRegion.getEndOffset(), oldRegion.getEndOffset());
                oldRegion.data = oldRegion.data->peek(peekStartOffset, peekEndOffset - peekStartOffset);
            }
        }
    }
}

void ChunkBuffer::mergeRegions(Region& previousRegion, Region& nextRegion)
{
    if (previousRegion.getEndOffset() == nextRegion.getStartOffset()) {
        // consecutive regions
        if (previousRegion.data->insertAtEnd(nextRegion.data)) {
            // merge into previous
            previousRegion.data = previousRegion.data->peek(0, previousRegion.data->getChunkLength());
            nextRegion.data = nullptr;
        }
        else if (nextRegion.data->insertAtBeginning(previousRegion.data)) {
            // merge into next
            previousRegion.data = nullptr;
            nextRegion.data = nextRegion.data->peek(0, previousRegion.data->getChunkLength());
        }
        else {
            // merge as a sequence
            auto sequenceChunk = std::make_shared<SequenceChunk>();
            sequenceChunk->insertAtEnd(previousRegion.data);
            sequenceChunk->insertAtEnd(nextRegion.data);
            previousRegion.data = sequenceChunk;
            nextRegion.data = nullptr;
        }
    }
}

void ChunkBuffer::replace(int64_t offset, const std::shared_ptr<Chunk>& chunk)
{
    Region newRegion(offset, chunk->isImmutable() ? chunk->dupShared() : chunk);
    sliceRegions(newRegion);
    if (regions.empty())
        regions.push_back(newRegion);
    else if (regions.back().getEndOffset() <= offset) {
        regions.push_back(newRegion);
        mergeRegions(regions[regions.size() - 2], regions[regions.size() - 1]);
        eraseEmptyRegions(regions.end() - 2, regions.end());
    }
    else if (offset + chunk->getChunkLength() <= regions.front().getStartOffset()) {
        regions.insert(regions.begin(), newRegion);
        mergeRegions(regions[0], regions[1]);
        eraseEmptyRegions(regions.begin(), regions.begin() + 2);
    }
    else {
        auto it = std::upper_bound(regions.begin(), regions.end(), newRegion, Region::compareStartOffset);
        it = regions.insert(it, newRegion);
        auto& previousRegion = *(it - 1);
        auto& region = *it;
        auto& nextRegion = *(it + 1);
        mergeRegions(previousRegion, region);
        mergeRegions(region.data != nullptr ? region : previousRegion, nextRegion);
        eraseEmptyRegions(it - 1, it + 1);
    }
}

void ChunkBuffer::clear(int64_t offset, int64_t length)
{
    for (auto it = regions.begin(); it != regions.end(); it++) {
        auto region = *it;
        if (region.offset == offset && region.data->getChunkLength() == length) {
            regions.erase(it);
            return;
        }
    }
    auto data = std::make_shared<ByteCountChunk>(length);
    Region clearedRegion(offset, data);
    sliceRegions(clearedRegion);
}

} // namespace

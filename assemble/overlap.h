//(c) 2016 by Authors
//This file is a part of ABruijn program.
//Released under the BSD license (see LICENSE file)

#pragma once

#include "kmer_index.h"
#include "fasta.h"


struct OverlapRange
{
	OverlapRange(FastaRecord::Id curId, FastaRecord::Id extId, 
				 int32_t curInit, int32_t extInit): 
		curId(curId), curBegin(curInit), curEnd(curInit), 
		extId(extId), extBegin(extInit), extEnd(extInit){}
	int32_t curRange() const {return curEnd - curBegin;}
	int32_t extRange() const {return extEnd - extBegin;}

	void reverse()
	{
		std::swap(curId, extId);
		std::swap(curBegin, extBegin);
		std::swap(curEnd, extEnd);
	}

	void complement(int32_t curLen, int32_t extLen)
	{
		std::swap(curBegin, curEnd);
		curBegin = curLen - curBegin;
		curEnd = curLen - curEnd;

		std::swap(extBegin, extEnd);
		extBegin = extLen - extBegin;
		extEnd = extLen - extEnd;

		curId = curId.rc();
		extId = extId.rc();
	}

	//current read
	FastaRecord::Id curId;
	int32_t curBegin;
	int32_t curEnd;
	//extension read
	FastaRecord::Id extId;
	int32_t extBegin;
	int32_t extEnd;
};

class OverlapDetector
{
public:
	OverlapDetector(int maximumJump, int minimumOverlap,
					int maximumOverhang):
		_maximumJump(maximumJump), _minimumOverlap(minimumOverlap),
		_maximumOverhang(maximumOverhang) 
	{}
	
	void findAllOverlaps(const VertexIndex& vertexIndex, 
						 const SequenceContainer& seqContainer);

	

	typedef std::unordered_map<FastaRecord::Id, 
					   std::vector<OverlapRange>> OverlapIndex;
	const OverlapIndex& getOverlapIndex() const {return _overlapIndex;}
private:
	std::vector<OverlapRange> getReadOverlaps(FastaRecord::Id readId, 
						 					  const VertexIndex& vertexIndex,
						 					  const SequenceContainer& seqContainer);

	bool  goodStart(int32_t currentPos, int32_t extensionPos, 
				   int32_t currentLength, int32_t extensionLength);
	bool  overlapTest(const OverlapRange& ovlp, int32_t curLen, int32_t extLen);

	enum JumpRes {J_END, J_INCONS, J_CLOSE, J_FAR};
	JumpRes jumpTest(int32_t currentPrev, int32_t currentNext,
				     int32_t extensionPrev, int32_t extensionNext);

	int _maximumJump;
	int _minimumOverlap;
	int _maximumOverhang;

	OverlapIndex _overlapIndex;
	std::vector<std::vector<char>> _overlapMatrix;
};

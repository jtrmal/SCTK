/*
 * asclite
 * Author: Jerome Ajot, Nicolas Radde, Chris Laprun
 *
 * This software was developed at the National Institute of Standards and Technology by
 * employees of the Federal Government in the course of their official duties.  Pursuant to
 * Title 17 Section 105 of the United States Code this software is not subject to copyright
 * protection within the United States and is in the public domain. asclite is
 * an experimental system.  NIST assumes no responsibility whatsoever for its use by any party.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST MAKES NO EXPRESS
 * OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING MERCHANTABILITY,
 * OR FITNESS FOR A PARTICULAR PURPOSE.
 */
 
/**
 * Internal representation of an hypothesis file or a reference file.
 */

#include "speech.h" // class's header file
#include "speechset.h"

// class constructor
Speech::Speech() {}

// class constructor
Speech::Speech(SpeechSet* parent)
{
  parentSet = parent;
}

Speech::Speech(const vector<Segment *>& segments)
{
	m_segments = vector<Segment *>(segments);
}

// class destructor
Speech::~Speech()
{
	for(size_t i=0; i<m_segments.size(); ++i)
	{
		Segment* ptr_elt = m_segments[i];
		
		if(ptr_elt)
			delete ptr_elt;
	}
	
	m_segments.clear();
}

bool Speech::PerformCaseSensitiveAlignment()
{
	return parentSet->PerformCaseSensitiveAlignment();
}

bool Speech::AreFragmentsCorrect()
{
	return parentSet->AreFragmentsCorrect();
}

bool Speech::UseOptionallyDeletable()
{
	return parentSet->UseOptionallyDeletable();
}

/**
 * Retrieves the SpeechSet in which this Speech is located 
 */
SpeechSet* Speech::GetParentSpeechSet()
{
  return parentSet;
}

/**
* Return the next Segment starting at the specified time for
* the specified source and channel. If the time is in the middle of
* A segment return the segment itself.
*/
Segment* Speech::NextSegment(int time, string source, string channel)
{
	int last_endTime = 0;	
	
	for (size_t i=0 ; i < m_segments.size() ; ++i)
	{
        Segment *currentSegment = m_segments[i];
		
		if (currentSegment->GetSource() == source && currentSegment->GetChannel() == channel)
		{
			if (currentSegment->GetEndTime() > time && last_endTime <= time)
			{
				return currentSegment;
			}
            
			last_endTime = currentSegment->GetEndTime();
		}
	}
    
	return NULL;
}

/**
 * Return the segments of this speech by the given time
 */
vector<Segment*> Speech::GetSegmentsByTime(int start, int end, string source, string channel)
{
	vector<Segment*> res;
    
	for (size_t i=0 ; i < m_segments.size() ; ++i)
	{
		Segment *currentSegment = m_segments[i];
		if (currentSegment->GetSource() == source && currentSegment->GetChannel() == channel)
		{
			int s_mid = ((currentSegment->GetEndTime()+currentSegment->GetStartTime()) / 2);
            
			if (s_mid >= start && s_mid < end)
			{
				res.push_back(currentSegment);
			}
		}
	}
	return res;
}

string Speech::ToString()
{
	std::ostringstream oss;
	oss << "<Speech NbOfSegments='" << NbOfSegments() << "'>";
	
	for(size_t i = 0; i < NbOfSegments(); ++i)
    {
        Segment *seg = GetSegment(i);
        oss << seg->ToString() << endl;
    }	
	oss << "</Speech>";
	return oss.str();
}
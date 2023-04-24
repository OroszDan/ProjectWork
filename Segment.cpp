#include "Segment.h"

Segment::Segment(const int64_t id, const float_t length, const Junction* from, const Junction* to) :
	m_Id(id), m_Length(length), m_From(from), m_To(to)
{}

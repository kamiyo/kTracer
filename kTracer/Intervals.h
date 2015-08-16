#pragma once

#include "typedefs.h"

class Interval;

typedef std::vector<Interval, std::allocator<Interval> >::iterator IntervalIterator;
typedef std::vector<Interval, std::allocator<Interval> >::const_iterator constIntervalIterator;

class Intervals
{
public:
	Intervals() {}
	~Intervals() {}
	inline void add(Interval i) {
		m_intervals.push_back(i);
	}
	inline void add(Intervals ii) {
		insert(end(), ii.begin(), ii.end());
	}
	inline Interval pop() {
		Interval temp = m_intervals.back();
		m_intervals.pop_back();
		return temp;
	}
	inline IntervalIterator begin() { return m_intervals.begin(); }
	inline IntervalIterator end() { return m_intervals.end(); }
	inline IntervalIterator erase(constIntervalIterator i) { return m_intervals.erase(i); }
	inline IntervalIterator insert(constIntervalIterator i
								 , constIntervalIterator otherBegin
								 , constIntervalIterator otherEnd)
	{ return m_intervals.insert(i, otherBegin, otherEnd); }
	
	inline Interval& operator[](int i) { return m_intervals[i]; }
	inline Interval operator[](int i) const { return m_intervals[i]; }
	inline size_t size() const { return m_intervals.size(); }


private:
	std::vector<Interval> m_intervals;
};

class Interval
{
public:
	Interval() { setEmpty(); }
	Interval(double t) : m_min(t), m_max(t) {}
	Interval(double min, double max) : m_min(min), m_max(max) {}
	~Interval() {}

	double min() const { return m_min; }
	double max() const { return m_max; }
	inline double& min() { return m_min; }
	inline double& max() { return m_max; }

	inline void setEmpty() { m_min = INF, m_max = nINF; }
	inline void setInfinite() { m_min = nINF, m_max = INF; }
	inline bool isEmpty() const { return m_min > m_max; }
	inline bool isInfinite() { return (m_min == nINF || m_max == INF); }

	inline double size() const { return m_max - m_min; }
	Intervals merged(const Interval& other) const {
		Intervals result;
		if (isEmpty()) {
			result.add(other);
			return result;
		}
		if (other.isEmpty()) {
			result.add(*this);
			return result;
		}
		if (intersects(other)) {
			Interval isect(std::max(min(), other.min()), std::min(max(), other.max()));
			result.add(isect);
			return result;
		}
// left off here		




	}
	Interval intersection(const Interval& other) const;
	Intervals difference(const Interval& other) const;

	inline bool operator==(const Interval& rhs) {
		return (min() == rhs.min() && max() == rhs.max());
	}
	inline bool operator!=(const Interval& rhs) {
		return (min() != rhs.min() || max() != rhs.max());
	}
	inline bool intersects(const Interval& other) const {
		return (max() > other.min() && other.max() > min());
	}
	static bool intersects(const Interval& a, const Interval& b) {
		return (a.max() > b.min() && b.max() > a.min());
	}

private:
	double m_min, m_max;
};
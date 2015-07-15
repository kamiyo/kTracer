#include "Hilbert.h"

Hilbert::Hilbert(int x, int y) : m_dim(Vector2i(x, y)) {
	int m = (x > y) ? x : y;
	int nOrder = 1;
	int temp = 2;
	while (temp < m) {
		temp = temp << 1;
		nOrder++;
	}
	m_points.resize(x, y);
	m_size = 0;
	nX = nY = 0;
	hilbert(nOrder, N, E, S, W);
}

void Hilbert::move(int d) {
	switch (d)
	{
	case N: nY--; break;
	case E: nX++; break;
	case S: nY++; break;
	case W: nX--; break;
	}
}

void Hilbert::hilbert(int i, int front, int right, int behind, int left) {
	if (i == 0) {
		if (nX < m_dim.x() && nY < m_dim.y())
		{
			m_points(m_size++) = Vector2i(nX, nY);
		}
	}
	else {
		hilbert(i - 1, left, behind, right, front);
		move(right);
		hilbert(i - 1, front, right, behind, left);
		move(behind);
		hilbert(i - 1, front, right, behind, left);
		move(left);
		hilbert(i - 1, right, front, left, behind);
	}
}
#pragma once

#include <unordered_map>

template <typename T>
class ConwayGrid
{
private:
	struct CellPosition {
		long x, y;

		bool operator==(const CellPosition& right) const {
			return x == right.x && y == right.y;
		}
	};
public:
	void set(long x, long y, T value) {
		_gridState[{x, y}] = value;
	}

	T get(long x, long y) const {
		auto it = _gridState.find({ x, y });
		if (it != _gridState.end()) {
			return false;
		}
		return it->second;
	}

	const auto begin() const {
		return _gridState.begin();
	}

	const auto end() const {
		return _gridState.end();
	}

	void clear() {
		_gridState.clear();
	}
private:
	struct CellPositionHash {
		size_t operator()(const CellPosition& cell) const;
	};
	std::unordered_map<CellPosition, T, CellPositionHash> _gridState;
};


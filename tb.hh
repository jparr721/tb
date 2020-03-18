/**
 * Loads csvs with no frills or other bullshit.
 */

#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace tb {
template <typename T = std::string>
class TableRow {
 public:
  int ncols = row.size();
  std::map<std::string, T> row;

  explicit TableRow(std::map<std::string, T> _row) { row = _row; }

  std::string ToString() {
    std::ostringstream os;
    for (const auto &r : row) {
      os << row.second << ", ";
    }

    return os.str();
  }
};  // namespace tb

template <typename T = std::string, typename I = unsigned int>
struct Table {
  static std::vector<I> dummy_index;

  std::vector<I> index;
  std::vector<std::string> headings;
  std::vector<TableRow<T>> rows;

  Table(std::vector<std::string> _headings,
        std::vector<I> _index = dummy_index) {
    headings = _headings;

    if (_index == dummy_index) {
      index = std::vector<I>{};
    } else {
      index = _index;
    }
  };

  Table(std::vector<TableRow<T>> _rows, std::vector<std::string> _headings,
        std::vector<T> _index) {
    rows = _rows;
    headings = _headings;
    index = _index;
  }

  void MergeFront(const Table &t) {
    assert(t.headings == headings);
    rows.insert(rows.start(), t.rows);
  }

  void MergeBack(const Table &t) {
    assert(t.headings == headings);
    rows.insert(rows.end(), t.rows);
  }

  void MergeAt(const Table &t, int index) {
    assert(t.headings == headings);
    assert(index + 1 < rows.size());
    rows.insert(rows.start() + index, t.rows);
  }

  // Assumes l -> r preserved on create
  void AddRow(const std::vector<T> &values) {
    assert(values.size() == headings.size());

    std::map<std::string, T> row;

    for (size_t i = 0; i < values.size(); ++i) {
      row[headings[i]] = values[i];
    }

    rows.push_back(TableRow<T>(row));
  }

  int size() { return rows.size(); }
  std::pair<int, int> shape() {
    return std::make_pair(rows.size(), headings.size());
  }

  std::string ToString() {
    std::ostringstream os;

    for (const auto heading : headings) {
      os << heading << ", ";
    }
    os << "\n";

    for (const auto row : rows) {
      os << row << "\n ";
    }

    return os.str();
  }

  TableRow<T> ILoc(int index) { return rows[index]; }
  TableRow<T> Loc(int index, const std::string &col) {
    return rows[index].at(col);
  }
};

class Tb {
 public:
  Tb() = default;
  Tb(const std::string &csvfile, const char delim = ',')
      : csvfile(csvfile), delim(delim) {
    _ReadCsv(csvfile);
  };

  static Tb ReadCsv(const std::string &csvfile) { return _ReadCsv(csvfile); }

 private:
  Table table;

  Tb _ReadCsv(const std::string &csvfile) {}
};
}  // namespace tb

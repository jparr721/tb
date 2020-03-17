/**
 * Loads csvs with no frills or other bullshit.
 */

#pragma once

#include <cassert>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace tb {
template <typename T = std::string>
struct TableRow {
  explicit TableRow(std::map<std::string, T> row) : row(row){};
  int ncols = row.size();

  // Map is indexed by the column and some value of type T
  std::map<std::string, T> row;
};

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

  T operator[](int index) { return rows[index]; }
  Table operator+(const Table &t) {
    assert(t.headings == headings);
    rows.insert(rows.end(), t.rows);
  }

  void Merge(const Table &t, const bool front = false) {
    assert(t.headings == headings);
    front ? rows.insert(rows.start(), t.rows) : rows.insert(rows.end(), t.rows);
  }

  // Assumes l -> r preserved on create
  void AddRow(const std::vector<T> &values) {
    assert(values.size() == headings.size());

    std::map<std::string, T> row;

    // Dual iterate
    for (size_t i = 0; i < values.size(); ++i) {
      row[headings[i]] = values[i];
    }

    rows.push_back(TableRow<T>(row));
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

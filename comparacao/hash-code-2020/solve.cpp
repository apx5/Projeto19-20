// vim: expandtab shiftwidth=2

#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <tuple>
#include <vector>

#include <fplus/fplus.hpp>

typedef std::pair<int, std::tuple<int, int, int, std::vector<int>>>
library_desc_t;

struct libraries {
  int n_books;
  int n_libraries;
  int n_days;
  std::vector<int> book_scores;
  std::vector<library_desc_t> libraries;
};

typedef std::vector<std::tuple<int, int, std::vector<int>>> output_t;

std::pair<int, int> on_ts_bd (library_desc_t pt)
{
  int ts, bd;
  std::tie(std::ignore, ts, bd, std::ignore) = pt.second;
  return std::make_pair(ts, bd);
}

struct libraries read_libraries (void)
{
  std::string input(std::istreambuf_iterator<char>{std::cin}, {});
  std::vector<std::vector<int>> values = fplus::fwd::apply(
      input,
      fplus::fwd::split_lines(false),
      fplus::fwd::transform([](std::string line) -> std::vector<int> {
        return fplus::fwd::apply(
            line,
            fplus::fwd::split_words(false),
            fplus::fwd::transform(fplus::read_value_unsafe<int>));
        }));

  struct libraries ret;

  { /* Primeira linha */
    ret.n_books = values[0][0];
    ret.n_libraries = values[0][1];
    ret.n_days = values[0][2];
  }

  { /* Segunda linha */
    ret.book_scores = values[1];
  }

  { /* Resto */
    ret.libraries.reserve(ret.n_libraries);
    for (int i = 0; i < ret.n_libraries; i++) {
      std::vector<int> props = values[2 * i + 2 + 0];
      std::vector<int> books = values[2 * i + 2 + 1];

      int n_books = props[0];
      int sign_up = props[1];
      int books_per_day = props[2];

      ret.libraries.push_back(
          std::make_pair(
            i,
            std::make_tuple(n_books,
              sign_up,
              books_per_day,
              books)));
    }
  }

  ret.libraries = fplus::sort_on(&on_ts_bd, ret.libraries);

  return ret;
}

struct libraries distinct (struct libraries libs)
{
  libs.libraries = fplus::fwd::apply(
      libs.libraries,
      fplus::fwd::fold_left(
        [](std::pair<std::vector<library_desc_t>, std::set<int>> a,
          library_desc_t b) {
        std::vector<library_desc_t> ret = a.first;
        std::set<int> s = a.second;

        int id = b.first;
        int ts, bd;
        std::vector<int> bs;
        std::tie(std::ignore, ts, bd, bs) = b.second;

        std::set<int> bss(bs.begin(), bs.end());
        std::set<int> bs_ = fplus::set_difference(bss, s);
        std::set<int> ss = fplus::set_merge(s, bs_);

        return std::make_pair(
            fplus::prepend_elem(
              std::make_pair(
                id,
                std::make_tuple(
                  bs_.size(),
                  ts,
                  bd,
                  std::vector<int>(bs_.begin(), bs_.end()))),
              ret),
            ss);
        },
          std::make_pair(std::vector<library_desc_t>(), std::set<int>())),
          fplus::fwd::fst(),
          fplus::fwd::keep_if([](library_desc_t pt)
              { return !std::get<3>(pt.second).empty(); }),
          fplus::fwd::transform(
              fplus::fwd::transform_snd(
                [libs](std::tuple<int, int, int, std::vector<int>> tup) {
                int nb, ts, bd;
                std::vector<int> bs;
                std::tie(nb, ts, bd, bs) = tup;
                bs = fplus::sort_on([libs](int bid) { return libs.book_scores[bid]; }, bs);
                return std::make_tuple(nb, ts, bd, bs);
                }))
  );
  return libs;
}

std::vector<library_desc_t>
solve_ (int n_days, std::vector<library_desc_t> libs)
{
  std::vector<library_desc_t> ret;
  int len = libs.size();
  assert(len > 0);

  for (int i = 0; i < len && n_days > 0; i++) {
    library_desc_t e = libs[i];
    int ts = std::get<1>(e.second);

    if (ts <= n_days) {
      ret.push_back(e);
      n_days -= ts;
    }
  }

  return ret;
}

output_t solve (struct libraries libs)
{
  libs = distinct(libs);
  return fplus::fwd::apply(
      solve_(libs.n_days, libs.libraries),
      fplus::fwd::sort_on(&on_ts_bd),
      fplus::fwd::transform([](library_desc_t pt) {
        int id = pt.first;
        int nb;
        std::vector<int> bs;
        std::tie(nb, std::ignore, std::ignore, bs) = pt.second;
        return std::make_tuple(id, nb, bs);
        }));
}

void output_to_string (output_t output)
{
  std::cout << output.size() << std::endl;

  for (const std::tuple<int, int, std::vector<int>> & lib : output) {
    int x, y;
    std::vector<int> l;
    std::tie(x, y, l) = lib;

    std::cout << x << " " << y << std::endl;

    int len = l.size();
    assert(len > 0);
    std::cout << l[0];

    for (int i = 1; i < len; i++)
      std::cout << " " << l[i];

    std::cout << std::endl;
  }
}

//void output_to_string (output_t output)
//{
//  std::string str = fplus::fwd::apply(
//      output.libs,
//      fplus::fwd::transform_and_concat(
//        [](auto tup) {
//        auto x = std::get<0>(tup);
//        auto y = std::get<1>(tup);
//        auto l = std::get<2>(tup);
//        return fplus::fwd::apply(
//            fplus::prepend_elem(fplus::prepend_elem(x, fplus::singleton_seq(y)), l),
//            fplus::fwd::transform([](auto l) {
//              return fplus::fwd::apply(l,
//                  fplus::fwd::show(),
//                  fplus::fwd::join(" "));
//              }));
//        }),
//      fplus::fwd::prepend_elem(fplus::show(output.n_libs)),
//      fplus::fwd::join("\n"));
//
//  std::cout << str;
//}

int main (void)
{
  output_to_string(solve(read_libraries()));
  return 0;
}

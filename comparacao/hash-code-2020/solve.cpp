#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <tuple>
#include <vector>

#include <fplus/fplus.hpp>

struct libraries {
	int n_books;
	int n_libraries;
	int n_days;
	std::vector<int> book_score;
	std::vector<std::pair<int, std::tuple<int, int, int, std::vector<int>>>> libraries;
};

struct output {
	int n_libs;
	std::vector<std::tuple<int, int, std::vector<int>>> libs;
};

std::pair<int, int> on_ts_bd (std::pair<int, std::tuple<int, int, int, std::vector<int>>> pt)
{
	auto tup = pt.second;
	return std::make_pair(
			std::get<1>(tup),
			std::get<2>(tup));
}

struct libraries read_libraries (void)
{
	struct libraries ret;
	std::string input(std::istreambuf_iterator<char>{std::cin}, {});
	std::vector<std::vector<int>> values_ = fplus::fwd::apply(input,
			fplus::fwd::split_lines(false),
			fplus::fwd::transform([](std::string line) -> std::vector<int> {
				return fplus::fwd::apply(line,
						fplus::fwd::split_words(false),
						fplus::fwd::transform(fplus::read_value_unsafe<int>));
				}));
	std::list<std::vector<int>> values(values_.begin(), values_.end());

	{ /* Primeira linha */
		std::vector<int> line = values.front();
		values.pop_front();

		ret.n_books = line[0];
		ret.n_libraries = line[1];
		ret.n_days = line[2];
	}

	{ /* Segunda linha */
		ret.book_score = values.front();
		values.pop_front();
	}

	{ /* Resto */
		ret.libraries.reserve(ret.n_libraries);

		for (int i = 0; i < ret.n_libraries; i++) {
			std::vector<int> props = values.front();
			values.pop_front();

			std::vector<int> books = values.front();
			values.pop_front();

			int n_books = props[0];
			int sign_up = props[1];
			int books_per_day = props[2];

			auto tup = std::make_tuple(n_books, sign_up, books_per_day, books);

			ret.libraries.push_back(std::make_pair(i, tup));
		}
	}

	ret.libraries = fplus::sort_on(&on_ts_bd, ret.libraries);

	return ret;
}

struct libraries distinct (struct libraries libs)
{
	libs.libraries = fplus::fwd::apply(libs.libraries,
			fplus::fwd::fold_left(
				[](auto a, auto b) {
				auto ret = fplus::fst(a);
				std::set<int> s = fplus::snd(a);

				auto id = fplus::fst(b);
				auto ts = std::get<1>(fplus::snd(b));
				auto bd = std::get<2>(fplus::snd(b));
				std::vector<int> bs = std::get<3>(fplus::snd(b));

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
		std::make_pair(std::vector<std::pair<int, std::tuple<int, int, int, std::vector<int>>>>(), std::set<int>())),
		fplus::fwd::fst(),
		fplus::fwd::keep_if([](auto pt) { return !std::get<3>(pt.second).empty(); }),
		fplus::fwd::transform(fplus::fwd::transform_snd([libs](auto tup) {
					return std::make_tuple(
							std::get<0>(tup),
							std::get<1>(tup),
							std::get<2>(tup),
							fplus::sort_on([libs](int bid) { return libs.book_score[bid]; }, std::get<3>(tup)));
					}))
	);
	return libs;
}

std::vector<std::pair<int, std::tuple<int, int, int, std::vector<int>>>> solve_ (int n_days, std::vector<std::pair<int, std::tuple<int, int, int, std::vector<int>>>> libs)
{
	std::vector<std::pair<int, std::tuple<int, int, int, std::vector<int>>>> ret;

	for (int i = 0; i < n_days && !libs.empty(); i++) {
		auto e = fplus::head(libs);
		auto ts = std::get<1>(e.second);
		libs = fplus::tail(libs);

		if (ts <= n_days) {
			ret.push_back(e);
			n_days -= ts;
		}
	}

	return ret;
}

struct output solve (struct libraries libs)
{
	struct output ret;
	libs = distinct(libs);
	ret.libs = fplus::fwd::apply(solve_(libs.n_days, libs.libraries),
			fplus::fwd::sort_on(&on_ts_bd),
			fplus::fwd::transform([](auto pt) {
				int id = pt.first;
				auto nb = std::get<0>(pt.second);
				auto bs = std::get<3>(pt.second);
				return std::make_tuple(id, nb, bs);
				}));

	ret.n_libs = ret.libs.size();

	return ret;
}

void output_to_string (struct output output)
{
	std::cout << output.n_libs << std::endl;

	for (const auto & lib : output.libs) {
		auto x = std::get<0>(lib);
		auto y = std::get<1>(lib);
		auto l = std::get<2>(lib);

		std::cout << x << " " << y << std::endl;

		int len = l.size();
		assert(len > 0);
		std::cout << l[0];

		for (int i = 1; i < len; i++)
			std::cout << " " << l[i];

		std::cout << std::endl;
	}
}

//void output_to_string (struct output output)
//{
//	std::string str = fplus::fwd::apply(
//			output.libs,
//			fplus::fwd::transform_and_concat(
//				[](auto tup) {
//				auto x = std::get<0>(tup);
//				auto y = std::get<1>(tup);
//				auto l = std::get<2>(tup);
//				return fplus::fwd::apply(
//						fplus::prepend_elem(fplus::prepend_elem(x, fplus::singleton_seq(y)), l),
//						fplus::fwd::transform([](auto l) {
//							return fplus::fwd::apply(l,
//									fplus::fwd::show(),
//									fplus::fwd::join(" "));
//							}));
//				}),
//			fplus::fwd::prepend_elem(fplus::show(output.n_libs)),
//			fplus::fwd::join("\n"));
//
//	std::cout << str;
//}

int main (void)
{
	output_to_string(solve(read_libraries()));
	return 0;
}

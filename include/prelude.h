
#include <range/v3/all.hpp>

/*
 * all :: (a -> Bool) -> [a] -> Bool
 * all pred = foldl (\ret x -> ret && pred x) True
 */
template <typename In, typename Pred>
bool all (In && in, Pred pred)
{
	return ranges::accumulate(in, true, [pred] (bool ret, auto x) { return ret && pred(x); });
}

/*
 * any :: (a -> Bool) -> [a] -> Bool
 * all pred = foldl (\ret x -> ret || pred x) False
 */
template <typename In, typename Pred>
bool any (In && in, Pred pred)
{
	return ranges::accumulate(in, false, [pred] (bool ret, auto x) { return ret || pred(x); });
}

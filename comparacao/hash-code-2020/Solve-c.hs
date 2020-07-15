import Data.List hiding (length)
import qualified Data.Vector as V hiding (map, concatMap, zip)
import qualified Data.Set as S

data Libraries = Libraries {
    nBooks :: Int,
    nLibraries :: Int,
    nDays :: Int,
    bookScore :: V.Vector Int,
    --             ID   nBooks Signup books/day books
    libraries :: [(Int, (Int,   Int,   Int,      V.Vector Int))]
    } deriving Show

data Output = Output {
    nLibs :: Int,
    --        lib  nb   books
    libs :: [(Int, Int, V.Vector Int)]
    } deriving Show

readLibraries :: String -> Libraries
readLibraries = proc . map (map (read :: String -> Int) . words) . lines
    where
        proc ([nb, nl, nd]:scores:library_desc) = Libraries nb nl nd (V.fromList scores) (proc3 . zip [0..] . proc2 $ library_desc)

        proc3 libs = sortOn onBD libs
            where
                onBD (_, (_, _, bd, _)) = bd
                ts_nb_bd (_, (nb, ts, bd, _)) = (fromIntegral ts) / ((fromIntegral nb)/(fromIntegral bd))
                m_nb_bd (_, (nb, ts, bd, bs)) = negate $ ((fromIntegral $ V.sum bs)/(fromIntegral $ V.length bs)) * (fromIntegral nb / fromIntegral bd)
                total_score (_, (_, _, _, bs)) = negate $ V.sum bs

        proc2 ([nb, su, bd]:books:t) = (nb, su, bd, V.fromList books):(proc2 t)
        proc2 _ = []

ordBookScore :: Libraries -> Int -> Int
ordBookScore l id = (V.! id) . bookScore $ l

outputToString :: Output -> String
outputToString (Output nLibs libs) = unlines . ((show nLibs):) . concatMap (map (unwords . map show) . (\(x, y, l) -> [[x, y], V.toList l])) $ libs

solve :: Libraries -> Output
--solve l = Output 5 [(0, 1, [0]), (1, 1, [0]), (2, 1, [0]), (3, 1, [0]), (4, 1, [0])]
solve l = cenas . sortOn onScore . distinct . sortOn onTS $ solve' (nDays l) (libraries l)
    where
        distinct :: [(Int, (Int, Int, Int, V.Vector Int))] -> [(Int, (Int, Int, Int, V.Vector Int))]
        distinct = map (\(id, (nb, ts, bd, bs)) -> (id, (nb, ts, bd, V.fromList . (sortOn (ordBookScore l)) $ V.toList bs))) . filter (\(_, (_, _, _, bs)) -> not $ V.null bs) . fst . foldl' folder ([], S.empty)
            where
                folder (ret, s) e@(id, (nb, ts, bd, bs)) = let
                    bss = S.fromList . V.toList $ bs
                    bs' = S.difference bss s
                    ss = S.union s bs'
                    in ((id, (S.size bs', ts, bd, V.fromList . S.toList $ bs')):ret, ss)

        cenas = (\l -> Output (length l) l) . map (\(id, (nb, ts, bd, bs)) -> (id, nb, bs))

        solve' 0 _ = []
        solve' _ [] = []
        solve' nd (e@(id, (nb, ts, _, bs)):t)
            | ts <= nd = e:(solve' (nd - ts) t)
            | otherwise = solve' nd t

        onTS (id, (_, ts, _, bs)) = ts
        onScore (id, (_, _, _, bs)) = V.sum bs

main = interact (outputToString . solve . readLibraries)

import Data.List hiding (length)
import qualified Data.Vector as V hiding (map, concatMap, zip)
import qualified Data.Set as S

--                  ID    nBooks  Signup books/day books
type LibraryDesc = (Int, (Int,    Int,   Int,      V.Vector Int))

data Libraries = Libraries {
  nBooks :: Int,
  nLibraries :: Int,
  nDays :: Int,
  bookScore :: V.Vector Int,
  libraries :: [LibraryDesc]
  } deriving Show

--                        lib  nb   books
newtype Output = Output [(Int, Int, V.Vector Int)]

onScoreTS (_, (_, ts, _, bs)) = (V.sum bs, ts)
onTSScore (_, (_, ts, _, bs)) = (ts, V.sum bs)
onTS (_, (_, ts, _, bs)) = ts
onScore (_, (_, _, _, bs)) = V.sum bs
onNBTS (_, (nb, ts, _, _)) = (negate nb, ts)
onBD (_, (_, _, bd, _)) = bd
ts_nb_bd (_, (nb, ts, bd, _)) = (fromIntegral ts) / ((fromIntegral nb)/(fromIntegral bd))
m_nb_bd (_, (nb, ts, bd, bs)) = negate $ ((fromIntegral $ V.sum bs)/(fromIntegral $ V.length bs)) * (fromIntegral nb / fromIntegral bd)
total_score (_, (_, _, _, bs)) = negate $ V.sum bs
onTSBD (_, (_, ts, bd, _)) = (ts, bd)

readLibraries :: String -> Libraries
readLibraries = proc . map (map read . words) . lines
  where
    proc ([nb, nl, nd]:scores:library_desc) = Libraries nb nl nd scs libs
      where
        scs = V.fromList scores
        libs = (proc3 . zip [0..] $ proc2 library_desc)

    proc3 = sortOn onTSBD

    proc2 ([nb, su, bd]:books:t) = (nb, su, bd, V.fromList books):(proc2 t)
    proc2 _ = []

outputToString :: Output -> String
outputToString (Output libs) = unlines
                             . ((show nLibs):)
                             $ concatMap mapper libs
  where
    mapper = map (unwords . map show)
           . (\(x, y, l) -> [[x, y], V.toList l])
    nLibs = length libs

ordBookScore :: Libraries -> Int -> Int
ordBookScore l id = (V.! id) $ bookScore l

solve :: Libraries -> Output
solve l = cenas . sortOn onTSBD $ solve' (nDays l) (distinct $ libraries l)
  where
    distinct :: [LibraryDesc] -> [LibraryDesc]
    distinct = map mapper . filter pred . fst . foldl' folder def
      where
        mapper (id, (nb, ts, bd, bs)) = (id, (nb, ts, bd, sorted))
          where
            sorted = V.fromList . sortOn (ordBookScore l) $ V.toList bs

        pred (_, (_, _, _, bs)) = not $ V.null bs

        def = ([], S.empty)
        folder (ret, s) (id, (nb, ts, bd, bs)) = let
          bss = S.fromList $ V.toList bs
          bs' = S.difference bss s
          ss = S.union s bs'
          bs'' = V.fromList $ S.toList bs'
          in ((id, (S.size bs', ts, bd, bs'')):ret, ss)

    cenas = Output . map (\(id, (nb, ts, bd, bs)) -> (id, nb, bs))

    solve' 0 _ = []
    solve' _ [] = []
    solve' nd (e@(_, (_, ts, _, _)):t)
      | ts <= nd = e:(solve' (nd - ts) t)
      | otherwise = solve' nd t

main = interact (outputToString . solve . readLibraries)
